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
 * picrouter.h,v.0.2 2012/07/29
 */

#include <plib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "myDelay.h"

#include "USB/usb.h"
#include "HardwareProfile.h"
#include "USB/usb_function_hid.h"
#include "USB/usb_function_midi.h"

#include "TCPIP Stack/TCPIP.h"

#include "button.h"
#include "analog.h"

/** CONFIGURATION **************************************************/
#pragma config UPLLEN   = ON        // USB PLL Enabled
#pragma config FPLLMUL  = MUL_20    // PLL Multiplier
#pragma config UPLLIDIV = DIV_5     // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_5     // PLL Input Divider
#pragma config FPLLODIV = DIV_1     // PLL Output Divider
#pragma config FPBDIV   = DIV_1     // Peripheral Clock divisor
#pragma config FWDTEN   = OFF       // Watchdog Timer
//#pragma config WDTPS    = PS1       // Watchdog Timer Postscale
//#pragma config FCKSM    = CSDCMD    // Clock Switching & Fail Safe Clock Monitor
//#pragma config OSCIOFNC = OFF       // CLKO Enable
#pragma config POSCMOD  = HS        // Primary Oscillator
//#pragma config IESO     = OFF       // Internal/External Switch-over
#pragma config FSOSCEN  = OFF       // Secondary Oscillator Enable (KLO was off)
#pragma config FNOSC    = PRIPLL    // Oscillator Selection
#pragma config FVBUSONIO = OFF
//#pragma config CP       = OFF       // Code Protect
//#pragma config BWP      = OFF       // Boot Flash Write Protect
//#pragma config PWP      = OFF       // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx1  // ICE/ICD Comm Channel Select
//#pragma config DEBUG    = ON        // Background Debugger Enable
#pragma config FMIIEN = OFF // external PHY in RMII/default configuration
#pragma config FETHIO = ON

/** DEFINITIONS ****************************************************/
#define NVM_PROGRAM_PAGE 0xbd006000
#define NVM_PAGE_SIZE    4096

/** VARIABLES ******************************************************/
unsigned int pagebuff[1024];

BOOL usbAttachFlag = FALSE;

//for USB_MIDI
USB_HANDLE MIDITxHandle = 0;
USB_HANDLE MIDIRxHandle  = 0;
unsigned char ReceivedHidDataBuffer[64];
unsigned char ToSendHidDataBuffer[64];

//for USB_HID
USB_HANDLE HIDTxHandle = 0;
USB_HANDLE HIDRxHandle = 0;
unsigned char ReceivedMidiDataBuffer[64];
unsigned char ToSendMidiDataBuffer[64];
USB_AUDIO_MIDI_EVENT_PACKET midiData;

USB_VOLATILE BYTE msCounter;


//osc messages
BYTE prefix[] = "/pic";
BYTE msgLed[] = "/led";
BYTE msgSw[]  = "/sw";
BYTE msgAdc[] = "/adc";
BYTE msgPwm[] = "/pwm";
BYTE zero[40];

APP_CONFIG AppConfig;

BYTE currentState;
BYTE prevState;
BYTE statePressCount = 0;
BYTE stateReleaseCount = 0;
BOOL stateFlag = FALSE;
BOOL stateFlag2 = FALSE;//midi

UDP_SOCKET RxSocket;
UDP_SOCKET TxSocket;
BOOL initReceiveFlag = FALSE;
BOOL initSendFlag = FALSE;
BYTE remoteIP[] = {192ul, 168ul, 1ul, 255ul};

WORD remotePort = 8000;
WORD localPort  = 8080;

// MAC address Initialization
static ROM BYTE SerializedMACAddress[6] = {MY_DEFAULT_MAC_BYTE1, MY_DEFAULT_MAC_BYTE2, MY_DEFAULT_MAC_BYTE3, MY_DEFAULT_MAC_BYTE4, MY_DEFAULT_MAC_BYTE5, MY_DEFAULT_MAC_BYTE6};

DWORD initialCount = 0;
BOOL oscSendFlag[2] = {FALSE, FALSE};
WORD currentAnalogValue[13];
WORD prevAnalogValue[13];

void setupPitch(void);

WORD getAnalogInput(BYTE port);
void UDPControlTask(void);
void UDPSendTask(void);

void USBControlTask(void);
void HIDControlTask(void);
//void MIDIControlTask(void);
void sendNote(void);
void sendControlChange(void);
void receiveMIDIDatas(void);