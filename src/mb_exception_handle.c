/******************************************************************************
*
* (c) Copyright 2013 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file mb_exception_handle.c
*
* This file contains a design example using the MicroBlaze exception features.
*
* @note		None.
*
* <pre>
*
* MODIFICATION HISTORY:
*
* Ver       Who       Date               Changes
* -----   ----     --------    ---------------------------------------------
* 1.00a    Hank     03/10/14       First release
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/



#include "xil_exception.h"

#include "mb_interface.h"

#include "microblaze_exceptions_i.h"
#include "microblaze_exceptions_g.h"

#include "dbg_print.h"

#include "mb_stack_unwind.h"
#include "mb_exception_handle.h"
#include "mb_exception_trigger.h"
#include "mb_exception_report.h"



/************************** Constant Definitions *****************************/

#define EX_HANDLER_STACK_SIZ            (4*21)  /* It is defined in hw_exception_handler.S */


/**************************** Type Definitions *******************************/



/***************** Macros (Inline Functions) Definitions *********************/

/*
	EX_HANDLER_STACK_SIZE:
		Stack pointer is subtracted by EX_HANDLER_STACK_SIZE in the asm exception handler
		Stack pointer is subtracted by a value in the C exception handler too
*/
/*
       mb_excep_unaligned_access_handler:
	c000f490:   brki          r11, 8 
	c000f494:   or            r0 , r0 , r0 
	c000f498:   addik         r1 , r1 , -88
	c000f49c:   swi           r15, r1 , 0 
	c000f4a0:   swi           r19, r1 , 80
	we compare the stack operation instruction location to function location,
	 and check the result. If it is less than 0x10, then we suppose 
	 that the stack operation instruction is found.
	 No document. Is it correct?
*/
#define get_sp_before_excep( func_pointer ) 	({   \
		unsigned int	u32_current_pc; \
		unsigned int 	u32_r1_sp_raw; \
		unsigned int 	u32_frame_size; \
		unsigned int	u32_r1_sp_temp; \
		unsigned int	*pu32_frame_size_pc; \
		u32_r1_sp_raw = mfgpr(r1); \
		u32_current_pc = mfpc( );   \
		u32_frame_size = mb_find_frame_size( u32_current_pc, &pu32_frame_size_pc );   \
		if( ( (unsigned int)pu32_frame_size_pc - (unsigned int)func_pointer )>0x10 )  xil_printf(" get_sp_before_excep error for %s, the stack operation location: %p might be wrong.", #func_pointer, pu32_frame_size_pc );	 \
		if( 0 == u32_frame_size )  xil_printf("get_sp_before_excep error for %s, frame size is zero.", #func_pointer );   \
			u32_r1_sp_temp = u32_r1_sp_raw+u32_frame_size+EX_HANDLER_STACK_SIZ; \
			u32_r1_sp_temp; })

/*	r17: If MicroBlaze is configured to support hardware exceptions, this register is loaded 
	with the address of the instruction following the instruction causing the HW exception, 
	except for exceptions in delay slots that use BTR instead (see Branch Target Register 
	(BTR)); if not, it is a general purpose register. 
	R17 is not modified during the code before C exception handler.
	*/ 
#define get_pc_before_excep( )  mfgpr(r17)

#define get_excep_info_ptr( ui_excep_cnt, a_excep_info_array ) 	({   \
		mb_excep_info_st *p_excep_info_element_temp; \
		unsigned int	u32_excep_info_num; \
		unsigned int 	u32_serial_num; \
		u32_excep_info_num = (sizeof(a_excep_info_array)/sizeof(mb_excep_info_st) ); \
		u32_serial_num = ui_excep_cnt%u32_excep_info_num;		\
		p_excep_info_element_temp = &a_excep_info_array[u32_serial_num];  \
		memset( p_excep_info_element_temp, 0, sizeof(mb_excep_info_st) ); \
		p_excep_info_element_temp; } )




/************************** Variable Definitions *****************************/


/* They are defined in lscript.ld */
extern int __text_start[];
extern int __text_end[];
extern int _stack_end[];
extern int __stack[];

unsigned int g_ui_excep_unaligned_access_cnt=0;
unsigned int g_ui_excep_illegal_instr_cnt=0;
unsigned int g_ui_excep_m_axi_i_cnt=0;
unsigned int g_ui_excep_m_axi_d_cnt=0;
unsigned int g_ui_excep_div_cnt=0;
unsigned int g_ui_excep_fpu_cnt=0;
unsigned int g_ui_excep_stack_overflow_cnt=0;
unsigned int g_ui_excep_mmu_cnt=0;

