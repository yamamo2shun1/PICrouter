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
 * picrouter.h,v.1.15.1 2014/06/26
 */

#define CURRENT_VERSION "1.15.1"

#define _SUPPRESS_PLIB_WARNING 1

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

#if defined(USE_CC3000)
#include "cc3000.h"
#endif

//#define USE_SPI_SRAM
#ifdef USE_SPI_SRAM
#include "sram.h"
#endif

#if defined(USE_RN131) || defined(USE_LSD_BLE112)
#if 1//BRGH=1
  #define baud460800 42   //80000000 / (4 * 460800) - 1 = 460.8kbps@80MHz
  #define baud230400 86   //80000000 / (4 * 230400) - 1 = 230.4kbps@80MHz
  #define baud115200 173  //80000000 / (4 * 115200) - 1 = 115.2kbps@80MHz
  #define baud57600  346  //80000000 / (4 * 57600) - 1 = 57.6kbps@80MHz
  #define baud38400  519  //80000000 / (4 * 38400) - 1 = 38.4kbps@80MHz
  #define baud19200  1041 //80000000 / (4 * 19200) - 1 = 19.2kbps@80MHz
  #define baud9600   2082 //80000000 / (4 * 9600) - 1 = 9.6kbps@80MHz
  #define baud4800   4166 //80000000 / (4 * 4800) - 1 = 4.8kbps@80MHz
#else//BRGH=0
  #define baud460800 10   //80000000 / (16 * 460800) - 1 = 460.8kbps@80MHz
  #define baud230400 21   //80000000 / (16 * 230400) - 1 = 230.4kbps@80MHz
  #define baud115200 42   //80000000 / (16 * 115200) - 1 = 115.2kbps@80MHz
  #define baud57600  86   //80000000 / (16 * 57600) - 1 = 57.6kbps@80MHz
  #define baud38400  129  //80000000 / (16 * 38400) - 1 = 38.4kbps@80MHz
  #define baud19200  259  //80000000 / (16 * 19200) - 1 = 19.2kbps@80MHz
  #define baud9600   520  //80000000 / (16 * 9600) - 1 = 9.6kbps@80MHz
  #define baud4800   1040 //80000000 / (16 * 4800) - 1 = 4.8kbps@80MHz
#endif
  //#define configint UART_ERR_INT_DIS | UART_RX_INT_DIS | UART_TX_INT_DIS
  #define configint UART_ERR_INT_EN | UART_RX_INT_EN | UART_INT_PR6 | UART_INT_SUB_PR3 | UART_TX_INT_DIS
#endif

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

//static DEVICE_MODE device_mode = MODE_DEVICE;
static DEVICE_MODE device_mode = MODE_HOST;

#if defined(USB_USE_HID)
#define MINIMUM_POLL_INTERVAL           (0x0A)        // Minimum Polling rate for HID reports is 10ms

#define USAGE_PAGE_BUTTONS              (0x09)
#define USAGE_PAGE_GEN_DESKTOP          (0x01)

APP_STATE App_State_Mouse = DEVICE_NOT_CONNECTED;

HID_DATA_DETAILS Appl_Mouse_Buttons_Details;
HID_DATA_DETAILS Appl_XY_Axis_Details;

HID_REPORT_BUFFER  Appl_raw_report_buffer;

HID_USER_DATA_SIZE Appl_Button_report_buffer[3];
HID_USER_DATA_SIZE Appl_XY_report_buffer[3];

BYTE ErrorCounter;
BOOL ReportBufferUpdated;
#endif

// CDC(Host)
#if defined(USB_USE_CDC)
BYTE USB_CDC_IN_Data_Array[MAX_NO_OF_IN_BYTES];
BYTE USB_CDC_OUT_Data_Array[MAX_NO_OF_OUT_BYTES];

BOOL cdcSendFlag = FALSE;
WORD cdcReceiveInterval = 0;
BYTE cdcOutDataLength;
#endif

BYTE ErrorDriver;
BYTE NumOfBytesRcvd;

BOOL bUsbHostInitialized = FALSE;

// MIDI packet used to translate OSC to USB_MIDI(Host), with flag
#if defined(USB_USE_MIDI)
USB_AUDIO_MIDI_PACKET OSCTranslatedToUSB;
#endif

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
void receiveMIDIDatas(void);

#if defined(USB_USE_HID)
    void App_Detect_Device(void);
    void App_ProcessInputReport(void);
    BOOL USB_HID_DataCollectionHandler(void);
#elif defined(USB_USE_CDC)
    void USB_CDC_RxTxHandler(void);
#endif
