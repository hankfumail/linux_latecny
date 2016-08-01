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
* @file mb_exception_report.c
*
* This file contains a design example using the MicroBlaze exception features.
*
* @note        None.
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
#include "xil_cache.h"

#include "xil_cache.h"
#include "microblaze_exceptions_i.h"
#include "microblaze_exceptions_g.h"

#include "dbg_print.h"
#include "reg_report.h"

#include "mb_exception_handle.h"
#include "mb_exception_trigger.h"
#include "mb_exception_report.h"
#include "mb_stack_unwind.h"



/************************** Constant Definitions *****************************/

#define EX_HANDLER_STACK_SIZ            (4*21)  /* It is defined in hw_exception_handler.S */


/**************************** Type Definitions *******************************/



/***************** Macros (Inline Functions) Definitions *********************/





/************************** Variable Definitions *****************************/

/* It is defined in hw_exception_handler.S */
extern MB_ExceptionVectorTableEntry MB_ExceptionVectorTable[];


/* They are defined in lscript.ld */
extern int __text_start[];
extern int __text_end[];

extern int __CTOR_LIST__[];
extern int __CTOR_END__[];

extern int __DTOR_LIST__[];
extern int __DTOR_END__[];

extern int __rodata_start[];
extern int __rodata_end[];

extern int __sdata2_start[];
extern int __sdata2_end[];

extern int __sbss2_start[];
extern int __sbss2_end[];

extern int __data_start[];
extern int __data_end[];

extern int __sdata_start[];
extern int __sdata_end[];

extern int __sbss_start[];
extern int __sbss_end[];

extern int __tdata_start[];
extern int __tdata_end[];

extern int __tbss_start[];
extern int __tbss_end[];

extern int __bss_start[];
extern int __bss_end[];

extern int _heap_start[];
extern int _heap_end[];

extern int _stack_end[];
extern int __stack[];


/************************** Function Prototypes ******************************/

extern int main( void );




/************************** Function Implementation ******************************/






void mb_elf_section_report( void )
{

    print_var_hex( __text_start );
    print_var_hex( __text_end );

    
    print_var_hex( __CTOR_LIST__ );
    print_var_hex( __CTOR_END__ );    

    
    print_var_hex( __DTOR_LIST__ );
    print_var_hex( __DTOR_END__ );    

    
    print_var_hex( __rodata_start );
    print_var_hex( __rodata_end );

    print_var_hex( __sdata2_start );
    print_var_hex( __sdata2_end );
        
    print_var_hex( __sbss2_start );
    print_var_hex( __sbss2_end );

    print_var_hex( __data_start );
    print_var_hex( __data_end );    

    print_var_hex( __sdata_start );
    print_var_hex( __sdata_end );
        
    print_var_hex( __sbss_start );
    print_var_hex( __sbss_end );    

    print_var_hex( __tdata_start );
    print_var_hex( __tdata_end );

    print_var_hex( __tbss_start );
    print_var_hex( __tbss_end );

    print_var_hex( __bss_start );
    print_var_hex( __bss_end );

    print_var_hex( _heap_start );
    print_var_hex( _heap_end );
    
    print_var_hex( _stack_end );
    print_var_hex( __stack );


}