mb_excep_info_st g_unaligned_excep_info[MB_EXCEP_INFO_NUM];
mb_excep_info_st g_illegal_instr_excep_info[MB_EXCEP_INFO_NUM];
mb_excep_info_st g_axi_i_excep_info[MB_EXCEP_INFO_NUM];
mb_excep_info_st g_axi_d_excep_info[MB_EXCEP_INFO_NUM];
mb_excep_info_st g_div_excep_info[MB_EXCEP_INFO_NUM];
mb_excep_info_st g_fpu_excep_info[MB_EXCEP_INFO_NUM];
mb_excep_info_st g_stack_overflow_excep_info[MB_EXCEP_INFO_NUM];


/************************** Function Prototypes ******************************/


/*
The user exception handler code must either be defined with either attribute svc_handler or 
attribute svc_table_handler:
void function_name() __attribute__((svc_handler));
void function_name() __attribute__((svc_table_handler (ID)));
The first attribute ensures that the compiler will emit an indirect call to the handler with a brki 
rD,0x8 instruction, and emit an rtbd instruction to return from the handler. This means that when 
the MMU is enabled the handler function is executed in privileged mode.

*/
void mb_excep_unaligned_access_handler() __attribute__((svc_handler));
void mb_excep_illegal_instr_handler() __attribute__((svc_handler));
void mb_excep_m_axi_i_handler() __attribute__((svc_handler));
void mb_excep_m_axi_d_handler() __attribute__((svc_handler));
void mb_excep_div_zero_handler() __attribute__((svc_handler));
void mb_excep_fpu_handler() __attribute__((svc_handler));
void mb_excep_stack_overflow_handler() __attribute__((svc_handler));




/************************** Function Implementation ******************************/



/* verified. OK. */
/* If no unaligned access handler is register, the default handler will load the value by byte. */
void mb_excep_unaligned_access_handler(void *Data)
{
	unsigned int u32_r1_sp;
	unsigned int u32_r17_instr_loc;
	mb_excep_info_st *p_excep_info_element;

	//	get stack pointer (register R1)  before exception
#if 1
	
	u32_r1_sp = get_sp_before_excep( mb_excep_unaligned_access_handler );
#else

	u32_r1_sp = ({   
	unsigned int	u32_current_pc; 
	unsigned int 	u32_r1_sp_raw; 
	unsigned int 	u32_frame_size; 
	unsigned int	u32_r1_sp_temp; 
	unsigned int	*pu32_frame_size_pc; 
	u32_r1_sp_raw = mfgpr(r1); 
	u32_current_pc = mfpc( );   
	u32_frame_size = mb_find_frame_size( u32_current_pc, &pu32_frame_size_pc );   
	if( ( (unsigned int)pu32_frame_size_pc - (unsigned int)mb_excep_unaligned_access_handler )<0x10 )  
				xil_printf(" get_sp_before_excep error for mb_excep_unaligned_access_handler" );   
		u32_r1_sp_temp = u32_r1_sp_raw+u32_frame_size+EX_HANDLER_STACK_SIZ; 
		u32_r1_sp_temp; });

#endif


	//print_var_hex(u32_r1_sp);
	
	//	get instruction location (register R17)  before exception
	u32_r17_instr_loc = get_pc_before_excep( );
	//print_var_hex(u32_r17_instr_loc);

#if 1
	p_excep_info_element = get_excep_info_ptr( g_ui_excep_unaligned_access_cnt, g_unaligned_excep_info );
#else

	{   
		unsigned int	u32_excep_info_num; 
		unsigned int 	u32_serial_num; 
		u32_excep_info_num = (sizeof(g_unaligned_excep_info)/sizeof(mb_excep_info_st) ); 
		u32_serial_num = g_ui_excep_unaligned_access_cnt%u32_excep_info_num;		
		p_excep_info_element = &g_unaligned_excep_info[u32_serial_num];  
	}

#endif
	
	g_ui_excep_unaligned_access_cnt++;
	dbg_print("\n\r\n\r");
	dbg_print_var(g_ui_excep_unaligned_access_cnt);
	
	mb_excep_info_handle( g_ui_excep_unaligned_access_cnt, p_excep_info_element, u32_r17_instr_loc, u32_r1_sp );

}

