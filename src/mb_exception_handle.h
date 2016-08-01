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
* @file mb_exception_handle.h
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
* 1.00a    Hank     04/10/14       First release
* </pre>
*
******************************************************************************/
		 
#ifndef __MB_EXCEPTION_HANDLE_H_
#define __MB_EXCEPTION_HANDLE_H_

/***************************** Include Files *********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xil_exception.h"
#include "reg_report.h"
#include "dbg_print.h"

/************************** Constant Definitions *****************************/
//#define MB_EXCEP_DEBUG_SHOW

//#define MB_EXCEP_AXI_I_ENABLE
//#define MB_EXCEP_STACK_ENABLE


#define MB_STACK_INFO_U32_NUM	256
#define MB_EXCEP_INFO_NUM		4
#define MB_STACK_CALL_INFO_NUM	16


/**************************** Type Definitions *******************************/


typedef  struct  {
	u32	    u32_frame_ptr;
	u32	    u32_frame_size;
	u32  	u32_func_entry;
	u32	    u32_return_address;
}mb_call_info_st;


typedef  struct  {
	u32  	u32_msr;
	u32	    u32_ear;
	u32	    u32_esr;
	u32	    u32_fsr;
	u32	    u32_edr;
	u32	    u32_r17_instr_loc;
	u32	    u32_r1_sp;
	u32	    ui32_sp_backup_begin;
	u32 	u32_stack_content[MB_STACK_INFO_U32_NUM];
	mb_call_info_st  call_info[MB_STACK_CALL_INFO_NUM];
}mb_excep_info_st;



typedef void (*mb_exception_axi_i_call) (void );



/***************** Macros (Inline Functions) Definitions *********************/

#define check_sp_safty( ui_safty_margin ) 	{   \
        extern int _stack_end[]; \
		unsigned int 	u32_r1_sp_raw; \
		u32_r1_sp_raw = mfgpr(r1); \
		if( ( u32_r1_sp_raw - (unsigned int)_stack_end )<(ui_safty_margin) )  \
            xil_printf("Stack pointer: 0x%08x, Stack low limit: 0x%08x, stack is risky in function %s in file: %s", \
            u32_r1_sp_raw, (unsigned int)_stack_end, __func__, __FILE__ );  }

#define mfpc()         ({  unsigned int _rval;         \
                            __asm__ __volatile__ (      \
                                "mfs\t%0,rpc\n" : "=d"(_rval) \
                            );                          \
                            _rval;                      \
                        })
#define put_illegal_instr()         ({  __asm__ __volatile__ (	".long	0xfc000008" );})


/************************** Variable Definitions *****************************/


extern unsigned int g_ui_excep_unaligned_access_cnt;
extern unsigned int g_ui_excep_illegal_instr_cnt;
extern unsigned int g_ui_excep_m_axi_i_cnt;
extern unsigned int g_ui_excep_m_axi_d_cnt;
extern unsigned int g_ui_excep_div_cnt;
extern unsigned int g_ui_excep_fpu_cnt;
extern unsigned int g_ui_excep_stack_overflow_cnt;
extern unsigned int g_ui_excep_mmu_cnt;

extern mb_excep_info_st g_unaligned_excep_info[MB_EXCEP_INFO_NUM];
extern mb_excep_info_st g_illegal_instr_excep_info[MB_EXCEP_INFO_NUM];
extern mb_excep_info_st g_axi_i_excep_info[MB_EXCEP_INFO_NUM];
extern mb_excep_info_st g_axi_d_excep_info[MB_EXCEP_INFO_NUM];
extern mb_excep_info_st g_div_excep_info[MB_EXCEP_INFO_NUM];
extern mb_excep_info_st g_fpu_excep_info[MB_EXCEP_INFO_NUM];
extern mb_excep_info_st g_stack_overflow_excep_info[MB_EXCEP_INFO_NUM];


/************************** Function Prototypes ******************************/

void mb_excep_unaligned_access_handler();
void mb_excep_illegal_instr_handler();
void mb_excep_m_axi_i_handler();
void mb_excep_m_axi_d_handler();
void mb_excep_div_zero_handler();
void mb_excep_fpu_handler();
void mb_excep_stack_overflow_handler();


void mb_excep_info_handle( unsigned int u32_exception_num,
		mb_excep_info_st *p_excep_info,
		unsigned int u32_r17_pc, unsigned int u32_r1_sp  );

int mb_exception_init( void );


#endif 		/* __MB_EXCEPTION_HANDLE_H_  */	