/* In MB doc, bit 31 is the lowest bit, bit 0 is the highest bit */
void mb_msr_report( unsigned int u32_msr )
{
    static reg_defintion2_st     mb_msr_reg_definition[] =
    {
        /*     Type                    a_shift_num            a_mask            b_shift_num        b_mask         value     name                    info          */
        {  REG_BITS_TYPE_CHECK,      31-0,         0x1,      0x0,      0x0,     0x1, "Arithmetic Carry Copy", "Carry (No Borrow) Copy" },
        {  REG_BITS_TYPE_CHECK,     31-17,         0x1,      0x0,      0x0,     0x1, "Virtual Protected Mode Save", "MMU address translation and access protection enabled(Only available when configured with an MMU)" },
        {  REG_BITS_TYPE_CHECK,     31-18,         0x1,      0x0,      0x0,     0x1, "Virtual Protected Mode", "MMU address translation and access protection enabled(Only available when configured with an MMU)" },
        {  REG_BITS_TYPE_CHECK,     31-19,         0x1,      0x0,      0x0,     0x0, "User Mode Save", "Privileged Mode Saved, all instructions are allowed" },
        {  REG_BITS_TYPE_CHECK,     31-19,         0x1,      0x0,      0x0,     0x1, "User Mode Save", "User Mode Saved, certain instructions are not allowed(Only available when configured with an MMU)" },
        {  REG_BITS_TYPE_CHECK,     31-20,         0x1,      0x0,      0x0,     0x0, "User Mode", "Privileged Mode, all instructions are allowed" },
        {  REG_BITS_TYPE_CHECK,     31-20,         0x1,      0x0,      0x0,     0x1, "User Mode", "User Mode, certain instructions are not allowed(Only available when configured with an MMU)" },
        {  REG_BITS_TYPE_CHECK,     31-21,         0x1,      0x0,      0x0,     0x1, "Processor Version Register exists", "Processor Version Register exists" },
        {  REG_BITS_TYPE_CHECK,     31-22,         0x1,      0x0,      0x0,     0x1, "Exception In Progress", "Hardware exception in progress" },
        {  REG_BITS_TYPE_CHECK,     31-23,         0x1,      0x0,      0x0,     0x0, "Exception Enable", "Hardware exceptions disabled" },
        {  REG_BITS_TYPE_CHECK,     31-23,         0x1,      0x0,      0x0,     0x1, "Exception Enable", "Hardware exceptions enabled" },
        {  REG_BITS_TYPE_CHECK,     31-24,         0x1,      0x0,      0x0,     0x1, "Data Cache Enable", " Data Cache enabled" },
        {  REG_BITS_TYPE_CHECK,     31-25,         0x1,      0x0,      0x0,     0x1, "Division by Zero or Division Overflow", " Division by zero or division overflow has occurred" },
        {  REG_BITS_TYPE_CHECK,     31-26,         0x1,      0x0,      0x0,     0x1, "Instruction Cache Enable", "Instruction Cache enabled" },
        {  REG_BITS_TYPE_CHECK,     31-27,         0x1,      0x0,      0x0,     0x1, "Stream (FSL or AXI) Error", "Get or getd control type mismatch. This bit is sticky." },
        {  REG_BITS_TYPE_CHECK,     31-28,         0x1,      0x0,      0x0,     0x1, "Break in Progress", " Break in Progress" },
        {  REG_BITS_TYPE_CHECK,     31-29,         0x1,      0x0,      0x0,     0x1, "Arithmetic Carry", "Carry (No Borrow)" },
        {  REG_BITS_TYPE_CHECK,     31-30,         0x1,      0x0,      0x0,     0x1, "Interrupt Enable", "Interrupts enabled" },
        {  0xffffffff,  0xffffffff,  0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, "", "" }
    };

    register2_report( u32_msr, "MicroBlaze Machine Status Register", mb_msr_reg_definition );
    xil_printf("\n\r");

}


