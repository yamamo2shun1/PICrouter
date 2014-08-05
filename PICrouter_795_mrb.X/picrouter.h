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
 * picrouter.h,v.0.2.5 2014/08/05
 */

#ifndef PICROUTER_H
#define PICROUTER_H

#ifdef	__cplusplus
extern "C" {
#endif

#define CURRENT_VERSION "0.2.5"

#include <plib.h>
#include <stdlib.h>

#include "HardwareProfile.h"

#include "TCPIP Stack/ZeroconfMulticastDNS.h"

#include "TimeDelay.h"
#include "iosetting.h"
#include "analog.h"
#include "osc.h"
#include "usb_callback.h"

#include "mrbconf.h"
#include "mruby.h"

#include "mruby/irep.h"
#include "mruby/string.h"
#include "mruby/value.h"

/** CONFIGURATIONS **************************************************/
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

/** VARIABLES ******************************************************/

// for mruby
mrb_state* mrb;

// Host
#if 0
typedef enum
{
    MODE_DEVICE,
    MODE_HOST,
    MODE_UNKNOWN
} DEVICE_MODE;
#endif

static DEVICE_MODE device_mode = MODE_DEVICE;
//static DEVICE_MODE device_mode = MODE_HOST;

// MIDI packet used to translate OSC to USB_MIDI(Host), with flag
USB_AUDIO_MIDI_PACKET OSCTranslatedToUSB;

// MIDI(Device)
USB_AUDIO_MIDI_EVENT_PACKET midiData;

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
mrb_value mrb_config_timer5(mrb_state* mrb, mrb_value self);

// I/O
mrb_value mrb_onboard_led(mrb_state* mrb, mrb_value self);
mrb_value mrb_init_io_ports(mrb_state* mrb, mrb_value self);
mrb_value mrb_set_port_io_type(mrb_state* mrb, mrb_value self);
mrb_value mrb_get_port_io_type(mrb_state* mrb, mrb_value self);
mrb_value mrb_output_port(mrb_state* mrb, mrb_value self);
mrb_value mrb_input_port(mrb_state* mrb, mrb_value self);

// OSC
mrb_value mrb_init_osc_config(mrb_state* mrb, mrb_value self);
mrb_value mrb_network_tasks(mrb_state* mrb, mrb_value self);
mrb_value mrb_receive_osc_task(mrb_state* mrb, mrb_value self);
mrb_value mrb_process_standard_messages(mrb_state* mrb, mrb_value self);
mrb_value mrb_process_midi_messages(mrb_state* mrb, mrb_value self);
mrb_value mrb_process_cdc_messages(mrb_state* mrb, mrb_value self);
mrb_value mrb_process_system_messages(mrb_state* mrb, mrb_value self);
mrb_value mrb_send_osc_task(mrb_state* mrb, mrb_value self);
mrb_value mrb_set_osc_address(mrb_state* mrb, mrb_value self);
mrb_value mrb_set_osc_typetag(mrb_state* mrb, mrb_value self);
mrb_value mrb_add_osc_int_arg(mrb_state* mrb, mrb_value self);
mrb_value mrb_add_osc_float_arg(mrb_state* mrb, mrb_value self);
mrb_value mrb_add_osc_string_arg(mrb_state* mrb, mrb_value self);
mrb_value mrb_clear_osc_message(mrb_state* mrb, mrb_value self);
mrb_value mrb_flush_osc_message(mrb_state* mrb, mrb_value self);
mrb_value mrb_get_osc_packet(mrb_state* mrb, mrb_value self);
mrb_value mrb_process_osc_packet(mrb_state* mrb, mrb_value self);
mrb_value mrb_compare_osc_prefix(mrb_state* mrb, mrb_value self);
mrb_value mrb_compare_osc_address(mrb_state* mrb, mrb_value self);
mrb_value mrb_compare_typetag_at_index(mrb_state* mrb, mrb_value self);
mrb_value mrb_get_int_arg_at_index(mrb_state* mrb, mrb_value self);
mrb_value mrb_get_float_arg_at_index(mrb_state* mrb, mrb_value self);
mrb_value mrb_get_string_arg_at_index(mrb_state* mrb, mrb_value self);

// USB Device
mrb_value mrb_usb_device_init(mrb_state* mrb, mrb_value self);
mrb_value mrb_usb_device_tasks(mrb_state* mrb, mrb_value self);

mrb_value mrb_hid_ctrl_task(mrb_state* mrb, mrb_value self);

//void convertHidToOsc(void);

mrb_value mrb_send_note(mrb_state* mrb, mrb_value self);
mrb_value mrb_send_cc(mrb_state* mrb, mrb_value self);

mrb_value mrb_receive_midi_datas(mrb_state* mrb, mrb_value self);

// USB Host
mrb_value mrb_usb_host_init(mrb_state* mrb, mrb_value self);
mrb_value mrb_usb_host_tasks(mrb_state* mrb, mrb_value self);

mrb_value mrb_convert_midi_to_osc(mrb_state* mrb, mrb_value self);

#if defined(USB_USE_CDC)
    mrb_value mrb_usb_cdc_rxtx_handler(mrb_state* mrb, mrb_value self);
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* PICROUTER_H */