/* verified. OK. */
void mb_excep_illegal_instr_handler(void *Data)
{
	unsigned int u32_r1_sp;
	unsigned int u32_r17_instr_loc;
	mb_excep_info_st *p_excep_info_element;

	//	get stack pointer (register R1)  before exception
	u32_r1_sp = get_sp_before_excep( mb_excep_illegal_instr_handler );
	//print_var_hex(u32_r1_sp);
	
	//	get instruction location (register R17)  before exception
	u32_r17_instr_loc = get_pc_before_excep( );
	//print_var_hex(u32_r17_instr_loc);

	p_excep_info_element = get_excep_info_ptr( g_ui_excep_illegal_instr_cnt, g_illegal_instr_excep_info );
	
	g_ui_excep_illegal_instr_cnt++;
	dbg_print("\n\r\n\r");
	dbg_print_var(g_ui_excep_illegal_instr_cnt);
	
	mb_excep_info_handle( g_ui_excep_illegal_instr_cnt, p_excep_info_element, u32_r17_instr_loc, u32_r1_sp );

}


#ifdef  MB_EXCEP_AXI_I_ENABLE
void mb_excep_m_axi_i_handler(void *Data)
{
	unsigned int u32_r1_sp;
	unsigned int u32_r17_instr_loc;
	mb_excep_info_st *p_excep_info_element;

	//	get stack pointer (register R1)  before exception
	u32_r1_sp = get_sp_before_excep( mb_excep_m_axi_i_handler );
	//print_var_hex(u32_r1_sp);
	
	//	get instruction location (register R17)  before exception
	u32_r17_instr_loc = get_pc_before_excep( );
	//print_var_hex(u32_r17_instr_loc);

	p_excep_info_element = get_excep_info_ptr( g_ui_excep_m_axi_i_cnt, g_axi_i_excep_info );
	
	g_ui_excep_m_axi_i_cnt++;
	dbg_print("\n\r\n\r");
	dbg_print_var(g_ui_excep_m_axi_i_cnt);
	
	mb_excep_info_handle( g_ui_excep_m_axi_i_cnt, p_excep_info_element, u32_r17_instr_loc, u32_r1_sp );

}
#endif	/*  MB_EXCEP_AXI_I_ENABLE */



void mb_excep_m_axi_d_handler(void *Data)
{
	unsigned int u32_r1_sp;
	unsigned int u32_r17_instr_loc;
	mb_excep_info_st *p_excep_info_element;

	//	get stack pointer (register R1)  before exception
	u32_r1_sp = get_sp_before_excep( mb_excep_m_axi_d_handler );
	//print_var_hex(u32_r1_sp);
	
	//	get instruction location (register R17)  before exception
	u32_r17_instr_loc = get_pc_before_excep( );
	//print_var_hex(u32_r17_instr_loc);

	p_excep_info_element =  get_excep_info_ptr( g_ui_excep_m_axi_d_cnt, g_axi_d_excep_info );
	
	g_ui_excep_m_axi_d_cnt++;
	dbg_print("\n\r\n\r");
	dbg_print_var(g_ui_excep_m_axi_d_cnt);
	
	mb_excep_info_handle( g_ui_excep_m_axi_d_cnt, p_excep_info_element, u32_r17_instr_loc, u32_r1_sp );

}

/* verified. OK. */
void mb_excep_div_zero_handler(void *Data)
{
	unsigned int u32_r1_sp;
	unsigned int u32_r17_instr_loc;
	mb_excep_info_st *p_excep_info_element;

	//	get stack pointer (register R1)  before exception
	u32_r1_sp = get_sp_before_excep( mb_excep_div_zero_handler );
	//print_var_hex(u32_r1_sp);
	
	//	get instruction location (register R17)  before exception
	u32_r17_instr_loc = get_pc_before_excep( );
	//print_var_hex(u32_r17_instr_loc);

	p_excep_info_element = get_excep_info_ptr( g_ui_excep_div_cnt, g_div_excep_info );
	
	g_ui_excep_div_cnt++;
	dbg_print("\n\r\n\r");
	dbg_print_var(g_ui_excep_div_cnt);
	
	mb_excep_info_handle( g_ui_excep_div_cnt, p_excep_info_element, u32_r17_instr_loc, u32_r1_sp );

}