void mb_esr_report( unsigned int u32_esr )
{
    static reg_defintion2_st     mb_esr_reg_definition[] =
    {
        /*     Type                    a_shift_num    a_mask    b_shift_num        b_mask         value     name                    info          */
        {  REG_BITS_TYPE_CHECK,     31-19,    0x1,      0,          0x0,        0x01, "Delay Slot Exception.", "Caused by delay slot instruction" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,      0,          0x0,        0x00, "Exception Cause", "Stream exception" },
        {  REG_BITS_TYPE_SHOW_DEC,  31-26,    0xf,      0,          0x0,        0x00, "Stream (FSL or AXI) index for stream exception", "" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,      0,          0x0,        0x01, "Exception Cause", "Unaligned data access exception" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,(31-20)-8,      0x100,       0x001, "Exception Cause", "Unaligned data access exception: unaligned halfword access" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,(31-20)-8,      0x100,       0x101, "Exception Cause", "Unaligned data access exception: unaligned word access" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,(31-21)-8,      0x100,       0x001, "Exception Cause", "Unaligned data access exception: unaligned load access" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,(31-21)-8,      0x100,       0x101, "Exception Cause", "Unaligned data access exception: unaligned store access" },
        {  REG_BITS_TYPE_SHOW_DEC,  31-26,   0x1f,      0,          0x0,       0x000, "Source/Destination Register used for unaligned exception", "" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,      0,          0x0,        0x02, "Exception Cause", "Illegal op-code exception" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,      0,          0x0,        0x03, "Exception Cause", "Instruction bus error exception exception" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,(31-20)-8,      0x100,       0x103, "Exception Cause", "Instruction bus error exception: Exception caused by ILMB ECC correctable or uncorrectable error" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,      0,          0x0,        0x04, "Exception Cause", "Data bus error exception" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,(31-20)-8,      0x100,       0x104, "Exception Cause", "Data bus error exception: Exception caused by DLMB ECC correctable or uncorrectable error" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,      0,          0x0,        0x05, "Exception Cause", "Divide exception" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,(31-20)-8,      0x100,       0x005, "Exception Cause", "Divide exception: Divide-By-Zero" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,(31-20)-8,      0x100,       0x105, "Exception Cause", "Divide exception: Division Overflow" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,      0,          0x0,        0x06, "Exception Cause", "Floating point unit exception" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,      0,          0x0,        0x07, "Exception Cause", "Privileged instruction exception" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,      0,          0x0,        0x08, "Exception Cause", "Stack protection violation exception" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,      0,          0x0,        0x09, "Exception Cause", "Data storage exception" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,      0,          0x0,        0x0a, "Exception Cause", "Instruction storage exception" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,      0,          0x0,        0x0b, "Exception Cause", "Data TLB miss exception" },
        {  REG_BITS_TYPE_CHECK,     31-31,   0x1f,      0,          0x0,        0x0c, "Exception Cause", "Instruction TLB miss exception" },
        {  0xffffffff,      0xffffffff,  0xffffffff,    0xffffffff,  0xffffffff,  0xffffffff, "", "" }
    };

    register2_report( u32_esr, "MicroBlaze Exception Status Register", mb_esr_reg_definition  );
    xil_printf("\n\r");

}



void mb_fsr_report( unsigned int u32_fsr )
{
    static reg_defintion2_st     mb_fsr_reg_definition[] =
    {
        /*     Type                    a_shift_num    a_mask    b_shift_num        b_mask         value     name                    info          */
        {  REG_BITS_TYPE_CHECK,     31-27,    0x1,      0,          0x0,        0x01, "IO", "Invalid operation" },
        {  REG_BITS_TYPE_CHECK,     31-28,    0x1,      0,          0x0,        0x01, "DZ", "Divide-by-zero" },
        {  REG_BITS_TYPE_CHECK,     31-29,    0x1,      0,          0x0,        0x01, "OF", "Overflow" },
        {  REG_BITS_TYPE_CHECK,     31-30,    0x1,      0,          0x0,        0x01, "UF", "Underflow" },
        {  REG_BITS_TYPE_CHECK,     31-31,    0x1,      0,          0x0,        0x01, "DO", "Denormalized operand error" },
        {  0xffffffff,      0xffffffff,  0xffffffff,    0xffffffff,  0xffffffff,  0xffffffff, "", "" }
    };

    register2_report( u32_fsr, "MicroBlaze Floating Point Status Register", mb_fsr_reg_definition );
    xil_printf("\n\r");

}



