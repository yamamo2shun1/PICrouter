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
 * picrouter.h,v.1.12.0 2014/02/24
 */

#define CURRENT_VERSION "1.11.0"

#include <plib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "TimeDelay.h"

#include "HardwareProfile.h"
#include "usb_callback.h"

#include "TCPIP Stack/TCPIP.h"
#include "TCPIP Stack/ZeroconfMulticastDNS.h"

#include "iosetting.h"
#include "button.h"
#include "analog.h"
#include "encoder.h"
#include "osc.h"

//#define USE_SPI_SRAM
#ifdef USE_SPI_SRAM
#include "sram.h"
#endif

//#define USE_LCD

// LED_PAD_8 or LED_PAD_16
//#define USE_LED_PAD

// RGB_PAD_8 or RGB_PAD_16
//#define USE_RGB_PAD

// LED_ENC(incremental type)
//#define USE_INC_ENC

// LED_ENC(absolute type)
//#define USE_ABS_ENC

/** CONFIGURATION **************************************************/
/*
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
*/

/** DEFINITIONS ****************************************************/
//#define NVM_PROGRAM_PAGE 0xbd006000
#define NVM_DATA 0x9D07F000
#define NVM_PAGE_SIZE    4096

/** VARIABLES ******************************************************/

//for USB_MIDI
USB_AUDIO_MIDI_EVENT_PACKET midiData;

// Host
typedef enum
{
    MODE_DEVICE,
    MODE_HOST,
    MODE_UNKNOWN
} DEVICE_MODE;

//static DEVICE_MODE device_mode = MODE_DEVICE;
static DEVICE_MODE device_mode = MODE_HOST;

// CDC(Host)
#if defined(USB_USE_CDC)
BYTE USB_CDC_IN_Data_Array[MAX_NO_OF_IN_BYTES];
BYTE USB_CDC_OUT_Data_Array[MAX_NO_OF_OUT_BYTES];

BOOL cdcSendFlag = FALSE;
WORD cdcReceiveInterval = 0;
BYTE cdcOutDataLength;
BYTE ErrorDriver;
BYTE NumOfBytesRcvd;
#endif

BOOL bUsbHostInitialized = FALSE;

// MIDI packet used to translate OSC to USB_MIDI(Host), with flag
USB_AUDIO_MIDI_PACKET OSCTranslatedToUSB;

//PWM
BOOL onTimer2 = FALSE;
BOOL onSquare[PWM_NUM];
LONG freq;
LONG width;
INT16 duty[PWM_NUM];

BYTE currentState;
BYTE prevState;
BOOL stateFlag = FALSE;
BOOL stateFlag2 = FALSE;//midi
DWORD dwLastIP = 0;

void initIOPorts(void);

void receiveOSCTask(void);

void USBControlTask(void);
void HIDControlTask(void);
void convertMidiToOsc(void);
void convertHidToOsc(void);
void sendNote(void);
void sendControlChange(void);
void receiveMIDIDatas(void);

#if defined(USB_USE_HID)
    void App_Detect_Device(void);
    void App_ProcessInputReport(void);
    BOOL USB_HID_DataCollectionHandler(void);
#elif defined(USB_USE_CDC)
    void USB_CDC_RxTxHandler(void);
#endif