/* verified. OK. */
void mb_excep_fpu_handler(void *Data)
{
	unsigned int u32_r1_sp;
	unsigned int u32_r17_instr_loc;
	mb_excep_info_st *p_excep_info_element;

	//	get stack pointer (register R1)  before exception
	u32_r1_sp = get_sp_before_excep( mb_excep_fpu_handler );
	//print_var_hex(u32_r1_sp);
	
	//	get instruction location (register R17)  before exception
	u32_r17_instr_loc = get_pc_before_excep( );
	//print_var_hex(u32_r17_instr_loc);

	p_excep_info_element = get_excep_info_ptr( g_ui_excep_fpu_cnt, g_fpu_excep_info );
	
	g_ui_excep_fpu_cnt++;
	dbg_print("\n\r\n\r");
	dbg_print_var(g_ui_excep_fpu_cnt);
	
	mb_excep_info_handle( g_ui_excep_fpu_cnt, p_excep_info_element, u32_r17_instr_loc, u32_r1_sp );

}



#ifdef  MB_EXCEP_STACK_ENABLE
/*
 Default exception handler enter dead loop when stack overflow happened.
 So mb_excep_stack_overflow_handler is not tested now
*/
void mb_excep_stack_overflow_handler(void *Data)
{

	unsigned int u32_r1_sp;
	unsigned int u32_r17_instr_loc;
	mb_excep_info_st *p_excep_info_element;

	//	get stack pointer (register R1)  before exception
	u32_r1_sp = get_sp_before_excep( mb_excep_stack_overflow_handler );
	//print_var_hex(u32_r1_sp);
	
	//	get instruction location (register R17)  before exception
	u32_r17_instr_loc = get_pc_before_excep( );
	//print_var_hex(u32_r17_instr_loc);

	p_excep_info_element = get_excep_info_ptr( g_ui_excep_stack_overflow_cnt, g_stack_overflow_excep_info );
	
	g_ui_excep_stack_overflow_cnt++;
	dbg_print("\n\r\n\r");
	dbg_print_var(g_ui_excep_stack_overflow_cnt);
	
	mb_excep_info_handle( g_ui_excep_stack_overflow_cnt, p_excep_info_element, u32_r17_instr_loc, u32_r1_sp );
	
}
#endif	/*  MB_EXCEP_STACK_ENABLE */