void mb_excep_handler_report( void )
{
    int i;

    MB_ExceptionVectorTableEntry *pExceptionVectorTable;

    xil_printf("Exception table address: %p.\n\r", MB_ExceptionVectorTable );
    for( i =0; i<8; i++)
    {
        pExceptionVectorTable = &MB_ExceptionVectorTable[i];
        xil_printf("No.%d exception table, function: 0x%08x, data: 0x%08x.\n\r", 
                i, (unsigned int)pExceptionVectorTable->Handler, (unsigned int)pExceptionVectorTable->CallBackRef );
    }
    xil_printf("function mb_excep_unaligned_access_handler location: %p.\n\r", mb_excep_unaligned_access_handler );
    xil_printf("function mb_excep_illegal_instr_handler location: %p.\n\r", mb_excep_illegal_instr_handler );

#ifdef  MB_EXCEP_AXI_I_ENABLE
    xil_printf("function mb_excep_m_axi_i_handler location: %p.\n\r", mb_excep_m_axi_i_handler );
#endif    /*  MB_EXCEP_AXI_I_ENABLE */

    xil_printf("function mb_excep_m_axi_d_handler location: %p.\n\r", mb_excep_m_axi_d_handler );
    xil_printf("function mb_excep_div_zero_handler location: %p.\n\r", mb_excep_div_zero_handler );
    xil_printf("function mb_excep_fpu_handler location: %p.\n\r", mb_excep_fpu_handler );

#ifdef  MB_EXCEP_STACK_ENABLE
    xil_printf("function mb_excep_stack_overflow_handler location: %p.\n\r", mb_excep_stack_overflow_handler );
#endif    /*  MB_EXCEP_STACK_ENABLE */

    xil_printf("Informamtion g_unaligned_excep_info location: %p.\n\r", g_unaligned_excep_info );
    xil_printf("Informamtion g_illegal_instr_excep_info location: %p.\n\r", g_illegal_instr_excep_info );
    
#ifdef  MB_EXCEP_AXI_I_ENABLE
    xil_printf("Informamtion g_axi_i_excep_info location: %p.\n\r", g_axi_i_excep_info );
#endif    /*  MB_EXCEP_AXI_I_ENABLE */

    xil_printf("Informamtion g_axi_d_excep_info location: %p.\n\r", g_axi_d_excep_info );
    xil_printf("Informamtion g_div_excep_info location: %p.\n\r", g_div_excep_info );
    xil_printf("Informamtion g_fpu_excep_info location: %p.\n\r", g_fpu_excep_info );
    
#ifdef  MB_EXCEP_STACK_ENABLE
    xil_printf("Informamtion g_stack_overflow_excep_info location: %p.\n\r", g_stack_overflow_excep_info );
#endif    /*  MB_EXCEP_STACK_ENABLE */

    
#ifdef MICROBLAZE_EXCEPTIONS_ENABLED
    xil_printf ("MICROBLAZE_EXCEPTIONS_ENABLED defined.\r\n");
#else  /*   MICROBLAZE_EXCEPTIONS_ENABLED   */
    xil_printf ("MICROBLAZE_EXCEPTIONS_ENABLED not defined.\r\n");
#endif  /*   MICROBLAZE_EXCEPTIONS_ENABLED   */
    
#ifdef XPAR_MICROBLAZE_USE_STACK_PROTECTION
    xil_printf ("XPAR_MICROBLAZE_USE_STACK_PROTECTION defined.\r\n");
#else  /*   XPAR_MICROBLAZE_USE_STACK_PROTECTION   */
    xil_printf ("XPAR_MICROBLAZE_USE_STACK_PROTECTION not defined.\r\n");
#endif  /*  XPAR_MICROBLAZE_USE_STACK_PROTECTION    */
    
#ifdef MICROBLAZE_CAN_HANDLE_EXCEPTIONS_IN_DELAY_SLOTS
    xil_printf ("MICROBLAZE_CAN_HANDLE_EXCEPTIONS_IN_DELAY_SLOTS defined.\r\n");
#else  /*   MICROBLAZE_CAN_HANDLE_EXCEPTIONS_IN_DELAY_SLOTS   */
    xil_printf ("MICROBLAZE_CAN_HANDLE_EXCEPTIONS_IN_DELAY_SLOTS defined.\r\n");
#endif  /*  MICROBLAZE_CAN_HANDLE_EXCEPTIONS_IN_DELAY_SLOTS    */

#ifdef NO_UNALIGNED_EXCEPTIONS
    xil_printf ("NO_UNALIGNED_EXCEPTIONS defined.\r\n");
#else  /*   NO_UNALIGNED_EXCEPTIONS   */
    xil_printf ("NO_UNALIGNED_EXCEPTIONS not defined.\r\n");
#endif  /*  NO_UNALIGNED_EXCEPTIONS    */
    
#ifdef MICROBLAZE_FP_EXCEPTION_ENABLED
    xil_printf ("MICROBLAZE_FP_EXCEPTION_ENABLED defined.\r\n");
#else  /*   MICROBLAZE_FP_EXCEPTION_ENABLED   */
    xil_printf ("MICROBLAZE_FP_EXCEPTION_ENABLED not defined.\r\n");
#endif  /*  MICROBLAZE_FP_EXCEPTION_ENABLED    */

#ifdef MICROBLAZE_FP_EXCEPTION_DECODE
    xil_printf ("MICROBLAZE_FP_EXCEPTION_DECODE defined.\r\n");
#else  /*   MICROBLAZE_FP_EXCEPTION_DECODE   */
    xil_printf ("MICROBLAZE_FP_EXCEPTION_DECODE not defined.\r\n");
#endif  /*  MICROBLAZE_FP_EXCEPTION_DECODE    */

}





