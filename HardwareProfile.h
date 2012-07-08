/*
 * Copylight (C) 2012, Shunichi Yamamoto, tkrworks.net
 *
 * This file is part of PICrouter.
 *
 * PICrouter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option ) any later version.
 *
 * PICrouter is distributed in the hope that it will be useful,
 * but WITHIOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PICrouter. if not, see <http:/www.gnu.org/licenses/>.
 *
 * HardwareProfile.c,v.0.1 2012/07/08
 */

#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

	/*******************************************************************/
    /******** USB stack hardware selection options *********************/
    /*******************************************************************/
    //This section is the set of definitions required by the MCHPFSUSB
    //  framework.  These definitions tell the firmware what mode it is
    //  running in, and where it can find the results to some information
    //  that the stack needs.
    //These definitions are required by every application developed with
    //  this revision of the MCHPFSUSB framework.  Please review each
    //  option carefully and determine which options are desired/required
    //  for your application.

    #define USE_SELF_POWER_SENSE_IO
    #define tris_self_power     TRISDbits.TRISD5    // Input
    #define self_power          1

    #define USE_USB_BUS_SENSE_IO
    #define tris_usb_bus_sense  TRISGbits.TRISG9    // Input
    #define USB_BUS_SENSE       1


	// Clock frequency values
	// These directly influence timed events using the Tick module.  They also are used for UART and SPI baud rate generation.
	#define GetSystemClock()		(80000000ul)			// Hz
	#define GetInstructionClock()	(GetSystemClock()/1)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Might need changing if using Doze modes.
	#define GetPeripheralClock()	(GetSystemClock()/1)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Divisor may be different if using a PIC32 since it's configurable.

	// External National PHY configuration
	#define	PHY_RMII				// external PHY runs in RMII mode
	//#define	PHY_CONFIG_ALTERNATE	// alternate configuration used
	#define	PHY_ADDRESS			0x1	// the address of the National DP83848 PHY

	//State
	#define HIGH 1
	#define LOW  0

	//On Board Switch
	#define initSW() TRISDbits.TRISD10 = 1;
	#define SW_State() PORTDbits.RD10

	//On Board LED Settings
	#define initLEDs()     TRISDbits.TRISD8 = 0;\
    	                   TRISDbits.TRISD9 = 0;\
        	               LATDbits.LATD8 = 0;\
            	           LATDbits.LATD9 = 0;
	#define LED_1          LATDbits.LATD8
	#define LED_2          LATDbits. LATD9
	#define LED_1_On()     LED_1 = 1;
	#define LED_2_On()     LED_2 = 1;
	#define LED_1_Off()    LED_1 = 0;
	#define LED_2_Off()    LED_2 = 0;
	#define LED_1_Toggle() LED_1 = !LED_1;
	#define LED_2_Toggle() LED_2 = !LED_2;

	//AD Pins Settings
    #define AD_PORT0_IO(state)   TRISBbits.TRISB0   = state;
    #define AD_PORT1_IO(state)   TRISBbits.TRISB1   = state;
    #define AD_PORT2_IO(state)   TRISBbits.TRISB2   = state;
    #define AD_PORT3_IO(state)   TRISBbits.TRISB3   = state;
    #define AD_PORT4_IO(state)   TRISBbits.TRISB4   = state;
    #define AD_PORT5_IO(state)   TRISBbits.TRISB5   = state;
    #define AD_PORT6_IO(state)   TRISBbits.TRISB6   = state;
    #define AD_PORT7_IO(state)   TRISBbits.TRISB7   = state;
    #define AD_PORT8_IO(state)   TRISBbits.TRISB8   = state;
    #define AD_PORT9_IO(state)   TRISBbits.TRISB9   = state;
    #define AD_PORT10_IO(state)  TRISBbits.TRISB10  = state;
    #define AD_PORT11_IO(state)  TRISBbits.TRISB11  = state;
    #define AD_PORT12_IO(state)  TRISBbits.TRISB12  = state;
    #define AD_PORT13_IO(state)  TRISBbits.TRISB13  = state;

	#define AD_PORT0_OUT(state)  LATBbits.LATB0  = state;
	#define AD_PORT1_OUT(state)  LATBbits.LATB1  = state;
	#define AD_PORT2_OUT(state)  LATBbits.LATB2  = state;
	#define AD_PORT3_OUT(state)  LATBbits.LATB3  = state;
	#define AD_PORT4_OUT(state)  LATBbits.LATB4  = state;
	#define AD_PORT5_OUT(state)  LATBbits.LATB5  = state;
	#define AD_PORT6_OUT(state)  LATBbits.LATB6  = state;
	#define AD_PORT7_OUT(state)  LATBbits.LATB7  = state;
	#define AD_PORT8_OUT(state)  LATBbits.LATB8  = state;
	#define AD_PORT9_OUT(state)  LATBbits.LATB9  = state;
	#define AD_PORT10_OUT(state) LATBbits.LATB10 = state;
	#define AD_PORT11_OUT(state) LATBbits.LATB11 = state;
	#define AD_PORT12_OUT(state) LATBbits.LATB12 = state;
	#define AD_PORT13_OUT(state) LATBbits.LATB13 = state;

	#define AD_PORT0_IN  PORTBbits.RB0
	#define AD_PORT1_IN  PORTBbits.RB1
	#define AD_PORT2_IN  PORTBbits.RB2
	#define AD_PORT3_IN  PORTBbits.RB3
	#define AD_PORT4_IN  PORTBbits.RB4
	#define AD_PORT5_IN  PORTBbits.RB5
	#define AD_PORT6_IN  PORTBbits.RB6
	#define AD_PORT7_IN  PORTBbits.RB7
	#define AD_PORT8_IN  PORTBbits.RB8
	#define AD_PORT9_IN  PORTBbits.RB9
	#define AD_PORT10_IN PORTBbits.RB10
	#define AD_PORT11_IN PORTBbits.RB11
	#define AD_PORT12_IN PORTBbits.RB12
	#define AD_PORT13_IN PORTBbits.RB13

    #define BTN_PAD_00 PORTFbits.RF5
    #define BTN_PAD_01 PORTGbits.RG7
    #define BTN_PAD_10 PORTFbits.RF0
    #define BTN_PAD_11 PORTBbits.RB14
    #define BTN_LED_00(state) LATGbits.LATG6 = state;
    #define BTN_LED_01(state) LATFbits.LATF1 = state;
    #define BTN_LED_10(state) LATGbits.LATG8 = state;
    #define BTN_LED_11(state) LATFbits.LATF4 = state;

#endif  //HARDWARE_PROFILE_H
