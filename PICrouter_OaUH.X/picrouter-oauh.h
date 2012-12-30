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
 * picrouter-oauh.h,v.0.90 2012/12/30
 */

#include <plib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "myDelay.h"

#include "HardwareProfile.h"
#include "usb_config.h"
#include "USB/usb.h"
#include "USB/usb_host_midi.h"

#include "TCPIP Stack/TCPIP.h"

//#define USE_PITCH
//#define USE_OPT_DRUM
//#define USE_LED_PAD_16
//#define USE_LED_PAD_64
//#define USE_LED_ENC
 
#include "button.h"
#include "analog.h"
#include "encoder.h"
#include "osc.h"

/** CONFIGURATION **************************************************/
#pragma config UPLLEN   = ON        // USB PLL Enabled
#pragma config FPLLMUL  = MUL_20    // PLL Multiplier
//test #pragma config FPLLMUL  = MUL_15    // PLL Multiplier
#pragma config UPLLIDIV = DIV_5     // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_5     // PLL Input Divider
#pragma config FPLLODIV = DIV_1     // PLL Output Divider
#pragma config FPBDIV   = DIV_1     // Peripheral Clock divisor
#pragma config FWDTEN   = OFF       // Watchdog Timer
//test #pragma config WDTPS    = PS1       // Watchdog Timer Postscale
//test #pragma config FCKSM    = CSDCMD    // Clock Switching & Fail Safe Clock Monitor
//test #pragma config OSCIOFNC = OFF       // CLKO Enable
#pragma config POSCMOD  = HS        // Primary Oscillator
//test #pragma config IESO     = OFF       // Internal/External Switch-over
#pragma config FSOSCEN  = OFF       // Secondary Oscillator Enable (KLO was off)
#pragma config FNOSC    = PRIPLL    // Oscillator Selection
#pragma config FVBUSONIO = OFF
//test #pragma config CP       = OFF       // Code Protect
//test #pragma config BWP      = OFF       // Boot Flash Write Protect
//test #pragma config PWP      = OFF       // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx1  // ICE/ICD Comm Channel Select
//tes #pragma config DEBUG    = ON        // Background Debugger Enable
#pragma config FMIIEN = OFF // external PHY in RMII/default configuration
#pragma config FETHIO = ON

/** DEFINITIONS ****************************************************/
#define NVM_DATA 0x9D07F000

//for USB_HOST
USB_AUDIO_MIDI_PACKET RxHostMidiDataBuffer;
USB_AUDIO_MIDI_PACKET TxHostMidiDataBuffer;

// Application specific
#define MIDI_USB_BUFFER_SIZE     (BYTE)4
#define MIDI_UART_BUFFER_SIZE    (BYTE)64
#define NUM_MIDI_PKTS_IN_USB_PKT (BYTE)1

// PWM
#define PWM_NUM 4

typedef enum
{
	STATE_INITIALIZE = 0,
	STATE_IDLE,
	STATE_READY,
	STATE_ERROR
} PROC_STATE;

typedef enum
{
    TX_DATA = 0,
    TX_DATA_WAIT,
    RX_DATA,
    RX_DATA_WAIT,
    TX_REAL_TIME_DATA,
    TX_REAL_TIME_DATA_WAIT
} TX_RX_STATE;

typedef struct
{
    TX_RX_STATE             TransferState;      // The transfer state of the endpoint
    BYTE                    numOfMIDIPackets;   // Each USB Packet sent from a device has the possibility of holding more than one MIDI packet,
                                                //  so this is to keep track of how many MIDI packets are within a USB packet (between 1 and 16, or 4 and 64 bytes)
    USB_AUDIO_MIDI_PACKET*  bufferStart;        // The 2D buffer for the endpoint. There are MIDI_USB_BUFFER_SIZE USB buffers that are filled with numOfMIDIPackets
                                                //  MIDI packets. This allows for MIDI_USB_BUFFER_SIZE USB packets to be saved, with a possibility of up to 
                                                //  numOfMIDIPackets MID packets within each USB packet.
    USB_AUDIO_MIDI_PACKET*  pBufReadLocation;   // Pointer to USB packet that is being read from
    USB_AUDIO_MIDI_PACKET*  pBufWriteLocation;  // Pointer to USB packet that is being written to
} ENDPOINT_BUFFER;