int mb_excep_report_excep_counter( void )
{

    print_var_non_zero(g_ui_excep_unaligned_access_cnt);
    print_var_non_zero(g_ui_excep_illegal_instr_cnt);
    print_var_non_zero(g_ui_excep_m_axi_i_cnt);
    print_var_non_zero(g_ui_excep_m_axi_d_cnt);
    print_var_non_zero(g_ui_excep_div_cnt);
    print_var_non_zero(g_ui_excep_fpu_cnt);
    print_var_non_zero(g_ui_excep_stack_overflow_cnt);
    print_var_non_zero(g_ui_excep_mmu_cnt);
    print_var_non_zero(g_ui_excep_mmu_cnt);

    return 0;
}




void mb_mem_dump( u32 u32_mem_addr, u32 u32_num )
{
    int i, i_status;
    u32 u32_loop;
    u32 *pu32_mem_loop;

    i_status =  mb_mem_addr_check( u32_mem_addr );
    if( 0 == i_status ) 
	{
	    u32_loop = (u32_num+15)/16+1;
	    pu32_mem_loop = (u32 *)(u32_mem_addr&0xfffffff0);
	    for( i=0; i<u32_loop; i++ )
        {
            xil_printf( "0x%08x:  %08x    %08x    %08x    %08x\r\n",
                (unsigned int)(&pu32_mem_loop[i*4+0]),
                pu32_mem_loop[i*4+0], pu32_mem_loop[i*4+1],
                pu32_mem_loop[i*4+2], pu32_mem_loop[i*4+3] );
        }
	}
}


