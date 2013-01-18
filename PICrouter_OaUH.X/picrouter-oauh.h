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
 * picrouter-oauh.h,v.0.94 2013/01/18
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

#include "iosetting.h"
#include "button.h"
#include "analog.h"
#include "encoder.h"
#include "osc.h"

/** CONFIGURATION **************************************************/
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

/** DEFINITIONS ****************************************************/
#define NVM_DATA 0x9D07F000

//　for USB_HOST
// Application specific
#define MIDI_USB_BUFFER_SIZE     (BYTE)4
#define MIDI_UART_BUFFER_SIZE    (BYTE)64
#define NUM_MIDI_PKTS_IN_USB_PKT (BYTE)1

USB_AUDIO_MIDI_PACKET RxHostMidiDataBuffer;
USB_AUDIO_MIDI_PACKET TxHostMidiDataBuffer;

BOOL bUsbHostInitialized = FALSE;

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

BYTE midiType = 0;
BYTE midiCh = 0;
BYTE midiNum = 0;
BYTE midiVal = 0;

// PWM
#define PWM_NUM 4

BOOL onTimer23 = FALSE;
BOOL onSquare[PWM_NUM];
LONG freq;
LONG width;
INT16 duty[PWM_NUM];

void sendSpiOneWord(WORD msb, DWORD usec, BYTE spi_id);
void sendSpiTwoWord(WORD msb, WORD lsb, DWORD usec, BYTE spi_id);
void sendSpiFourWord(WORD msb0, WORD lsb0, WORD msb1, WORD lsb1, DWORD usec, BYTE spi_id);

void receiveOSCTask(void);
void sendOSCTask(void);

// USB Host
void convertMidiToOsc(void);
BOOL USB_ApplicationEventHandler(BYTE address, USB_EVENT event, void *data, DWORD size);
