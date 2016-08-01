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
* @file Mb_exception_trigger.h
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
			 
#ifndef __MB_EXCEPTION_TRIGGER_H_
#define __MB_EXCEPTION_TRIGGER_H_


/***************************** Include Files *********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xil_exception.h"
#include "reg_report.h"
#include "dbg_print.h"


/************************** Constant Definitions *****************************/



/**************************** Type Definitions *******************************/



/***************** Macros (Inline Functions) Definitions *********************/



/************************** Variable Definitions *****************************/



/************************** Function Prototypes ******************************/




void mb_check_func_name_main(unsigned int ui_pc, unsigned int ui_offset_limit );

void mb_check_func_name_mem(unsigned int ui_pc, unsigned int ui_offset_limit );
void mb_excep_mem_access( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size );
void mb_excep_mem_call1( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size );
void mb_excep_mem_call2( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size );
void mb_excep_mem_call3( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size );
void mb_excep_mem_call4( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size );
void mb_excep_mem_call5( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size );
void mb_excep_mem_call6( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size );
void mb_excep_mem_call7( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size );
void mb_excep_mem_call8( unsigned int *pu32_mem_region, unsigned int u32_rw_flag, unsigned int u32_size );



void mb_check_func_name_illegal_instr(unsigned int ui_pc, unsigned int ui_offset_limit );
void mb_excep_illegal_instr_generate( void );
void mb_excep_illegal_instr_call1( void );
void mb_excep_illegal_instr_call2( void );
void mb_excep_illegal_instr_call3( void );
void mb_excep_illegal_instr_call4( void );
void mb_excep_illegal_instr_call5( void );

void mb_check_func_name_axi_i(unsigned int ui_pc, unsigned int ui_offset_limit );
void mb_excep_axi_i_fetch( unsigned int u32_instr_loc );
void mb_excep_axi_i_call1( unsigned int u32_instr_loc );
void mb_excep_axi_i_call2( unsigned int u32_instr_loc );
void mb_excep_axi_i_call3( unsigned int u32_instr_loc );
void mb_excep_axi_i_call4( unsigned int u32_instr_loc );
void mb_excep_axi_i_call5( unsigned int u32_instr_loc );


void mb_check_func_name_divide(unsigned int ui_pc, unsigned int ui_offset_limit );
int mb_excep_divide_zero( void );
void mb_excep_divide_call1( void );
void mb_excep_divide_call2( void );
void mb_excep_divide_call3( void );
void mb_excep_divide_call4( void );
void mb_excep_divide_call5( void );

void mb_check_func_name_fp(unsigned int ui_pc, unsigned int ui_offset_limit );
void mb_excep_fp( void );
void mb_excep_fp_call1( void );
void mb_excep_fp_call2( void );
void mb_excep_fp_call3( void );
void mb_excep_fp_call4( void );
void mb_excep_fp_call5( void );


void mb_check_func_name_stack(unsigned int ui_pc, unsigned int ui_offset_limit );
void mb_excep_stack_underflow( void );
void mb_excep_stack_call1( void );
void mb_excep_stack_call2( void );
void mb_excep_stack_call3( void );
void mb_excep_stack_call4( void );
void mb_excep_stack_call5( void );

#endif 		/* __MB_EXCEPTION_TRIGGER_H_  */	

