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
* @file Mb_stack_unwind.c
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




#include "dbg_print.h"

#include "Mb_exception_handle.h"
#include "mb_exception_handle.h"
#include "mb_exception_trigger.h"
#include "mb_exception_report.h"



/************************** Constant Definitions *****************************/

//#define	MB_STACK_SEARCH_UPWARD_AXI_D_EXCP_TEST


/**************************** Type Definitions *******************************/

typedef struct  {
    char *name;
    char *ip;
    unsigned base;
    unsigned size;
}memory_range_st;



/***************** Macros (Inline Functions) Definitions *********************/


/************************** Variable Definitions *****************************/

memory_range_st memory_ranges[] = {

#if 1
    {
		"lmb_0",
		"lmb",
		0x0,
		0x00002000,
	},
#endif

#if 1
	{
		"axi_bram_0",
		"axi_bram",
		0xCC000000,
		0x00010000,
	},
#endif


#if 1
    {
        "ddr_0",
        "ddr",
        0xC0000000,
        0x08000000,
    },
#endif


};



/* They are defined in lscript.ld */
extern int __text_start[];
extern int __text_end[];
extern int _stack_end[];
extern int __stack[];





/************************** Function Prototypes ******************************/
extern void _start( void );
extern int main( void );




/************************** Function Implementation ******************************/



int mb_mem_addr_check( unsigned int ui_addr )
{
    int i;
    int i_mem_num;
    unsigned int ui_addr_limit;

    i_mem_num =  sizeof(memory_ranges)/sizeof(memory_range_st);

    for( i=0; i<i_mem_num; i++)
    {
        ui_addr_limit = memory_ranges[i].base + memory_ranges[i].base;
        if( ( ui_addr < memory_ranges[i].base )
            || ( ui_addr >= ui_addr_limit ) )
            {
                return -1;
            }
    }

    
    return 0;
}




int mb_pc_check( unsigned int ui_pc )
{
    if( 0 == (ui_pc&0x3) )
    {
        /*  The memory address of the vectors begins from 0.
                 The address range 0x28 to 0x4F is reserved for future software support by Xilinx.
                 Use 0x50 instead of 0x28 in the future. */
        if( ( (ui_pc > (unsigned int)__text_start )
                    &&  (ui_pc <= (unsigned int)__text_end ) )
            || ( (ui_pc > (unsigned int)_start )
                    &&  (ui_pc <= ( (unsigned int)_start ) + 0x28) )  )
        {
            return 0;
        }
    }

    return -1;
}

int mb_stack_check( unsigned int ui_sp )
{

	if( (ui_sp < (unsigned int)_stack_end )
		|| (ui_sp > (unsigned int)__stack ) 
		|| ( 0 != (ui_sp&0x3) ) )
	{
		return -1;
	}

   
    return 0;
}



/*
  The method is learned from MicroBlaze Linux.
*/

/*
* On Microblaze, finding the previous stack frame is a little tricky.
* At this writing (3/2010), Microblaze does not support CONFIG_FRAME_POINTERS,
* and even if it did, gcc (4.1.2) does not store the frame pointer at
* a consistent offset within each frame. To determine frame size, it is
* necessary to search for the assembly instruction that creates or reclaims
* the frame and extract the size from it.
*
* Microblaze stores the stack pointer in r1, and creates a frame via
*
*     addik r1, r1, -FRAME_SIZE
*
* The frame is reclaimed via
*
*     addik r1, r1, FRAME_SIZE
*
* Frame creation occurs at or near the top of a function.
* Depending on the compiler, reclaim may occur at the end, or before
* a mid-function return.
*
* A stack frame is usually not created in a leaf function.
*
*/

/**
 * mb_get_frame_size - Extract the stack adjustment from an
 *					"addik r1, r1, adjust" instruction
 * @instr : Microblaze instruction
 *
 * Return - Number of stack bytes the instruction reserves or reclaims
 */
unsigned long mb_get_frame_size(unsigned long instr)
{
		return abs((s16)(instr & 0xFFFF));
}


/**
 * This code is copied from MicroBlaze Linux Kernel
 * mb_find_frame_size - Search backward to find the instruction that creates
 *						 the stack frame (hopefully, for the same function the
 *						 initial PC is in).
 * @pc : Program counter at which to begin the search
 *
 * Return - PC at which stack frame creation occurs
 *			NULL if this cannot be found, i.e. a leaf function
 */
