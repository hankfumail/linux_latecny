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
* @file mb_exception_report.h
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
				 
#ifndef __MB_EXCEPTION_REPORT_H_
#define __MB_EXCEPTION_REPORT_H_

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




void mb_elf_section_report( void );
void mb_msr_report( unsigned int u32_msr );
void mb_esr_report( unsigned int u32_esr );
void mb_fsr_report( unsigned int u32_fsr );
void mb_excep_handler_report( void );

int mb_excep_report_excep_counter( void );
void mb_mem_dump( u32 u32_mem_addr, u32 u32_num );
int mb_excep_report_simple_info( mb_excep_info_st *p_info );
int mb_excep_report_info( char *ps_excep_name, unsigned int u32_counter, mb_excep_info_st *p_info );


void mb_excep_unaligned_access_show( void );
void mb_excep_illegal_instr_show( void );
void mb_excep_m_axi_i_show( void );
void mb_excep_m_axi_d_show( void );
void mb_excep_div_zero_show( void );
void mb_excep_fpu_show( void );
void mb_excep_stack_overflow_show( void ); 





#endif 		/* __MB_EXCEPTION_REPORT_H_  */	

