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
* @file mb_exception_trigger.c
*
* This file is used to generate exception for MicroBlaze.
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



#include "mb_interface.h"

#include "dbg_print.h"

#include "mb_exception_handle.h"
#include "mb_exception_trigger.h"
#include "mb_exception_report.h"
#include "Mb_core_reg_report.h"



/************************** Constant Definitions *****************************/



/**************************** Type Definitions *******************************/



/***************** Macros (Inline Functions) Definitions *********************/




/************************** Variable Definitions *****************************/




/************************** Function Prototypes ******************************/

extern int main( void );




/************************** Function Implementation ******************************/





void mb_check_func_name_main
(
	unsigned int	ui_pc,
	unsigned int	ui_offset_limit 
)
{
	int i_pc_diff;

	i_pc_diff = (unsigned int)ui_pc-(unsigned int)main;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find main() at address 0x%08x.\n\r", ui_pc );
	}		
}



void mb_check_func_name_mem
(
	unsigned int	ui_pc,
	unsigned int	ui_offset_limit 
)
{
	int i_pc_diff;

	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_mem_access;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_mem_access() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_mem_call1;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_mem_call1() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_mem_call2;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_mem_call2() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_mem_call3;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_mem_call3() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_mem_call4;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_mem_call4() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_mem_call5;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_mem_call5() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_mem_call6;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_mem_call6() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_mem_call7;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_mem_call7() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_mem_call8;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_mem_call8() at address 0x%08x.\n\r", ui_pc );
	}		
}


void mb_excep_mem_access( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size )
{
	static unsigned int uiMemExecNum=0;

    unsigned int *pui_mem_access = (unsigned int *)pu32_mem_region;
    unsigned short *pus_mem_access = (unsigned short *)pu32_mem_region;
    unsigned char *puc_mem_access = (unsigned char *)pu32_mem_region;

	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_mem_access);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);

	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

    /*  
          Set very big safty bargin in order to test 'check_sp_safty' functionality
         */
    check_sp_safty(0x8000);

	uiMemExecNum++;
    if( 0 == u32_rw_flag )
    {
        xil_printf("\n\r\n\rWrite memory address: %p", pu32_mem_region);
        if( 0 == u32_size )
        {
            xil_printf(" in byte mode.\n\r");
            *puc_mem_access = uiMemExecNum;
        }
        else if( 1 == u32_rw_flag )
        {
            xil_printf(" in half-word mode.\n\r");
            *pus_mem_access = uiMemExecNum;
        }
        else
        {
            xil_printf(" in word mode.\n\r");
            *pui_mem_access = uiMemExecNum;
        }
    }
    else
    {
        xil_printf("\n\r\n\rRead memory address: %p", pu32_mem_region);
        if( 0 == u32_size )
        {
            xil_printf(" in byte mode.\n\r");
            uiMemExecNum = *puc_mem_access;
        }
        else if( 1 == u32_rw_flag )
        {
            xil_printf(" in half-word mode.\n\r");
            uiMemExecNum = *pus_mem_access;
        }
        else
        {
            xil_printf(" in word mode.\n\r");
            uiMemExecNum = *pui_mem_access;
        }
    }
	
}



void mb_excep_mem_call1( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size )
{

	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_mem_call1);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);

	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

    check_sp_safty(0x800);

	mb_excep_mem_access( pu32_mem_region, u32_rw_flag, u32_size );
}


void mb_excep_mem_call2( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_mem_call2);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

    check_sp_safty(0x800);
	
	mb_excep_mem_call1( pu32_mem_region, u32_rw_flag, u32_size );
}

void mb_excep_mem_call3( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_mem_call3);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

    check_sp_safty(0x800);
	
	mb_excep_mem_call2( pu32_mem_region, u32_rw_flag, u32_size );
}

void mb_excep_mem_call4( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_mem_call4);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

    check_sp_safty(0x800);
	
	mb_excep_mem_call3( pu32_mem_region, u32_rw_flag, u32_size );
}

void mb_excep_mem_call5( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_mem_call5);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

    check_sp_safty(0x800);
	
	mb_excep_mem_call4( pu32_mem_region, u32_rw_flag, u32_size );
}

void mb_excep_mem_call6( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_mem_call6);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

    check_sp_safty(0x800);
	
	mb_excep_mem_call5( pu32_mem_region, u32_rw_flag, u32_size );
}

void mb_excep_mem_call7( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_mem_call7);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

    check_sp_safty(0x800);
	
	mb_excep_mem_call6( pu32_mem_region, u32_rw_flag, u32_size );
}