int mb_excep_report_simple_info( mb_excep_info_st *p_info )
{
    int j;
    
	xil_printf("MSR: 0x%08lx.\r\n", p_info->u32_msr );
	mb_msr_report( p_info->u32_msr );

	xil_printf("EAR: 0x%08lx.\r\n", p_info->u32_ear );
	xil_printf("ESR: 0x%08lx.\r\n", p_info->u32_esr );
	mb_esr_report( p_info->u32_esr );

	xil_printf("FSR: 0x%08lx.\r\n", p_info->u32_fsr );
	mb_fsr_report( p_info->u32_fsr );

	xil_printf("EDR: 0x%08lx.\r\n", p_info->u32_edr );

	xil_printf("Exception instruction address: 0x%08lx.\r\n", p_info->u32_r17_instr_loc-4 );
	{
		unsigned int *pu32_instr;

		pu32_instr = (unsigned int *)(p_info->u32_r17_instr_loc-4);

		xil_printf( "Instruction at 0x%08x:  %08x    %08x    %08x    %08x\r\n",
			(unsigned int)pu32_instr,
			pu32_instr[0], pu32_instr[1],
			pu32_instr[2], pu32_instr[3] );
		xil_printf( "Instruction at 0x%08x:  %08x    %08x    %08x    %08x\r\n",
			(unsigned int)(&pu32_instr[4]),
			pu32_instr[4], pu32_instr[5],
			pu32_instr[6], pu32_instr[7] );
	}


	xil_printf("Stack pointer: 0x%08lx.\r\n", p_info->u32_r1_sp );

	xil_printf( "\r\nStack contents:\r\n");
	for( j=0; j<MB_STACK_INFO_U32_NUM/4; j++ )
	{
		xil_printf( "0x%08lx:  %08lx    %08lx    %08lx    %08lx\r\n",
			p_info->ui32_sp_backup_begin+j*16,
			p_info->u32_stack_content[j*4+0],
			p_info->u32_stack_content[j*4+1],
			p_info->u32_stack_content[j*4+2],
			p_info->u32_stack_content[j*4+3] );
	}

    return 0;
}


int mb_excep_report_info( char *ps_excep_name, unsigned int u32_counter, mb_excep_info_st *p_info )
{
    int i, j;
    
    xil_printf("%s exception inofrmation counter: %d.\r\n", ps_excep_name, u32_counter );
    for( i=0; (i<MB_EXCEP_INFO_NUM)&&(i<u32_counter); i++ )
    {
        xil_printf("No. %d %s exception inofrmation:\r\n", i, ps_excep_name);

        mb_excep_report_simple_info( p_info );
        
        xil_printf( "Call informaion:\r\n");
        for( j=0; j<MB_STACK_CALL_INFO_NUM; j++ )
        {
            if( 0 != p_info->call_info[j].u32_frame_ptr )
            {
                xil_printf( "Frame pointer:0x%08lx, Frame size:%ld, function entry:0x%08lx, return address:0x%08lx.\r\n",
                    p_info->call_info[j].u32_frame_ptr,
                    p_info->call_info[j].u32_frame_size,
                    p_info->call_info[j].u32_func_entry,
                    p_info->call_info[j].u32_return_address );
            }
        }

        p_info++;
            
    }

    return 0;
}


void mb_excep_unaligned_access_show( void )
{
    mb_excep_report_info( "Unaligned Exception", 
                g_ui_excep_unaligned_access_cnt, g_unaligned_excep_info);
}

void mb_excep_illegal_instr_show( void )
{
    mb_excep_report_info( "AXI Illegal Instruction Exception", 
                g_ui_excep_illegal_instr_cnt, g_illegal_instr_excep_info);
}


#ifdef  MB_EXCEP_AXI_I_ENABLE
void mb_excep_m_axi_i_show( void )
{
    mb_excep_report_info( "AXI Instruction Exception", 
                g_ui_excep_m_axi_i_cnt, g_axi_i_excep_info);
}
#endif    /*  MB_EXCEP_AXI_I_ENABLE */



void mb_excep_m_axi_d_show( void )
{
    mb_excep_report_info( "AXI Data Exception", 
                g_ui_excep_m_axi_d_cnt, g_axi_d_excep_info);
}

void mb_excep_div_zero_show( void )
{
    mb_excep_report_info( "Divide by Zero Exception", 
                g_ui_excep_div_cnt, g_div_excep_info);
}

void mb_excep_fpu_show( void )
{
    mb_excep_report_info( "FPU Exception", 
                g_ui_excep_fpu_cnt, g_fpu_excep_info);
}



#ifdef  MB_EXCEP_STACK_ENABLE
void mb_excep_stack_overflow_show( void ) 
{
    mb_excep_report_info( "Stack Overflow Exception", 
                g_ui_excep_stack_overflow_cnt, g_stack_overflow_excep_info);
}
#endif    /*  MB_EXCEP_STACK_ENABLE */


