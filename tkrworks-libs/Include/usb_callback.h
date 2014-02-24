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
 * usb_callback.h,v.0.2.0 2014/02/22
 */

#ifndef USB_CALLBACK_H
#define USB_CALLBACK_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <GenericTypeDefs.h>

#include "usb_config.h"
#include "USB/usb.h"
#include "USB/usb_function_midi.h"
#include "USB/usb_host_midi.h"
#include "USB/usb_function_hid.h"
#if 0
#if defined(USB_USE_HID) // hid host
    #include "USB/usb_host_hid_parser.h"
    #include "USB/usb_host_hid.h"
#endif
#endif
#if defined(USB_USE_CDC) // cdc host
    #include "USB/usb_host_cdc.h"
    #include "USB/usb_host_cdc_interface.h"
#endif

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
#endif

#define MIDI_USB_BUFFER_SIZE     (BYTE)4
#define NUM_MIDI_PKTS_IN_USB_PKT (BYTE)1

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
    BYTE                   numOfMIDIPackets;  // Each USB Packet sent from a device has
                                              // the possibility of holding more than
                                              // one MIDI packet, so this is to keep
                                              // track of how many MIDI packets are
                                              // within a USB packet (between 1 and
                                              // 16, or 4 and 64 bytes)
    USB_AUDIO_MIDI_PACKET* bufferStart;       // The 2D buffer for the endpoint. There
                                              // are MIDI_USB_BUFFER_SIZE USB buffers
                                              // that are filled with numOfMIDIPackets
                                              // MIDI packets. This allows for
                                              // MIDI_USB_BUFFER_SIZE USB packets to be
                                              // saved, with a possibility of up to
                                              // numOfMIDIPackets MID packets within each USB packet.
    USB_AUDIO_MIDI_PACKET* pBufReadLocation;  // Pointer to USB packet that is being read from
    USB_AUDIO_MIDI_PACKET* pBufWriteLocation; // Pointer to USB packet that is being written to
} ENDPOINT_BUFFER;

typedef struct
{
    BYTE rcvFlag;
    BYTE type;
    BYTE ch;
    BYTE num;
    BYTE val;
} HOST_MIDI_PACKET;

void* deviceHandle; // Handle to the attached device
PROC_STATE ProcState;
ENDPOINT_BUFFER* endpointBuffers;

unsigned char getRcvMidiDataBuffer(BYTE index);
void setRcvMidiDataBuffer(BYTE index, unsigned char value);
void setSndMidiDataBuffer(BYTE index, unsigned char value);

unsigned char getRcvHidDataBuffer(BYTE index);
void setSndHidDataBuffer(BYTE index, unsigned char value);

APPL_STATE getApplCDCState(void);
void setApplCDCState(APPL_STATE state);

BOOL checkUSBState(void);
BOOL midiHadleBusy(void);
void midiRxOnePacket(void);
void midiTxOnePacket(void);
BOOL hidHadleBusy(void);
void hidRxOnePacket(void);
void hidTxOnePacket(void);
void putHostMIDIPacket(USB_AUDIO_MIDI_PACKET packet);
HOST_MIDI_PACKET getHostMIDIPacket(void);

void USBCBSuspend(void);
#if 0
void __attribute__ ((interrupt)) _USB1Interrupt(void);
#endif
void USBCBWakeFromSuspend(void);
void USBCB_SOF_Handler(void);
void USBCBErrorHandler(void);
void USBCBCheckOtherReq(void);
void USBCBStdSetDscHandler(void);
void USBCBInitEP(void);
void USBCBSendResume(void);
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size);
BOOL USB_ApplicationEventHandler(BYTE address, USB_EVENT event, void *data, DWORD size);

#ifdef	__cplusplus
}
#endif

#endif	/* USB_CALLBACK_H */

