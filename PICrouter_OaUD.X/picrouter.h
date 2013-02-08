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
 * picrouter.h,v.0.8 2013/02/08
 */

#include <plib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "myDelay.h"

#include "HardwareProfile.h"
#include "usb_config.h"
#include "USB/usb.h"
#include "USB/usb_function_hid.h"
#include "USB/usb_function_midi.h"

#include "TCPIP Stack/TCPIP.h"

#include "iosetting.h"
#include "button.h"
#include "analog.h"
#include "encoder.h"
#include "osc.h"

/** CONFIGURATION **************************************************/
#pragma config UPLLEN    = ON       // USB PLL Enabled
#pragma config FPLLMUL   = MUL_20   // PLL Multiplier
#pragma config UPLLIDIV  = DIV_5    // USB PLL Input Divider
#pragma config FPLLIDIV  = DIV_5    // PLL Input Divider
#pragma config FPLLODIV  = DIV_1    // PLL Output Divider
#pragma config FPBDIV    = DIV_1    // Peripheral Clock divisor
#pragma config FWDTEN    = OFF      // Watchdog Timer
#pragma config POSCMOD   = HS       // Primary Oscillator
#pragma config FSOSCEN   = OFF      // Secondary Oscillator Enable (KLO was off)
#pragma config FNOSC     = PRIPLL   // Oscillator Selection
#pragma config FVBUSONIO = OFF
#pragma config ICESEL    = ICS_PGx1 // ICE/ICD Comm Channel Select
#pragma config FMIIEN    = OFF      // external PHY in RMII/default configuration
#pragma config FETHIO    = ON

/** DEFINITIONS ****************************************************/
//#define NVM_PROGRAM_PAGE 0xbd006000
#define NVM_DATA 0x9D07F000
#define NVM_PAGE_SIZE    4096

// PWM
#define PWM_NUM 4

/** VARIABLES ******************************************************/

//for USB_MIDI
USB_HANDLE MIDITxHandle = NULL;
USB_HANDLE MIDIRxHandle  = NULL;
unsigned char ReceivedHidDataBuffer[64];
unsigned char ToSendHidDataBuffer[64];

//for USB_HID
USB_HANDLE HIDTxHandle = NULL;
USB_HANDLE HIDRxHandle = NULL;
unsigned char ReceivedMidiDataBuffer[64];
unsigned char ToSendMidiDataBuffer[64];
USB_AUDIO_MIDI_EVENT_PACKET midiData;

USB_VOLATILE BYTE msCounter;

//PWM
BOOL onTimer23 = FALSE;
BOOL onSquare[PWM_NUM];
LONG freq;
LONG width;
INT16 duty[PWM_NUM];

BYTE currentState;
BYTE prevState;
BOOL stateFlag = FALSE;
BOOL stateFlag2 = FALSE;//midi

void sendSpiOneWord(WORD msb, DWORD usec, BYTE spi_id);
void sendSpiTwoWord(WORD msb, WORD lsb, DWORD usec, BYTE spi_id);
void sendSpiFourWord(WORD msb0, WORD lsb0, WORD msb1, WORD lsb1, DWORD usec, BYTE spi_id);

void receiveOSCTask(void);
void sendOSCTask(void);

void USBControlTask(void);
void HIDControlTask(void);
void sendNote(void);
void sendControlChange(void);
void receiveMIDIDatas(void);