// *****************************************************************************
// Global Variables
// *****************************************************************************

void* deviceHandle;   // Handle to the attached device

PROC_STATE ProcState;
ENDPOINT_BUFFER* endpointBuffers;

// MIDI packet used to translate MIDI UART to MIDI USB, with flag
USB_AUDIO_MIDI_PACKET OSCTranslatedToUSB;

BYTE usbState = 0;

BOOL somethingToSend;

WORD NumGets;
WORD NumSends;

BYTE midiType = 0;
BYTE midiCh = 0;
BYTE midiNum = 0;
BYTE midiVal = 0;

//PWM
BOOL onTimer23 = FALSE;
BOOL onSquare[PWM_NUM];
LONG freq;
LONG width;
INT16 duty[PWM_NUM];

//Custom OSC Messages
char* prefix;
char msgLed[]   = "/led";
char msgPress[] = "/press";
char msgSw[]    = "/sw";
char msgAdc[]   = "/adc";
char zero[40];

APP_CONFIG AppConfig;
//BYTE myDHCPBindCount = 0xFF;

#define DEFAULT_HOST_NAME "PICrouter-OaUH"

UDP_SOCKET RxSocket;
UDP_SOCKET TxSocket;
BOOL initReceiveFlag = FALSE;
BOOL initSendFlag = FALSE;
char* hostName;
BYTE remoteIP[] = {192ul, 168ul, 1ul, 255ul};

WORD remotePort = 8000;
WORD localPort  = 8080;

DWORD dwLedData = 0;
DWORD dwLedSequence[100] = {0};
BYTE intensity[32] = {0};
BOOL ledOn = FALSE;
WORD ledCount = 0;
BYTE ledIntensity = 10;
BYTE ledIntensityIndex = 0;

WORD ledState = 0;
WORD matrixLed[4] = {0};
const WORD matrixLedData[4][4] = {{0x01, 0x03, 0x10, 0x12},
                                  {0x00, 0x02, 0x11, 0x13},
                                  {0x33, 0x31, 0x22, 0x21},
                                  {0x32, 0x30, 0x23, 0x20}};

BYTE currentState;
BYTE prevState = 1;
BOOL stateFlag = FALSE;

BYTE currentSwitch;
BYTE prevSwitch = 1;
BOOL switchFlag = FALSE;

BYTE midiValue = 0;
BYTE data0;
BYTE data1;
BYTE data2;
BOOL sendMidiFlag = FALSE;

//BYTE currentValue[USE_ADC_NUM];
//BYTE prevValue[2][USE_ADC_NUM];
WORD currentValue[USE_ADC_NUM];
//WORD prevValue[2][USE_ADC_NUM];

// MAC address Initialization
static ROM BYTE SerializedMACAddress[6] = {MY_DEFAULT_MAC_BYTE1, MY_DEFAULT_MAC_BYTE2, 
        MY_DEFAULT_MAC_BYTE3, MY_DEFAULT_MAC_BYTE4, MY_DEFAULT_MAC_BYTE5, MY_DEFAULT_MAC_BYTE6};

#define putcSPI4(data_out)  do{while(!SPI4STATbits.SPITBE); SPI4BUF=(data_out); }while(0)

void sendSpiOneWord(WORD msb, DWORD usec);
void sendSpiTwoWord(WORD msb, WORD lsb, DWORD usec);
void sendSpiFourWord(WORD msb0, WORD lsb0, WORD msb1, WORD lsb1, DWORD usec);

#ifdef USE_PITCH
  void setupPitch(void);
#endif
#ifdef USE_OPT_DRUM
  void setupOptDrum(void);
#endif
#ifdef USE_LED_PAD_16
  void setupLedPad16(void);
#endif
#ifdef USE_LED_PAD_64
  void setupLedPad64(void);
#endif
#ifdef USE_LED_ENC
  void setupLedEnc(void);
#endif

void UDPControlTask(void);
void UDPSendTask(void);

void sendPad(void);
void sendAdc(void);
void sendEnc(void);

// USB Host
//test void convertOscToMidi(BYTE buffer);
void convertMidiToOsc(void);
BOOL USB_ApplicationEventHandler(BYTE address, USB_EVENT event, void *data, DWORD size);