unsigned long mb_find_frame_size(unsigned int pc, unsigned int **pui_func_begin )
{
		int i;

		unsigned int *p_pc;
        
        // Check PC/SP in order to avoid illegal memory access
        // exception assembly code might not be in .text section ?? 
        if( ( 0 != (pc&0x3) )
            ||  (pc < (unsigned int)__text_start )
            ||  (pc >= (unsigned int)__text_end ) )
        {
            xil_printf("Invalid program counter: %p.\n\r", pc );
            return 0;
        }


		/* NOTE: Distance to search is arbitrary
		 *		 250 works well for most things,
		 *		 750 picks up things like tcp_recvmsg(),
		 *		1000 needed for fat_fill_super()
		 */
		for (i = 0; i < 1000; i++, pc-=4) {
				unsigned long instr;
				s16 frame_size;

				p_pc = (unsigned int *) pc;
				instr = *p_pc;
				//xil_printf("Instruction address: %p, instrcution: 0x%08lx\n\r", p_pc, instr );
				
				if(  0xBA0C0018 == instr )
				{
					/* Find breakpoint. 
					     Sometimes 0xBA0C0018 is replaced by the breakpoints.
					*/
					xil_printf("Find breakpoint at %p.\n\rMight fail to unwind stack.\n\r", p_pc);
					*pui_func_begin = p_pc;
					return 0;
				}		

				/* addik r1, r1, foo ? */
				if ((instr & 0xFFFF0000) != 0x30210000)
						continue;		/* No */

				frame_size = mb_get_frame_size(instr);
				if ((frame_size < 8) || (frame_size & 3)) {
						//dbg_print("Invalid frame size %d at 0x%p\n", frame_size, p_pc);
						return 0;
				}

				*pui_func_begin = p_pc;
				//dbg_print("Found frame creation at 0x%p, size %d\n", p_pc, frame_size);
				return frame_size;
		}

		return 0;
}



