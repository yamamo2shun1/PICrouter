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
 * usb_descriptor.c,v.0.1 2012/07/08
 */
 
#ifndef __USB_DESCRIPTORS_C
#define __USB_DESCRIPTORS_C

/** INCLUDES *******************************************************/
#include "./USB/usb.h"
#include "./USB/usb_function_hid.h"
#include "./USB/usb_function_midi.h"

#define PITCH

/** CONSTANTS ******************************************************/

/* Device Descriptor */
ROM USB_DEVICE_DESCRIPTOR device_dsc=
{
    0x12,    // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,                // DEVICE descriptor type
    0x0200,                 // USB Spec Release Number in BCD format
    0xEF,                   // Class Code "MISC_DEVICE" (ex: uses IAD descriptor)  
    0x02,                   // Subclass code
    0x01,                   // Protocol code
    USB_EP0_BUFF_SIZE,      // Max packet size for EP0, see usb_config.h
    0x04D8,                 // Vendor ID
#ifdef PITCH
    0xF81F,                 // Product ID: PItCh
#else
    0xF81C,                 // Product ID: PICrouter
#endif
    0x0002,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x00,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

/* Configuration 1 Descriptor */
/* copied from the midi10.pdf USB Device Class Specification for MIDI Devices */
ROM BYTE configDescriptor1[]={
    /* Configuration Descriptor 9 */
    0x09,                          // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,  // CONFIGURATION descriptor type
    141,0x00,                      // Total length of data for this cfg
    3,                             // Number of interfaces in this cfg
    1,                             // Index value of this configuration
    0,                             // Configuration string index
    _DEFAULT | _SELF,              // Attributes, see usb_device.h
    250,                           // Max power consumption (2X mA)

    /* Interface Descriptor  9(18) */
    0x09,//sizeof(USB_INTF_DSC),  // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,     // INTERFACE descriptor type
    0x00,                         // Interface Number
    0,                            // Alternate Setting Number
    2,                            // Number of endpoints in this intf
    HID_INTF,                     // Class code
    0,                            // Subclass code
    0,                            // Protocol code
    0,                            // Interface string index

    /* HID Class-Specific Descriptor 9(27) */
    0x09,                 // Size of this descriptor in bytes
    DSC_HID,              // HID descriptor type
    0x11,0x01,            // HID Spec Release Number in BCD format (1.11)
    0x00,                 // Country Code (0x00 for Not supported)
    HID_NUM_OF_DSC,       // Number of class descriptors, see usbcfg.h
    DSC_RPT,              // Report descriptor type
    HID_RPT01_SIZE,0x00,  // Size of the report descriptor
    
    /* Endpoint Descriptor 7(34) */
    0x07,/*sizeof(USB_EP_DSC)*/
    USB_DESCRIPTOR_ENDPOINT,  //Endpoint Descriptor
    HID_EP | _EP_IN,          //EndpointAddress
    _INTERRUPT,               //Attributes
    0x40,0x00,                //size
    0x01,                     //Interval

    /* Endpoint Descriptor 7(41) */
    0x07,/*sizeof(USB_EP_DSC)*/
    USB_DESCRIPTOR_ENDPOINT,  //Endpoint Descriptor
    HID_EP | _EP_OUT,         //EndpointAddress
    _INTERRUPT,               //Attributes
    0x40,0x00,                //size
    0x01,                     //Interval

    //---------------IAD Descriptor------------------------------------
    /* Interface Association Descriptor: MIDI Function 1 8(49) */ 
    0x08,  // Size of this descriptor in bytes 
    0x0B,  // Interface assocication descriptor type 
    0x01,  // The first associated interface 
    2,     // Number of contiguous associated interface 
    1,     // bInterfaceClass of the first interface 
    1,     // bInterfaceSubclass of the first interface 
    0,     // bInterfaceProtocol of the first interface 
    0,     // Interface string index                         

    /* Interface Descriptor 9(58) */
    0x09,                      // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,  // INTERFACE descriptor type
    0x01,                      // Interface Number
    0,                         // Alternate Setting Number
    0,                         // Number of endpoints in this intf
    1,                         // Class code
    1,                         // Subclass code
    0,                         // Protocol code
    0,                         // Interface string index

    /* MIDI Adapter Class-specific AC Interface Descriptor 9(67) */
    0x09,       //bLength
    0x24,       //bDescriptorType - CS_INTERFACE
    0x01,       //bDescriptorSubtype - HEADER
    0x00,0x01,  //bcdADC
    0x09,0x00,  //wTotalLength
    0x01,       //bInCollection
    0x02,       //baInterfaceNr(2)

    /* MIDI Adapter Standard MS Interface Descriptor 9(76) */
    0x09,       //bLength
    0x04,       //bDescriptorType
    0x02,       //bInterfaceNumber
    0x00,       //bAlternateSetting
    0x02,       //bNumEndpoints
    0x01,       //bInterfaceClass
    0x03,       //bInterfaceSubclass
    0x00,       //bInterfaceProtocol
    0x00,       //iInterface

    /* MIDI Adapter Class-specific MS Interface Descriptor 7(83) */
    0x07,       //bLength
    0x24,       //bDescriptorType - CS_INTERFACE
    0x01,       //bDescriptorSubtype - MS_HEADER
    0x00,0x01,  //BcdADC
    0x41,0x00,  //wTotalLength

    /* MIDI Adapter MIDI IN Jack Descriptor (Embedded) 6(89) */
    0x06,       //bLength
    0x24,       //bDescriptorType - CS_INTERFACE
    0x02,       //bDescriptorSubtype - MIDI_IN_JACK
    0x01,       //bJackType - EMBEDDED
    0x01,       //bJackID
    0x00,       //iJack

    /* MIDI Adapter MIDI IN Jack Descriptor (External) 6(95) */
    0x06,       //bLength
    0x24,       //bDescriptorType - CS_INTERFACE
    0x02,       //bDescriptorSubtype - MIDI_IN_JACK
    0x02,       //bJackType - EXTERNAL
    0x02,       //bJackID
    0x00,       //iJack

    /* MIDI Adapter MIDI OUT Jack Descriptor (Embedded) 9(104) */
    0x09,       //bLength
    0x24,       //bDescriptorType - CS_INTERFACE
    0x03,       //bDescriptorSubtype - MIDI_OUT_JACK
    0x01,       //bJackType - EMBEDDED
    0x03,       //bJackID
    0x01,       //bNrInputPins
    0x02,       //BaSourceID(1)
    0x01,       //BaSourcePin(1)
    0x00,       //iJack

    /* MIDI Adapter MIDI OUT Jack Descriptor (External) 9(113) */
    0x09,       //bLength
    0x24,       //bDescriptorType - CS_INTERFACE
    0x03,       //bDescriptorSubtype - MIDI_OUT_JACK
    0x02,       //bJackType - EXTERNAL
    0x04,       //bJackID
    0x01,       //bNrInputPins
    0x01,       //BaSourceID(1)
    0x01,       //BaSourcePin(1)
    0x00,       //iJack

    /* MIDI Adapter Standard Bulk OUT Endpoint Descriptor 9(122) */
    0x09,       //bLength
    0x05,       //bDescriptorType - ENDPOINT
    0x01,       //bEndpointAddress - OUT
    0x02,       //bmAttributes
    0x40,0x00,  //wMaxPacketSize
    0x00,       //bInterval
    0x00,       //bRefresh
    0x00,       //bSynchAddress

    /* MIDI Adapter Class-specific Bulk OUT Endpoint Descriptor 5(127) */
    0x05,       //bLength
    0x25,       //bDescriptorType - CS_ENDPOINT
    0x01,       //bDescriptorSubtype - MS_GENERAL
    0x01,       //bNumEmbMIDIJack
    0x01,       //BaAssocJackID(1)

    /* MIDI Adapter Standard Bulk IN Endpoint Descriptor 9(136) */
    0x09,       //bLength
    0x05,       //bDescriptorType
    0x81,       //bEndpointAddress
    0x02,       //bmAttributes
    0x40,0x00,  //wMaxPacketSize
    0x00,       //bInterval
    0x00,       //bRefresh
    0x00,       //bSynchAddress

    /* MIDI Adapter Class-specific Bulk IN Endpoint Descriptor 5(141) */
    0x05,       //bLength
    0x25,       //bDescriptorType - CS_ENDPOINT
    0x01,       //bDescriptorSubtype - MS_GENERAL
    0x01,       //bNumEmbMIDIJack
    0x03        //BaAssocJackID(1)
};

#if 1
//Class specific descriptor - HID 
ROM struct {
    BYTE report[HID_RPT01_SIZE];
} hid_rpt01 = {
{
    0x06, 0x00, 0xFF,       // Usage Page = 0xFF00 (Vendor Defined Page 1)
    0x09, 0x01,             // Usage (Vendor Usage 1)
    0xA1, 0x01,             // Collection (Application)

    0x19, 0x01,             //      Usage Minimum 
    0x29, 0x40,             //      Usage Maximum   //64 input usages total (0x01 to 0x40)
    0x15, 0x00,             //      Logical Minimum (data bytes in the report may have minimum value = 0x00)
    0x26, 0xFF,0x00,        //      Logical Maximum (data bytes in the report may have maximum value = 0x00FF = unsigned 255)
    0x75, 0x08,             //      Report Size: 8-bit field size
    0x95, 0x40,             //      Report Count: Make sixty-four 8-bit fields (the next time the parser hits an "Input", "Output", or "Feature" item)
    0x81, 0x00,             //      Input (Data, Array, Abs): Instantiates input packet fields based on the above report size, count, logical min/max, and usage.
    0x19, 0x01,             //      Usage Minimum 
    0x29, 0x40,             //      Usage Maximum   //64 output usages total (0x01 to 0x40)
    0x91, 0x00,             //      Output (Data, Array, Abs): Instantiates output packet fields.  Uses same report size and count as "Input" fields, since nothing new/different was specified to the parser since the "Input" item.

    0xC0,                   // End Collection
}
};                  
#endif

//Language code string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[1];}sd000={
    sizeof(sd000),
    USB_DESCRIPTOR_STRING,
    {0x0409}
};

//Manufacturer string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[8];}sd001={
    sizeof(sd001),
    USB_DESCRIPTOR_STRING,
    {'t','k','r','w','o','r','k','s'}
};

//Product string descriptor
#ifdef PITCH
ROM struct{BYTE bLength;BYTE bDscType;WORD string[5];}sd002={
    sizeof(sd002),
    USB_DESCRIPTOR_STRING,
    {'P','I','t','C','h'}
};
#else
ROM struct{BYTE bLength;BYTE bDscType;WORD string[9];}sd002={
    sizeof(sd002),
    USB_DESCRIPTOR_STRING,
    {'P','I','C','r','o','u','t','e','r'}
};
#endif

//Array of configuration descriptors
ROM BYTE *ROM USB_CD_Ptr[]=
{
    (ROM BYTE *ROM)&configDescriptor1
};

//Array of string descriptors
ROM BYTE *ROM USB_SD_Ptr[]=
{
    (ROM BYTE *ROM)&sd000,
    (ROM BYTE *ROM)&sd001,
    (ROM BYTE *ROM)&sd002
};

/** EOF usb_descriptors.c ***************************************************/

#endif
