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
 * HardwareProfile.h,v.0.6 2013/02/15
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

    #define IO_OUT 0
    #define IO_IN  1

	//On Board Switch
	#define initSW() TRISDbits.TRISD10 = 1;
	#define SW_State() PORTDbits.RD10

	//On Board LED Settings
	#define initLEDs()     TRISDbits.TRISD8 = 0;\
    	                   TRISDbits.TRISD9 = 0;\
        	               LATDbits.LATD8 = 0;\
            	           LATDbits.LATD9 = 0;
	#define LED_1          LATDbits.LATD8
	#define LED_2          LATDbits.LATD9
	#define LED_1_On()     LED_1 = 1;
	#define LED_2_On()     LED_2 = 1;
	#define LED_1_Off()    LED_1 = 0;
	#define LED_2_Off()    LED_2 = 0;
	#define LED_1_Toggle() LED_1 = !LED_1;
	#define LED_2_Toggle() LED_2 = !LED_2;

	//AD Pins Settings
    #define ADC_AN0_IO(state)   TRISBbits.TRISB0  = state;
    #define ADC_AN1_IO(state)   TRISBbits.TRISB1  = state;
    #define ADC_AN2_IO(state)   TRISBbits.TRISB2  = state;
    #define ADC_AN3_IO(state)   TRISBbits.TRISB3  = state;
    #define ADC_AN4_IO(state)   TRISBbits.TRISB4  = state;
    #define ADC_AN5_IO(state)   TRISBbits.TRISB5  = state;
    #define ADC_AN6_IO(state)   TRISBbits.TRISB6  = state;
    #define ADC_AN7_IO(state)   TRISBbits.TRISB7  = state;
    #define ADC_AN8_IO(state)   TRISBbits.TRISB8  = state;
    #define ADC_AN9_IO(state)   TRISBbits.TRISB9  = state;
    #define ADC_AN10_IO(state)  TRISBbits.TRISB10 = state;
    #define ADC_AN11_IO(state)  TRISBbits.TRISB11 = state;
    #define ADC_AN12_IO(state)  TRISBbits.TRISB12 = state;
    #define ADC_AN13_IO(state)  TRISBbits.TRISB13 = state;

	#define ADC_AN0_OUT(state)  LATBbits.LATB0  = state;
	#define ADC_AN1_OUT(state)  LATBbits.LATB1  = state;
	#define ADC_AN2_OUT(state)  LATBbits.LATB2  = state;
	#define ADC_AN3_OUT(state)  LATBbits.LATB3  = state;
	#define ADC_AN4_OUT(state)  LATBbits.LATB4  = state;
	#define ADC_AN5_OUT(state)  LATBbits.LATB5  = state;
	#define ADC_AN6_OUT(state)  LATBbits.LATB6  = state;
	#define ADC_AN7_OUT(state)  LATBbits.LATB7  = state;
	#define ADC_AN8_OUT(state)  LATBbits.LATB8  = state;
	#define ADC_AN9_OUT(state)  LATBbits.LATB9  = state;
	#define ADC_AN10_OUT(state) LATBbits.LATB10 = state;
	#define ADC_AN11_OUT(state) LATBbits.LATB11 = state;
	#define ADC_AN12_OUT(state) LATBbits.LATB12 = state;
	#define ADC_AN13_OUT(state) LATBbits.LATB13 = state;

	#define ADC_AN0_IN()  PORTBbits.RB0
	#define ADC_AN1_IN()  PORTBbits.RB1
	#define ADC_AN2_IN()  PORTBbits.RB2
	#define ADC_AN3_IN()  PORTBbits.RB3
	#define ADC_AN4_IN()  PORTBbits.RB4
	#define ADC_AN5_IN()  PORTBbits.RB5
	#define ADC_AN6_IN()  PORTBbits.RB6
	#define ADC_AN7_IN()  PORTBbits.RB7
	#define ADC_AN8_IN()  PORTBbits.RB8
	#define ADC_AN9_IN()  PORTBbits.RB9
	#define ADC_AN10_IN() PORTBbits.RB10
	#define ADC_AN11_IN() PORTBbits.RB11
	#define ADC_AN12_IN() PORTBbits.RB12
	#define ADC_AN13_IN() PORTBbits.RB13

    // PWM
    #define PWM_OC1_IO(state) TRISDbits.TRISD0 = state;
    #define PWM_OC3_IO(state) TRISDbits.TRISD2 = state;
    #define PWM_OC4_IO(state) TRISDbits.TRISD3 = state;
    #define PWM_OC5_IO(state) TRISDbits.TRISD4 = state;

    #define PWM_OC1_OUT(state) LATDbits.LATD0 = state;
    #define PWM_OC3_OUT(state) LATDbits.LATD2 = state;
    #define PWM_OC4_OUT(state) LATDbits.LATD3 = state;
    #define PWM_OC5_OUT(state) LATDbits.LATD4 = state;

    #define PWM_OC1_IN() PORTDbits.RD0
    #define PWM_OC3_IN() PORTDbits.RD2
    #define PWM_OC4_IN() PORTDbits.RD3
    #define PWM_OC5_IN() PORTDbits.RD4

    // DIO
    #define D_PORT0_IO(state) TRISCbits.TRISC13 = state;
    #define D_PORT1_IO(state) TRISCbits.TRISC14 = state;
    #define D_PORT2_IO(state) TRISFbits.TRISF0  = state;
    #define D_PORT3_IO(state) TRISFbits.TRISF1  = state;

    #define D_PORT0_OUT(state) LATCbits.LATC13 = state;
    #define D_PORT1_OUT(state) LATCbits.LATC14 = state;
    #define D_PORT2_OUT(state) LATFbits.LATF0  = state;
    #define D_PORT3_OUT(state) LATFbits.LATF1  = state;

    #define D_PORT0_IN() PORTCbits.RC13
    #define D_PORT1_IN() PORTCbits.RC14
    #define D_PORT2_IN() PORTFbits.RF0
    #define D_PORT3_IN() PORTFbits.RF1

    // SPI4
    #define SPI_SCK4_IO(state) TRISBbits.TRISB14 = state;
    #define SPI_SDI4_IO(state) TRISFbits.TRISF4  = state;
    #define SPI_SDO4_IO(state) TRISFbits.TRISF5  = state;

    #define SPI_SCK4_OUT(state) LATBbits.LATB14 = state;
    #define SPI_SDI4_OUT(state) LATFbits.LATF4  = state;
    #define SPI_SDO4_OUT(state) LATFbits.LATF5  = state;

    #define SPI_SCK4_IN() PORTBbits.RB14
    #define SPI_SDI4_IN() PORTFbits.RF4
    #define SPI_SDO4_IN() PORTFbits.RF5

    // SPI2
    #define SPI_SCK2_IO(state) TRISGbits.TRISG6 = state;
    #define SPI_SDI2_IO(state) TRISGbits.TRISG7 = state;
    #define SPI_SDO2_IO(state) TRISGbits.TRISG8 = state;

    #define SPI_SCK2_OUT(state) LATGbits.LATG6 = state;
    #define SPI_SDI2_OUT(state) LATGbits.LATG7 = state;
    #define SPI_SDO2_OUT(state) LATGbits.LATG8 = state;

    #define SPI_SCK2_IN() PORTGbits.RG6
    #define SPI_SDI2_IN() PORTGbits.RG7
    #define SPI_SDO2_IN() PORTGbits.RG8

    #define SPI_2 0
    #define SPI_4 1
    #define putcSPI4(data_out)  do{while(!SPI4STATbits.SPITBE); SPI4BUF=(data_out); }while(0)

#endif  //HARDWARE_PROFILE_H
