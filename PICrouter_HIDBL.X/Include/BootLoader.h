// Copyright (c) 2002-2010,  Microchip Technology Inc.
//
// Microchip licenses this software to you solely for use with Microchip
// products.  The software is owned by Microchip and its licensors, and
// is protected under applicable copyright laws.  All rights reserved.
//
// SOFTWARE IS PROVIDED "AS IS."  MICROCHIP EXPRESSLY DISCLAIMS ANY
// WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  IN NO EVENT SHALL
// MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
// CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR
// EQUIPMENT, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY
// OR SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED
// TO ANY DEFENSE THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION,
// OR OTHER SIMILAR COSTS.
//
// To the fullest extent allowed by law, Microchip and its licensors
// liability shall not exceed the amount of fees, if any, that you
// have paid directly to Microchip to use this software.
//
// MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE
// OF THESE TERMS.
#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

#define PROGRAM_FLASH_END_ADRESS (0x9D000000+BMXPFMSZ-1)
#define SYS_FREQ (80000000L)

//-------User configurable macros begin---------
#define MAJOR_VERSION 1
#define MINOR_VERSION 0


/* APP_FLASH_BASE_ADDRESS and APP_FLASH_END_ADDRESS reserves program Flash for the application*/ 
/* Rule: 
 		1)The memory regions kseg0_program_mem, kseg0_boot_mem, exception_mem and 
 		kseg1_boot_mem of the application linker script must fall with in APP_FLASH_BASE_ADDRESS 
 		and APP_FLASH_END_ADDRESS
 		 
 		2)The base address and end address must align on  4K address boundary */
 		
#define APP_FLASH_BASE_ADDRESS 	0x9D006000
#define APP_FLASH_END_ADDRESS   PROGRAM_FLASH_END_ADRESS

/* Address of  the Flash from where the application starts executing */
/* Rule: Set APP_FLASH_BASE_ADDRESS to _RESET_ADDR value of application linker script*/
#define USER_APP_RESET_ADDRESS 	(0x9D006000 + 0x1000 + 0x970)


//-------User configurable macros end-----------

	#include "Include/Usb/Usb_Tasks.h"

#endif