void mb_excep_mem_call8( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_mem_call8);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

    check_sp_safty(0x800);

	mb_excep_mem_call7( pu32_mem_region, u32_rw_flag, u32_size );
}




void mb_check_func_name_illegal_instr
(
	unsigned int	ui_pc,
	unsigned int	ui_offset_limit 
)
{
	int i_pc_diff;

	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_illegal_instr_generate;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_illegal_instr_generate() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_illegal_instr_call1;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_illegal_instr_call1() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_illegal_instr_call2;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_illegal_instr_call2() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_illegal_instr_call3;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_illegal_instr_call3() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_illegal_instr_call4;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_illegal_instr_call4() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_illegal_instr_call5;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_illegal_instr_call5() at address 0x%08x.\n\r", ui_pc );
	}		
}


void mb_excep_illegal_instr_generate( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_illegal_instr_generate);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);

	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	put_illegal_instr();
}



void mb_excep_illegal_instr_call1( void )
{

	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_illegal_instr_call1);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);

	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	mb_excep_illegal_instr_generate(  );
}


void mb_excep_illegal_instr_call2( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_illegal_instr_call2);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	
	mb_excep_illegal_instr_call1( );
}

void mb_excep_illegal_instr_call3( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_illegal_instr_call3);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	
	mb_excep_illegal_instr_call2( );
}

void mb_excep_illegal_instr_call4( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_illegal_instr_call4);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	
	mb_excep_illegal_instr_call3( );
}

void mb_excep_illegal_instr_call5( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_illegal_instr_call5);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	
	mb_excep_illegal_instr_call4( );
}



#ifdef	MB_EXCEP_AXI_I_ENABLE

void mb_check_func_name_axi_i
(
	unsigned int	ui_pc,
	unsigned int	ui_offset_limit 
)
{
	int i_pc_diff;

	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_axi_i_fetch;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_axi_i_fetch() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_axi_i_call1;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_axi_i_call1() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_axi_i_call2;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_axi_i_call2() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_axi_i_call3;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_axi_i_call3() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_axi_i_call4;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_axi_i_call4() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_axi_i_call5;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_axi_i_call5() at address 0x%08x.\n\r", ui_pc );
	}		
}


void mb_excep_axi_i_fetch( unsigned int u32_instr_loc )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;
	mb_exception_axi_i_call  mb_exception_axi_i_call_illegal;

	dbg_print_func_begin;
	print_var_hex(mb_excep_axi_i_fetch);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	mb_exception_axi_i_call_illegal = (mb_exception_axi_i_call)u32_instr_loc;
	mb_exception_axi_i_call_illegal();
}


void mb_excep_axi_i_call1( unsigned int u32_instr_loc )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_axi_i_call1);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	mb_excep_axi_i_fetch( u32_instr_loc );
}



void mb_excep_axi_i_call2( unsigned int u32_instr_loc )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_axi_i_call2);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	mb_excep_axi_i_call1( u32_instr_loc );
}



void mb_excep_axi_i_call3( unsigned int u32_instr_loc )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_axi_i_call3);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);
	
	mb_excep_axi_i_call2( u32_instr_loc );
}



void mb_excep_axi_i_call4( unsigned int u32_instr_loc )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_axi_i_call4);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	mb_excep_axi_i_call3( u32_instr_loc );
}


void mb_excep_axi_i_call5( unsigned int u32_instr_loc )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_axi_i_call5);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	mb_excep_axi_i_call4( u32_instr_loc );
}

#endif	/*  MB_EXCEP_AXI_I_ENABLE */



void mb_check_func_name_divide
(
	unsigned int	ui_pc,
	unsigned int	ui_offset_limit 
)
{
	int i_pc_diff;

	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_divide_zero;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_divide_zero() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_divide_call1;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_divide_call1() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_divide_call2;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_divide_call2() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_divide_call3;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_divide_call3() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_divide_call4;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_divide_call4() at address 0x%08x.\n\r", ui_pc );
	}		
	
}




int mb_excep_divide_zero( void )
{
	static int i_divider, i_dividend, i_quotient; 
	unsigned int u32_r1_sp;

	dbg_print_func_begin;
	print_var_hex(mb_excep_divide_zero);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);

	i_divider = 100;
	i_dividend=0;
	i_quotient = i_divider/i_dividend;
	print_var_hex(i_quotient);

    return 0;
}


void mb_excep_divide_call1( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_divide_call1);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	mb_excep_divide_zero( );
}



void mb_excep_divide_call2( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_divide_call2);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);

	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);
	
	mb_excep_divide_call1( );
}



void mb_excep_divide_call3( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_divide_call3);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);
	
	mb_excep_divide_call2( );
}




