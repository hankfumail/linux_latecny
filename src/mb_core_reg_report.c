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


#include "xil_types.h"
#include "xil_io.h"

#include "reg_report.h"
#include "dbg_print.h"


#include "xio.h"
#include "xuartns550_l.h"



/***************** Macros (Inline Functions) Definitions *********************/




/**************************** Type Definitions *******************************/



/************************** Constant Definitions *****************************/



/************************** Variable Definitions *****************************/



/************************** Function Prototypes ******************************/



/************************** Function Implementation ******************************/






/* In MB doc, bit 31 is the lowest bit, bit 0 is the highest bit */
void mb_core_msr_report( void )
{
	unsigned int u32_msr;

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
	
	/* Get information about exception */
	u32_msr = mfmsr( );
	dbg_print_var_hex(u32_msr);

    register2_report( u32_msr, "MicroBlaze Machine Status Register", mb_msr_reg_definition );
    printf("\n\r");

}



void mb_core_esr_report( void )
{

	unsigned int u32_esr;

	u32_esr = mfesr( );
	dbg_print_var_hex(u32_esr);

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
    printf("\n\r");

}



void mb_core_fsr_report( void )
{

	unsigned int u32_fsr;

	u32_fsr = mffsr( );
	dbg_print_var_hex(u32_fsr);

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
    printf("\n\r");

}