/*
 Stack is copied to another memory buffer.
 We have to  count the offset of the memory buffer.
*/
int	mb_unwind_stack
( 
	unsigned int u32_stack_pointer,  /*  */
	unsigned int u32_pc,  /* */
	unsigned int u32_stack_high_address,
	unsigned int u32_stack_low_address,
	mb_call_info_st   *p_call_info
)
{
	int i,j;
	int i_frame_valid_flag=0;
	unsigned int u32_frame_size;
	unsigned int *pu32_frame_size_pc;
	unsigned int u32_pc_try;
	unsigned int *pu32_frame_pointer_try;
	unsigned int *pu32_frame_pointer_found;
	mb_call_info_st   *p_call_info_run;


	/*
	.stack (NOLOAD) : {
	   _stack_end = .;
	   . += _STACK_SIZE;
	   . = ALIGN(8);
	   _stack = .;
	   __stack = _stack;
	} > mcb_ddr3_S0_AXI_BASEADDR

	
	_stack_end is lower than __stack

	u32_stack_high_address:  __stack, 
	u32_stack_low_address:   _stack_end

	*/
	dbg_print_var_hex( u32_stack_pointer );
	dbg_print_var_hex( u32_pc );
	dbg_print_var_hex( u32_stack_low_address );
	dbg_print_var_hex( u32_stack_high_address );
	
	if( (u32_stack_pointer < (unsigned int)u32_stack_low_address )
		|| (u32_stack_pointer > (unsigned int)u32_stack_high_address ) 
		|| ( 0 != ((unsigned int)u32_stack_pointer&0x3) ) )
	{
		print_var_hex( u32_stack_low_address );
		print_var_hex( u32_stack_high_address );
		xil_printf("Invalid stack pointer: 0x%08x\n\r", u32_stack_pointer );
		return -1;
	}
	dbg_print("\n\rAnalyze stack pointer: 0x%08x\n\r", u32_stack_pointer );

	/* 
	   Stack pointer always pointers to the return address
	   Sometimes stack pointer is corrupted. 
	   So try to find stack pointer by several check.  
	   Is it useful? 
	*/
	i_frame_valid_flag=0;
	for( i=1; i<500; i++ )
	{
		u32_pc_try = u32_pc;

#ifdef MB_STACK_SEARCH_UPWARD_AXI_D_EXCP_TEST
		// Might failed some times:: (u32_stack_pointer-0x100+i*4)
		// It might genearte axi data exception too.
		//pu32_frame_pointer_try = (unsigned int *)(u32_stack_pointer-0x100+i*4);
		pu32_frame_pointer_try = (unsigned int *)(u32_stack_pointer-0x200+i*4);
#else
		// Search stack from the middle to upward and downward ??
		if( 1 == (i%2) )
		{
			pu32_frame_pointer_try = (unsigned int *)(u32_stack_pointer+(i/2)*4);  /* Upward? */
		}
		else
		{
			pu32_frame_pointer_try = (unsigned int *)(u32_stack_pointer-(i/2)*4);  /* Downward? */
		}
#endif

		if(  (unsigned int)pu32_frame_pointer_try < u32_stack_low_address )
		{
			continue;
		}
		
		if(  (unsigned int)pu32_frame_pointer_try == u32_stack_pointer )
		{
			dbg_print("Try the original stack pointer: %p.\n\r", pu32_frame_pointer_try );
		}
		
		for( j=1; j<10; j++ )
		{
			dbg_print("\n\r\n\rNo. %d-%d stack check at %p\n\r", i, j, pu32_frame_pointer_try );
			if(  (unsigned int)pu32_frame_pointer_try >= u32_stack_high_address )
			{
				dbg_print("Stacker point is beyond upper limit.\n\r" );
				break;
			}
			
			u32_frame_size = mb_find_frame_size( u32_pc_try, &pu32_frame_size_pc );
			dbg_print("Frame size: 0x%08x, return address: 0x%08x, pu32_frame_size_pc: %p\n\r", 
					u32_frame_size, pu32_frame_pointer_try[0], pu32_frame_size_pc );
			if( (unsigned int)pu32_frame_size_pc == (unsigned int)main )
			{
				/* Find  main(). */
				i_frame_valid_flag=1;
				dbg_print("Find main()at %p(main:0x%08x).\n\r", pu32_frame_size_pc, (unsigned int)main);
				break;
			}
			if(  0 == u32_frame_size )
			{
				dbg_print("Invalid function prologue, no stack pointer modification code.\n\r");
				break;
			}		
			
			u32_pc_try = *pu32_frame_pointer_try;
			/* We check in instruction only */
			if( ( 0 != (u32_pc_try&0x3) )
			    ||  (u32_pc_try < (unsigned int)__text_start )
			    ||  (u32_pc_try >= (unsigned int)__text_end ) )
			{
				dbg_print("Invalid stack pointer: %p.\n\r", u32_pc_try );
				break;
			}

			pu32_frame_pointer_try = (unsigned int *)((unsigned int)pu32_frame_pointer_try+u32_frame_size);
			if( (  (unsigned int)pu32_frame_pointer_try < (unsigned int)u32_stack_low_address )
				|| (  (unsigned int)pu32_frame_pointer_try > (unsigned int)u32_stack_high_address )
				|| ( 0 != ((unsigned int)pu32_frame_pointer_try&0x3) ) )
			{
				dbg_print("Invalid stack pointer: %p.\n\r", pu32_frame_pointer_try );
				break;
			}
		}
		
		if( j>=10 )
		{
			/* 
			Ten tries all are correct. 
			We suppose that we find the correct stack.
			*/
			i_frame_valid_flag=1;
		}
		if( 0 != i_frame_valid_flag )
		{
			break;
		}
	}

	if( 0 == i_frame_valid_flag )
	{
		xil_printf("\n\rFailed to find valid stack.\n\r" );
		return 0;
	}

#ifdef MB_STACK_SEARCH_UPWARD_AXI_D_EXCP_TEST
	pu32_frame_pointer_found = (unsigned int *)(u32_stack_pointer-0x200+i*4);
#else	
	if( 1 == (i%2) )
	{
		pu32_frame_pointer_found = (unsigned int *)(u32_stack_pointer+(i/2)*4);  /* Upward? */
	}
	else
	{
		pu32_frame_pointer_found = (unsigned int *)(u32_stack_pointer-(i/2)*4);  /* Downward? */
	}
#endif
	
	dbg_print_var_hex( i_frame_valid_flag );
	dbg_print_var_hex( pu32_frame_pointer_try );
	dbg_print_var_hex( pu32_frame_pointer_found );
	dbg_print("\n\r" );
	
	u32_pc_try = u32_pc;
	pu32_frame_pointer_try = (unsigned int *)pu32_frame_pointer_found;
	p_call_info_run = p_call_info;
	for( i=0; i<200; i++ )
	{
//#ifdef  MB_EXCEP_DEBUG_SHOW
    	if( i<4 )
        {   
    		xil_printf("\n\rNo. %d Frame at %p\n\r", i, pu32_frame_pointer_try );
    		u32_frame_size = mb_find_frame_size( u32_pc_try, &pu32_frame_size_pc );
    		xil_printf("Frame size: 0x%08x, return address: 0x%08x, pu32_frame_size_pc: %p\n\r", 
    				u32_frame_size, pu32_frame_pointer_try[0], pu32_frame_size_pc );
        }
//#endif	/*  MB_EXCEP_DEBUG_SHOW */
    
		if( i<MB_STACK_CALL_INFO_NUM )
		{
			p_call_info_run->u32_frame_ptr = (unsigned int)pu32_frame_pointer_try;
			p_call_info_run->u32_frame_size = u32_frame_size;
			p_call_info_run->u32_func_entry= (unsigned int)pu32_frame_size_pc;
			p_call_info_run->u32_return_address = pu32_frame_pointer_try[0];
			p_call_info_run++;
		}

#ifdef  MB_EXCEP_DEBUG_SHOW
		/* For debug */
		mb_check_func_name_divide( (unsigned int)pu32_frame_size_pc, 0x20 );
		mb_check_func_name_mem( (unsigned int)pu32_frame_size_pc, 0x20 );
		mb_check_func_name_illegal_instr( (unsigned int)pu32_frame_size_pc, 0x20 );

#ifdef  MB_EXCEP_AXI_I_ENABLE
		mb_check_func_name_axi_i( (unsigned int)pu32_frame_size_pc, 0x20 );
#endif	/*  MB_EXCEP_AXI_I_ENABLE */

		mb_check_func_name_fp( (unsigned int)pu32_frame_size_pc, 0x20 );
		
#ifdef  MB_EXCEP_STACK_ENABLE
		mb_check_func_name_stack( (unsigned int)pu32_frame_size_pc, 0x20 );
#endif	/*  MB_EXCEP_STACK_ENABLE */

#endif /*  MB_EXCEP_DEBUG_SHOW  */	

		//mb_check_func_name_main( (unsigned int)pu32_frame_size_pc, 0x20 );
		if( (unsigned int)pu32_frame_size_pc == (unsigned int)main )
		{
			xil_printf("Find main()exactly at %p(main:0x%08x).\n\r", pu32_frame_size_pc, (unsigned int)main);
			break;
		}
		else if( (  (unsigned int)pu32_frame_size_pc-(unsigned int)main ) <0x20)
		{
			/* Find  main(). */
			xil_printf("Might find main()at %p(main:0x%08x).\n\r", pu32_frame_size_pc, (unsigned int)main);
			break;
		}
		if(  0 == u32_frame_size )
		{
			xil_printf("No more stack frame.\n\r");
			break;
		}		

		u32_pc_try = *pu32_frame_pointer_try;
        if( ( 0 != (u32_pc_try&0x3) )
            ||  (u32_pc_try < (unsigned int)__text_start )
            ||  (u32_pc_try >= (unsigned int)__text_end ) )
        {
            xil_printf("Invalid stack pointer: %p.\n\r", pu32_frame_pointer_try );
            break;
        }
        
		pu32_frame_pointer_try = (unsigned int *)((unsigned int)pu32_frame_pointer_try+u32_frame_size);
		if( (  (unsigned int)pu32_frame_pointer_try < (unsigned int)u32_stack_low_address )
			|| (  (unsigned int)pu32_frame_pointer_try > (unsigned int)u32_stack_high_address )
			|| ( 0 != ((unsigned int)pu32_frame_pointer_try&0x3) ) )
		{
			xil_printf("No more Frame.\n\r" );
			break;
		}

        
	}
	xil_printf("\n\r" );

	return 0;
}


