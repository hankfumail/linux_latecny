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


#include <stdio.h>

//#include "sleep.h"

#include "platform.h"

#include "xil_types.h"
#include "xil_io.h"

#include "reg_report.h"
#include "dbg_print.h"

/***************** Macros (Inline Functions) Definitions *********************/




/**************************** Type Definitions *******************************/



/************************** Constant Definitions *****************************/



/************************** Variable Definitions *****************************/



/************************** Function Prototypes ******************************/



/************************** Function Implementation ******************************/










/****************************************************************************
*
* Read and report Auxiliary Control Register.
*
* @param    None.
*
* @return    None.
*
* @note        None.
*
****************************************************************************/
void register_report( u32 u32_reg_value, char *ps_reg_name, reg_defintion_st *p_reg_info  )
{
    int        i;
    u32     u32_temp;

    for( i=0; i<1000; i++)
    {
        if( ( 0xffffffff == p_reg_info[i].type ) 
            &&( 0xffffffff == p_reg_info[i].mask ) 
            && ( 0xffffffff == p_reg_info[i].value ) )
        {
            break;
        }

        //u32_temp = (u32_reg_value>>p_reg_info[i].shift_right_num)&u32_reg_value&p_reg_info[i].mask;
        u32_temp = (u32_reg_value>>p_reg_info[i].shift_right_num)&p_reg_info[i].mask;
        if( REG_BITS_TYPE_SHOW_DEC == p_reg_info[i].type )
        {
            printf("%s register %s: %ld.\n\r", ps_reg_name, p_reg_info[i].name, u32_temp);
        }
        else if( REG_BITS_TYPE_SHOW_HEX == p_reg_info[i].type )
        {
            printf("%s register %s: 0x%lx.\n\r", ps_reg_name, p_reg_info[i].name, u32_temp);
        }
        else if( REG_BITS_TYPE_SHOW_02HEX == p_reg_info[i].type )
        {
            printf("%s register %s: 0x%02lx.\n\r", ps_reg_name, p_reg_info[i].name, u32_temp);
        }
        else if( REG_BITS_TYPE_SHOW_04HEX == p_reg_info[i].type )
        {
            printf("%s register %s: 0x%04lx.\n\r", ps_reg_name, p_reg_info[i].name, u32_temp);
        }
        else if( REG_BITS_TYPE_SHOW_08HEX == p_reg_info[i].type )
        {
            printf("%s register %s: 0x%08lx.\n\r", ps_reg_name, p_reg_info[i].name, u32_temp);
        }
        else if( REG_BITS_TYPE_CHECK == p_reg_info[i].type )
        {
            if( u32_temp == p_reg_info[i].value )
            {
                printf("%s register %s: %s.\n\r", ps_reg_name, p_reg_info[i].name, p_reg_info[i].info );
            }
        }
    
    }


}



/****************************************************************************
*
* Read and report Auxiliary Control Register.
*
* @param    None.
*
* @return    None.
*
* @note        None.
*
****************************************************************************/
void register2_report( u32 u32_reg_value, char *ps_reg_name, reg_defintion2_st *p_reg_info  )
{
    int        i;
    u32     u32_temp_b;
    u32     u32_temp;
    reg_defintion2_st *p_reg_info_run;

    /* SDK 14.7 debugger does not show p_reg_info[i]'s value */
    p_reg_info_run = p_reg_info;
    for( i=0; i<1000; i++, p_reg_info_run++ )
    {
        if( ( 0xffffffff == p_reg_info_run->type ) 
            &&( 0xffffffff == p_reg_info_run->a_mask ) 
            && ( 0xffffffff == p_reg_info_run->value ) )
        {
            break;
        }

        u32_temp = (u32_reg_value>>p_reg_info_run->a_shift_right_num)&p_reg_info_run->a_mask;
        u32_temp_b = (u32_reg_value>>p_reg_info_run->b_shift_right_num)&p_reg_info_run->b_mask;
        u32_temp |=u32_temp_b;
        if( REG_BITS_TYPE_SHOW_DEC == p_reg_info_run->type )
        {
            printf("%s register %s: %ld.\n\r", ps_reg_name, p_reg_info_run->name, u32_temp);
        }
        else if( REG_BITS_TYPE_SHOW_HEX == p_reg_info_run->type )
        {
            printf("%s register %s: 0x%lx.\n\r", ps_reg_name, p_reg_info_run->name, u32_temp);
        }
        else if( REG_BITS_TYPE_SHOW_02HEX == p_reg_info_run->type )
        {
            printf("%s register %s: 0x%02lx.\n\r", ps_reg_name, p_reg_info_run->name, u32_temp);
        }
        else if( REG_BITS_TYPE_SHOW_04HEX == p_reg_info_run->type )
        {
            printf("%s register %s: 0x%04lx.\n\r", ps_reg_name, p_reg_info_run->name, u32_temp);
        }
        else if( REG_BITS_TYPE_SHOW_08HEX == p_reg_info_run->type )
        {
            printf("%s register %s: 0x%08lx.\n\r", ps_reg_name, p_reg_info_run->name, u32_temp);
        }
        else if( REG_BITS_TYPE_CHECK == p_reg_info_run->type )
        {
            if( u32_temp == p_reg_info_run->value )
            {
                printf("%s register %s: %s.\n\r", ps_reg_name, p_reg_info_run->name, p_reg_info_run->info );
            }
        }
        
    
    }


}

/* 
    Sometimes we only print one file if the other field of  this register has a specific value
    Example: MicroBlaze Exception Status Register (ESR): Source/Destination Register
    It should be printed when the exception is Unaligned Data Access
    So we need expand the structure 
*/


void register_read_report( u32 u32_reg_addr, char *ps_reg_name, reg_defintion_st *p_reg_info  )
{
    u32 u32_reg_value;

    u32_reg_value = Xil_In32( u32_reg_addr );
    register_report( u32_reg_value, ps_reg_name, p_reg_info  );
}