void mb_excep_info_handle
( 	
	unsigned int u32_exception_num, 
	mb_excep_info_st *p_excep_info,
	unsigned int u32_r17_pc,
	unsigned int u32_r1_sp
)
{
    int     i_status;
    int     i_pc_status;
    int     i_stack_status;
	unsigned int u32_msr;
	unsigned int u32_ear;
	unsigned int u32_esr;
	unsigned int u32_fsr;
	unsigned int u32_edr;
	unsigned int u32_stack_begin;
	unsigned int u32_stack_buf_size;

	dbg_print_func_begin;

    dbg_ptr_limit_check( p_excep_info, RTN_VOID );
    ptr_limit_check( p_excep_info, RTN_VOID );

	/* Get information about exception */
	u32_msr = mfmsr( );
	dbg_print_var_hex(u32_msr);
	p_excep_info->u32_msr = u32_msr;

	u32_ear = mfear( );
	dbg_print_var_hex(u32_ear);
	p_excep_info->u32_ear = u32_ear;

	u32_esr = mfesr( );
	dbg_print_var_hex(u32_esr);
	p_excep_info->u32_esr = u32_esr;

	u32_fsr = mffsr( );
	dbg_print_var_hex(u32_fsr);
	p_excep_info->u32_fsr = u32_fsr;

	u32_edr = mfedr( );
	dbg_print_var_hex(u32_edr);
	p_excep_info->u32_edr = u32_edr;

	// PC  before exception
	dbg_print_var_hex(u32_r17_pc);
	p_excep_info->u32_r17_instr_loc = u32_r17_pc;

	//	stack pointer (register R1)  before exception
	dbg_print_var_hex(u32_r1_sp);
	p_excep_info->u32_r1_sp = u32_r1_sp;

    // exception assembly code might not be in .text section ?? 
    // Check PC/SP in order to avoid illegal memory access

    i_pc_status =  mb_pc_check( u32_r17_pc );
    i_stack_status = mb_stack_check( u32_r1_sp );
	if( ( 0 != i_pc_status ) 
		|| ( 0 != i_stack_status ) )
	{
		xil_printf("Invalid program pointer: 0x%08x or stack pointer: 0x%08x\n\r", 
                u32_r17_pc, u32_r1_sp );

		xil_printf("\n\rInstruction:\n\r");
        mb_mem_dump( u32_r17_pc-16, 64 );

		xil_printf("\n\rStack:\n\r");
        mb_mem_dump( u32_r1_sp-16, 256 );
        

        /* Should we loop forever here??? */
        
		return;
	}

	u32_stack_buf_size = sizeof(p_excep_info->u32_stack_content);
	if( (u32_r1_sp+u32_stack_buf_size)>(unsigned int)__stack)
	{
	    // Refine saved stack size in order to avoid illegal memory access
		u32_stack_buf_size = (unsigned int)__stack-u32_r1_sp;
	}
    
	u32_stack_begin = (u32_r1_sp-0x20)&0xffffffe0;
	if( (unsigned int)_stack_end>u32_stack_begin)
	{
		u32_stack_begin = (unsigned int)_stack_end;
	}
	memcpy(p_excep_info->u32_stack_content, 
			(u32 *)u32_stack_begin, u32_stack_buf_size );
	
	/* stack addres is always higher than data section. */
	p_excep_info->ui32_sp_backup_begin = u32_stack_begin;

	/* Unwind stack */
	i_status = mb_unwind_stack( u32_r1_sp, u32_r17_pc, (unsigned int)__stack, 
                	(unsigned int)_stack_end, p_excep_info->call_info );
	if( 0 != i_status)
	{
	    /* Failed to unwind stack */
        mb_excep_report_simple_info( p_excep_info );
        mb_msr_report( p_excep_info->u32_msr );
        mb_esr_report( p_excep_info->u32_esr );
        mb_fsr_report( p_excep_info->u32_fsr );
	}

#ifdef  MB_EXCEP_DEBUG_SHOW
    {
        unsigned int u32_dump_len;

    	dbg_print("\n\r\n\r");
    	dbg_print("Dump stack of system mode that causes this exception::\n\r");


        u32_dump_len = (unsigned int)__stack-u32_stack_begin;
    	if( u32_dump_len >256  )
    	{
    		u32_dump_len = 256;
    	}
        mb_mem_dump( u32_r1_sp, u32_dump_len );
        
        dbg_print("\n\r\n\r\n\r");
    }
#endif /*  MB_EXCEP_DEBUG_SHOW  */	
	
}





int mb_exception_init( void )
{

	Xil_ExceptionRegisterHandler( XIL_EXCEPTION_ID_UNALIGNED_ACCESS, mb_excep_unaligned_access_handler, 0);
	Xil_ExceptionRegisterHandler( XIL_EXCEPTION_ID_ILLEGAL_OPCODE, mb_excep_illegal_instr_handler, 0);

#ifdef  MB_EXCEP_AXI_I_ENABLE
	Xil_ExceptionRegisterHandler( XIL_EXCEPTION_ID_M_AXI_I_EXCEPTION, mb_excep_m_axi_i_handler, 0);
#endif	/*  MB_EXCEP_AXI_I_ENABLE */

	Xil_ExceptionRegisterHandler( XIL_EXCEPTION_ID_M_AXI_D_EXCEPTION, mb_excep_m_axi_d_handler, 0);
	Xil_ExceptionRegisterHandler( XIL_EXCEPTION_ID_DIV_BY_ZERO, mb_excep_div_zero_handler, 0);
	Xil_ExceptionRegisterHandler( XIL_EXCEPTION_ID_FPU, mb_excep_fpu_handler, 0);

#ifdef  MB_EXCEP_STACK_ENABLE
	Xil_ExceptionRegisterHandler( XIL_EXCEPTION_ID_STACK_VIOLATION, mb_excep_stack_overflow_handler, 0);
#endif	/*  MB_EXCEP_STACK_ENABLE */

	/*
	.stack (NOLOAD) : {
	   _stack_end = .;
	   . += _STACK_SIZE;
	   . = ALIGN(8);
	   _stack = .;
	   __stack = _stack;
	} > mcb_ddr3_S0_AXI_BASEADDR

	u32_stack_high_address:  __stack, 
	u32_stack_low_address:   _stack_end

	*/
	mtslr(_stack_end);
	mtshr(__stack);

#if 1
	mb_excep_handler_report( );
#endif

	Xil_ExceptionInit();
	//Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
	//		(Xil_ExceptionHandler)XIntc_InterruptHandler,
	//		(void *)IntcInstancePtr);

	Xil_ExceptionEnable();


    return 0;
}


