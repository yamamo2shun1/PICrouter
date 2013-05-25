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
 * usb_config.c,v.1.0.0 2013/05/25
 */

#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "USB/usb.h"
#include "USB/usb_host_midi.h"
#if 0
    #include "USB/usb_host_hid.h"
#endif

// *****************************************************************************
// Client Driver Function Pointer Table for the USB Embedded Host foundation
// *****************************************************************************

CLIENT_DRIVER_TABLE usbClientDrvTable[] =
{
#if 0
    {
        USBHostHIDInitialize,
        USBHostHIDEventHandler,
        0
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
#if 0
    { INIT_CL_SC_P( 0x03ul, 0x01ul, 0x02ul ), 0, 0, {TPL_CLASS_DRV} }, // HID Class
    { INIT_CL_SC_P( 0x03ul, 0x01ul, 0x01ul ), 0, 0, {TPL_CLASS_DRV} }, // HID Class
    { INIT_CL_SC_P( 0x03ul, 0x00ul, 0x00ul ), 0, 0, {TPL_CLASS_DRV} }, // HID Class
#endif
    { INIT_CL_SC_P( 0x01ul, 0x03ul, 0x00ul ), 0, 0, {TPL_CLASS_DRV} }  // MIDI Audio Subclass
};

