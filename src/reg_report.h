/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
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

     
#ifndef __REG_REPORT_H_
#define __REG_REPORT_H_


#include <stdio.h>
#include "platform.h"
#include "xil_types.h"




/***************** Macros (Inline Functions) Definitions *********************/

#define        REG_BITS_TYPE_SHOW_DEC        10        /* Print name and value */
#define        REG_BITS_TYPE_SHOW_HEX        11        /* Print name and value */
#define        REG_BITS_TYPE_SHOW_02HEX        11        /* Print name and value */
#define        REG_BITS_TYPE_SHOW_04HEX        11        /* Print name and value */
#define        REG_BITS_TYPE_SHOW_08HEX        11        /* Print name and value */
#define        REG_BITS_TYPE_CHECK        20        /* Check value, print information if it equals */






/**************************** Type Definitions *******************************/

/* 
    These two structures takes a lot of memory.
    It would be better to declare static or global variable for them.
    */
typedef struct{
    u32          type;
    u32          shift_right_num;
    u32          mask;
    u32            value;
    char         name[64];
    char        info[256-64-4-4-4-4];
}reg_defintion_st;

typedef  struct{
    u32          type;
    u32          a_shift_right_num;
    u32          a_mask;
    u32          b_shift_right_num;
    u32          b_mask;
    u32            value;
    char         name[64];
    char        info[256-64-4-4-4-4-4-4];
}reg_defintion2_st;

/*
reg_defintion2_st    data_fault_status_reg_definition[] =
{
    *    Type                               a_shift_num       a_mask    b_shift_num   b_mask         value   name       info          *
    {  REG_BITS_TYPE_CHECK,         0x1,         0x0,         0x6,         0x1,         0x0, "            ", "            " },
    {  REG_BITS_TYPE_CHECK,         0x1,         0x0,         0x6,         0x1,         0x0, "            ", "            " },
    {  0xffffffff,    0xffffffff,  0xffffffff,  0xffffffff,  0xffffffff,  0xffffffff, "", "" }
};
*/

/************************** Constant Definitions *****************************/


/************************** Variable Definitions *****************************/



/************************** Function Prototypes ******************************/

void register_report( u32 u32_reg_value, char *ps_reg_name, reg_defintion_st *p_reg_info  );
void register2_report( u32 u32_reg_value, char *ps_reg_name,  reg_defintion2_st *p_reg_info  );
void register_read_report( u32 u32_reg_addr, char *ps_reg_name, reg_defintion_st *p_reg_info  );



#endif /* __REG_REPORT_H_  */

