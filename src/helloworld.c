/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 *
 */

#include <stdio.h>
#include <stdlib.h>


#include "platform.h"

#include "xil_exception.h"
#include "xil_cache.h"

#include "xil_cache.h"
#include "microblaze_exceptions_i.h"
#include "microblaze_exceptions_g.h"

#include "dbg_print.h"

#include "mb_exception_handle.h"
#include "mb_exception_trigger.h"
#include "mb_exception_report.h"




int main()
{
    init_platform();

    printf("Hello exception\n\r");

	mb_elf_section_report( );

#if 0	
	mb_exception_init( );
	
	mb_excep_report_excep_counter( );
	
	mb_excep_mem_call8( (unsigned int *)0x10000001, 0, 1 );   /* Unaligned, exception generated, OK */
	mb_excep_unaligned_access_show( );
	mb_excep_mem_call8( (unsigned int *)0x10000001, 0, 2 );   /* Unaligned, exception generated, OK */
	mb_excep_unaligned_access_show( );
	
	mb_excep_mem_call8( (unsigned int *)0x10000001, 1, 1 );   /* Unaligned, exception generated, OK */
	mb_excep_unaligned_access_show( );
	mb_excep_mem_call8( (unsigned int *)0x10000001, 1, 2 );   /* Unaligned, exception generated, OK */
	mb_excep_unaligned_access_show( );

	
	mb_excep_illegal_instr_call5( );   /* Illgeal instruction, exception generated, OK */
	mb_excep_illegal_instr_show( );

#ifdef	MB_EXCEP_AXI_I_ENABLE
	/* No OK */
	//mb_excep_axi_i_call4( 0xb0000000 );    /* AXI instruction exception, unused address, no exception */
	mb_excep_axi_i_call4( 0xfffffff8 );    /* System hangs during debugging. */
	mb_excep_m_axi_i_show( );
#endif	/*  MB_EXCEP_AXI_I_ENABLE */

	/* 0xb0000000 and 0xfffffff8 both are unused address. 
	     Why the AXI data exception is generated only for 0xfffffff8
	*/
	mb_excep_mem_call8( (unsigned int *)0xfffffff8, 0, 2 );  /* AXI data exception, exception generated, OK. */
	//mb_excep_mem_call8( (unsigned int *)0xb0000000 );  /* AXI data exception, unused address, no exception */
	mb_excep_m_axi_d_show( );
#endif

	mb_excep_divide_call5( );   /* Divide by zero, exception generated, OK */
	mb_excep_div_zero_show( );


	mb_excep_fp_call5( );    /* FP, exception generated, OK */
	mb_excep_fpu_show( );


#ifdef	MB_EXCEP_STACK_ENABLE
	/* Stack, default assembler exception handler does not call C handler now. */
	mb_excep_stack_call5( );   /* Stack, does not call handler now. */
	mb_excep_stack_overflow_show( );
#endif	/*  MB_EXCEP_STACK_ENABLE  */

	mb_excep_report_excep_counter( );
	
	printf("Exception completed.\n\r");

    return 0;
}


