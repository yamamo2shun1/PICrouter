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
 * usb_config.c,v.1.1.0 2013/06/19
 */

#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "USB/usb.h"
#include "USB/usb_host_midi.h"
#if defined(USB_USE_HID)
    #include "USB/usb_host_hid.h"
#elif defined(USB_USE_CDC)
    #include "usb/usb_host_cdc.h"
#endif

// *****************************************************************************
// Client Driver Function Pointer Table for the USB Embedded Host foundation
// *****************************************************************************

CLIENT_DRIVER_TABLE usbClientDrvTable[] =
{
#if defined(USB_USE_HID)
    {
        USBHostHIDInitialize,
        USBHostHIDEventHandler,
        0
    }
    ,
#elif defined(USB_USE_CDC)
    {
        USBHostCDCInitialize,
        USBHostCDCEventHandler,
    }
    ,
    {
        USBHostCDCInitialize,
        USBHostCDCEventHandler,
    }
    ,
#endif
    {
        USBHostMIDIInit,
        USBHostMIDIEventHandler,
        0
    }
};

// *****************************************************************************
// USB Embedded Host Targeted Peripheral List (TPL)
// *****************************************************************************

USB_TPL usbTPL[] =
{
#if defined(USB_USE_HID)
    { INIT_CL_SC_P( 0x03ul, 0x01ul, 0x02ul ), 0, 0, {TPL_CLASS_DRV} }, // HID Class
    { INIT_CL_SC_P( 0x03ul, 0x01ul, 0x01ul ), 0, 0, {TPL_CLASS_DRV} }, // HID Class
    { INIT_CL_SC_P( 0x03ul, 0x00ul, 0x00ul ), 0, 0, {TPL_CLASS_DRV} }, // HID Class
#elif defined(USB_USE_CDC)
    { INIT_CL_SC_P( 2ul, 2ul, 1ul ), 0, 0, {TPL_CLASS_DRV} }, // Communication Interface
	 // Communication Device/Interface Class , Abstract Control Mode , Common AT Commands
    { INIT_CL_SC_P( 0x0Aul, 0ul, 0ul ), 0, 0, {TPL_CLASS_DRV} },  // Data Interface
#endif
    { INIT_CL_SC_P( 0x01ul, 0x03ul, 0x00ul ), 0, 0, {TPL_CLASS_DRV} }  // MIDI Audio Subclass
};