void mb_excep_divide_call4( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_divide_call4);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);
	
	mb_excep_divide_call3( );
}


void mb_excep_divide_call5( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_divide_call5);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);
	
	mb_excep_divide_call4( );
}




void mb_check_func_name_fp
(
	unsigned int	ui_pc,
	unsigned int	ui_offset_limit 
)
{
	int i_pc_diff;

	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_fp;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_fp() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_fp_call1;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_fp_call1() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_fp_call2;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_fp_call2() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_fp_call3;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_fp_call3() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_fp_call4;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_fp_call4() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_fp_call5;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_fp_call5() at address 0x%08x.\n\r", ui_pc );
	}		
}


void mb_excep_fp( void )
{
	static float f_zero, f_quotient_zero, f_quotient_zero2;
	static float f_small, f_too_small, f_too_small2;
	static float f_big, f_too_big, f_too_big2;
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_fp);
    mb_core_fsr_report( );

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	/* SP float data range  1.175**(10^-38)   ~~  3.40282**(10^38)  */
	f_small =1e-37;
	f_too_small = f_small/10000;
    mb_core_fsr_report( );
    mtfsr(0);
	f_too_small2 = f_too_small*10000;
	print_var_hex(f_too_small2);
    mb_core_fsr_report( );
    mtfsr(0);

	/* SP float data range  1.175**(10^-38)   ~~  3.40282**(10^38)  */
	f_big =1e37;
	f_too_big = f_big*10000;
    mb_core_fsr_report( );
    mtfsr(0);
	f_too_big2 = f_too_big/10000;
	print_var_hex(f_too_big2);
    mb_core_fsr_report( );
    mtfsr(0);

	/* SP float Divide by zero.  */
    f_zero = 0;
    f_quotient_zero  = f_big/f_zero;
    mb_core_fsr_report( );
    mtfsr(0);
	f_quotient_zero2 = f_quotient_zero*10000;
	print_var_hex(f_quotient_zero2);
    mb_core_fsr_report( );
    mtfsr(0);
    
}


void mb_excep_fp_call1( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_fp_call1);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	mb_excep_fp( );
}



void mb_excep_fp_call2( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_fp_call2);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	mb_excep_fp_call1( );
}



void mb_excep_fp_call3( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_fp_call3);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);
	
	mb_excep_fp_call2( );
}



void mb_excep_fp_call4( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_fp_call4);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	mb_excep_fp_call3( );
}

void mb_excep_fp_call5( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_fp_call5);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	mb_excep_fp_call4( );
}


#ifdef	MB_EXCEP_STACK_ENABLE

void mb_check_func_name_stack
(
	unsigned int	ui_pc,
	unsigned int	ui_offset_limit 
)
{
	int i_pc_diff;

	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_stack_underflow;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_stack_underflow() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_stack_call1;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_stack_call1() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_stack_call2;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_stack_call2() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_stack_call3;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_stack_call3() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_stack_call4;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_stack_call4() at address 0x%08x.\n\r", ui_pc );
	}		
	i_pc_diff = (unsigned int)ui_pc-(unsigned int)mb_excep_stack_call5;
	if(  i_pc_diff < ui_offset_limit )
	{
		xil_printf("Might find mb_excep_stack_call5() at address 0x%08x.\n\r", ui_pc );
	}		
}


void mb_excep_stack_underflow( void )
{
	static float f_divider, f_dividend, f_quotient; 
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_stack_underflow);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	/* SP float data range  1.175**(10^-38)   ~~  3.40282**(10^38)  */
	f_divider =1e-37;
	f_dividend = 100;
	f_quotient = f_divider/f_dividend;
	print_var_hex(f_quotient);
}


void mb_excep_stack_call1( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_stack_call1);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	mb_excep_stack_underflow( );
}



void mb_excep_stack_call2( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_stack_call2);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	mb_excep_stack_call1( );
}



void mb_excep_stack_call3( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_stack_call3);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);
	
	mb_excep_stack_call2( );
}



void mb_excep_stack_call4( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_stack_call4);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	mb_excep_stack_call3( );
}

void mb_excep_stack_call5( void )
{
	unsigned int u32_r1_sp;
	unsigned int u32_r15_return_function;

	dbg_print_func_begin;
	print_var_hex(mb_excep_stack_call5);

	u32_r1_sp = mfgpr(r1);
	print_var_hex(u32_r1_sp);
	
	u32_r15_return_function = mfgpr(r15);
	print_var_hex(u32_r15_return_function);

	mb_excep_stack_call4( );
}

#endif	/*  MB_EXCEP_STACK_ENABLE  */


