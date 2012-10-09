/*********************************************************************
 *
 *                  PIC32 Boot Loader
 *
 *********************************************************************
 * FileName:        Bootloader.c
 * Dependencies:
 * Processor:       PIC32
 *
 * Compiler:        MPLAB C32
 *                  MPLAB IDE
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the �Company�) for its PIC32 Microcontroller is intended
 * and supplied to you, the Company�s customer, for use solely and
 * exclusively on Microchip PIC32 Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * $Id:  $
 * $Name: $
 *
 **********************************************************************/
#include "Include/GenericTypeDefs.h"
#include "Include/HardwareProfile/HardwareProfile.h"
#include "Include/BootLoader.h"
#include "Include/FrameWork/Framework.h"
#include <stdlib.h>
#include <plib.h>


// Configuring the Device Configuration Registers
// 80Mhz Core/Periph, Pri Osc w/PLL, Write protect Boot Flash
    #pragma config UPLLEN    = ON        // USB PLL Enabled
    #pragma config FPLLMUL   = MUL_20    // PLL Multiplier
    #pragma config UPLLIDIV  = DIV_5     // USB PLL Input Divider
    #pragma config FPLLIDIV  = DIV_5     // PLL Input Divider
    #pragma config FPLLODIV  = DIV_1     // PLL Output Divider
    #pragma config FPBDIV    = DIV_1     // Peripheral Clock divisor
    #pragma config FWDTEN    = OFF       // Watchdog Timer
    #pragma config POSCMOD   = HS        // Primary Oscillator
    #pragma config FSOSCEN   = OFF       // Secondary Oscillator Enable (KLO was off)
    #pragma config FNOSC     = PRIPLL    // Oscillator Selection
    #pragma config FVBUSONIO = OFF
    #pragma config ICESEL    = ICS_PGx1  // ICE/ICD Comm Channel Select
    #pragma config FMIIEN    = OFF // external PHY in RMII/default configuration
    #pragma config FETHIO    = ON
    #pragma config DEBUG     = OFF

#define SWITCH_PRESSED 0
#define PAD_PRESSED 1


BOOL CheckTrigger(void);
void JumpToApp(void);
BOOL ValidAppPresent(void);
/********************************************************************
* Function: 	main()
*
* Precondition: 
*
* Input: 		None.
*
* Output:		None.
*
* Side Effects:	None.
*
* Overview: 	Main entry function. If there is a trigger or 
*				if there is no valid application, the device 
*				stays in firmware upgrade mode.
*
*			
* Note:		 	None.
********************************************************************/
INT main(void)
{
	UINT pbClk;

	// Setup configuration
	pbClk = SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
	
	InitLED();

    TRISBbits.TRISB15 = 1;//test

    TRISFbits.TRISF5 = 0;
    TRISGbits.TRISG7 = 0;
    LATFbits.LATF5 = 0;
    LATGbits.LATG7 = 0;

    TRISFbits.TRISF5 = 1;
    TRISGbits.TRISG7 = 1;
	
	// Enter firmware upgrade mode if there is a trigger or if the application is not valid
	if(CheckTrigger() || !ValidAppPresent())
	{
		// Initialize the transport layer - UART/USB/Ethernet
		TRANS_LAYER_Init(pbClk);
		
		while(!FRAMEWORK_ExitFirmwareUpgradeMode()) // Be in loop till framework recieves "run application" command from PC
		{
			// Enter firmware upgrade mode.
			// Be in loop, looking for commands from PC
			TRANS_LAYER_Task(); // Run Transport layer tasks
			FRAMEWORK_FrameWorkTask(); // Run frame work related tasks (Handling Rx frame, process frame and so on)
			// Blink LED (Indicates the user that bootloader is running).
			BlinkLED();	
		}
		// Close trasnport layer.
		TRANS_LAYER_Close();
	}

	
	// No trigger + valid application = run application.
	JumpToApp();
	
	return 0;
}			


/********************************************************************
* Function: 	CheckTrigger()
*
* Precondition: 
*
* Input: 		None.
*
* Output:		TRUE: If triggered
				FALSE: No trigger
*
* Side Effects:	None.
*
* Overview: 	Checks if there is a trigger to enter 
				firmware upgrade mode.
*
*			
* Note:		 	None.
********************************************************************/
BOOL  CheckTrigger(void)
{
	if(ReadSwitchStatus() == SWITCH_PRESSED || (ReadExtSw0Status() == PAD_PRESSED && ReadExtSw1Status() == PAD_PRESSED))
	{
		// Switch is pressed
		return TRUE;		
	}	
	else
	{
		// Switch is not pressed.
		return FALSE;	
	}	
}	


/********************************************************************
* Function: 	JumpToApp()
*
* Precondition: 
*
* Input: 		None.
*
* Output:		
*
* Side Effects:	No return from here.
*
* Overview: 	Jumps to application.
*
*			
* Note:		 	None.
********************************************************************/
void JumpToApp(void)
{	
	void (*fptr)(void);
	fptr = (void (*)(void))USER_APP_RESET_ADDRESS;
	fptr();
}	


/********************************************************************
* Function: 	ValidAppPresent()
*
* Precondition: 
*
* Input: 		None.
*
* Output:		TRUE: If application is valid.
*
* Side Effects:	None.
*
* Overview: 	Logic: Check application vector has 
				some value other than "0xFFFFFFFF"
*
*			
* Note:		 	None.
********************************************************************/
BOOL ValidAppPresent(void)
{
	DWORD *AppPtr;
	AppPtr = (DWORD *)USER_APP_RESET_ADDRESS;
	if(*AppPtr == 0xFFFFFFFF)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}			


/*********************End of File************************************/


