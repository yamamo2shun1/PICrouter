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
 * usb_config.h,v.1.1.0 2013/06/24
 */

/*********************************************************************
 * Descriptor specific type definitions are defined in: usbd.h
 ********************************************************************/

#ifndef USBCFG_H
#define USBCFG_H

/** DEFINITIONS ****************************************************/
#define USB_EP0_BUFF_SIZE 8 // Valid Options: 8, 16, 32, or 64 bytes.
                            // Using larger options take more SRAM, but
                            // does not provide much advantage in most types
                            // of applications.  Exceptions to this, are applications
                            // that use EP0 IN or OUT for sending large amounts of
                            // application related data.
									
#define USB_MAX_NUM_INT   3 // For tracking Alternate Setting
#define USB_MAX_EP_NUMBER 4

//Device descriptor - if these two definitions are not defined then
//  a ROM USB_DEVICE_DESCRIPTOR variable by the exact name of device_dsc
//  must exist.
#define USB_USER_DEVICE_DESCRIPTOR &device_dsc
#define USB_USER_DEVICE_DESCRIPTOR_INCLUDE extern ROM USB_DEVICE_DESCRIPTOR device_dsc

//Configuration descriptors - if these two definitions do not exist then
//  a ROM BYTE *ROM variable named exactly USB_CD_Ptr[] must exist.
#define USB_USER_CONFIG_DESCRIPTOR USB_CD_Ptr
#define USB_USER_CONFIG_DESCRIPTOR_INCLUDE extern ROM BYTE *ROM USB_CD_Ptr[]

//Make sure only one of the below "#define USB_PING_PONG_MODE"
//is uncommented.
//#define USB_PING_PONG_MODE USB_PING_PONG__NO_PING_PONG
#define USB_PING_PONG_MODE USB_PING_PONG__FULL_PING_PONG
//#define USB_PING_PONG_MODE USB_PING_PONG__EP0_OUT_ONLY
//#define USB_PING_PONG_MODE USB_PING_PONG__ALL_BUT_EP0		//NOTE: This mode is not supported in PIC18F4550 family rev A3 devices


#define USB_POLLING
//#define USB_INTERRUPT

/* Parameter definitions are defined in usb_device.h */
#define USB_PULLUP_OPTION USB_PULLUP_ENABLE
//#define USB_PULLUP_OPTION USB_PULLUP_DISABLED

#define USB_TRANSCEIVER_OPTION USB_INTERNAL_TRANSCEIVER
//External Transceiver support is not available on all product families.  Please
//  refer to the product family datasheet for more information if this feature
//  is available on the target processor.
//#define USB_TRANSCEIVER_OPTION USB_EXTERNAL_TRANSCEIVER

#define USB_SPEED_OPTION USB_FULL_SPEED
//#define USB_SPEED_OPTION USB_LOW_SPEED //(not valid option for PIC24F devices)

//------------------------------------------------------------------------------------------------------------------
//Option to enable auto-arming of the status stage of control transfers, if no
//"progress" has been made for the USB_STATUS_STAGE_TIMEOUT value.
//If progress is made (any successful transactions completing on EP0 IN or OUT)
//the timeout counter gets reset to the USB_STATUS_STAGE_TIMEOUT value.
//
//During normal control transfer processing, the USB stack or the application 
//firmware will call USBCtrlEPAllowStatusStage() as soon as the firmware is finished
//processing the control transfer.  Therefore, the status stage completes as 
//quickly as is physically possible.  The USB_ENABLE_STATUS_STAGE_TIMEOUTS 
//feature, and the USB_STATUS_STAGE_TIMEOUT value are only relevant, when:
//1.  The application uses the USBDeferStatusStage() API function, but never calls
//      USBCtrlEPAllowStatusStage().  Or:
//2.  The application uses host to device (OUT) control transfers with data stage,
//      and some abnormal error occurs, where the host might try to abort the control
//      transfer, before it has sent all of the data it claimed it was going to send.
//
//If the application firmware never uses the USBDeferStatusStage() API function,
//and it never uses host to device control transfers with data stage, then
//it is not required to enable the USB_ENABLE_STATUS_STAGE_TIMEOUTS feature.

#define USB_ENABLE_STATUS_STAGE_TIMEOUTS    //Comment this out to disable this feature.  

