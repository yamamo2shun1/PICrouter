/*
 * Copylight (C) 2013, Shunichi Yamamoto, tkrworks.net
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
 * picrouter.h,v.0.1.0 2013/11/13
 */

#ifndef PICROUTER_H
#define	PICROUTER_H

#ifdef	__cplusplus
extern "C" {
#endif

#define CURRENT_VERSION "0.1.0"

#include <plib.h>
#include <stdlib.h>

#include "HardwareProfile.h"

#include "TCPIP Stack/ZeroconfMulticastDNS.h"

#include "TimeDelay.h"
#include "iosetting.h"
#include "analog.h"
#include "osc.h"

// USB Libraries
#include "usb_config.h"
#include "USB/usb.h"
#include "USB/usb_function_hid.h"
#include "USB/usb_function_midi.h"
#include "USB/usb_host_midi.h"
#if defined(USB_USE_CDC) // cdc host
    #include "USB/usb_host_cdc.h"
    #include "USB/usb_host_cdc_interface.h"
#endif

#include "mrbconf.h"
#include "mruby.h"

#include "mruby/irep.h"
#include "mruby/string.h"
#include "mruby/value.h"


/** CONFIGURATIONS **************************************************/
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

/** VARIABLES ******************************************************/

// for mruby
mrb_state* mrb;

// for USB_MIDI
USB_HANDLE MIDITxHandle = NULL;
USB_HANDLE MIDIRxHandle  = NULL;
unsigned char ReceivedMidiDataBuffer[64];
unsigned char ToSendMidiDataBuffer[64];
USB_AUDIO_MIDI_EVENT_PACKET midiData;

// for USB_HID
USB_HANDLE HIDTxHandle = NULL;
USB_HANDLE HIDRxHandle = NULL;
unsigned char ReceivedHidDataBuffer[64];
unsigned char ToSendHidDataBuffer[64];

// Host
typedef enum
{
    MODE_DEVICE,
    MODE_HOST,
    MODE_UNKNOWN
} DEVICE_MODE;

static DEVICE_MODE device_mode = MODE_DEVICE;
//static DEVICE_MODE device_mode = MODE_HOST;

#if defined(USB_USE_CDC)
    typedef enum _APPL_STATE
    {
        DEVICE_NOT_CONNECTED,
        DEVICE_CONNECTED, /* Device Enumerated  - Report Descriptor Parsed */
        READY_TO_TX_RX,
        GET_IN_DATA,
        GET_IN_DATA_WAIT,
        SEND_OUT_DATA,
        SEND_OUT_DATA_WAIT,
        ERROR_REPORTED      /* need to add application states for data interface */
    } APPL_STATE;

    #define MAX_NO_OF_IN_BYTES  64
    #define MAX_NO_OF_OUT_BYTES 64

    volatile APPL_STATE  APPL_CDC_State;

    BYTE USB_CDC_IN_Data_Array[MAX_NO_OF_IN_BYTES];
    BYTE USB_CDC_OUT_Data_Array[MAX_NO_OF_OUT_BYTES];

    BOOL cdcSendFlag = FALSE;
    WORD cdcReceiveInterval = 0;
    BYTE cdcOutDataLength;
    BYTE ErrorDriver;
    BYTE NumOfBytesRcvd;
#endif


#define MIDI_USB_BUFFER_SIZE     (BYTE)4
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
    TX_RX_STATE            TransferState;     // The transfer state of the endpoint
    BYTE                   numOfMIDIPackets;  // Each USB Packet sent from a device has the possibility of holding more than one MIDI packet,
                                              //  so this is to keep track of how many MIDI packets are within a USB packet (between 1 and 16, or 4 and 64 bytes)
    USB_AUDIO_MIDI_PACKET* bufferStart;       // The 2D buffer for the endpoint. There are MIDI_USB_BUFFER_SIZE USB buffers that are filled with numOfMIDIPackets
                                              //  MIDI packets. This allows for MIDI_USB_BUFFER_SIZE USB packets to be saved, with a possibility of up to
                                              //  numOfMIDIPackets MID packets within each USB packet.
    USB_AUDIO_MIDI_PACKET* pBufReadLocation;  // Pointer to USB packet that is being read from
    USB_AUDIO_MIDI_PACKET* pBufWriteLocation; // Pointer to USB packet that is being written to
} ENDPOINT_BUFFER;

void* deviceHandle; // Handle to the attached device
PROC_STATE ProcState;
ENDPOINT_BUFFER* endpointBuffers;

// MIDI packet used to translate MIDI UART to MIDI USB, with flag
USB_AUDIO_MIDI_PACKET OSCTranslatedToUSB;

BYTE midiType = 0;
BYTE midiCh   = 0;
BYTE midiNum  = 0;
BYTE midiVal  = 0;

USB_VOLATILE BYTE msCounter;

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


/** FUNCTIONS ******************************************************/
void mrb_init_define_methods(void);

// Basic
mrb_value mrb_delay_10us(mrb_state* mrb, mrb_value self);
mrb_value mrb_delay_1ms(mrb_state* mrb, mrb_value self);
mrb_value mrb_onboard_led(mrb_state* mrb, mrb_value self);

// I/O
mrb_value mrb_init_io_ports(mrb_state* mrb, mrb_value self);
mrb_value mrb_set_port_io_type(mrb_state* mrb, mrb_value self);
mrb_value mrb_get_port_io_type(mrb_state* mrb, mrb_value self);
mrb_value mrb_output_port(mrb_state* mrb, mrb_value self);
mrb_value mrb_input_port(mrb_state* mrb, mrb_value self);

// OSC
mrb_value mrb_init_osc_config(mrb_state* mrb, mrb_value self);
mrb_value mrb_network_tasks(mrb_state* mrb, mrb_value self);
mrb_value mrb_receive_osc_task(mrb_state* mrb, mrb_value self);

// USB Device
mrb_value mrb_usb_device_init(mrb_state* mrb, mrb_value self);
mrb_value mrb_usb_device_tasks(mrb_state* mrb, mrb_value self);

mrb_value mrb_hid_ctrl_task(mrb_state* mrb, mrb_value self);

//void convertMidiToOsc(void);
//void convertHidToOsc(void);

mrb_value mrb_send_note(mrb_state* mrb, mrb_value self);
mrb_value mrb_send_cc(mrb_state* mrb, mrb_value self);

mrb_value mrb_receive_midi_datas(mrb_state* mrb, mrb_value self);

// USB Host
mrb_value mrb_usb_host_init(mrb_state* mrb, mrb_value self);
mrb_value mrb_usb_host_tasks(mrb_state* mrb, mrb_value self);

mrb_value mrb_convert_midi_to_osc(mrb_state* mrb, mrb_value self);

#if defined(USB_USE_CDC)
    void USBHostCDC_Clear_Out_DATA_Array(void);
    mrb_value mrb_usb_cdc_rxtx_handler(mrb_state* mrb, mrb_value self);
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* PICROUTER_H */

