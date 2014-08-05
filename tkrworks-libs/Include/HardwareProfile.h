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
 * HardwareProfile.h,v.0.8 2014/02/25
 */

#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

/*******************************************************************/
/******** USB stack hardware selection options *********************/
/*******************************************************************/
// This section is the set of definitions required by the MCHPFSUSB
// framework.  These definitions tell the firmware what mode it is
// running in, and where it can find the results to some information
//  that the stack needs.
// These definitions are required by every application developed with
// this revision of the MCHPFSUSB framework.  Please review each
// option carefully and determine which options are desired/required
// for your application.

#define USE_SELF_POWER_SENSE_IO
#define tris_self_power TRISDbits.TRISD5    // Input
#define self_power      1

#define USE_USB_BUS_SENSE_IO
#define tris_usb_bus_sense TRISGbits.TRISG9 // Input
#define USB_BUS_SENSE      1


// Clock frequency values
// These directly influence timed events using the Tick module.
// They also are used for UART and SPI baud rate generation.
#define GetSystemClock()      (80000000ul)         // Hz
#define GetInstructionClock() (GetSystemClock()/1) // Normally GetSystemClock()/1 for PIC32.
                                                   // Might need changing if using Doze modes.
#define GetPeripheralClock()  (GetSystemClock()/1) // Normally GetSystemClock()/1 for PIC32.
                                                   // Divisor may be different if using a PIC32
                                                   // since it's configurable.

// using the PIC32 internal MAC interface
//#define TCPIP_IF_PIC32INT

// External National PHY configuration
#define PHY_RMII // external PHY runs in RMII mode
//#define PHY_CONFIG_ALTERNATE // alternate configuration used
#define PHY_ADDRESS 0x1 // the address of the National DP83848 PHY


//#define NVM_PROGRAM_PAGE 0xbd006000
#define NVM_DATA 0x9D07F000
#define NVM_PAGE_SIZE 4096

//#define USE_PITCH

//#define USE_LCD

// LED_PAD_8
//#define USE_LED_PAD_8

// RGB_PAD_16
//#define USE_RGB_PAD_16

// RGB_PAD_8L
//#define USE_RGB_PAD_8L

// LED_ENC(incremental type)
//#define USE_INC_ENC

// LED_ENC(absolute type)
//#define USE_ABS_ENC

// Microchip(Roving Networks) Wi-Fi module
//#define USE_RN131

// LSD-BLE112 Bluetooth Low Energy module
//#define USE_LSD_BLE112

// TI CC3000 Wi-Fi module
//#define USE_CC3000

// DAC+ADC_SHIELD
#define USE_DAC_PLUS_ADC

// Analog Devices AD7490(ADC)
#define USE_AD7490

// Analog Devices AD5328(DAC)
#define USE_AD5328

// Analog Devices ADG1414
#define USE_ADG1414

// Analog Devices ADG1433
#define USE_ADG1433

#endif  //HARDWARE_PROFILE_H