//Section 9.2.6 of the USB 2.0 specifications indicate that:
//1.  Control transfers with no data stage: Status stage must complete within 
//      50ms of the start of the control transfer.
//2.  Control transfers with (IN) data stage: Status stage must complete within 
//      50ms of sending the last IN data packet in fullfilment of the data stage.
//3.  Control transfers with (OUT) data stage: No specific status stage timing
//      requirement.  However, the total time of the entire control transfer (ex:
//      including the OUT data stage and IN status stage) must not exceed 5 seconds.
//
//Therefore, if the USB_ENABLE_STATUS_STAGE_TIMEOUTS feature is used, it is suggested
//to set the USB_STATUS_STAGE_TIMEOUT value to timeout in less than 50ms.  If the
//USB_ENABLE_STATUS_STAGE_TIMEOUTS feature is not enabled, then the USB_STATUS_STAGE_TIMEOUT
//parameter is not relevant.

#define USB_STATUS_STAGE_TIMEOUT (BYTE)45 //Approximate timeout in milliseconds, except when
                                          //USB_POLLING mode is used, and USBDeviceTasks() is called at < 1kHz
                                          //In this special case, the timeout becomes approximately:
//Timeout(in milliseconds) = ((1000 * (USB_STATUS_STAGE_TIMEOUT - 1)) / (USBDeviceTasks() polling frequency in Hz))
//------------------------------------------------------------------------------------------------------------------

#define USB_SUPPORT_DEVICE
#define USB_SUPPORT_HOST

#define USB_NUM_STRING_DESCRIPTORS 3

#define MAX_ALLOWED_CURRENT             (500)         // Maximum power we can supply in mA

/** DEVICE CLASS USAGE *********************************************/
#if 0
    #define USB_USE_HID//hid
#else
    #define USB_USE_CDC // cdc
#endif
#define USB_USE_MIDI

/** ENDPOINTS ALLOCATION *******************************************/
#define MIDI_EP             1

/* For HID Device */
#define USB_ENABLE_ALL_HANDLER

#define HID_INTF_ID          0x00
#define HID_EP               2
#define HID_INT_OUT_EP_SIZE  3
#define HID_INT_IN_EP_SIZE   3
#define HID_NUM_OF_DSC       1
#define HID_RPT01_SIZE       29

/** DEFINITIONS ****************************************************/

/* Host Configuration */
#if defined(USB_USE_MIDI)
    #define USB_ENABLE_TRANSFER_EVENT
#endif
#if defined(USB_USE_HID)
    #define NUM_TPL_ENTRIES 1//test0 2
#elif defined(USB_USE_CDC)
    #define NUM_TPL_ENTRIES 3
#else
    #define NUM_TPL_ENTRIES 1
#endif
#define USB_NUM_CONTROL_NAKS 200
//for hid? #define USB_NUM_CONTROL_NAKS 20
#define USB_SUPPORT_INTERRUPT_TRANSFERS
#define USB_SUPPORT_BULK_TRANSFERS
#define USB_NUM_BULK_NAKS 10
//test #define USB_NUM_INTERRUPT_NAKS 3
#define USB_NUM_INTERRUPT_NAKS 20
#define USB_INITIAL_VBUS_CURRENT (100/2)
#define USB_INSERT_TIME (250+1)
#define USB_HOST_APP_EVENT_HANDLER USB_ApplicationEventHandler

// Host MIDI Configuration
#if defined(USB_USE_MIDI)
    #define USB_MAX_MIDI_DEVICES 1
#endif

#define USB_MAX_CDC_DEVICES  1

// Host HID Client Driver Configuration
#if defined(USB_USE_HID)
    #define USB_MAX_HID_DEVICES 1
    #define HID_MAX_DATA_FIELD_SIZE 8
    #define APPL_COLLECT_PARSED_DATA USB_HID_DataCollectionHandler
#elif defined(USB_USE_CDC)
    #define USB_CDC_BAUDRATE_SUPPORTED 19200L
    #define USB_CDC_PARITY_TYPE 0
    #define USB_CDC_STOP_BITS 0
    #define USB_CDC_NO_OF_DATA_BITS 8
#endif

#if defined(USB_USE_HID)
    #define USBTasks()    \
    {                     \
        USBHostTasks();   \
        USBHostHIDTasks();\
    }
#elif defined(USB_USE_CDC)
    #define USBTasks()    \
    {                     \
        USBHostTasks();   \
        USBHostCDCTasks();\
    }
#else
    #define USBTasks() \
    {                  \
        USBHostTasks();\
    }
#endif

#define USBInitialize(x)\
{                       \
    USBHostInit(x);     \
}

#endif //USBCFG_H
