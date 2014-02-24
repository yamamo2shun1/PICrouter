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
 * picrouter.c,v.0.2.3 2014/02/22
 */

#include "picrouter.h"

extern const BYTE bytecode[];

//#define CHECK_MRB_MEM_SIZE
#ifdef CHECK_MRB_MEM_SIZE
void* heap_bottom;
size_t total_size = 0;
size_t max_usage_size = 0;
void* max_reached = 0;
int malloc_count = 0;

void* myallocf(struct mrb_state* mrb, void* p, size_t size, void* ud)
{
    if(size == 0)
    {
        free(p);
        return NULL;
    }
    else
    {
        malloc_count++;
        total_size += size;
        void* ret = realloc(p, size);

        if(ret > max_reached)
        {
            max_reached = ret;
        }
        return ret;
    }
}
#endif

static BYTE sendOSCTaskIndex = 0;
static BYTE swState0 = 0;
static BYTE swState1 = 0;

void _general_exception_handler(unsigned cause, unsigned status)
{
    Nop();
    Nop();
}

int main(int argc, char** argv) {
    BYTE i;

    // Enable optimal performance
    SYSTEMConfigPerformance(GetSystemClock());
    mOSCSetPBDIV(OSC_PB_DIV_1); // Use 1:1 CPU Core:Peripheral clocks

    // Enable the cache for the best performance
    CheKseg0CacheOn();

    DelayMs(100);
    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

#ifdef USE_SPI_SRAM
    initSPI2ForSRAM();
#endif

    initAnalogVariables();
#if defined(USE_LED_ENC)
    initEncoderVariables();
#endif

    // Onboard Switches and LEDs Initialization
    initSW();
    initLEDs();

#if defined(USE_LED_PAD)
    buttonInit();
#endif

    // PWM Initialization
    freq = 10000; // 10kHz
    width = GetSystemClock() / freq;
    for(i = 0; i < PWM_NUM; i++)
        duty[i] = 50;

    // USB I/O Setting
    #if defined(USE_USB_BUS_SENSE_IO)
        tris_usb_bus_sense = 1;
    #endif

    #if defined(USE_SELF_POWER_SENSE_IO)
        tris_self_power = 1;
    #endif

    #ifdef CHECK_MRB_MEM_SIZE
        heap_bottom = realloc(NULL, 1);

        mrb = mrb_open_allocf(myallocf, NULL);
        mrb_close(mrb);
        max_usage_size = (unsigned long)max_reached - (unsigned long)heap_bottom;
        
        LED_1_On();
    #else
        mrb = mrb_open();
        mrb_init_define_methods();
        mrb_load_irep(mrb, bytecode);

#if 0
        mrb_funcall(mrb, mrb_top_self(mrb), "send_osc_task", 0);
        DelayMs(500);

        mrb_funcall(mrb, mrb_top_self(mrb), "send_osc_task", 0);
        DelayMs(500);
        
        mrb_funcall(mrb, mrb_top_self(mrb), "send_osc_task", 0);
        DelayMs(500);
        
        mrb_funcall(mrb, mrb_top_self(mrb), "send_osc_task", 0);
        DelayMs(500);
#endif

        mrb_close(mrb);
    #endif

    return (EXIT_SUCCESS);
}

void mrb_init_define_methods(void)
{
    int ai = mrb_gc_arena_save(mrb);

    // Basic
    mrb_define_method(mrb, mrb->object_class, "delay_10us", mrb_delay_10us, ARGS_REQ(1));
    mrb_define_method(mrb, mrb->object_class, "delay_1ms", mrb_delay_1ms, ARGS_REQ(1));
    mrb_define_method(mrb, mrb->object_class, "config_timer5", mrb_config_timer5, ARGS_REQ(1));

    // I/O Port
    mrb_define_method(mrb, mrb->object_class, "onboard_led", mrb_onboard_led, ARGS_REQ(2));
    mrb_define_method(mrb, mrb->object_class, "init_io_ports", mrb_init_io_ports, ARGS_NONE());
    mrb_define_method(mrb, mrb->object_class, "set_port_io_type", mrb_set_port_io_type, ARGS_REQ(2));
    mrb_define_method(mrb, mrb->object_class, "get_port_io_type", mrb_get_port_io_type, ARGS_REQ(1));
    mrb_define_method(mrb, mrb->object_class, "output_port", mrb_output_port, ARGS_REQ(2));
    mrb_define_method(mrb, mrb->object_class, "input_port", mrb_input_port, ARGS_REQ(1));

    // OSC
    mrb_define_method(mrb, mrb->object_class, "init_osc_config", mrb_init_osc_config, ARGS_REQ(4));
    mrb_define_method(mrb, mrb->object_class, "network_tasks", mrb_network_tasks, ARGS_REQ(1));
    mrb_define_method(mrb, mrb->object_class, "receive_osc_task", mrb_receive_osc_task, ARGS_NONE());
    mrb_define_method(mrb, mrb->object_class, "process_standard_messages", mrb_process_standard_messages, ARGS_NONE());
    mrb_define_method(mrb, mrb->object_class, "process_midi_messages", mrb_process_midi_messages, ARGS_NONE());
    mrb_define_method(mrb, mrb->object_class, "process_cdc_messages", mrb_process_cdc_messages, ARGS_NONE());
    mrb_define_method(mrb, mrb->object_class, "process_system_messages", mrb_process_system_messages, ARGS_NONE());
    mrb_define_method(mrb, mrb->object_class, "set_osc_address", mrb_set_osc_address, ARGS_REQ(2));
    mrb_define_method(mrb, mrb->object_class, "set_osc_typetag", mrb_set_osc_typetag, ARGS_REQ(1));
    mrb_define_method(mrb, mrb->object_class, "add_osc_int_arg", mrb_add_osc_int_arg, ARGS_REQ(1));
    mrb_define_method(mrb, mrb->object_class, "add_osc_float_arg", mrb_add_osc_float_arg, ARGS_REQ(1));
    mrb_define_method(mrb, mrb->object_class, "add_osc_string_arg", mrb_add_osc_string_arg, ARGS_REQ(1));
    mrb_define_method(mrb, mrb->object_class, "clear_osc_message", mrb_clear_osc_message, ARGS_NONE());
    mrb_define_method(mrb, mrb->object_class, "flush_osc_message", mrb_flush_osc_message, ARGS_NONE());
    mrb_define_method(mrb, mrb->object_class, "get_osc_packet", mrb_get_osc_packet, ARGS_NONE());
    mrb_define_method(mrb, mrb->object_class, "process_osc_packet", mrb_process_osc_packet, ARGS_NONE());
    mrb_define_method(mrb, mrb->object_class, "compare_osc_prefix", mrb_compare_osc_prefix, ARGS_REQ(1));
    mrb_define_method(mrb, mrb->object_class, "compare_osc_address", mrb_compare_osc_address, ARGS_REQ(1));
    mrb_define_method(mrb, mrb->object_class, "compare_typetag_at_index", mrb_compare_typetag_at_index, ARGS_REQ(2));
    mrb_define_method(mrb, mrb->object_class, "get_int_arg_at_index", mrb_get_int_arg_at_index, ARGS_REQ(1));
    mrb_define_method(mrb, mrb->object_class, "get_float_arg_at_index", mrb_get_float_arg_at_index, ARGS_REQ(1));
    mrb_define_method(mrb, mrb->object_class, "get_string_arg_at_index", mrb_get_string_arg_at_index, ARGS_REQ(1));

    // USB Device
    mrb_define_method(mrb, mrb->object_class, "usb_device_init", mrb_usb_device_init, ARGS_NONE());
    mrb_define_method(mrb, mrb->object_class, "usb_device_tasks", mrb_usb_device_tasks, ARGS_NONE());

    // USB Device HID
    mrb_define_method(mrb, mrb->object_class, "hid_ctrl_task", mrb_hid_ctrl_task, ARGS_NONE());

    // USB Device MIDI
    mrb_define_method(mrb, mrb->object_class, "send_note", mrb_send_note, ARGS_REQ(3));
    mrb_define_method(mrb, mrb->object_class, "send_cc", mrb_send_cc, ARGS_REQ(3));
    mrb_define_method(mrb, mrb->object_class, "receive_midi_datas", mrb_receive_midi_datas, ARGS_NONE());

    // USB Host
    mrb_define_method(mrb, mrb->object_class, "usb_host_init", mrb_usb_host_init, ARGS_NONE());
    mrb_define_method(mrb, mrb->object_class, "usb_host_tasks", mrb_usb_host_tasks, ARGS_NONE());

    // USB Host MIDI
    mrb_define_method(mrb, mrb->object_class, "convert_midi_to_osc", mrb_convert_midi_to_osc, ARGS_NONE());

    // USB Host CDC
#if defined(USB_USE_CDC)
    mrb_define_method(mrb, mrb->object_class, "usb_cdc_rxtx_handler", mrb_usb_cdc_rxtx_handler, ARGS_NONE());
#endif

    mrb_gc_arena_restore(mrb, ai);
}

mrb_value mrb_delay_10us(mrb_state* mrb, mrb_value self)
{
    mrb_int t;
    mrb_get_args(mrb, "i", &t);

    Delay10us((WORD)t);

    return mrb_nil_value();
}

mrb_value mrb_delay_1ms(mrb_state* mrb, mrb_value self)
{
    mrb_int t;
    mrb_get_args(mrb, "i", &t);

    DelayMs((WORD)t);

    return mrb_nil_value();
}

mrb_value mrb_config_timer5(mrb_state* mrb, mrb_value self)
{
    mrb_int timer_interval;
    mrb_get_args(mrb, "i", &timer_interval);

    OpenTimer5(T5_ON | T5_SOURCE_INT | T5_PS_1_8, timer_interval);
    ConfigIntTimer5(T5_INT_ON | T5_INT_PRIOR_5);

    return mrb_nil_value();
}

mrb_value mrb_onboard_led(mrb_state* mrb, mrb_value self)
{
    mrb_int id;
    mrb_value state;

    mrb_get_args(mrb, "iS", &id, &state);

    switch(id)
    {
    case 1:
        if(!strcmp(RSTRING_PTR(state), "on"))
        {
            LED_1_On();
        }
        else if(!strcmp(RSTRING_PTR(state), "off"))
        {
            LED_1_Off();
        }
        else if(!strcmp(RSTRING_PTR(state), "toggle"))
        {
            LED_1_Toggle();
        }
        break;
    case 2:
        if(!strcmp(RSTRING_PTR(state), "on"))
        {
            LED_2_On();
        }
        else if(!strcmp(RSTRING_PTR(state), "off"))
        {
            LED_2_Off();
        }
        else if(!strcmp(RSTRING_PTR(state), "toggle"))
        {
            LED_2_Toggle();
        }
        break;
    }

    return mrb_nil_value();
}

mrb_value mrb_init_io_ports(mrb_state* mrb, mrb_value self)
{
    BYTE i = 0;

    for(i = 0; i < 14; i++)
        setAnPortDioType(i, IO_IN);

    for(i = 0; i < 4; i++)
    {
        setPwmPortDioType(i, IO_OUT);
        outputPwmPort(i, LOW);
    }

    for(i = 0; i < 4; i++)
    {
        setDigitalPortDioType(i, IO_OUT);
        outputDigitalPort(i, LOW);
    }

    setSpiPortDioType("sck2", IO_OUT);
    setSpiPortDioType("sdi2", IO_OUT);
    setSpiPortDioType("sdo2", IO_OUT);
    setSpiPortDioType("sck4", IO_OUT);
    setSpiPortDioType("sdi4", IO_OUT);
    setSpiPortDioType("sdo4", IO_OUT);
    outputSpiPort("sck2", LOW);
    outputSpiPort("sdi2", LOW);
    outputSpiPort("sdo2", LOW);
    outputSpiPort("sck4", LOW);
    outputSpiPort("sdi4", LOW);
    outputSpiPort("sdo4", LOW);

    return mrb_nil_value();
}

mrb_value mrb_set_port_io_type(mrb_state* mrb, mrb_value self)
{
    mrb_value name;
    mrb_value type;

    mrb_get_args(mrb, "SS", &name, &type);

    if(!strcmp(RSTRING_PTR(type), "in"))
       setPortIOType(RSTRING_PTR(name), IO_IN);
    else if(!strcmp(RSTRING_PTR(type), "out"))
       setPortIOType(RSTRING_PTR(name), IO_OUT);

    return mrb_nil_value();
}

mrb_value mrb_get_port_io_type(mrb_state* mrb, mrb_value self)
{
    mrb_value name;
    mrb_value str_type;
    BYTE type;

    mrb_get_args(mrb, "S", &name);

    type = getPortIOType(RSTRING_PTR(name));
    if(type == IO_IN)
        str_type = mrb_str_new_cstr(mrb, "in");
    else if(type == IO_OUT)
        str_type = mrb_str_new_cstr(mrb, "out");

    return str_type;
}

mrb_value mrb_output_port(mrb_state* mrb, mrb_value self)
{
    mrb_value name;
    mrb_value state;

    mrb_get_args(mrb, "SS", &name, &state);

    if(!strcmp(RSTRING_PTR(state), "high"))
        outputPort(RSTRING_PTR(name), HIGH);
    else if(!strcmp(RSTRING_PTR(state), "low"))
        outputPort(RSTRING_PTR(name), LOW);

    return mrb_nil_value();
}

mrb_value mrb_input_port(mrb_state* mrb, mrb_value self)
{
    mrb_value name;
    mrb_value str_state;
    BYTE state;

    mrb_get_args(mrb, "S", &name);

    state = inputPort(RSTRING_PTR(name));
    if(state == HIGH)
        str_state = mrb_str_new_cstr(mrb, "high");
    else if(state == LOW)
        str_state = mrb_str_new_cstr(mrb, "low");

    return str_state;
}

/**********************************************
*  OSC Generic I/O Processing Part
**********************************************/
mrb_value mrb_init_osc_config(mrb_state* mrb, mrb_value self)
{
    mrb_value osc_host_name;
    mrb_value osc_prefix;
    mrb_int local_port;
    mrb_int remote_port;

    mrb_get_args(mrb, "SSii", &osc_host_name, &osc_prefix, &local_port, &remote_port);
    
    setOSCPrefix(RSTRING_PTR(osc_prefix));
    setLocalPort(local_port);
    setRemotePort(remote_port);

    setOSCHostName(RSTRING_PTR(osc_host_name));

    TickInit();
    InitAppConfig();
    StackInit();
    ZeroconfLLInitialize();
    mDNSInitialize(getOSCHostName());
    mDNSServiceRegister((const char *)getOSCHostName(), // base name of the service
                        "_oscit._udp.local",            // type of the service
                        getLocalPort(),                 // TCP or UDP port, at which this service is available
                        ((const BYTE *)""),             // TXT info
                        1,                              // auto rename the service when if needed
                        NULL,                           // no callback function
                        NULL                            // no application context
                        );
    mDNSMulticastFilterRegister();

    //OpenTimer5(T5_ON | T5_SOURCE_INT | T5_PS_1_8, TIMER5_COUNT);
    //ConfigIntTimer5(T5_INT_ON | T5_INT_PRIOR_5);

    // Enable multi-vectored interrupts
    INTEnableSystemMultiVectoredInt();

    return mrb_nil_value();
}

mrb_value mrb_network_tasks(mrb_state* mrb, mrb_value self)
{
    mrb_int eth_state;

    mrb_get_args(mrb, "i", &eth_state);

    StackTask();

    switch(eth_state)
    {
        case 0:
#if 0
            NBNSTask();
            eth_state = 1;
            break;
        case 1:
#endif
            ZeroconfLLProcess();
            eth_state = 1;// 2;
            break;
        case 1:
            mDNSProcess();
            eth_state = 2;// 3;
            break;
        case 2:
            DHCPServerTask();
            eth_state = 0;
            break;
        default:
            ZeroconfLLProcess();
            mDNSProcess();
            eth_state = 0;
            break;
    }

    if(dwLastIP != AppConfig.MyIPAddr.Val)
    {
        dwLastIP = AppConfig.MyIPAddr.Val;
        mDNSFillHostRecord();
    }

    return mrb_fixnum_value(eth_state);
}

mrb_value mrb_receive_osc_task(mrb_state* mrb, mrb_value self)
{
    BYTE index;
    int i, j, k;
    //debug static BYTE testNum = 0;

    getOSCPacket();

    if(processOSCPacket())
    {
        //debug LED_2_On();

        if(compareOSCPrefix(getOSCPrefix()))
        {
#if defined(USE_LCD)
            else if(compareOSCAddress(msgSetLcdConfig))
            {
                BYTE mode_num;
                char* rsname;
                char* rwname;
                char* ename;
                char* db0name;
                char* db1name;
                char* db2name;
                char* db3name;
                char* db4name;
                char* db5name;
                char* db6name;
                char* db7name;

                //if(getArgumentsLength() != 8 && getArgumentsLength() != 12)
                if(getArgumentsLength() != 8)
                {
                    //sendOSCMessage(sysPrefix, msgError, "ss", msgSetLcdConfig, "need_8_or_12_arguments_at_leaset");
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLcdConfig, "need_8_arguments");
                    return mrb_nil_value();
                }

                if(compareTypeTagAtIndex(0, 'i') && compareTypeTagAtIndex(1, 's') && compareTypeTagAtIndex(2, 's') && compareTypeTagAtIndex(3, 's'))
                {
                    mode_num = getIntArgumentAtIndex(0);
                    rsname = getStringArgumentAtIndex(1);
                    rwname = getStringArgumentAtIndex(2);
                    ename = getStringArgumentAtIndex(3);
                    switch(mode_num)
                    {
                        case 4:
                            if(compareTypeTagAtIndex(4, 's') && compareTypeTagAtIndex(5, 's') && compareTypeTagAtIndex(6, 's') &&
                               compareTypeTagAtIndex(7, 's'))
                            {
                                db4name = getStringArgumentAtIndex(4);
                                db5name = getStringArgumentAtIndex(5);
                                db6name = getStringArgumentAtIndex(6);
                                db7name = getStringArgumentAtIndex(7);
                            }
                            else
                            {
                                sendOSCMessage(sysPrefix, msgError, "ss", msgSetLcdConfig, ": wrong_argument_type");
                                return mrb_nil_value();
                            }
                            initLcd(mode_num, rsname, rwname, ename, NULL, NULL, NULL, NULL, db4name, db5name, db6name, db7name);
                            break;
#if 0
                        case 8:
                            if(compareTypeTagAtIndex(4, 's') && compareTypeTagAtIndex(5, 's') && compareTypeTagAtIndex(6, 's') &&
                               compareTypeTagAtIndex(7, 's') && compareTypeTagAtIndex(8, 's') && compareTypeTagAtIndex(9, 's') &&
                               compareTypeTagAtIndex(10, 's') && compareTypeTagAtIndex(11, 's'))
                            {
                                db0name = getStringArgumentAtIndex(4);
                                db1name = getStringArgumentAtIndex(5);
                                db2name = getStringArgumentAtIndex(6);
                                db3name = getStringArgumentAtIndex(7);
                                db4name = getStringArgumentAtIndex(8);
                                db5name = getStringArgumentAtIndex(9);
                                db6name = getStringArgumentAtIndex(10);
                                db7name = getStringArgumentAtIndex(11);
                            }
                            else
                            {
                                sendOSCMessage(sysPrefix, msgError, "ss", msgSetLcdConfig, ": wrong_argument_type");
                                return mrb_nil_value();
                            }
                            initLcd(mode_num, rsname, rwname, ename, db0name, db1name, db2name, db3name, db4name, db5name, db6name, db7name);
                            break;
#endif
                        default:
                            sendOSCMessage(sysPrefix, msgError, "ss", msgSetLcdConfig, ": support_only_4_bit_mode");
                            return mrb_nil_value();
                            break;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLcdConfig, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgSetLcdText))
            {
                BYTE i = 0;
                BYTE line_num = 0;
                BYTE chars_num = 0;
                char line_text[20] = {0};
                BYTE len = getArgumentsLength();

                if(len < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLcdText, "need_2_arguments_at_least");
                    return mrb_nil_value();
                }

                if(compareTypeTagAtIndex(0, 'i') && compareTypeTagAtIndex(1, 's'))
                {
                    line_num = getIntArgumentAtIndex(0);
                    chars_num = strlen(getStringArgumentAtIndex(1));
                    if(chars_num <= 20)
                        strcpy(line_text, getStringArgumentAtIndex(1));
                    else
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLcdText, "string_must_be_less_than_20_chars");
                        return mrb_nil_value();
                    }

                    for(i = 2; i < len; i++)
                    {
                        if(compareTypeTagAtIndex(i, 's'))
                        {
                            chars_num += strlen(getStringArgumentAtIndex(i));
                            chars_num++;
                            if(chars_num <= 20)
                            {
                                strcat(line_text, " ");
                                strcat(line_text, getStringArgumentAtIndex(i));
                            }
                            else
                                break;
                        }
                        else
                        {
                            sendOSCMessage(sysPrefix, msgError, "ss", msgSetLcdText, ": wrong_argument_type");
                            return mrb_nil_value();
                        }
                    }
                    writeLineLcd(line_num, line_text);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLcdText, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgClearLcdText))
            {
                BYTE line_num;
                char* clear_all;
                char* clear_text = " ";

                if(getArgumentsLength() != 1)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgClearLcdText, "need_1_arguments");
                    return mrb_nil_value();
                }

                if(compareTypeTagAtIndex(0, 'i'))
                {
                    line_num = getIntArgumentAtIndex(0);
                    writeLineLcd(line_num, clear_text);
                }
                else if(compareTypeTagAtIndex(0, 's'))
                {
                    clear_all = getStringArgumentAtIndex(0);
                    if(!strcmp(clear_all, "all"))
                        eraseLcd();
                    else
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgClearLcdText, ": please_type_all");
                        return mrb_nil_value();
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgClearLcdText, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
#endif // #if defined(USE_LCD)
#if defined(USE_LED_PAD)
            else if(compareOSCAddress(msgLatticePadPinSelect))
            {
                if(getArgumentsLength() < 4)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticePadPinSelect, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's') && compareTypeTagAtIndex(2, 's') && compareTypeTagAtIndex(3, 's'))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    char* name_clk = getStringArgumentAtIndex(1);
                    char* name_shld = getStringArgumentAtIndex(2);
                    char* name_qh = getStringArgumentAtIndex(3);

                    if(strlen(name_clk) > 3 || strlen(name_shld) > 3 || strlen(name_qh) > 3)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgLatticePadPinSelect, ": too_long_string_length");
                        return mrb_nil_value();
                    }

                    setLatticePadPortClkName(index, name_clk);
                    setPortIOType(getLatticePadPortClkName(index), IO_OUT);
                    setLatticePadPortShLdName(index, name_shld);
                    setPortIOType(getLatticePadPortShLdName(index), IO_OUT);
                    setLatticePadPortQhName(index, name_qh);
                    setPortIOType(getLatticePadPortQhName(index), IO_IN);

                    setInitPadFlag(TRUE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticePadPinSelect, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgLatticeLedDrvPinSelect))
            {
                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeLedDrvPinSelect, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
                {
                    BYTE num = getIntArgumentAtIndex(0);
                    char* name_load = getStringArgumentAtIndex(1);

                    if(strlen(name_load) > 3)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeLedDrvPinSelect, ": too_long_string_length");
                        return mrb_nil_value();
                    }

                    setLatticePadPortLoadName(name_load);
                    setPortIOType(name_load, IO_OUT);
                    outputPort(name_load, LOW);

                    switch(num)
                    {
                        case 2:
                            setLatticeLedDriverSpiNumber(2);
                            SpiChnClose(2);
                            SpiChnOpen(SPI_CHANNEL2, SPICON_MSTEN | SPICON_SMP | SPICON_MODE16 | SPICON_CKE | SPICON_ON, 4);
                            setInitLatticeLedDrvFlag(TRUE);
                            break;
                        case 4:
                            setLatticeLedDriverSpiNumber(4);
                            SpiChnClose(4);
                            SpiChnOpen(SPI_CHANNEL4, SPICON_MSTEN | SPICON_SMP | SPICON_MODE16 | SPICON_CKE | SPICON_ON, 4);
                            setInitLatticeLedDrvFlag(TRUE);
                            break;
                        default:
                            sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeLedDrvPinSelect, ": out_of_range_value");
                            return mrb_nil_value();
                            break;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeLedDrvPinSelect, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgSetLatticePadConnectedNum))
            {
                if(compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f'))
                {
                    int num = getIntArgumentAtIndex(0);
                    if(num < 1)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticePadConnectedNum, ": out_of_range_value");
                        return mrb_nil_value();
                    }
                    setNumConnectedLatticePad(num);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticePadConnectedNum, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgGetLatticePadConnectedNum))
            {
                sendOSCMessage(getOSCPrefix(), msgLatticePadConnectedNum, "i", getNumConnectedLatticePad());
            }
            else if(compareOSCAddress(msgSetLatticeLed))
            {
                if(getArgumentsLength() < 4)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLed, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) &&
                   (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) &&
                   (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')) &&
                   (compareTypeTagAtIndex(3, 'i') || compareTypeTagAtIndex(3, 'f')))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    BYTE x = getIntArgumentAtIndex(1);
                    BYTE y = getIntArgumentAtIndex(2);
                    BYTE state = getIntArgumentAtIndex(3);
                    BYTE intensity;
                    WORD pos = (1 << y) << (x * 4);

                    if(x > 3 || y > 3 || state > 1)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLed, ": out_of_range_value");
                        return mrb_nil_value();
                    }

                    if(getArgumentsLength() == 4)
                        setLatticeIntensity(index, y + (x * 4), 100);
                    else if(getArgumentsLength() == 5)
                    {
                        if(compareTypeTagAtIndex(4, 'i') || compareTypeTagAtIndex(4, 'f'))
                        {
                            intensity = getIntArgumentAtIndex(4);
                            if(intensity > 100)
                                intensity = 100;
                            setLatticeIntensity(index, y + (x * 4), intensity);
                        }
                        else
                        {
                            sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLed, ": wrong_argument_type");
                            return mrb_nil_value();
                        }
                    }

                    if(state)
                        setLatticeLed(index, getLatticeLed(index) | pos);
                    else
                        setLatticeLed(index, getLatticeLed(index) & ~pos);

                    if(getLatticeLed(index) != 0)
                        setLatticeLedOn(index, TRUE);
                    else
                        setLatticeLedOn(index, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLed, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgSetLatticeLedColumn))
            {
                if(getArgumentsLength() < 3)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedColumn, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) &&
                   (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) &&
                   (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    BYTE column = getIntArgumentAtIndex(1);
                    BYTE data = getIntArgumentAtIndex(2);

                    if(column > 3 || data > 15)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedColumn, ": out_of_range_value");
                        return mrb_nil_value();
                    }

                    setLatticeLed(index, getLatticeLed(index) & ~(0x000F << (column * 4)));
                    setLatticeLed(index, getLatticeLed(index) | (data << (column * 4)));

                    if(getArgumentsLength() == 3)
                    {
                        for(i = 0; i < 4; i++)
                            setLatticeIntensity(index, i + (column * 4), 100);
                    }
                    else if(getArgumentsLength() == 7)
                    {
                        for(i = 0; i < 4; i++)
                            setLatticeIntensity(index, i + (column * 4), getIntArgumentAtIndex(3 + i));
                    }
                    else
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedColumn, ": too_few_intensity_value");
                        return mrb_nil_value();
                    }

                    if(getLatticeLed(index) != 0)
                        setLatticeLedOn(index, TRUE);
                    else
                        setLatticeLedOn(index, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedColumn, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgSetLatticeLedRow))
            {
                if(getArgumentsLength() < 3)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedColumn, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) &&
                   (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) &&
                   (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    BYTE row = getIntArgumentAtIndex(1);
                    BYTE data = getIntArgumentAtIndex(2);
                    WORD data1 = 0;

                    if(row > 3 || data > 15)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedRow, ": out_of_range_value");
                        return mrb_nil_value();
                    }

                    for(i = 0; i < 4; i++)
                    {
                        if((data >> i) & 0x01)
                            data1 |= (1 << (i * 4));
                    }
                    setLatticeLed(index, getLatticeLed(index) & ~(0x1111 << row));
                    setLatticeLed(index, getLatticeLed(index) | (data1 << row));

                    if(getArgumentsLength() == 3)
                    {
                        for(i = 0; i < 4; i++)
                            setLatticeIntensity(index, row + (i * 4), 100);
                    }
                    else if(getArgumentsLength() == 7)
                    {
                        for(i = 0; i < 4; i++)
                            setLatticeIntensity(index, row + (i * 4), getIntArgumentAtIndex(3 + i));
                    }
                    else
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedRow, ": too_few_intensity_value");
                        return mrb_nil_value();
                    }

                    if(getLatticeLed(index) != 0)
                        setLatticeLedOn(index, TRUE);
                    else
                        setLatticeLedOn(index, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedRow, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgSetLatticeLedAll))
            {
                if(getArgumentsLength() < 5)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedAll, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) &&
                   (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) &&
                   (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    BYTE data = 0;
                    WORD data1;

                    for(i = 0; i < 4; i++)
                    {
                        data = getIntArgumentAtIndex(i + 1);
                        if(data > 15)
                        {
                            sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedAll, ": out_of_range_value");
                            return mrb_nil_value();
                        }

                        data1 = 0;
                        for(j = 0; j < 4; j++)
                        {
                            if((data >> j) & 0x01)
                                data1 |= (1 << (j * 4));
                        }
                        setLatticeLed(index, getLatticeLed(index) & ~(0x1111 << i));
                        setLatticeLed(index, getLatticeLed(index) | (data1 << i));
                    }

                    if(getArgumentsLength() == 5)
                    {
                        for(i = 0; i < 16; i++)
                            setLatticeIntensity(index, i, 100);
                    }
                    else if(getArgumentsLength() == 21)
                    {
                        for(i = 0; i < 16; i++)
                        {
                            //setLatticeIntensity(index, i, getIntArgumentAtIndex(5 + i));
                            BYTE ii = (i - (i / 4) * 4) * 4 + (i / 4);
                            setLatticeIntensity(index, ii, getIntArgumentAtIndex(5 + i));
                        }
                    }
                    else
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedAll, ": too_few_intensity_value");
                        return mrb_nil_value();
                    }

                    if(getLatticeLed(index) != 0)
                        setLatticeLedOn(index, TRUE);
                    else
                        setLatticeLedOn(index, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedColumn, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgLatticeLedClear))
            {
                if(getArgumentsLength() < 1)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeLedClear, ": too_few_arguments");
                    return mrb_nil_value();
                }

                if(compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f'))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    setLatticeLed(index, 0);
                    setLatticeLedOn(index, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeLedClear, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgGetLatticeLedIntensity))
            {
                if(getArgumentsLength() < 3)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeLedIntensity, ": too_few_arguments");
                    return mrb_nil_value();
                }

                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) &&
                   (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) &&
                   (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    BYTE x = getIntArgumentAtIndex(1);
                    BYTE y = getIntArgumentAtIndex(2);

                    if(x > 3 || y > 3)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeLedIntensity, ": out_of_range_value");
                        return mrb_nil_value();
                    }

                    sendOSCMessage(getOSCPrefix(), msgLatticeLedIntensity, "iiii", index, x, y, getLatticeIntensity(index, y + (x * 4)));
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeLedIntensity, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgGetLatticeLedIntensityAll))
            {
                if(getArgumentsLength() < 1)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeLedIntensity, ": too_few_arguments");
                    return mrb_nil_value();
                }

                if(compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f'))
                {
                    BYTE index = getIntArgumentAtIndex(0);

                    sendOSCMessage(getOSCPrefix(), msgLatticeLedIntensity, "iiiiiiiiiiiiiiiii", index,
                                   getLatticeIntensity(index, 0), getLatticeIntensity(index, 1),
                                   getLatticeIntensity(index, 2), getLatticeIntensity(index, 3),
                                   getLatticeIntensity(index, 4), getLatticeIntensity(index, 5),
                                   getLatticeIntensity(index, 6), getLatticeIntensity(index, 7),
                                   getLatticeIntensity(index, 8), getLatticeIntensity(index, 9),
                                   getLatticeIntensity(index, 10), getLatticeIntensity(index, 11),
                                   getLatticeIntensity(index, 12), getLatticeIntensity(index, 13),
                                   getLatticeIntensity(index, 14), getLatticeIntensity(index, 15));
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeLedIntensity, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgLatticePadPinSelect))
            {
                if(getArgumentsLength() < 4)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticePadPinSelect, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's') && compareTypeTagAtIndex(2, 's') && compareTypeTagAtIndex(3, 's'))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    char* name_clk = getStringArgumentAtIndex(1);
                    char* name_shld = getStringArgumentAtIndex(2);
                    char* name_qh = getStringArgumentAtIndex(3);

                    if(strlen(name_clk) > 3 || strlen(name_shld) > 3 || strlen(name_qh) > 3)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgLatticePadPinSelect, ": too_long_string_length");
                        return mrb_nil_value();
                    }

                    setLatticePadPortClkName(index, name_clk);
                    setPortIOType(getLatticePadPortClkName(index), IO_OUT);
                    setLatticePadPortShLdName(index, name_shld);
                    setPortIOType(getLatticePadPortShLdName(index), IO_OUT);
                    setLatticePadPortQhName(index, name_qh);
                    setPortIOType(getLatticePadPortQhName(index), IO_IN);

                    setInitPadFlag(TRUE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticePadPinSelect, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgLatticeRgbDrvPinSelect))
            {
                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeRgbDrvPinSelect, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
                {
                    BYTE num = getIntArgumentAtIndex(0);
                    char* name_load = getStringArgumentAtIndex(1);

                    if(strlen(name_load) > 3)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeRgbDrvPinSelect, ": too_long_string_length");
                        return mrb_nil_value();
                    }

                    setLatticePadPortLoadName(name_load);
                    setPortIOType(name_load, IO_OUT);
                    outputPort(name_load, LOW);
                    //setLatticeRgbLedNumber(num, lnum);

                    switch(num)
                    {
                        case 2:
                            setLatticeRgbDriverSpiNumber(2);
                            SpiChnClose(2);
                            SpiChnOpen(SPI_CHANNEL2, SPICON_MSTEN | SPICON_SMP | SPICON_MODE16 | SPICON_CKE | SPICON_ON, 4);
                            setInitLatticeRgbDrvFlag(TRUE);
                            break;
                        case 4:
                            setLatticeRgbDriverSpiNumber(4);
                            SpiChnClose(4);
                            SpiChnOpen(SPI_CHANNEL4, SPICON_MSTEN | SPICON_SMP | SPICON_MODE16 | SPICON_CKE | SPICON_ON, 4);
                            setInitLatticeRgbDrvFlag(TRUE);
                            break;
                        default:
                            sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeRgbDrvPinSelect, ": out_of_range_value");
                            return mrb_nil_value();
                            break;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeRgbDrvPinSelect, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgSetLatticeRgbConnectedNum))
            {
                if(getArgumentsLength() < 1)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbConnectedNum, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if(compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f'))
                {
                    int num = getIntArgumentAtIndex(0);
                    if(num < 1)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbConnectedNum, ": out_of_range_value");
                        return mrb_nil_value();
                    }
                    setNumConnectedLatticeRgb(num);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbConnectedNum, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgGetLatticeRgbConnectedNum))
            {
                sendOSCMessage(getOSCPrefix(), msgLatticeRgbConnectedNum, "i", getNumConnectedLatticeRgb());
            }
            else if(compareOSCAddress(msgSetLatticeRgbSize))
            {
                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbSize, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    BYTE size = getIntArgumentAtIndex(1);

                    if(index > 3 || !(size == 8 || size == 16))
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeRgbDrvPinSelect, ": too_long_string_length");
                        return mrb_nil_value();
                    }

                    setLatticeRgbLedNumber(index, size);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbSize, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgGetLatticeRgbSize))
            {
                if(getArgumentsLength() < 1)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbSize, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if(compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f'))
                {
                    BYTE index = getIntArgumentAtIndex(0);

                    if(index > 4)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeRgbSize, ": too_long_string_length");
                        return mrb_nil_value();
                    }

                    sendOSCMessage(getOSCPrefix(), msgLatticeRgbSize, "ii", index, getLatticeRgbLedNumber(index));
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbSize, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgSetLatticeRgb))
            {
                if(getArgumentsLength() < 5)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgb, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) &&
                   (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) &&
                   (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')) &&
                   (compareTypeTagAtIndex(3, 'i') || compareTypeTagAtIndex(3, 'f')) &&
                   (compareTypeTagAtIndex(4, 'i') || compareTypeTagAtIndex(4, 'f')))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    BYTE layer = getIntArgumentAtIndex(1);
                    BYTE x = getIntArgumentAtIndex(2);
                    BYTE y = getIntArgumentAtIndex(3);
                    BYTE state = getIntArgumentAtIndex(4);
                    BYTE intensity;
                    WORD pos = (1 << y) << (x * 4);


                    if(x > 3 || y > 3 || state > 1)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgb, ": out_of_range_value");
                        return mrb_nil_value();
                    }

                    if(getArgumentsLength() == 5)
                        setLatticeRgbIntensity(index, layer, y + (x * 4), 100);
                    else if(getArgumentsLength() == 6)
                    {
                        if(compareTypeTagAtIndex(5, 'i') || compareTypeTagAtIndex(5, 'f'))
                        {
                            intensity = getIntArgumentAtIndex(5);
                            if(intensity > 100)
                                intensity = 100;

                            setLatticeRgbIntensity(index, layer, y + (x * 4), intensity);
                        }
                        else
                        {
                            sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgb, ": wrong_argument_type");
                            return mrb_nil_value();
                        }
                    }

                    if(state)
                        setLatticeRgb(index, layer, getLatticeRgb(index, layer) | pos);
                    else
                        setLatticeRgb(index, layer, getLatticeRgb(index, layer) & ~pos);

                    if(getLatticeRgb(index, layer) != 0)
                        setLatticeRgbOn(index, layer, TRUE);
                    else
                        setLatticeRgbOn(index, layer, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgb, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgSetLatticeRgbColumn))
            {
                if(getArgumentsLength() < 4)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbColumn, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) &&
                   (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) &&
                   (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')) &&
                   (compareTypeTagAtIndex(3, 'i') || compareTypeTagAtIndex(3, 'f')))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    BYTE layer = getIntArgumentAtIndex(1);
                    BYTE column = getIntArgumentAtIndex(2);
                    BYTE data = getIntArgumentAtIndex(3);

                    if(column > 3 || data > 15)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbColumn, ": out_of_range_value");
                        return mrb_nil_value();
                    }

                    setLatticeRgb(index, layer, getLatticeRgb(index, layer) & ~(0x000F << (column * 4)));
                    setLatticeRgb(index, layer, getLatticeRgb(index, layer) | (data << (column * 4)));

                    if(getArgumentsLength() == 4)
                    {
                        for(i = 0; i < 4; i++)
                            setLatticeRgbIntensity(index, layer, i + (column * 4), 100);
                    }
                    else if(getArgumentsLength() == 8)
                    {
                        for(i = 0; i < 4; i++)
                            setLatticeRgbIntensity(index, layer, i + (column * 4), getIntArgumentAtIndex(4 + i));
                    }
                    else
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbColumn, ": too_few_intensity_value");
                        return mrb_nil_value();
                    }

                    if(getLatticeRgb(index, layer) != 0)
                        setLatticeRgbOn(index, layer, TRUE);
                    else
                        setLatticeRgbOn(index, layer, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbColumn, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgSetLatticeRgbRow))
            {
                if(getArgumentsLength() < 4)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbRow, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) &&
                   (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) &&
                   (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')) &&
                   (compareTypeTagAtIndex(3, 'i') || compareTypeTagAtIndex(3, 'f')))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    BYTE layer = getIntArgumentAtIndex(1);
                    BYTE row = getIntArgumentAtIndex(2);
                    BYTE data = getIntArgumentAtIndex(3);
                    WORD data1 = 0;

                    if(row > 3 || data > 15)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbRow, ": out_of_range_value");
                        return mrb_nil_value();
                    }

                    for(i = 0; i < 4; i++)
                    {
                        if((data >> i) & 0x01)
                            data1 |= (1 << (i * 4));
                    }
                    setLatticeRgb(index, layer, getLatticeRgb(index, layer) & ~(0x1111 << row));
                    setLatticeRgb(index, layer, getLatticeRgb(index, layer) | (data1 << row));

                    if(getArgumentsLength() == 4)
                    {
                        for(i = 0; i < 4; i++)
                            setLatticeRgbIntensity(index, layer, row + (i * 4), 100);
                    }
                    else if(getArgumentsLength() == 8)
                    {
                        for(i = 0; i < 4; i++)
                            setLatticeRgbIntensity(index, layer, row + (i * 4), getIntArgumentAtIndex(4 + i));
                    }
                    else
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbRow, ": too_few_intensity_value");
                        return mrb_nil_value();
                    }

                    if(getLatticeRgb(index, layer) != 0)
                        setLatticeRgbOn(index, layer, TRUE);
                    else
                        setLatticeRgbOn(index, layer, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbRow, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgSetLatticeRgbAll))
            {
                if(getArgumentsLength() < 6)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbAll, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) &&
                   (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) &&
                   (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')) &&
                   (compareTypeTagAtIndex(3, 'i') || compareTypeTagAtIndex(3, 'f')))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    BYTE layer = getIntArgumentAtIndex(1);
                    BYTE data = 0;
                    WORD data1;

                    for(i = 0; i < 4; i++)
                    {
                        data = getIntArgumentAtIndex(i + 2);
                        if(data > 15)
                        {
                            sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbAll, ": out_of_range_value");
                            return mrb_nil_value();
                        }

                        data1 = 0;
                        for(j = 0; j < 4; j++)
                        {
                            if((data >> j) & 0x01)
                                data1 |= (1 << (j * 4));
                        }
                        setLatticeRgb(index, layer, getLatticeRgb(index, layer) & ~(0x1111 << i));
                        setLatticeRgb(index, layer, getLatticeRgb(index, layer) | (data1 << i));
                    }

                    if(getArgumentsLength() == 6)
                    {
                        for(i = 0; i < 16; i++)
                            setLatticeRgbIntensity(index, layer, i, 100);
                    }
                    else if(getArgumentsLength() == 22)
                    {
                        for(i = 0; i < 16; i++)
                        {
                            //setLatticeIntensity(index, i, getIntArgumentAtIndex(5 + i));
                            BYTE ii = (i - (i / 4) * 4) * 4 + (i / 4);
                            setLatticeRgbIntensity(index, layer, ii, getIntArgumentAtIndex(6 + i));
                        }
                    }
                    else
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbAll, ": too_few_intensity_value");
                        return mrb_nil_value();
                    }

                    if(getLatticeRgb(index, layer) != 0)
                        setLatticeRgbOn(index, layer, TRUE);
                    else
                        setLatticeRgbOn(index, layer, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbAll, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgLatticeRgbClear))
            {
                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeRgbClear, ": too_few_arguments");
                    return mrb_nil_value();
                }

                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) &&
                   (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    BYTE layer = getIntArgumentAtIndex(1);
                    setLatticeRgb(index, layer, 0);
                    setLatticeRgbOn(index, layer, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeRgbClear, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgGetLatticeRgbIntensity))
            {
                if(getArgumentsLength() < 4)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeRgbIntensity, ": too_few_arguments");
                    return mrb_nil_value();
                }

                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) &&
                   (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) &&
                   (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')) &&
                   (compareTypeTagAtIndex(3, 'i') || compareTypeTagAtIndex(3, 'f')))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    BYTE layer = getIntArgumentAtIndex(1);
                    BYTE x = getIntArgumentAtIndex(2);
                    BYTE y = getIntArgumentAtIndex(3);

                    if(x > 3 || y > 3)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeRgbIntensity, ": out_of_range_value");
                        return mrb_nil_value();
                    }

                    sendOSCMessage(getOSCPrefix(), msgLatticeRgbIntensity, "iiii", index, x, y, getLatticeRgbIntensity(index, layer, y + (x * 4)));
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeRgbIntensity, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgGetLatticeRgbIntensityAll))
            {
                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeRgbIntensity, ": too_few_arguments");
                    return mrb_nil_value();
                }

                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) &&
                   (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    BYTE layer = getIntArgumentAtIndex(1);

                    sendOSCMessage(getOSCPrefix(), msgLatticeRgbIntensity, "iiiiiiiiiiiiiiiii", index,
                                   getLatticeRgbIntensity(index, layer, 0), getLatticeRgbIntensity(index, layer, 1),
                                   getLatticeRgbIntensity(index, layer, 2), getLatticeRgbIntensity(index, layer, 3),
                                   getLatticeRgbIntensity(index, layer, 4), getLatticeRgbIntensity(index, layer, 5),
                                   getLatticeRgbIntensity(index, layer, 6), getLatticeRgbIntensity(index, layer, 7),
                                   getLatticeRgbIntensity(index, layer, 8), getLatticeRgbIntensity(index, layer, 9),
                                   getLatticeRgbIntensity(index, layer, 10), getLatticeRgbIntensity(index, layer, 11),
                                   getLatticeRgbIntensity(index, layer, 12), getLatticeRgbIntensity(index, layer, 13),
                                   getLatticeRgbIntensity(index, layer, 14), getLatticeRgbIntensity(index, layer, 15));
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeRgbIntensity, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
#endif // #if defined(USE_LED_PAD)
#if defined(USE_LED_ENC)
            else if(compareOSCAddress(msgRotaryIncEncPinSelect))
            {
                if(getArgumentsLength() < 4)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryIncEncPinSelect, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's') && compareTypeTagAtIndex(2, 's') && compareTypeTagAtIndex(3, 's'))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    char* name_a = getStringArgumentAtIndex(1);
                    char* name_b = getStringArgumentAtIndex(2);
                    char* name_sw = getStringArgumentAtIndex(3);
                    
                    if(strlen(name_a) > 3 || strlen(name_b) > 3 || strlen(name_sw) > 3)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryIncEncPinSelect, ": too_long_string_length");
                        return mrb_nil_value();
                    }

                    setIncEncoderPortAName(index, name_a);
                    setPortIOType(getIncEncoderPortAName(index), IO_IN);
                    setIncEncoderPortBName(index, name_b);
                    setPortIOType(getIncEncoderPortBName(index), IO_IN);
                    setIncEncoderPortSwName(index, name_sw);
                    setPortIOType(getIncEncoderPortSwName(index), IO_IN);

                    setInitIncEncFlag(TRUE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryIncEncPinSelect, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgRotaryAbsEncPinSelect))
            {
                if(getArgumentsLength() < 3)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryAbsEncPinSelect, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if(compareTypeTagAtIndex(0, 's') && compareTypeTagAtIndex(1, 's') && compareTypeTagAtIndex(2, 's'))
                {
                    char* name_cs = getStringArgumentAtIndex(0);
                    char* name_clk = getStringArgumentAtIndex(1);
                    char* name_do = getStringArgumentAtIndex(2);

                    if(strlen(name_cs) > 3 || strlen(name_clk) > 3 || strlen(name_do) > 3)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryAbsEncPinSelect, ": too_long_string_length");
                        return mrb_nil_value();
                    }

                    setAbsEncoderPortCsName(name_cs);
                    setPortIOType(name_cs, IO_OUT);
                    setAbsEncoderPortClkName(name_clk);
                    setPortIOType(name_clk, IO_OUT);
                    setAbsEncoderPortDoName(name_do);
                    setPortIOType(name_do, IO_IN);

                    setInitAbsEncFlag(TRUE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryAbsEncPinSelect, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgRotaryLedDrvPinSelect))
            {
                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryLedDrvPinSelect, ": too_few_arguments");
                    return mrb_nil_value();
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
                {
                    BYTE num = getIntArgumentAtIndex(0);
                    char* name_ss = getStringArgumentAtIndex(1);

                    if(strlen(name_ss) > 3)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryLedDrvPinSelect, ": too_long_string_length");
                        return mrb_nil_value();
                    }

                    setLedDriverPortSsName(name_ss);
                    setPortIOType(name_ss, IO_OUT);
                    outputPort(name_ss, LOW);

                    switch(num)
                    {
                        case 2:
                            setLedDriverSpiNumber(2);
                            SpiChnClose(2);
                            SpiChnOpen(SPI_CHANNEL2, SPICON_MSTEN | SPICON_SMP | SPICON_MODE16 | SPICON_ON, 4);
                            setInitLedDrvFlag(TRUE);
                            break;
                        case 4:
                            setLedDriverSpiNumber(4);
                            SpiChnClose(4);
                            SpiChnOpen(SPI_CHANNEL4, SPICON_MSTEN | SPICON_SMP | SPICON_MODE16 | SPICON_ON, 4);
                            setInitLedDrvFlag(TRUE);
                            break;
                        default:
                            sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryLedDrvPinSelect, ": out_of_range_value");
                            return mrb_nil_value();
                            break;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryLedDrvPinSelect, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgSetRotaryAbsEncConnectedNum))
            {
                if(compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f'))
                {
                    int num = getIntArgumentAtIndex(0);
                    if(num < 1)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryAbsEncConnectedNum, ": out_of_range_value");
                        return mrb_nil_value();
                    }
                    setNumConnectedAbsEnc(num);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryAbsEncConnectedNum, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgGetRotaryAbsEncConnectedNum))
            {
                sendOSCMessage(getOSCPrefix(), msgRotaryAbsEncConnectedNum, "i", getNumConnectedAbsEnc());
            }
            else if(compareOSCAddress(msgSetRotaryLedStep))
            {
                BYTE index;
                INT16 pos0, pos;
                if(getArgumentsLength() < 4)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedStep, ": too_few_arguments");
                    return mrb_nil_value();
                }

                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) || (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')) || (compareTypeTagAtIndex(3, 'i') || compareTypeTagAtIndex(3, 'f')))
                {
                    index = getIntArgumentAtIndex(0);
                    pos0 = getIntArgumentAtIndex(1);
                    if(pos0 + 16 > 31)
                        pos = pos0 - 16;
                    else
                        pos = pos0 + 16;

                    INT8 direction = getIntArgumentAtIndex(2);
                    INT8 len = getIntArgumentAtIndex(3);
                    setDwLedData(index, 0);

                    INT8 argLen = getArgumentsLength() - 4;

                    if(argLen >= len)
                        argLen = len;

                    for(i = 0; i < argLen; i++)
                    {
                        //debug sendOSCMessage(TxSocket, getOSCPrefix(), "/debug1", "iiii", argLen, len, pos, i);
                        if(!compareTypeTagAtIndex(i, 'i') && !compareTypeTagAtIndex(i, 'f'))
                        {
                            sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedStep, ": wrong_argument_type");
                            return mrb_nil_value();
                        }
                            
                        if(direction > 0)
                        {
                            if(pos + i < 32)
                                setIntensity(index, pos + i, getIntArgumentAtIndex(i + 4));
                            else
                                setIntensity(index, pos + i - 32, getIntArgumentAtIndex(i + 4));
                        }
                        else if(direction < 0)
                        {
                            if(pos >= i)
                                setIntensity(index, pos - i, getIntArgumentAtIndex(i + 4));
                            else
                                setIntensity(index, 32 + pos - i, getIntArgumentAtIndex(i + 4));
                        }
                    }

                    for(i = 0; i < len; i++)
                    {
                        if(direction > 0)
                        {
                            if(pos + i < 32)
                                setDwLedData(index, getDwLedData(index) | (1 << (pos + i)));
                            else
                                setDwLedData(index, getDwLedData(index) | (1 << (pos + i - 32)));
                        }
                        else if(direction < 0)
                        {
                            if(pos >= i)
                                setDwLedData(index, getDwLedData(index) | (1 << (pos - i)));
                            else
                                setDwLedData(index, getDwLedData(index) | (1 << (32 + pos - i)));
                        }
                    }

#if 0
                    for(j = 0; j < 100; j++)
                    {
                        setDwLedSequence(index, j, getDwLedData(index));

                        for(k = 0; k < 32; k++)
                        {
                            if(j >= getIntensity(index, k))
                                setDwLedSequence(index, j, getDwLedSequence(index, j) & ~(1 << k));
                        }
                    }
#endif

                    if(getDwLedData(index) != 0)
                        setLedOn(index, TRUE);
                    else
                        setLedOn(index, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedStep, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgSetRotaryLedBits))
            {
                BYTE index;
                BYTE argLen = getArgumentsLength() - 2;
                DWORD data0, data1;

                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedBits, ": too_few_arguments");
                    return mrb_nil_value();
                }

                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')))
                {
                    index = getIntArgumentAtIndex(0);
                    data0 = getIntArgumentAtIndex(1);
                    data1 = ((data0 & 0x0000FFFF) << 16) + ((data0 >> 16) & 0x0000FFFF);
                    setDwLedData(index, data1);

                    if(argLen >= 32)
                    {
                        for(i = 0; i < 32; i++)
                        {
                            if(!compareTypeTagAtIndex(i, 'i') && !compareTypeTagAtIndex(i, 'f'))
                            {
                                sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedBits, ": wrong_argument_type");
                                return mrb_nil_value();
                            }
                            //debug sendOSCMessage(TxSocket, getOSCPrefix(), "/debug0", "iiii", argLen, len, pos, i);
                            setIntensity(index, i, getIntArgumentAtIndex(i + 2));
                        }
                    }
                    else if(argLen < 32)
                    {
                        for(i = 0; i < argLen; i++)
                        {
                            if(!compareTypeTagAtIndex(i, 'i') && !compareTypeTagAtIndex(i, 'f'))
                            {
                                sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedBits, ": wrong_argument_type");
                                return mrb_nil_value();
                            }
                            //debug sendOSCMessage(TxSocket, getOSCPrefix(), "/debug1", "iiii", argLen, len, pos, i);
                            setIntensity(index, i, getIntArgumentAtIndex(i + 2));
                        }
                    }

#if 0
                    for(j = 0; j < 100; j++)
                    {
                        setDwLedSequence(index, j, getDwLedData(index));

                        for(k = 0; k < 32; k++)
                        {
                            if(j >= getIntensity(index, k))
                                setDwLedSequence(index, j, getDwLedSequence(index, j) & ~(1 << k));
                        }
                    }
#endif

                    if(getDwLedData(index) != 0)
                        setLedOn(index, TRUE);
                    else
                        setLedOn(index, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedBits, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgSetRotaryLedIntensity))
            {
                BYTE index, position, intensity;

                if(getArgumentsLength() < 3)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedIntensity, ": too_few_arguments");
                    return mrb_nil_value();
                }

                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) && (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')))
                {
                    index = getIntArgumentAtIndex(0);
                    if(getInitAbsEncFlag() && index >= getNumConnectedAbsEnc())
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedIntensity, ": out_of_range_value");
                        return mrb_nil_value();
                    }

                    position = getIntArgumentAtIndex(1) + 16;
                    if(position > 31)
                        position -= 32;
                    if(position > 31)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedIntensity, ": out_of_range_value");
                        return mrb_nil_value();
                    }

                    intensity = getIntArgumentAtIndex(2);
                    setIntensity(index, position, intensity);
                    if(intensity > 100)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedIntensity, ": out_of_range_value");
                        return mrb_nil_value();
                    }

#if 0
                    for(j = 0; j < 100; j++)
                    {
                        setDwLedSequence(index, j, getDwLedData(index));
                        for(k = 0; k < 32; k++)
                        {
                            if(k == position)
                                setIntensity(index, k, intensity);
                            if(j >= getIntensity(index, k))
                            //if(j >= getIntensity(index, position))
                                setDwLedSequence(index, j, getDwLedSequence(index, j) & ~(1 << k));
                                //setDwLedSequence(index, j, getDwLedSequence(index, j) & ~(1 << position));
                        }
                    }
#endif
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedBits, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgGetRotaryLedIntensity))
            {
                BYTE index, position0, position1, intensity;

                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetRotaryLedIntensity, ": too_few_arguments");
                    return mrb_nil_value();
                }

                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')))
                {
                    index = getIntArgumentAtIndex(0);
                    if(getInitAbsEncFlag() && index >= getNumConnectedAbsEnc())
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgGetRotaryLedIntensity, ": out_of_range_value");
                        return mrb_nil_value();
                    }

                    position0 = getIntArgumentAtIndex(1);
                    position1 = position0 + 16;
                    if(position1 > 31)
                        position1 -= 32;
                    
                    if(position1 > 31)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedIntensity, ": out_of_range_value");
                        return mrb_nil_value();
                    }

                    intensity = getIntensity(index, position1);

                    sendOSCMessage(getOSCPrefix(), msgRotaryLedIntensity, "iii", index, position0, intensity);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetRotaryLedIntensity, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
            else if(compareOSCAddress(msgSetRotaryLedIntensityAll))
            {
                BYTE index;
                DWORD all_intensity;

                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedIntensityAll, ": too_few_arguments");
                    return mrb_nil_value();
                }

                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')))
                {
                    index = getIntArgumentAtIndex(0);
                    all_intensity = getIntArgumentAtIndex(1);

#if 0
                    for(j = 0; j < 100; j++)
                    {
                        setDwLedSequence(index, j, getDwLedData(index));
                        for(k = 0; k < 32; k++)
                        {
                            setIntensity(index, k, all_intensity);
                            if(j >= getIntensity(index, k))
                                setDwLedSequence(index, j, getDwLedSequence(index, j) & ~(1 << k));
                        }
                    }
#else
                    for(k = 0; k < 32; k++)
                        setIntensity(index, k, all_intensity);
#endif
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedIntensityAll, ": wrong_argument_type");
                    return mrb_nil_value();
                }
            }
#endif // #if defined(USE_LED_ENC)
        }
        //debug LED_2_Off();
    }
    return mrb_nil_value();
}

mrb_value mrb_process_standard_messages(mrb_state* mrb, mrb_value self)
{
    BYTE i, index;
    if(compareOSCAddress(msgOnboardLed))
    {
        if(!(compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) || !compareTypeTagAtIndex(1, 's'))
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgOnboardLed, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(getIntArgumentAtIndex(0) == 0)
        {
            if(!strcmp(getStringArgumentAtIndex(1), "on"))
            {
                LED_1_On();
            }
            else if(!strcmp(getStringArgumentAtIndex(1), "off"))
            {
                LED_1_Off();
            }
            else
                sendOSCMessage(sysPrefix, msgError, "ss", msgOnboardLed, ": wrong_argument_string");
        }
        else if(getIntArgumentAtIndex(0) == 1)
        {
            if(!strcmp(getStringArgumentAtIndex(1), "on"))
            {
                LED_2_On();
            }
            else if(!strcmp(getStringArgumentAtIndex(1), "off"))
            {
                LED_2_Off();
            }
            else
                sendOSCMessage(sysPrefix, msgError, "ss", msgOnboardLed, ": wrong_argument_string");
        }
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgOnboardLed, ": wrong_argument_value");
    }
    // Port
    else if(compareOSCAddress(msgSetPortIO))
    {
        char* port_name;
        char* type;
 
        if(compareTypeTagAtIndex(0, 's') && compareTypeTagAtIndex(1, 's'))
        {
            port_name = getStringArgumentAtIndex(0);
            if(!comparePortNameAtIndex(port_name))
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetPortIO, ": wrang_argument_string");
                return mrb_nil_value();
            }
            type = getStringArgumentAtIndex(1);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetPortIO, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(!strcmp(type, "in"))
        {
            setPortIOType(port_name, IO_IN);
        }
        else if(!strcmp(type, "out"))
        {
            setPortIOType(port_name, IO_OUT);
            outputPort(port_name, LOW);
        }
    }
    else if(compareOSCAddress(msgGetPortIO))
    {
        char* port_name;

        if(compareTypeTagAtIndex(0, 's'))
        {
            port_name = getStringArgumentAtIndex(0);
            if(!comparePortNameAtIndex(port_name))
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetPortIO, ": wrang_argument_string");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetPortIO, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(getPortIOType(port_name))
            sendOSCMessage(getOSCPrefix(), msgPortIO, "ss", port_name, "in");
        else
            sendOSCMessage(getOSCPrefix(), msgPortIO, "ss", port_name, "out");
    }
    else if(compareOSCAddress(msgSetPortOut))
    {
        char* port_name;
        char* state;

        if(compareTypeTagAtIndex(0, 's') && compareTypeTagAtIndex(1, 's'))
        {
            port_name = getStringArgumentAtIndex(0);
            if(!comparePortNameAtIndex(port_name))
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetPortOut, ": wrang_argument_string");
                return mrb_nil_value();
            }
            state = getStringArgumentAtIndex(1);
            if(strcmp(state, "high") && strcmp(state, "low"))
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetPortOut, ": wrong_argument_string");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetPortOut, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(!strcmp(state, "high"))
            outputPort(port_name, HIGH);
        else if(!strcmp(state, "low"))
            outputPort(port_name, LOW);
    }
    else if(compareOSCAddress(msgGetPortIn))
    {
        char* port_name;

        if(compareTypeTagAtIndex(0, 's'))
        {
            port_name = getStringArgumentAtIndex(0);
            if(!comparePortNameAtIndex(port_name))
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetPortIn, ": wrang_argument_string");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetPortIn, ": wrong_argument_type");
            return mrb_nil_value();
        }

        BYTE state = inputPort(port_name);

        if(state)
            sendOSCMessage(getOSCPrefix(), msgPortIn, "ss", port_name, "high");
        else
            sendOSCMessage(getOSCPrefix(), msgPortIn, "ss", port_name, "low");
    }
    // A/D
    else if(compareOSCAddress(msgSetAdcEnable))
    {
        AD1CON1bits.ON = 0;

        BYTE id, anum;
        char* state;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
        {
            id = getIntArgumentAtIndex(0);
            if(id > AN_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcEnable, ": out_of_value_range");
                return mrb_nil_value();
            }
            state = getStringArgumentAtIndex(1);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcEnable, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(!strcmp(state, "on"))
        {
            setAnalogEnable(id, TRUE);
            setAnPortDioType(id, IO_IN);

            AD1PCFG &= ~(0x0001 << id);
            AD1CSSL |= (0x0001 << id);
        }
        else if(!strcmp(state, "off"))
        {
            setAnalogEnable(id, FALSE);
            setAnPortDioType(id, IO_IN);

            AD1PCFG |= (0x0001 << id);
            AD1CSSL &= ~(0x0001 << id);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcEnable, ": wrong_argument_string");
            return mrb_nil_value();
        }

        anum = 0;
        for(i = 0; i < AN_NUM; i++)
        {
            if(getAnalogEnable(i))
                anum++;
        }

        if(anum > 0)
        {
            AD1CON2 = 0x00000400;// 0000 0000 0000 0000 0000 0000 0100 0000
            AD1CON2 |= ((anum - 1) << 2);
            AD1CON3 = 0x00001F08;// 0000 0000 0000 0000 0001 1111 0000 1000
            AD1CHS  = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
            AD1CON1 = 0x000080E6;// 0000 0000 0000 0000 1000 0000 1110 0110
        }
        else
        {
            AD1PCFG = 0x0000FFFF;// 0000 0000 0000 0000 1111 1111 1111 1111
            AD1CSSL = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000

            AD1CON2 = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
            AD1CON3 = 0x00001F08;// 0000 0000 0000 0000 0001 1111 0000 1000
            AD1CHS  = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
            AD1CON1 = 0x00000000;// 0000 0000 0000 0000 1000 0000 0000 0000
        }
    }
    else if(compareOSCAddress(msgGetAdcEnable))
    {
        BYTE id;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
        {
            id = getIntArgumentAtIndex(0);
            if(id > AN_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcEnable, ": out_of_value_range");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcEnable, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(getAnalogEnable(id))
            sendOSCMessage(getOSCPrefix(), msgAdcEnable, "is", id, "on");
        else
            sendOSCMessage(getOSCPrefix(), msgAdcEnable, "is", id, "off");
    }
    else if(compareOSCAddress(msgSetAdcType))
    {
        BYTE id;
        char* type = NULL;

        if(getArgumentsLength() != 2)
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcType, ": must_be_2_arguments");
            return mrb_nil_value();
        }

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
        {
            id = getIntArgumentAtIndex(0);

            if(id > AN_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcType, ": out_of_value_range");
                return mrb_nil_value();
            }

            type = getStringArgumentAtIndex(1);

            if(!strcmp(type, "8bit"))
            {
                setAnalogType(id, BYTE_ORIGINAL);
            }
            else if(!strcmp(type, "10bit"))
            {
                setAnalogType(id, WORD_ORIGINAL);
            }
            else
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcType, ": must_be_8bit_or_10bit");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcType, ": wrong_argument_type");
            return mrb_nil_value();
        }
    }
    else if(compareOSCAddress(msgGetAdcType))
    {
        BYTE id;

        if(getArgumentsLength() != 1)
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcType, ": must_be_1_argument");
            return mrb_nil_value();
        }

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
        {
            id = getIntArgumentAtIndex(0);
            if(id > AN_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcType, ": out_of_value_range");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcType, ": wrong_argument_type");
            return mrb_nil_value();
        }

        switch(getAnalogType(id))
        {
        case BYTE_ORIGINAL:
            sendOSCMessage(getOSCPrefix(), msgAdcType, "is", id, "8bit");
            break;
        case WORD_ORIGINAL:
            sendOSCMessage(getOSCPrefix(), msgAdcType, "is", id, "10bit");
            break;
        default:
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcType, ": may_be_other_setting");
            break;
        }
    }
    else if(compareOSCAddress(msgSetAdcDio))
    {
        BYTE id;
        char* type;
 
        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
        {
            id = getIntArgumentAtIndex(0);
            if(id > AN_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcDio, ": out_of_value_range");
                return mrb_nil_value();
            }
            type = getStringArgumentAtIndex(1);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcDio, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(!strcmp(type, "in"))
        {
            setAnPortDioType(id, IO_IN);
        }
        else if(!strcmp(type, "out"))
        {
            setAnPortDioType(id, IO_OUT);
            outputAnPort(id, 0);
        }
    }
    else if(compareOSCAddress(msgGetAdcDio))
    {
        BYTE id;
        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
        {
            id = getIntArgumentAtIndex(0);
            if(id > AN_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcDio, ": out_of_value_range");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcDio, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(getAnPortDioType(id))
            sendOSCMessage(getOSCPrefix(), msgAdcDio, "is", id, "in");
        else
            sendOSCMessage(getOSCPrefix(), msgAdcDio, "is", id, "out");
    }
    else if(compareOSCAddress(msgSetAdcDo))
    {
        BYTE id;
        char* state;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
        {
            id = getIntArgumentAtIndex(0);
            if(id > AN_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcDo, ": out_of_value_range");
                return mrb_nil_value();
            }
            state = getStringArgumentAtIndex(1);
            if(strcmp(state, "high") && strcmp(state, "low"))
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcDo, ": wrong_argument_string");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcDo, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(!strcmp(state, "high"))
            outputAnPort(id, HIGH);
        else if(!strcmp(state, "low"))
            outputAnPort(id, LOW);
    }
    else if(compareOSCAddress(msgGetAdcDi))
    {
        BYTE id;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
        {
            id = getIntArgumentAtIndex(0);
            if(id > AN_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcDi, ": out_of_value_range");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcDi, ": wrong_argument_type");
            return mrb_nil_value();
        }

        BYTE state = inputAnPort(id);

        if(state)
            sendOSCMessage(getOSCPrefix(), msgAdcDi, "is", id, "high");
        else
            sendOSCMessage(getOSCPrefix(), msgAdcDi, "is", id, "low");
    }
    // PWM
    else if(compareOSCAddress(msgSetPwmEnable))
    {
        char* state;
        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
        {
            index = getIntArgumentAtIndex(0);
            if(index > 3)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmEnable, ": out_of_value_range");
                return mrb_nil_value();
            }
            state = getStringArgumentAtIndex(1);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmEnable, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(strcmp(state, "on") == 0)
        {
            LONG w = (LONG)(((float)duty[index] / 100.0) * (float)width);
            if(!onTimer2)
            {
                OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_1, width);
                ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);//syama0
                onTimer2 = TRUE;
            }
            switch(index)
            {
            case 0:
                OpenOC1(OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                break;
            case 1:
                OpenOC3(OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                break;
            case 2:
                OpenOC4(OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                break;
            case 3:
                OpenOC5(OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                break;
            }
            onSquare[index] = TRUE;
        }
        else if(strcmp(state, "off") == 0)
        {
            onSquare[index] = FALSE;
            if(onTimer2 && (!onSquare[0] && !onSquare[1] && !onSquare[2] && !onSquare[3]))
            {
                CloseTimer2();
                onTimer2 = FALSE;
            }
            switch(index)
            {
            case 0:
                CloseOC1();
                break;
            case 1:
                CloseOC3();
                break;
            case 2:
                CloseOC4();
                break;
            case 3:
                CloseOC5();
                break;
            }
        }
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmEnable, ": wrong_argument_string");
    }
    else if(compareOSCAddress(msgGetPwmEnable))
    {
        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
        {
            index = getIntArgumentAtIndex(0);
            if(index > PWM_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetPwmEnable, ": out_of_value_range");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetPwmEnable, ": wrong_argument_type");
            return mrb_nil_value();
        }

        sendOSCMessage(getOSCPrefix(), msgPwmEnable, "is", index, onSquare[index] ? "on" : "off");
    }
    else if(compareOSCAddress(msgSetPwmFreq))
    {
        WORD index;
        WORD period;//syama0

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
            freq = getIntArgumentAtIndex(0);
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetPwmEnable, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(freq < 20)
        {
            freq = 20;
        }
        else if(freq > 20000)
        {
            freq = 20000;
        }
        T2CONbits.TON = 0;
        TMR2 = 0;
        OC1CONbits.OCM = 0b000;
        OC3CONbits.OCM = 0b000;
        OC4CONbits.OCM = 0b000;
        OC5CONbits.OCM = 0b000;

        width = GetSystemClock() / freq;
        PR2 = width;
        OC1RS = width / 2;
        OC3RS = width / 2;
        OC4RS = width / 2;
        OC5RS = width / 2;

        OC1CONbits.OCM = 0b110;
        OC3CONbits.OCM = 0b110;
        OC4CONbits.OCM = 0b110;
        OC5CONbits.OCM = 0b110;
        T2CONbits.TON = 1;
    }
    else if(compareOSCAddress(msgGetPwmFreq))
    {
        sendOSCMessage(getOSCPrefix(), msgPwmFreq, "i", freq);
    }
    else if(compareOSCAddress(msgSetPwmDuty))
    {
        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')))
        {
            index = getIntArgumentAtIndex(0);
            if(index > PWM_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDuty, ": out_of_value_range");
                return mrb_nil_value();
            }

            duty[index] = getIntArgumentAtIndex(1);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDuty, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(duty[index] < 0)
        {
            duty[index] = 0;
        }
        else if(duty[index] > 100)
        {
            duty[index] = 100;
        }
        T2CONbits.TON = 0;
        TMR2 = 0;
        switch(index)
        {
        case 0:
            OC1CONbits.OCM = 0b000;
            OC1RS = (LONG)(((float)duty[index] / 100.0) * (float)width);
            OC1CONbits.OCM = 0b110;
            break;
        case 1:
            OC3CONbits.OCM = 0b000;
            OC3RS = (LONG)(((float)duty[index] / 100.0) * (float)width);
            OC3CONbits.OCM = 0b110;
            break;
        case 2:
            OC4CONbits.OCM = 0b000;
            OC4RS = (LONG)(((float)duty[index] / 100.0) * (float)width);
            OC4CONbits.OCM = 0b110;
            break;
        case 3:
            OC5CONbits.OCM = 0b000;
            OC5RS = (LONG)(((float)duty[index] / 100.0) * (float)width);
            OC5CONbits.OCM = 0b110;
            break;
        }
        T2CONbits.TON = 1;
    }
    else if(compareOSCAddress(msgGetPwmDuty))
    {
        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
        {
            index = getIntArgumentAtIndex(0);
            if(index > PWM_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetPwmDuty, ": out_of_value_range");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetPwmDuty, ": wrong_argument_type");
            return mrb_nil_value();
        }

        sendOSCMessage(getOSCPrefix(), msgPwmDuty, "ii", index, duty[index]);
    }
    else if(compareOSCAddress(msgSetPwmDio))
    {
        BYTE id;
        char* type;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
        {
            id = getIntArgumentAtIndex(0);
            if(id > PWM_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDio, ": out_of_value_range");
                return mrb_nil_value();
            }
            type = getStringArgumentAtIndex(1);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDio, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(!strcmp(type, "in"))
            setPwmPortDioType(id, IO_IN);
        else if(!strcmp(type, "out"))
        {
            setPwmPortDioType(id, IO_OUT);
            outputPwmPort(id, LOW);
        }
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDio, ": wrong_argument_string");
    }
    else if(compareOSCAddress(msgGetPwmDio))
    {
        BYTE id;
        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
        {
            id = getIntArgumentAtIndex(0);
            if(id > PWM_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetPwmDio, ": out_of_value_range");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetPwmDio, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(getPwmPortDioType(id))
            sendOSCMessage(getOSCPrefix(), msgPwmDio, "is", id, "in");
        else
            sendOSCMessage(getOSCPrefix(), msgPwmDio, "is", id, "out");
    }
    else if(compareOSCAddress(msgSetPwmDo))
    {
        BYTE id;
        char* state;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
        {
            id = getIntArgumentAtIndex(0);
            if(id > PWM_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDo, ": out_of_value_range");
                return mrb_nil_value();
            }
            state = getStringArgumentAtIndex(1);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDo, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(!strcmp(state, "high"))
            outputPwmPort(id, HIGH);
        else if(!strcmp(state, "low"))
            outputPwmPort(id, LOW);
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDo, ": wrong_argument_string");
    }
    else if(compareOSCAddress(msgGetPwmDi))
    {
        BYTE id;
        BYTE state;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
        {
            id = getIntArgumentAtIndex(0);
            if(id > PWM_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetPwmDi, ": out_of_value_range");
                return mrb_nil_value();
            }

            state = inputPwmPort(id);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetPwmDi, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(state)
            sendOSCMessage(getOSCPrefix(), msgPwmDi, "is", id, "high");
        else
            sendOSCMessage(getOSCPrefix(), msgPwmDi, "is", id, "low");
    }
    // D/IO
    else if(compareOSCAddress(msgSetDigitalDio))
    {
        BYTE id;
        char* type;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
        {
            id = getIntArgumentAtIndex(0);
            if(id > D_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetDigitalDio, ": out_of_value_range");
                return mrb_nil_value();
            }

            type = getStringArgumentAtIndex(1);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetDigitalDio, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(!strcmp(type, "in"))
            setDigitalPortDioType(id, IO_IN);
        else if(!strcmp(type, "out"))
        {
            setDigitalPortDioType(id, IO_OUT);
            outputDigitalPort(id, LOW);
        }
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetDigitalDio, ": wrong_argument_string");
    }
    else if(compareOSCAddress(msgGetDigitalDio))
    {
        BYTE id;
        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
        {
            id = getIntArgumentAtIndex(0);
            if(id > D_NUM - 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetDigitalDio, ": out_of_value_range");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetDigitalDio, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(getDigitalPortDioType(id))
            sendOSCMessage(getOSCPrefix(), msgDigitalDio, "is", id, "in");
        else
            sendOSCMessage(getOSCPrefix(), msgDigitalDio, "is", id, "out");
    }
    else if(compareOSCAddress(msgSetDigitalDo))
    {
        BYTE id;
        char* state;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
        {
            id = getIntArgumentAtIndex(0);
            if(id > D_NUM - 1)
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetDigitalDo, ": out_of_value_range");

            state = getStringArgumentAtIndex(1);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetDigitalDo, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(!strcmp(state, "high"))
            outputDigitalPort(id, HIGH);
        else if(!strcmp(state, "low"))
            outputDigitalPort(id, LOW);
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetDigitalDo, ": wrong_argument_string");
    }
    else if(compareOSCAddress(msgGetDigitalDi))
    {
        BYTE id;
        BYTE state;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
        {
            id = getIntArgumentAtIndex(0);
            if(id >= D_NUM)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetDigitalDi, ": out_of_value_range");
                return mrb_nil_value();
            }

            state = inputDigitalPort(id);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetDigitalDi, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(state)
            sendOSCMessage(getOSCPrefix(), msgDigitalDi, "is", id, "high");
        else
            sendOSCMessage(getOSCPrefix(), msgDigitalDi, "is", id, "low");
    }
    // SPI
    else if(compareOSCAddress(msgSetSpiConfig))
    {
        BYTE id;
        DWORD bitrate0 = 0;
        WORD bitrate = 0;
        DWORD spiFlags = 0;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')))
        {
            id = getIntArgumentAtIndex(0);
            bitrate0 = getIntArgumentAtIndex(1);
            if(bitrate0 >= 2 && bitrate0 <= 1024)
            {
                if((bitrate0 % 2) == 0)
                    bitrate = (WORD)bitrate0;
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiConfig, ": 1:not_even_number");
                    return mrb_nil_value();
                }
            }
            else
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiConfig, ": 1:out_of_value_range");
                return mrb_nil_value();
            }

            if(getArgumentsLength() < 3)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiConfig, ": need_3_arguments_at_leaset");
                return mrb_nil_value();
            }

            for(i = 2; i < getArgumentsLength(); i++)
            {
                char* flag;
                if(compareTypeTagAtIndex(i, 's'))
                    flag = getStringArgumentAtIndex(i);

                if(!strcmp(flag, "msten"))
                    spiFlags |= SPICON_MSTEN;
                else if(!strcmp(flag, "ckp"))
                    spiFlags |= SPICON_CKP;
                else if(!strcmp(flag, "ssen"))
                    spiFlags |= SPICON_SSEN;
                else if(!strcmp(flag, "cke"))
                    spiFlags |= SPICON_CKE;
                else if(!strcmp(flag, "smp"))
                    spiFlags |= SPICON_SMP;
                else if(!strcmp(flag, "mode16"))
                    spiFlags |= SPICON_MODE16;
                else if(!strcmp(flag, "mode32"))
                    spiFlags |= SPICON_MODE32;
                else if(!strcmp(flag, "dissdo"))
                    spiFlags |= SPICON_DISSDO;
                else if(!strcmp(flag, "sidl"))
                    spiFlags |= SPICON_SIDL;
                else if(!strcmp(flag, "frz"))
                    spiFlags |= SPICON_FRZ;
                else if(!strcmp(flag, "on"))
                    spiFlags |= SPICON_ON;
                else if(!strcmp(flag, "spife"))
                    spiFlags |= SPICON_SPIFE;
                else if(!strcmp(flag, "frmpol"))
                    spiFlags |= SPICON_FRMPOL;
                else if(!strcmp(flag, "frmsync"))
                    spiFlags |= SPICON_FRMSYNC;
                else if(!strcmp(flag, "frmen"))
                    spiFlags |= SPICON_FRMEN;
            }

            switch(id)
            {
            case 2:
                SpiChnClose(2);
                SpiChnOpen(SPI_CHANNEL2, spiFlags, bitrate);
                break;
            case 4:
                SpiChnClose(4);
                SpiChnOpen(SPI_CHANNEL4, spiFlags, bitrate);
                break;
            default:
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiConfig, ": 0:out_of_value_range");
                return mrb_nil_value();
            }
        }
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiConfig, ": wrong_argument_type");
    }
    else if(compareOSCAddress(msgDisableSpi))
    {
        BYTE id;

        if(compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f'))
        {
            id = getIntArgumentAtIndex(0);
            switch(id)
            {
            case 2:
                SpiChnClose(2);
                break;
            case 4:
                SpiChnClose(4);
                break;
            default:
                sendOSCMessage(sysPrefix, msgError, "ss", msgDisableSpi, ": out_of_range_value");
                break;
            }
        }
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgDisableSpi, ": wrong_argument_type");
    }
    else if(compareOSCAddress(msgSetSpiData))
    {
        BYTE id = 0;
        char* load_port;
        char* active_state;
        BYTE byte_num = 0;
        WORD data[4] = {0};
        //DWORD usec = 0;

        if(getArgumentsLength() < 5)
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiData, "need_4_arguments_at_leaset");
            return mrb_nil_value();
        }

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's') && compareTypeTagAtIndex(2, 's') && (compareTypeTagAtIndex(3, 'i') || compareTypeTagAtIndex(3, 'f')))
        {
            id = getIntArgumentAtIndex(0);
            if(id != 2 && id != 4)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiData, ": 0:wrong_argument_value");
                return mrb_nil_value();
            }

            load_port = getStringArgumentAtIndex(1);
            if(!comparePortNameAtIndex(load_port))
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiData, ": 1:wrong_argument_string");
                return mrb_nil_value();
            }

            active_state = getStringArgumentAtIndex(2);
            if(strcmp(active_state, "HL") && strcmp(active_state, "LH"))
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiData, ": 2:wrong_argument_string");
                return mrb_nil_value();
            }

            byte_num = getIntArgumentAtIndex(3);
            if(byte_num != 1 && byte_num != 2 && byte_num != 4)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiData, ": 3:wrong_argument_value");
                return mrb_nil_value();
            }

            if(getArgumentsLength() < byte_num + 4)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiData, ": too_few_arguments");
                return mrb_nil_value();
            }

            for(i = 0; i < byte_num; i++)
            {
                if(!(compareTypeTagAtIndex(i + 4, 'i') || compareTypeTagAtIndex(i + 4, 'f')))
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiData, ": 4:wrong_argument_type");
                    return mrb_nil_value();
                }

                data[i] = getIntArgumentAtIndex(i + 4);
            }

            if(getPortIOType(load_port) == IO_IN)
                setPortIOType(load_port, IO_OUT);

            if(!strcmp(active_state, "HL"))
                outputPort(load_port, HIGH);
            else if(!strcmp(active_state, "LH"))
                outputPort(load_port, LOW);
            switch(byte_num)
            {
            case 1:
                sendSpiOneWord(id, data[0], 1);
                break;
            case 2:
                sendSpiTwoWord(id, data[0], data[1], 1);
                break;
            case 4:
                sendSpiFourWord(id, data[0], data[1], data[2], data[3], 1);
                break;
            }
            if(!strcmp(active_state, "HL"))
                outputPort(load_port, LOW);
            else if(!strcmp(active_state, "LH"))
                outputPort(load_port, HIGH);
        }
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiData, ": wrong_argument_type");
    }
    else if(compareOSCAddress(msgGetSpiData))
    {
        BYTE id = 0;
        char* load_port;
        char* active_state;
        BYTE byte_num = 0;
        WORD data[4] = {0};
        //DWORD usec = 0;

        if(getArgumentsLength() < 4)
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiData, "need_4_arguments_at_leaset");
            return mrb_nil_value();
        }

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's') && compareTypeTagAtIndex(2, 's') && (compareTypeTagAtIndex(3, 'i') || compareTypeTagAtIndex(3, 'f')))
        {
            id = getIntArgumentAtIndex(0);
            if(id != 2 && id != 4)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiData, ": 0:wrong_argument_value");
                return mrb_nil_value();
            }

            load_port = getStringArgumentAtIndex(1);
            if(!comparePortNameAtIndex(load_port))
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiData, ": 1:wrong_argument_string");
                return mrb_nil_value();
            }

            active_state = getStringArgumentAtIndex(2);
            if(strcmp(active_state, "HL") && strcmp(active_state, "LH"))
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiData, ": 2:wrong_argument_string");
                return mrb_nil_value();
            }

            byte_num = getIntArgumentAtIndex(3);
            if(byte_num != 1 && byte_num != 2 && byte_num != 4)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiData, ": 3:wrong_argument_value");
                return mrb_nil_value();
            }

            if((compareTypeTagAtIndex(4, 'i') || compareTypeTagAtIndex(4, 'f')))
            {
                data[0] = getIntArgumentAtIndex(4);
            }
            else
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiData, ": 4:wrong_argument_type");
                return mrb_nil_value();
            }

            if(!strcmp(active_state, "HL"))
                outputPort(load_port, HIGH);
            else if(!strcmp(active_state, "LH"))
                outputPort(load_port, LOW);
            switch(byte_num)
            {
            case 1:
                sendSpiOneWord(id, data[0], 1);
                data[0] = receiveSpiOneWord(id, 1);
                sendOSCMessage(getOSCPrefix(), msgSpiData, "ii", id, data[0]);
                break;
            case 2:
                //data = receiveSpiTwoWord(id, 8);
                break;
            case 4:
                //data = receiveSpiFourWord(id, 8);
                break;
            }
            if(!strcmp(active_state, "HL"))
                outputPort(load_port, LOW);
            else if(!strcmp(active_state, "LH"))
                outputPort(load_port, HIGH);
        }
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiData, ": wrong_argument_type");
    }
    else if(compareOSCAddress(msgSetSpiDio))
    {
        char* name;
        char* type;

        if(compareTypeTagAtIndex(0, 's') && compareTypeTagAtIndex(1, 's'))
        {
            name = getStringArgumentAtIndex(0);
            if(strcmp(name, "sck2") && strcmp(name, "sdi2") && strcmp(name, "sdo2") &&
               strcmp(name, "sck4") && strcmp(name, "sdi4") && strcmp(name, "sdo4"))
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiDio, ": wrong_argument_string");
                return mrb_nil_value();
            }
            type = getStringArgumentAtIndex(1);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiDio, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(!strcmp(type, "in"))
            setSpiPortDioType(name, IO_IN);
        else if(!strcmp(type, "out"))
        {
            setSpiPortDioType(name, IO_OUT);
            outputSpiPort(name, LOW);
        }
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiDio, ": wrong_argument_string");
    }
    else if(compareOSCAddress(msgGetSpiDio))
    {
        char* name;

        if(compareTypeTagAtIndex(0, 's'))
        {
            name = getStringArgumentAtIndex(0);
            if(strcmp(name, "sck2") && strcmp(name, "sdi2") && strcmp(name, "sdo2") &&
               strcmp(name, "sck4") && strcmp(name, "sdi4") && strcmp(name, "sdo4"))
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiDio, ": wrong_argument_string");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiDio, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(getSpiPortDioType(name))
            sendOSCMessage(getOSCPrefix(), msgSpiDio, "ss", name, "in");
        else
            sendOSCMessage(getOSCPrefix(), msgSpiDio, "ss", name, "out");
    }
    else if(compareOSCAddress(msgSetSpiDo))
    {
        char* name;
        char* state;

        if(compareTypeTagAtIndex(0, 's') && compareTypeTagAtIndex(1, 's'))
        {
            name = getStringArgumentAtIndex(0);
            if(strcmp(name, "sck4") && strcmp(name, "sdi4") && strcmp(name, "sdo4") &&
               strcmp(name, "sck2") && strcmp(name, "sdi2") && strcmp(name, "sdo2"))
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiDo, ": wrong_argument_string");
                return mrb_nil_value();
            }

            state = getStringArgumentAtIndex(1);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiDo, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(!strcmp(state, "high"))
            outputSpiPort(name, HIGH);
        else if(!strcmp(state, "low"))
            outputSpiPort(name, LOW);
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiDo, ": wrong_argument_string");
    }
    else if(compareOSCAddress(msgGetSpiDi))
    {
        char* name;
        BYTE state;

        if(compareTypeTagAtIndex(0, 's'))
        {
            name = getStringArgumentAtIndex(0);
            if(strcmp(name, "sck4") && strcmp(name, "sdi4") && strcmp(name, "sdo4") &&
               strcmp(name, "sck2") && strcmp(name, "sdi2") && strcmp(name, "sdo2"))
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiDi, ": wrong_argument_string");
                return mrb_nil_value();
            }

            state = inputSpiPort(name);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiDi, ": wrong_argument_type");
            return mrb_nil_value();
        }

        if(state)
            sendOSCMessage(getOSCPrefix(), msgSpiDi, "ss", name, "high");
        else
            sendOSCMessage(getOSCPrefix(), msgSpiDi, "ss", name, "low");
    }
    // I2C
    else if(compareOSCAddress(msgEnableI2c))
    {
        BYTE id;
        char* cFlag = NULL;
        BOOL flag = FALSE;

        if(compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f') && compareTypeTagAtIndex(1, 's'))
        {
            id = getIntArgumentAtIndex(0);
            cFlag = getStringArgumentAtIndex(1);
            if(!strcmp(cFlag, "true"))
                flag = TRUE;
            else if(!strcmp(cFlag, "false"))
                flag = FALSE;
            else
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgEnableI2c, ": wrong_argument_string");
                return mrb_nil_value();
            }

            switch(id)
            {
            case 3:
                setPortIOType("b2", IO_IN);
                setPortIOType("b3", IO_IN);
                I2CEnable(I2C3, flag);
                break;
            case 4:
                setPortIOType("g7", IO_IN);
                setPortIOType("g8", IO_IN);
                I2CEnable(I2C4, flag);
                break;
            case 5:
                setPortIOType("f4", IO_IN);
                setPortIOType("f5", IO_IN);
                I2CEnable(I2C5, flag);
                break;
            default:
                sendOSCMessage(sysPrefix, msgError, "ss", msgEnableI2c, ": out_of_range_value");
                break;
            }
        }
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgEnableI2c, ": wrong_argument_type");
    }
    else if(compareOSCAddress(msgSetI2cConfig))
    {
        BYTE id;
        DWORD i2cFlags = 0;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
        {
            id = getIntArgumentAtIndex(0);

            if(getArgumentsLength() < 1)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cConfig, ": need_1_arguments_at_leaset");
                return mrb_nil_value();
            }

            for(i = 1; i < getArgumentsLength(); i++)
            {
                char* flag;
                if(compareTypeTagAtIndex(i, 's'))
                    flag = getStringArgumentAtIndex(i);
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cConfig, ": wrong_argument_type");
                    return mrb_nil_value();
                }

                if(!strcmp(flag, "enable_slave_clock_stretching"))
                    i2cFlags |= I2C_ENABLE_SLAVE_CLOCK_STRETCHING;
                else if(!strcmp(flag, "enable_smb_support"))
                    i2cFlags |= I2C_ENABLE_SMB_SUPPORT;
                else if(!strcmp(flag, "enable_high_speed"))
                    i2cFlags |= I2C_ENABLE_HIGH_SPEED;
                else if(!strcmp(flag, "stop_in_idle"))
                    i2cFlags |= I2C_STOP_IN_IDLE;
            }

            switch(id)
            {
            case 3:
                I2CConfigure(I2C3, i2cFlags);
                break;
            case 4:
                I2CConfigure(I2C4, i2cFlags);
                break;
            case 5:
                I2CConfigure(I2C5, i2cFlags);
                break;
            default:
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cConfig, ": 0:out_of_value_range");
                return mrb_nil_value();
            }
        }
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cConfig, ": wrong_argument_type");
    }
    else if(compareOSCAddress(msgSetI2cFreq))
    {
        BYTE id;
        DWORD i2cFreq = 0;
        DWORD actualFreq = 0;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')))
        {
            id = getIntArgumentAtIndex(0);
            i2cFreq = getIntArgumentAtIndex(1);

            if(getArgumentsLength() < 2)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cConfig, ": need_2_arguments_at_leaset");
                return mrb_nil_value();
            }

            switch(id)
            {
            case 3:
                actualFreq = I2CSetFrequency(I2C3, GetPeripheralClock(), i2cFreq);
                break;
            case 4:
                actualFreq = I2CSetFrequency(I2C4, GetPeripheralClock(), i2cFreq);
                break;
            case 5:
                actualFreq = I2CSetFrequency(I2C5, GetPeripheralClock(), i2cFreq);
                break;
            default:
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cConfig, ": 0:out_of_value_range");
                return mrb_nil_value();
            }
            if((actualFreq - i2cFreq) > i2cFreq / 10)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cConfig, ": bad_frequency");
                return mrb_nil_value();
            }
        }
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cConfig, ": wrong_argument_type");
    }
    else if(compareOSCAddress(msgSetI2cSlaveAddress))
    {
        BYTE id;
        WORD address;
        WORD mask;
        DWORD slvAdrsFlags = 0;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) && (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')))
        {
            id = getIntArgumentAtIndex(0);
            address = getIntArgumentAtIndex(1);
            mask = getIntArgumentAtIndex(2);

            if(getArgumentsLength() < 4)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cSlaveAddress, ": need_4_arguments_at_leaset");
                return mrb_nil_value();
            }

            for(i = 3; i < getArgumentsLength(); i++)
            {
                char* flag;
                if(compareTypeTagAtIndex(i, 's'))
                    flag = getStringArgumentAtIndex(i);
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cSlaveAddress, ": wrong_argument_type");
                    return mrb_nil_value();
                }

                if(!strcmp(flag, "use_7bit_address"))
                    slvAdrsFlags |= I2C_USE_7BIT_ADDRESS;
                else if(!strcmp(flag, "use_10bit_address"))
                    slvAdrsFlags |= I2C_USE_10BIT_ADDRESS;
                else if(!strcmp(flag, "enable_general_call_address"))
                    slvAdrsFlags |= I2C_ENABLE_GENERAL_CALL_ADDRESS;
                else if(!strcmp(flag, "use_reserved_addresses"))
                    slvAdrsFlags |= I2C_USE_RESERVED_ADDRESSES;
            }

            switch(id)
            {
            case 3:
                I2CSetSlaveAddress(I2C3, address, mask, slvAdrsFlags);
                break;
            case 4:
                I2CSetSlaveAddress(I2C4, address, mask, slvAdrsFlags);
                break;
            case 5:
                I2CSetSlaveAddress(I2C5, address, mask, slvAdrsFlags);
                break;
            default:
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cSlaveAddress, ": 0:out_of_value_range");
                return mrb_nil_value();
            }
        }
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cSlaveAddress, ": wrong_argument_type");
    }
    else if(compareOSCAddress(msgSetI2cData))
    {
        BYTE id;
        BYTE slave_address;
        BYTE chip_address;
        WORD address = 0;
        DWORD data = 0;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) &&
           (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')) && (compareTypeTagAtIndex(3, 'i') || compareTypeTagAtIndex(3, 'f')) &&
           (compareTypeTagAtIndex(4, 'i') || compareTypeTagAtIndex(4, 'f')))
        {
            id = getIntArgumentAtIndex(0);
            slave_address = getIntArgumentAtIndex(1);
            chip_address = getIntArgumentAtIndex(2);
            address = getIntArgumentAtIndex(3);
            data = getIntArgumentAtIndex(4);

            switch(id)
            {
            case 3:
                if(!startI2C(I2C_3))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 0:");
                idleI2C(I2C_3);

                setAddressToI2C(I2C_3, slave_address | (chip_address << 1), 'w');
                idleI2C(I2C_3);

                if(!checkAckI2C(I2C_3))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 1:");

                idleI2C(I2C_3);
                setDataToI2C(I2C_3, address >> 8);
                idleI2C(I2C_3);

                if(!checkAckI2C(I2C_3))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 2:");

                idleI2C(I2C_3);
                setDataToI2C(I2C_3, address & 0x00FF);
                idleI2C(I2C_3);

                if(!checkAckI2C(I2C_3))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 3:");

                idleI2C(I2C_3);
                setDataToI2C(I2C_3, data);
                idleI2C(I2C_3);

                if(!checkAckI2C(I2C_3))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 4:");

                stopI2C(I2C_3);
                idleI2C(I2C_3);
                DelayMs(5);
                break;
            case 4:
                if(!startI2C(I2C_4))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 0:");
                idleI2C(I2C_4);

                setAddressToI2C(I2C_4, slave_address | (chip_address << 1), 'w');
                idleI2C(I2C_4);

                if(!checkAckI2C(I2C_4))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 1:");

                idleI2C(I2C_4);
                setDataToI2C(I2C_4, address >> 8);
                idleI2C(I2C_4);

                if(!checkAckI2C(I2C_4))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 2:");

                idleI2C(I2C_4);
                setDataToI2C(I2C_4, address & 0x00FF);
                idleI2C(I2C_4);

                if(!checkAckI2C(I2C_4))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 3:");

                idleI2C(I2C_4);
                setDataToI2C(I2C_4, data);
                idleI2C(I2C_4);

                if(!checkAckI2C(I2C_4))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 4:");

                stopI2C(I2C_4);
                idleI2C(I2C_4);
                DelayMs(5);
                break;
            case 5:
                if(!startI2C(I2C_5))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 0:");
                idleI2C(I2C_5);

                setAddressToI2C(I2C_5, slave_address | (chip_address << 1), 'w');
                idleI2C(I2C_5);

                if(!checkAckI2C(I2C_5))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 1:");

                idleI2C(I2C_5);
                setDataToI2C(I2C_5, address >> 8);
                idleI2C(I2C_5);

                if(!checkAckI2C(I2C_5))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 2:");

                idleI2C(I2C_5);
                setDataToI2C(I2C_5, address & 0x00FF);
                idleI2C(I2C_5);

                if(!checkAckI2C(I2C_5))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 3:");

                idleI2C(I2C_5);
                setDataToI2C(I2C_5, data);
                idleI2C(I2C_5);

                if(!checkAckI2C(I2C_5))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 4:");

                stopI2C(I2C_5);
                idleI2C(I2C_5);
                DelayMs(5);
                break;
            }
        }
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cData, ": wrong_argument_type");
    }
    else if(compareOSCAddress(msgGetI2cData))
    {
        BYTE id;
        BYTE slave_address = 0;
        BYTE chip_address = 0;
        WORD address = 0;
        BYTE data;

        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) && (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')) && (compareTypeTagAtIndex(3, 'i') || compareTypeTagAtIndex(3, 'f')))
        {
            id = getIntArgumentAtIndex(0);
            slave_address = getIntArgumentAtIndex(1);
            chip_address = getIntArgumentAtIndex(2);
            address = getIntArgumentAtIndex(3);

            switch(id)
            {
            case 3:
                if(!startI2C(I2C_3))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 0:");
                idleI2C(I2C_3);

                setAddressToI2C(I2C_3, slave_address | (chip_address << 1), 'w');
                idleI2C(I2C_3);

                if(!checkAckI2C(I2C_3))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 1:");

                idleI2C(I2C_3);
                setDataToI2C(I2C_3, address >> 8);
                idleI2C(I2C_3);

                if(!checkAckI2C(I2C_3))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 2:");

                idleI2C(I2C_3);
                setDataToI2C(I2C_3, address & 0x00FF);
                idleI2C(I2C_3);

                if(!checkAckI2C(I2C_3))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 3:");

                restartI2C(I2C_3);

                setAddressToI2C(I2C_3, slave_address | (chip_address << 1), 'r');
                idleI2C(I2C_3);

                if(!checkAckI2C(I2C_3))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 4:");

                data = getDataFromI2C(I2C_3);

                //I2C3CONbits.ACKDT = 1;
                //I2C3CONbits.ACKEN = 1;
                //while(I2C3CONbits.ACKEN == 1);

                stopI2C(I2C_3);
                idleI2C(I2C_3);

                sendOSCMessage(getOSCPrefix(), msgI2cData, "iii", id, address, data);
                break;
            case 4:
                if(!startI2C(I2C_4))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 0:");
                idleI2C(I2C_4);

                setAddressToI2C(I2C_4, slave_address | (chip_address << 1), 'w');
                idleI2C(I2C_4);

                if(!checkAckI2C(I2C_4))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 1:");

                idleI2C(I2C_4);
                setDataToI2C(I2C_4, address >> 8);
                idleI2C(I2C_4);

                if(!checkAckI2C(I2C_4))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 2:");

                idleI2C(I2C_4);
                setDataToI2C(I2C_4, address & 0x00FF);
                idleI2C(I2C_4);

                if(!checkAckI2C(I2C_4))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 3:");

                restartI2C(I2C_4);

                setAddressToI2C(I2C_4, slave_address | (chip_address << 1), 'r');
                idleI2C(I2C_4);

                if(!checkAckI2C(I2C_4))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 4:");

                data = getDataFromI2C(I2C_4);

                //I2C4CONbits.ACKDT = 1;
                //I2C4CONbits.ACKEN = 1;
                //while(I2C4CONbits.ACKEN == 1);
                            
                stopI2C(I2C_4);
                idleI2C(I2C_4);

                sendOSCMessage(getOSCPrefix(), msgI2cData, "iii", id, address, data);
                break;
            case 5:
                if(!startI2C(I2C_5))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 0:");
                idleI2C(I2C_5);

                setAddressToI2C(I2C_5, slave_address | (chip_address << 1), 'w');
                idleI2C(I2C_5);

                if(!checkAckI2C(I2C_5))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 1:");

                idleI2C(I2C_5);
                setDataToI2C(I2C_5, address >> 8);
                idleI2C(I2C_5);

                if(!checkAckI2C(I2C_5))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 2:");

                idleI2C(I2C_5);
                setDataToI2C(I2C_5, address & 0x00FF);
                idleI2C(I2C_5);

                if(!checkAckI2C(I2C_5))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 3:");

                restartI2C(I2C_5);

                setAddressToI2C(I2C_5, slave_address | (chip_address << 1), 'r');
                idleI2C(I2C_5);

                if(!checkAckI2C(I2C_5))
                    sendOSCMessage(sysPrefix, msgError, "s", "I2C Error 4:");

                data = getDataFromI2C(I2C_5);

                //I2C5CONbits.ACKDT = 1;
                //I2C5CONbits.ACKEN = 1;
                //while(I2C5CONbits.ACKEN == 1);

                stopI2C(I2C_5);
                idleI2C(I2C_5);

                sendOSCMessage(getOSCPrefix(), msgI2cData, "iii", id, address, data);
                break;
            }
        }
        else
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetI2cData, ": wrong_argument_type");
    }
    return mrb_nil_value();
}

mrb_value mrb_process_midi_messages(mrb_state* mrb, mrb_value self)
{
    if(compareOSCAddress(msgSetNote)) // Note On/Off
    {
        BYTE ch = getIntArgumentAtIndex(0);
        BYTE num = getIntArgumentAtIndex(1);
        BYTE vel = getIntArgumentAtIndex(2);

        OSCTranslatedToUSB.Val = 0;
        OSCTranslatedToUSB.CableNumber = 0;
        OSCTranslatedToUSB.CodeIndexNumber = MIDI_CIN_NOTE_ON;
        OSCTranslatedToUSB.DATA_0 = 0x90 + ch;
        OSCTranslatedToUSB.DATA_1 = num;
        OSCTranslatedToUSB.DATA_2 = vel;
        
        putHostMIDIPacket(OSCTranslatedToUSB);
    }
    else if(compareOSCAddress(msgSetCc)) // Control Change
    {
        BYTE ch = getIntArgumentAtIndex(0);
        BYTE num = getIntArgumentAtIndex(1);
        BYTE val = getIntArgumentAtIndex(2);
        
        OSCTranslatedToUSB.Val = 0;
        OSCTranslatedToUSB.CableNumber = 0;
        OSCTranslatedToUSB.CodeIndexNumber = MIDI_CIN_CONTROL_CHANGE;
        OSCTranslatedToUSB.DATA_0 = 0xB0 + ch;
        OSCTranslatedToUSB.DATA_1 = num;
        OSCTranslatedToUSB.DATA_2 = val;

        putHostMIDIPacket(OSCTranslatedToUSB);
    }
    else if(compareOSCAddress(msgSetKp)) // Polyphonic Key Pressure
    {
        BYTE ch = getIntArgumentAtIndex(0);
        BYTE num = getIntArgumentAtIndex(1);
        BYTE prs = getIntArgumentAtIndex(2);
        
        OSCTranslatedToUSB.Val = 0;
        OSCTranslatedToUSB.CableNumber = 0;
        OSCTranslatedToUSB.CodeIndexNumber = MIDI_CIN_POLY_KEY_PRESS;
        OSCTranslatedToUSB.DATA_0 = 0xA0 + ch;
        OSCTranslatedToUSB.DATA_1 = num;
        OSCTranslatedToUSB.DATA_2 = prs;

        putHostMIDIPacket(OSCTranslatedToUSB);        
    }
    else if(compareOSCAddress(msgSetPc)) // Program Change
    {
        BYTE ch = getIntArgumentAtIndex(0);
        BYTE pnum = getIntArgumentAtIndex(1);
        
        OSCTranslatedToUSB.Val = 0;
        OSCTranslatedToUSB.CableNumber = 0;
        OSCTranslatedToUSB.CodeIndexNumber = MIDI_CIN_PROGRAM_CHANGE;
        OSCTranslatedToUSB.DATA_0 = 0xC0 + ch;
        OSCTranslatedToUSB.DATA_1 = pnum;
        
        putHostMIDIPacket(OSCTranslatedToUSB);
    }
    else if(compareOSCAddress(msgSetCp)) // Channel Pressure
    {
        BYTE ch = getIntArgumentAtIndex(0);
        BYTE prs = getIntArgumentAtIndex(1);
        
        OSCTranslatedToUSB.Val = 0;
        OSCTranslatedToUSB.CableNumber = 0;
        OSCTranslatedToUSB.CodeIndexNumber = MIDI_CIN_CHANNEL_PREASURE;
        OSCTranslatedToUSB.DATA_0 = 0xD0 + ch;
        OSCTranslatedToUSB.DATA_1 = prs;
     
        putHostMIDIPacket(OSCTranslatedToUSB);   
    }
    else if(compareOSCAddress(msgSetPb)) // Pitch Bend
    {
        BYTE ch = getIntArgumentAtIndex(0);
        BYTE msb = getIntArgumentAtIndex(1);
        BYTE lsb = getIntArgumentAtIndex(2);
        
        OSCTranslatedToUSB.Val = 0;
        OSCTranslatedToUSB.CableNumber = 0;
        OSCTranslatedToUSB.CodeIndexNumber = MIDI_CIN_PITCH_BEND_CHANGE;
        OSCTranslatedToUSB.DATA_0 = 0xE0 + ch;
        OSCTranslatedToUSB.DATA_1 = msb;
        OSCTranslatedToUSB.DATA_2 = lsb;
        
        putHostMIDIPacket(OSCTranslatedToUSB);
    }
    
    return mrb_nil_value();
}

#if defined(USB_USE_CDC)
mrb_value mrb_process_cdc_messages(mrb_state* mrb, mrb_value self)
{
    BYTE i;

    if(compareOSCAddress(msgSetData))
    {
        WORD data = 0;
        cdcOutDataLength = getArgumentsLength();
        if(cdcOutDataLength >= MAX_NO_OF_OUT_BYTES)
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetData, ": too_many_arguments");
            return mrb_nil_value();
        }
        
        for(i = 0; i < cdcOutDataLength; i++)
        {
            data = getIntArgumentAtIndex(i);
            if(data > 255)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetData, ": out_of_range_value");
                return mrb_nil_value();
            }
            USB_CDC_OUT_Data_Array[i] = (BYTE)data;
        }
        
        cdcSendFlag = TRUE;
        setApplCDCState(READY_TO_TX_RX);
        
        //_USBHostCDC_TerminateTransfer(USB_SUCCESS);
    }

    return mrb_nil_value();
}
#endif

mrb_value mrb_process_system_messages(mrb_state* mrb, mrb_value self)
{
    int i;

    // System Setting
    if(compareOSCAddress(msgSetRemoteIp))
    {
        char* rip;
        WORD ip[4] = {0};
        
        if(compareTypeTagAtIndex(0, 's'))
            rip = getStringArgumentAtIndex(0);
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetRemoteIp, ": wrong_argument_type");
            return mrb_nil_value();
        }
        
        ip[0] = atoi(strtok(rip, "."));
        if(rip == NULL)
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetRemoteIp, ": wrong_argument_string");
            return mrb_nil_value();
        }
        if(ip[0] > 255)
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetRemoteIp, ": wrong_argument_string");
            return mrb_nil_value();
        }
        ip[1] = atoi(strtok(NULL, "."));
        if(rip == NULL)
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetRemoteIp, ": wrong_argument_string");
            return mrb_nil_value();
        }
        if(ip[1] > 255)
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetRemoteIp, ": wrong_argument_string");
            return mrb_nil_value();
        }
        ip[2] = atoi(strtok(NULL, "."));
        if(rip == NULL)
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetRemoteIp, ": wrong_argument_string");
            return mrb_nil_value();
        }
        if(ip[2] > 255)
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetRemoteIp, ": wrong_argument_string");
            return mrb_nil_value();
        }
        ip[3] = atoi(strtok(NULL, "."));
        if(ip[3] > 255)
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetRemoteIp, ": wrong_argument_string");
            return mrb_nil_value();
        }
        setRemoteIpAtIndex(0, (BYTE)ip[0]);
        setRemoteIpAtIndex(1, (BYTE)ip[1]);
        setRemoteIpAtIndex(2, (BYTE)ip[2]);
        setRemoteIpAtIndex(3, (BYTE)ip[3]);
        setInitSendFlag(FALSE);
        setChCompletedFlag(TRUE);
        closeOSCSendPort();
    }
    else if(compareOSCAddress(msgGetRemoteIp))
    {
        char rip[15];
        sprintf(rip, "%d.%d.%d.%d", getRemoteIpAtIndex(0), getRemoteIpAtIndex(1), getRemoteIpAtIndex(2), getRemoteIpAtIndex(3));
        sendOSCMessage(sysPrefix, msgRemoteIp, "s", rip);
    }
    else if(compareOSCAddress(msgSetRemotePort))
    {
        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
        {
            if(getIntArgumentAtIndex(0) < 0)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgGetRemoteIp, ": out_of_value_range");
                return mrb_nil_value();
            }
            setRemotePort(getIntArgumentAtIndex(0));
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgGetRemoteIp, ": wrong_argument_type");
            return mrb_nil_value();
        }
        
        closeOSCSendPort();
        setInitSendFlag(FALSE);
        setChCompletedFlag(TRUE);
    }
    else if(compareOSCAddress(msgGetRemotePort))
    {
        sendOSCMessage(sysPrefix, msgRemotePort, "i", getRemotePort());
    }
    else if(compareOSCAddress(msgSetHostName))
    {
        char* srcName = NULL;
        char* np = NULL;
        int len = 0;
        
        if(compareTypeTagAtIndex(0, 's'))
        {
            srcName = getStringArgumentAtIndex(0);
            np = strstr(srcName, ".local");
            if(np != NULL)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetHostName, ": please_delete_.local");
                return mrb_nil_value();
            }
            
            //len = np - srcName;
            //if(len >= 32)
            if(strlen(srcName) >= 32)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetHostName, ": too_long_string");
                return mrb_nil_value();
            }
            
            mDNSServiceDeRegister();
            clearOSCHostName();
            setOSCHostName(srcName);
            
            //debug sendOSCMessage(sysPrefix, msgError, "ss", srcName, hostName);
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetHostName, ": wrong_argument_type");
            return mrb_nil_value();
        }
        
        mDNSServiceDeRegister();
        
        mDNSInitialize(getOSCHostName());
        mDNSServiceRegister((const char *)getOSCHostName(), // base name of the service
                            "_oscit._udp.local",    // type of the service
                            8080,                   // TCP or UDP port, at which this service is available
                            ((const BYTE *)""),     // TXT info
                            1,                      // auto rename the service when if needed
                            NULL,                   // no callback function
                            NULL                    // no application context
            );
        mDNSMulticastFilterRegister();
        dwLastIP = 0;
        sendOSCMessage(sysPrefix, msgConfiguration, "s", "succeeded");
    }
    else if(compareOSCAddress(msgGetHostName))
    {
        char fullHostName[32] = {0};
        sprintf(fullHostName, "%s.local", getOSCHostName());
        sendOSCMessage(sysPrefix, msgHostName, "s", fullHostName);
    }
    else if(compareOSCAddress(msgGetHostIp))
    {
        char hip[15] = {0};
        BYTE hip0 = AppConfig.MyIPAddr.Val & 0xFF;
        BYTE hip1 = (AppConfig.MyIPAddr.Val >> 8) & 0xFF;
        BYTE hip2 = (AppConfig.MyIPAddr.Val >> 16) & 0xFF;
        BYTE hip3 = (AppConfig.MyIPAddr.Val >> 24) & 0xFF;
        sprintf(hip, "%d.%d.%d.%d", hip0, hip1, hip2, hip3);
        sendOSCMessage(sysPrefix, msgHostIp, "s", hip);
    }
    else if(compareOSCAddress(msgGetHostMac))
    {
        char macaddr[18] = {0};
        sprintf(macaddr, "%02X:%02X:%02X:%02X:%02X:%02X", AppConfig.MyMACAddr.v[0], AppConfig.MyMACAddr.v[1], AppConfig.MyMACAddr.v[2], AppConfig.MyMACAddr.v[3], AppConfig.MyMACAddr.v[4], AppConfig.MyMACAddr.v[5]);
        sendOSCMessage(sysPrefix, msgHostMac, "s", macaddr);
    }
    else if(compareOSCAddress(msgSetHostPort))
    {
        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
        {
            if(getIntArgumentAtIndex(0) < 0)
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetHostPort, ": out_of_value_range");
                return mrb_nil_value();
            }
            setLocalPort(getIntArgumentAtIndex(0));
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetHostPort, ": wrong_argument_type");
            return mrb_nil_value();
        }
        
        closeOSCReceivePort();
        setInitReceiveFlag(FALSE);
        setChCompletedFlag(TRUE);
    }
    else if(compareOSCAddress(msgGetHostPort))
    {
        sendOSCMessage(sysPrefix, msgHostPort, "i", getLocalPort());
    }
    else if(compareOSCAddress(msgSetPrefix))
    {
        if(compareTypeTagAtIndex(0, 's'))
        {
            char* str = getStringArgumentAtIndex(0);
            if(str[0] != '/')
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSetPrefix, ": must_begin_slash");
                return mrb_nil_value();
            }
            clearOSCPrefix();
            setOSCPrefix(getStringArgumentAtIndex(0));
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSetPrefix, ": wrong_argument_type");
            return mrb_nil_value();
        }
        sendOSCMessage(sysPrefix, msgConfiguration, "s", "succeeded");
    }
    else if(compareOSCAddress(msgGetPrefix))
    {
        sendOSCMessage(sysPrefix, msgPrefix, "s", getOSCPrefix());
    }
    else if(compareOSCAddress(msgDiscoverDevices))
    {
        char hip[22], rip[24], macaddr[26], hport[14], rport[16];
        
        LED_1_On();
        
        mT5IntEnable(0);
        
        closeOSCSendPort();
        
        sprintf(hip, "HostIP=%d.%d.%d.%d", AppConfig.MyIPAddr.v[0], AppConfig.MyIPAddr.v[1], AppConfig.MyIPAddr.v[2], AppConfig.MyIPAddr.v[3]);
        sprintf(rip, "RemoteIP=%d.%d.%d.%d", getRemoteIpAtIndex(0), getRemoteIpAtIndex(1), getRemoteIpAtIndex(2), getRemoteIpAtIndex(3));
        sprintf(macaddr, "HostMAC=%02X:%02X:%02X:%02X:%02X:%02X", AppConfig.MyMACAddr.v[0], AppConfig.MyMACAddr.v[1], AppConfig.MyMACAddr.v[2], AppConfig.MyMACAddr.v[3], AppConfig.MyMACAddr.v[4], AppConfig.MyMACAddr.v[5]);
        sprintf(hport, "HostPort=%d", getLocalPort());
        sprintf(rport, "RemotePort=%d", getRemotePort());
        
        while(!getInitDiscoverFlag())
            setInitDiscoverFlag(openDiscoverPort());
        
        while(!isDiscoverPutReady());
        
        sendOSCMessage(sysPrefix, msgDiscoveredDevice, "sssss", hip, macaddr, hport, rip, rport);
        
        closeDiscoverPort();
        
        mT5IntEnable(1);
        
        LED_1_Off();
    }
    else if(compareOSCAddress(msgSwitchUsbMode))
    {
        char* dm;
        if(compareTypeTagAtIndex(0, 's'))
            dm = getStringArgumentAtIndex(0);
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSwitchUsbMode, ": wrong_argument_type");
            return mrb_nil_value();
        }
        
        if(!strcmp(dm, "host"))
        {
            if(device_mode == MODE_DEVICE)
                USBSoftDetach();
            
            device_mode = MODE_HOST;
        }
        else if(!strcmp(dm, "device"))
        {
            device_mode = MODE_DEVICE;
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSwitchUsbMode, ": out_of_value_range");
            return mrb_nil_value();
        }
        sendOSCMessage(sysPrefix, msgConfiguration, "s", "succeeded");
    }
    else if(compareOSCAddress(msgGetUsbMode))
    {
        if(device_mode == MODE_DEVICE)
            sendOSCMessage(sysPrefix, msgUsbMode, "s", "device");
        else if(device_mode == MODE_HOST)
            sendOSCMessage(sysPrefix, msgUsbMode, "s", "host");
    }
    else if(compareOSCAddress(msgWriteNvmData))
    {
        if(getArgumentsLength() < 2)
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgWriteNvmData, ": too_few_arguments");
            return mrb_nil_value();
        }
        if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')))
        {
            WORD address = getIntArgumentAtIndex(0);
            WORD data = getIntArgumentAtIndex(1);
            
            if(address >= 1 && address <=7)
            {
                if(!NVMWriteWord((void*)(NVM_DATA + 512 * address), data))
                    sendOSCMessage(sysPrefix, msgNvmData, "s", "write:succeeded");
                else
                    sendOSCMessage(sysPrefix, msgNvmData, "s", "write:failed");
            }
            else
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgWriteNvmData, ": out_of_range_address");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgWriteNvmData, ": wrong_argument_type");
            return mrb_nil_value();
        }
    }
    else if(compareOSCAddress(msgReadNvmData))
    {
        if(getArgumentsLength() < 1)
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgReadNvmData, ": too_few_arguments");
            return mrb_nil_value();
        }
        if(compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f'))
        {
            DWORD tmpData = 0;
            WORD address = getIntArgumentAtIndex(0);
            
            if(address >= 1 && address <=7)
            {
                tmpData = *(DWORD *)(NVM_DATA + 512 * address);
                sendOSCMessage(sysPrefix, msgNvmData, "ii", address, tmpData);
            }
            else
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgReadNvmData, ": out_of_range_address");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgReadNvmData, ": wrong_argument_type");
            return mrb_nil_value();
        }
    }
    else if(compareOSCAddress(msgClearNvmData))
    {
        BYTE index, checkstate = 0;
        NVMErasePage((void *)NVM_DATA);
        for(index = 1; index <= 7; index++)
        {
            checkstate += NVMWriteWord((void*)(NVM_DATA + 512 * index), 0);
            DelayMs(1);
        }
        if(!checkstate)
            sendOSCMessage(sysPrefix, msgNvmData, "s", "clear:succeeded");
        else
            sendOSCMessage(sysPrefix, msgNvmData, "s", "clear:failed");
    }
    else if(compareOSCAddress(msgSoftReset))
    {
        char* reset_mode;
        if(compareTypeTagAtIndex(0, 's'))
        {
            reset_mode = getStringArgumentAtIndex(0);
            if(strcmp(reset_mode, "normal") && strcmp(reset_mode, "bootloader"))
            {
                sendOSCMessage(sysPrefix, msgError, "ss", msgSoftReset, ": out_of_value_range");
                return mrb_nil_value();
            }
        }
        else
        {
            sendOSCMessage(sysPrefix, msgError, "ss", msgSoftReset, ": wrong_argument_type");
            return mrb_nil_value();
        }
        
        if(!strcmp(reset_mode, "bootloader"))
        {
            NVMErasePage((void*)NVM_DATA);
            NVMWriteWord((void*)(NVM_DATA), (unsigned int)0x01);
        }
        SoftReset();
    }
    else if(compareOSCAddress(msgDebug))
    {
        //debug testNum++;
        
        clearOSCBundle();
        for(i = 0; i < getArgumentsLength(); i++)
        {
            if(compareTypeTagAtIndex(i, 'i'))
                appendOSCMessageToBundle(sysPrefix, msgDebug, "ii", i, getIntArgumentAtIndex(i));
            else if(compareTypeTagAtIndex(i, 'f'))
            {
                appendOSCMessageToBundle(sysPrefix, msgDebug, "if", i, getFloatArgumentAtIndex(i));
                
                //debug appendOSCMessageToBundle(sysPrefix, msgDebug, "ifi", i, getFloatArgumentAtIndex(i), testNum - 1);
                //debug testNum = 0;
            }
            else if(compareTypeTagAtIndex(i, 's'))
                appendOSCMessageToBundle(sysPrefix, msgDebug, "is", i, getStringArgumentAtIndex(i));
            else if(compareTypeTagAtIndex(i, 'T'))
                appendOSCMessageToBundle(sysPrefix, msgDebug, "iT", i);
            else if(compareTypeTagAtIndex(i, 'F'))
                appendOSCMessageToBundle(sysPrefix, msgDebug, "iF", i);
            else if(compareTypeTagAtIndex(i, 'N'))
                appendOSCMessageToBundle(sysPrefix, msgDebug, "iN", i);
            else if(compareTypeTagAtIndex(i, 'I'))
                appendOSCMessageToBundle(sysPrefix, msgDebug, "iI", i);
        }
        sendOSCBundle();
    }
    else if(compareOSCAddress(msgGetVersion))
    {
        sendOSCMessage(sysPrefix, msgVersion, "s", CURRENT_VERSION);
    }

    return mrb_nil_value();
}

mrb_value mrb_set_osc_address(mrb_state* mrb, mrb_value self)
{
    mrb_value prefix;
    mrb_value address;

    mrb_get_args(mrb, "SS", &prefix, &address);

    setOSCAddress(RSTRING_PTR(prefix), RSTRING_PTR(address));
    return mrb_nil_value();
}

mrb_value mrb_set_osc_typetag(mrb_state* mrb, mrb_value self)
{
    mrb_value typetag;

    mrb_get_args(mrb, "S", &typetag);

    setOSCTypeTag(RSTRING_PTR(typetag));
    return mrb_nil_value();
}

mrb_value mrb_add_osc_int_arg(mrb_state* mrb, mrb_value self)
{
    mrb_int value;

    mrb_get_args(mrb, "i", &value);

    addOSCIntArgument(value);
    return mrb_nil_value();
}

mrb_value mrb_add_osc_float_arg(mrb_state* mrb, mrb_value self)
{
    mrb_float value;

    mrb_get_args(mrb, "f", &value);

    addOSCFloatArgument(value);
    return mrb_nil_value();
}

mrb_value mrb_add_osc_string_arg(mrb_state* mrb, mrb_value self)
{
    mrb_value str;

    mrb_get_args(mrb, "S", &str);

    addOSCStringArgument(RSTRING_PTR(str));
    return mrb_nil_value();
}

mrb_value mrb_clear_osc_message(mrb_state* mrb, mrb_value self)
{
    clearOSCMessage();
    return mrb_nil_value();
}

mrb_value mrb_flush_osc_message(mrb_state* mrb, mrb_value self)
{
    flushOSCMessage();
    return mrb_nil_value();
}

mrb_value mrb_get_osc_packet(mrb_state* mrb, mrb_value self)
{
    getOSCPacket();
    return mrb_nil_value();
}

mrb_value mrb_process_osc_packet(mrb_state* mrb, mrb_value self)
{
    mrb_value mflag;
    BOOL cflag = FALSE;
    cflag = processOSCPacket();
    if(cflag)
        mflag = mrb_true_value();
    else
        mflag = mrb_false_value();
    return mflag;
}

mrb_value mrb_compare_osc_prefix(mrb_state* mrb, mrb_value self)
{
    mrb_value prefix;
    mrb_value mflag;
    BOOL cflag = FALSE;

    mrb_get_args(mrb, "S", &prefix);

    cflag = compareOSCPrefix(RSTRING_PTR(prefix));
    if(cflag)
        mflag = mrb_true_value();
    else
        mflag = mrb_false_value();
    return mflag;
}

mrb_value mrb_compare_osc_address(mrb_state* mrb, mrb_value self)
{
    mrb_value address;
    mrb_value mflag;
    BOOL cflag = FALSE;

    mrb_get_args(mrb, "S", &address);

    cflag = compareOSCAddress(RSTRING_PTR(address));
    if(cflag)
        mflag = mrb_true_value();
    else
        mflag = mrb_false_value();
    return mflag;
}

mrb_value mrb_compare_typetag_at_index(mrb_state* mrb, mrb_value self)
{
    mrb_int index;
    mrb_value typetag;
    mrb_value mflag;
    BOOL cflag = FALSE;
    char* ttstr;

    mrb_get_args(mrb, "iS", &index, &typetag);
    ttstr = RSTRING_PTR(typetag);
    cflag = compareTypeTagAtIndex(index, ttstr[0]);
    if(cflag)
        mflag = mrb_true_value();
    else
        mflag = mrb_false_value();
    return mflag;
}

mrb_value mrb_get_int_arg_at_index(mrb_state* mrb, mrb_value self)
{
    mrb_int index;
    mrb_value mvalue;
    int value;

    mrb_get_args(mrb, "i", &index);

    value = getIntArgumentAtIndex(index);

    return mrb_fixnum_value(value);
}

mrb_value mrb_get_float_arg_at_index(mrb_state* mrb, mrb_value self)
{
    mrb_int index;
    mrb_value mvalue;
    float value;

    mrb_get_args(mrb, "i", &index);

    value = getFloatArgumentAtIndex(index);

    return mrb_fixnum_value(value);
}

mrb_value mrb_get_string_arg_at_index(mrb_state* mrb, mrb_value self)
{
    mrb_int index;
    mrb_value mvalue;
    char* str;

    mrb_get_args(mrb, "i", &index);

    str = getStringArgumentAtIndex(index);

    return mrb_str_new_cstr(mrb, str);
}

void __ISR(_TIMER_5_VECTOR, IPL5) sendOSCTask(void)
{
    int i, j;

    static unsigned int count = 0;

    if(!getInitSendFlag())
    {
        setInitSendFlag(openOSCSendPort(getRemoteIp(), getRemotePort()));
        if(getChCompletedFlag())
        {
            sendOSCMessage(sysPrefix, msgConfiguration, "s", "succeeded");
            setChCompletedFlag(FALSE);
        }
    }

    if(getInitSendFlag())
    {
        switch(sendOSCTaskIndex)
        {
            case 0:
                swState1 = SW_State();

#if 0//debug
                if(count == 100)
                {
                    if(swState1)
                    {
                        //mT5IntEnable(0);
                        LED_1_Toggle();
                        //int ai = mrb_gc_arena_save(mrb);
                        //mrb_funcall(mrb, mrb_top_self(mrb), "send_osc_task", 0);
                        //mrb_gc_arena_restore(mrb, ai);
                        //mT5IntEnable(1);
                    }
                    else
                    {
                        LED_2_Toggle();
                        //mrb_funcall(mrb, mrb_top_self(mrb), "sendOSCTask", 0);
                    }
                    count = 0;
                }
                count++;
#endif//debug

                if(swState1 != swState0)
                {
                    if(swState1)
                        sendOSCMessage(getOSCPrefix(), msgOnboardSw1, "s", "off");
                    else
                        sendOSCMessage(getOSCPrefix(), msgOnboardSw1, "s", "on");
                }
                swState0 = swState1;

                sendOSCTaskIndex = 1;
                break;
            case 1:
                j = 0;
                for(i = 0; i < AN_NUM; i++)
                {
                    if(getAnalogEnable(i))
                    {
                        analogInHandle(i, (LONG)ReadADC10(j));
                        j++;
                    }
                }
                sendAdc();
                sendOSCTaskIndex = 0;
                break;
#if defined(USE_LED_PAD) 
                sendOSCTaskIndex = 2;
                break;
           case 2:
                //for(i = 0; i < getNumConnectedAbsEnc(); i++)
                //{
                //    incEncoderHandle(i);
                //}
                sendPad16();

                sendOSCTaskIndex = 3;
                break;
#endif
#if defined(USE_LED_ENC)
            case 3:
                for(i = 0; i < getNumConnectedAbsEnc(); i++)
                {
                    incEncoderHandle(i);
                    sendEncInc32(i);
                }
                
                sendOSCTaskIndex = 4;
                break;
            case 4:
                absEncoderHandle();
                for(i = 0; i < getNumConnectedAbsEnc(); i++)
                {
                    sendEncAbs32(i);
                }
                
                sendOSCTaskIndex = 0;
                break;
#endif
            default:
                sendOSCTaskIndex = 0;
                break;
        }
    }
    mT5ClearIntFlag();
}

mrb_value mrb_usb_device_init(mrb_state* mrb, mrb_value self)
{
    USBDeviceInit();

    return mrb_nil_value();
}

mrb_value mrb_usb_device_tasks(mrb_state* mrb, mrb_value self)
{
    USBDeviceTasks();

    return mrb_nil_value();
}

mrb_value mrb_hid_ctrl_task(mrb_state* mrb, mrb_value self)
{
    BYTE u8Data[128] = {0};

    // User Application USB tasks
    if(!checkUSBState())
        return mrb_nil_value();

    if(!hidHandleBusy())
    {
        hidRxOnePacket();
        if(getRcvHidDataBuffer(0) == 0x80)
        {
            //LED_1_Toggle();
        }
        switch(getRcvHidDataBuffer(0))
        {
            case 0x80:// ADC Enable
                if(getRcvHidDataBuffer(1))
                {
                    AD1CON1bits.ON = 0;

                    int i;
                    WORD anum = 0;
                    WORD id = getRcvHidDataBuffer(2);
                    BYTE state = getRcvHidDataBuffer(3);

                    if(id > 13 || state > 1)
                        return mrb_nil_value();

                    if(state == 1)
                        setAnalogEnable(id, TRUE);
                    else if(state == 0)
                        setAnalogEnable(id, FALSE);

                    for(i = 0; i < AN_NUM; i++)
                    {
                        if(getAnalogEnable(i))
                            anum++;
                    }
                    setAnPortDioType(id, IO_IN);

                    if(state == 1)
                    {
                        AD1PCFG &= ~(0x0001 << id);
                        AD1CSSL |= (0x0001 << id);
                    }
                    else
                    {
                        AD1PCFG |= (0x0001 << id);
                        AD1CSSL &= ~(0x0001 << id);
                    }

                    if(anum > 0)
                    {
                        AD1CON2 = 0x00000400;// 0000 0000 0000 0000 0000 0000 0000 0000
                        AD1CON2 |= ((anum - 1) << 2);
                        AD1CON3 = 0x00001F08;// 0000 0000 0000 0000 0000 1111 0000 1000
                        AD1CHS  = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
                        AD1CON1 = 0x000080E6;// 0000 0000 0000 0000 1000 0000 1110 0110
                    }
                    else
                    {
                        AD1PCFG = 0x0000FFFF;// 0000 0000 0000 0000 1111 1111 1111 1111
                        AD1CSSL = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000

                        AD1CON2 = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
                        AD1CON1 = 0x00000000;// 0000 0000 0000 0000 1000 0000 0000 0000
                        AD1CHS  = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
                        AD1CON3 = 0x00001F08;// 0000 0000 0000 0000 0000 1111 0000 1000
                    }
                }
                else
                {
                    BYTE id = getRcvHidDataBuffer(2);

                    if(id > 13)
                        return mrb_nil_value();

                    setSndHidDataBuffer(0, 0x80);
                    setSndHidDataBuffer(1,3);
                    setSndHidDataBuffer(2, getRcvHidDataBuffer(2));
                    setSndHidDataBuffer(3, getAnalogEnable(getRcvHidDataBuffer(2)));
                    setSndHidDataBuffer(4, getAnalogByte(getRcvHidDataBuffer(2), BYTE_ORIGINAL));
                    setSndHidDataBuffer(5, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                break;
            case 0x81:
                if(getRcvHidDataBuffer(1))
                {
                    if(getRcvHidDataBuffer(3) == 0)
                    {
                        setAnPortDioType(getRcvHidDataBuffer(2), IO_IN);
                    }
                    else if(getRcvHidDataBuffer(3) == 1)
                    {
                        setAnPortDioType(getRcvHidDataBuffer(2), IO_OUT);
                    }
                }
                else
                {
                    BYTE id = getRcvHidDataBuffer(2);

                    if(id > 13)
                        return mrb_nil_value();

                    setSndHidDataBuffer(0, 0x81);
                    setSndHidDataBuffer(1, 2);
                    setSndHidDataBuffer(2, getRcvHidDataBuffer(2));
                    setSndHidDataBuffer(3, ioAnPort[getRcvHidDataBuffer(2)]);
                    setSndHidDataBuffer(5, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                break;
            case 0x82:
                if(getRcvHidDataBuffer(1) == 1)
                {
                    outputAnPort(getRcvHidDataBuffer(2), getRcvHidDataBuffer(3));
                }
                else if(getRcvHidDataBuffer(1) == 0)
                {
                    setSndHidDataBuffer(0, 0x82);
                    setSndHidDataBuffer(1, 2);
                    setSndHidDataBuffer(2, getRcvHidDataBuffer(2));
                    setSndHidDataBuffer(3, inputAnPort(getRcvHidDataBuffer(2)));
                    setSndHidDataBuffer(4, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                break;
            case 0x83:
                if(getRcvHidDataBuffer(1) == 1)
                {
                    BYTE index = getRcvHidDataBuffer(2);
                    if(index > 4)
                        return mrb_nil_value();

                    if(getRcvHidDataBuffer(3) == 1)
                    {
                        LONG w = (LONG)(((float)duty[index] / 100.0) * (float)width);
                        if(!onTimer2)
                        {
                            OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_1, width);
                            onTimer2 = TRUE;
                        }
                        switch(index)
                        {
                            case 0:
                                OpenOC1(OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                                break;
                            case 1:
                                OpenOC3(OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                                break;
                            case 2:
                                OpenOC4(OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                                break;
                            case 3:
                                OpenOC5(OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                                break;
                        }
                        onSquare[index] = TRUE;
                    }
                    else if(getRcvHidDataBuffer(3) == 0)
                    {
                        onSquare[index] = FALSE;
                        if(onTimer2 && (!onSquare[0] && !onSquare[1] && !onSquare[2] && !onSquare[3]))
                        {
                            CloseTimer2();
                            onTimer2 = FALSE;
                        }
                        switch(index)
                        {
                            case 0:
                                CloseOC1();
                                break;
                            case 1:
                                CloseOC3();
                                break;
                            case 2:
                                CloseOC4();
                                break;
                            case 3:
                                CloseOC5();
                                break;
                        }
                    }
                }
                else if(getRcvHidDataBuffer(1) == 0)
                {
                    BYTE index = getRcvHidDataBuffer(2);

                    setSndHidDataBuffer(0, 0x83);
                    setSndHidDataBuffer(1, 2);
                    setSndHidDataBuffer(2, index);
                    setSndHidDataBuffer(3, onSquare[index]);
                    setSndHidDataBuffer(4, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                break;
            case 0x84:
                if(getRcvHidDataBuffer(1) == 1)
                {
                    freq = (getRcvHidDataBuffer(2) << 8) | getRcvHidDataBuffer(3);
                    if(freq < 20)
                    {
                        freq = 20;
                    }
                    else if(freq > 20000)
                    {
                        freq = 20000;
                    }
                    T2CONbits.TON = 0;
                    TMR2 = 0;
                    OC1CONbits.OCM = 0b000;
                    width = GetSystemClock() / freq;
                    PR2 = width;
                    OC1RS = width / 2;
                    OC1CONbits.OCM = 0b110;
                    T2CONbits.TON = 1;
                }
                else if(getRcvHidDataBuffer(1) == 0)
                {
                    setSndHidDataBuffer(0, 0x84);
                    setSndHidDataBuffer(1, 2);
                    setSndHidDataBuffer(2, (BYTE)(freq >> 8));
                    setSndHidDataBuffer(3, (BYTE)freq);
                    setSndHidDataBuffer(4, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                break;
            case 0x85:
                if(getRcvHidDataBuffer(1) == 1)
                {
                    BYTE index = getRcvHidDataBuffer(2);
                    duty[index] = getRcvHidDataBuffer(3);
                    if(duty[index] < 0)
                    {
                        duty[index] = 0;
                    }
                    else if(duty[index] > 100)
                    {
                        duty[index] = 100;
                    }
                    T2CONbits.TON = 0;
                    TMR2 = 0;
                    switch(index)
                    {
                        case 0:
                            OC1CONbits.OCM = 0b000;
                            OC1RS = (LONG)(((float)duty[index] / 100.0) * (float)width);
                            OC1CONbits.OCM = 0b110;
                            break;
                        case 1:
                            OC3CONbits.OCM = 0b000;
                            OC3RS = (LONG)(((float)duty[index] / 100.0) * (float)width);
                            OC3CONbits.OCM = 0b110;
                            break;
                        case 2:
                            OC4CONbits.OCM = 0b000;
                            OC4RS = (LONG)(((float)duty[index] / 100.0) * (float)width);
                            OC4CONbits.OCM = 0b110;
                            break;
                        case 3:
                            OC5CONbits.OCM = 0b000;
                            OC5RS = (LONG)(((float)duty[index] / 100.0) * (float)width);
                            OC5CONbits.OCM = 0b110;
                            break;
                    }
                    T2CONbits.TON = 1;
                }
                else if(getRcvHidDataBuffer(1) == 0)
                {
                    BYTE index = getRcvHidDataBuffer(2);

                    setSndHidDataBuffer(0, 0x85);
                    setSndHidDataBuffer(1, 2);
                    setSndHidDataBuffer(2, index);
                    setSndHidDataBuffer(3, duty[index]);
                    setSndHidDataBuffer(4, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                break;
            case 0x86:
                if(getRcvHidDataBuffer(1) == 1)
                {
                    BYTE id = getRcvHidDataBuffer(2);
                    BYTE type = getRcvHidDataBuffer(3);

                    if(type == 0)
                    {
                        setPwmPortDioType(id, IO_IN);
                    }
                    else if(type == 1)
                    {
                        setPwmPortDioType(id, IO_OUT);
                    }
                }
                else
                {
                    BYTE id = getRcvHidDataBuffer(2);

                    if(id > 3)
                        return mrb_nil_value();

                    setSndHidDataBuffer(0, 0x86);
                    setSndHidDataBuffer(1, 2);
                    setSndHidDataBuffer(2, getRcvHidDataBuffer(2));
                    setSndHidDataBuffer(3, ioPwmPort[getRcvHidDataBuffer(2)]);
                    setSndHidDataBuffer(5, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                break;
            case 0x87:
                if(getRcvHidDataBuffer(1) == 1)
                {
                    BYTE id = getRcvHidDataBuffer(2);
                    BYTE state = getRcvHidDataBuffer(3);
                    outputPwmPort(id, state);
                }
                else if(getRcvHidDataBuffer(1) == 0)
                {
                    BYTE id = getRcvHidDataBuffer(2);
                    BYTE state = inputPwmPort(id);

                    setSndHidDataBuffer(0, 0x87);
                    setSndHidDataBuffer(1, 2);
                    setSndHidDataBuffer(2, id);
                    setSndHidDataBuffer(3, state);
                    setSndHidDataBuffer(4, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                break;
            case 0x88:
                if(getRcvHidDataBuffer(1) == 1)
                {
                    BYTE id = getRcvHidDataBuffer(2);
                    BYTE type = getRcvHidDataBuffer(3);

                    if(type == 0)
                    {
                        setDigitalPortDioType(id, IO_IN);
                    }
                    else if(type == 1)
                    {
                        setDigitalPortDioType(id, IO_OUT);
                    }
                }
                else
                {
                    BYTE id = getRcvHidDataBuffer(2);

                    if(id > 3)
                        return mrb_nil_value();

                    setSndHidDataBuffer(0, 0x88);
                    setSndHidDataBuffer(1, 2);
                    setSndHidDataBuffer(2, getRcvHidDataBuffer(2));
                    setSndHidDataBuffer(3, ioDPort[getRcvHidDataBuffer(2)]);
                    setSndHidDataBuffer(5, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                break;
            case 0x89:
                if(getRcvHidDataBuffer(1) == 1)
                {
                    BYTE id = getRcvHidDataBuffer(2);
                    BYTE state = getRcvHidDataBuffer(3);
                    outputDigitalPort(id, state);
                }
                else if(getRcvHidDataBuffer(1) == 0)
                {
                    BYTE id = getRcvHidDataBuffer(2);
                    BYTE state = inputDigitalPort(id);

                    setSndHidDataBuffer(0, 0x89);
                    setSndHidDataBuffer(1, 2);
                    setSndHidDataBuffer(2, id);
                    setSndHidDataBuffer(3, state);
                    setSndHidDataBuffer(4, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                break;
            case 0x8A:
                if(getRcvHidDataBuffer(1) == 1)
                {
                    BYTE id = getRcvHidDataBuffer(2);
                    BYTE type = getRcvHidDataBuffer(3);

                    switch(id)
                    {
                        case 0:
                            if(type == 0)
                                setSpiPortDioType("sck4", IO_IN);
                            else if(type == 1)
                                setSpiPortDioType("sck4", IO_OUT);
                            break;
                        case 1:
                            if(type == 0)
                                setSpiPortDioType("sdi4", IO_IN);
                            else if(type == 1)
                                setSpiPortDioType("sdi4", IO_OUT);
                            break;
                        case 2:
                            if(type == 0)
                                setSpiPortDioType("sdo4", IO_IN);
                            else if(type == 1)
                                setSpiPortDioType("sdo4", IO_OUT);
                            break;
                        case 3:
                            if(type == 0)
                                setSpiPortDioType("sck2", IO_IN);
                            else if(type == 1)
                                setSpiPortDioType("sck2", IO_OUT);
                            break;
                        case 4:
                            if(type == 0)
                                setSpiPortDioType("sdi2", IO_IN);
                            else if(type == 1)
                                setSpiPortDioType("sdi2", IO_OUT);
                            break;
                        case 5:
                            if(type == 0)
                                setSpiPortDioType("sdo2", IO_IN);
                            else if(type == 1)
                                setSpiPortDioType("sdo2", IO_OUT);
                            break;
                    }
                }
                else
                {
                    BYTE id = getRcvHidDataBuffer(2);

                    if(id > 5)
                        return mrb_nil_value();

                    setSndHidDataBuffer(0, 0x8A);
                    setSndHidDataBuffer(1, 2);
                    setSndHidDataBuffer(2, getRcvHidDataBuffer(2));
                    setSndHidDataBuffer(3, ioSpiPort[getRcvHidDataBuffer(2)]);
                    setSndHidDataBuffer(5, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                break;
            case 0x8B:
                if(getRcvHidDataBuffer(1) == 1)
                {
                    BYTE id = getRcvHidDataBuffer(2);
                    BYTE state = getRcvHidDataBuffer(3);
                    switch(id)
                    {
                        case 0:
                            outputSpiPort("sck4", state);
                            break;
                        case 1:
                            outputSpiPort("sdi4", state);
                            break;
                        case 2:
                            outputSpiPort("sdo4", state);
                            break;
                        case 3:
                            outputSpiPort("sck2", state);
                            break;
                        case 4:
                            outputSpiPort("sdi2", state);
                            break;
                        case 5:
                            outputSpiPort("sdo2", state);
                            break;
                    }
                }
                else if(getRcvHidDataBuffer(1) == 0)
                {
                    BYTE id = getRcvHidDataBuffer(2);
                    BYTE state = 0;
                    switch(id)
                    {
                        case 0:
                            state = inputSpiPort("sck4");
                            break;
                        case 1:
                            state = inputSpiPort("sdi4");
                            break;
                        case 2:
                            state = inputSpiPort("sdo4");
                            break;
                        case 3:
                            state = inputSpiPort("sck2");
                            break;
                        case 4:
                            state = inputSpiPort("sdi2");
                            break;
                        case 5:
                            state = inputSpiPort("sdo2");
                            break;
                    }

                    setSndHidDataBuffer(0, 0x8B);
                    setSndHidDataBuffer(1, 2);
                    setSndHidDataBuffer(2, id);
                    setSndHidDataBuffer(3, state);
                    setSndHidDataBuffer(4, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                break;
            case 0xF0:// Host IP
                if(!getRcvHidDataBuffer(1))
                {
                    setSndHidDataBuffer(0, 0xF0);
                    setSndHidDataBuffer(1, 4);
                    setSndHidDataBuffer(2, (BYTE)((AppConfig.MyIPAddr.Val >> 0) & 0x000000FF));
                    setSndHidDataBuffer(3, (BYTE)((AppConfig.MyIPAddr.Val >> 8) & 0x000000FF));
                    setSndHidDataBuffer(4, (BYTE)((AppConfig.MyIPAddr.Val >> 16) & 0x000000FF));
                    setSndHidDataBuffer(5, (BYTE)((AppConfig.MyIPAddr.Val >> 24) & 0x000000FF));
                    setSndHidDataBuffer(6, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                break;
            case 0xF1:// Remote IP
                if(!getRcvHidDataBuffer(1))
                {
                    setSndHidDataBuffer(0, 0xF1);
                    setSndHidDataBuffer(1, 4);
                    setSndHidDataBuffer(2, getRemoteIpAtIndex(0));
                    setSndHidDataBuffer(3, getRemoteIpAtIndex(1));
                    setSndHidDataBuffer(4, getRemoteIpAtIndex(2));
                    setSndHidDataBuffer(5, getRemoteIpAtIndex(3));
                    setSndHidDataBuffer(6, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                else
                {
                    setRemoteIpAtIndex(0, getRcvHidDataBuffer(2));
                    setRemoteIpAtIndex(1, getRcvHidDataBuffer(3));
                    setRemoteIpAtIndex(2, getRcvHidDataBuffer(4));
                    setRemoteIpAtIndex(3, getRcvHidDataBuffer(5));
                    setInitSendFlag(FALSE);
                    closeOSCSendPort();
                }
                break;
            case 0xF2:// Local Port
                if(!getRcvHidDataBuffer(1))
                {
                    setSndHidDataBuffer(0, 0xF2);
                    setSndHidDataBuffer(1, 2);
                    setSndHidDataBuffer(2, (BYTE)((getLocalPort() >> 0) & 0x00FF));
                    setSndHidDataBuffer(3, (BYTE)((getLocalPort() >> 8) & 0x00FF));
                    setSndHidDataBuffer(4, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                else
                {
                    setLocalPort(getRcvHidDataBuffer(2) | (getRcvHidDataBuffer(3) << 8));
                    setInitReceiveFlag(FALSE);
                    closeOSCReceivePort();
                }
                break;
            case 0xF3:// Remote Port
                if(!getRcvHidDataBuffer(1))
                {
                    setSndHidDataBuffer(0, 0xF3);
                    setSndHidDataBuffer(1, 2);
                    setSndHidDataBuffer(2, (BYTE)((getRemotePort() >> 0) & 0x00FF));
                    setSndHidDataBuffer(3, (BYTE)((getRemotePort() >> 8) & 0x00FF));
                    setSndHidDataBuffer(4, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                else
                {
                    setRemotePort(getRcvHidDataBuffer(2) | (getRcvHidDataBuffer(3) << 8));
                    setInitSendFlag(FALSE);
                    closeOSCSendPort();
                }
                break;
            case 0xF4:// Mac Address
                if(!getRcvHidDataBuffer(1))
                {
                    setSndHidDataBuffer(0, 0xF4);
                    setSndHidDataBuffer(1, 6);
                    setSndHidDataBuffer(2, AppConfig.MyMACAddr.v[0]);
                    setSndHidDataBuffer(3, AppConfig.MyMACAddr.v[1]);
                    setSndHidDataBuffer(4, AppConfig.MyMACAddr.v[2]);
                    setSndHidDataBuffer(5, AppConfig.MyMACAddr.v[3]);
                    setSndHidDataBuffer(6, AppConfig.MyMACAddr.v[4]);
                    setSndHidDataBuffer(7, AppConfig.MyMACAddr.v[5]);
                    setSndHidDataBuffer(8, 0x00);

                    if(!hidHandleBusy())
                        hidTxOnePacket();
                }
                break;
            case 0xF5:// Soft Reset
                if(getRcvHidDataBuffer(1))
                {
                    NVMErasePage((void*)NVM_DATA);
                    NVMWriteWord((void*)(NVM_DATA), (unsigned int)0x01);
                }
                SoftReset();
                break;
#if 0//test
            case 0xF6:// Save Current Settings
                if(getRcvHidDataBuffer(1])
                {
                    u8Data[0] = getRemoteIpAtIndex(0);
                    u8Data[1] = getRemoteIpAtIndex(1);
                    u8Data[2] = getRemoteIpAtIndex(2);
                    u8Data[3] = getRemoteIpAtIndex(3);
                    u8Data[4] = (BYTE)((getLocalPort() >> 0) & 0x00FF);
                    u8Data[5] = (BYTE)((getLocalPort() >> 8) & 0x00FF);
                    u8Data[6] = (BYTE)((getRemotePort() >> 0) & 0x00FF);
                    u8Data[7] = (BYTE)((getRemotePort() >> 8) & 0x00FF);

                    //NVMWriteRow((void *)NVM_PROGRAM_PAGE, (void *)u8Data);
                }
                else
                {
                    //memcpy(u8Data, (void *)NVM_PROGRAM_PAGE, 8);

                    setRemoteIpAtIndex(0, u8Data[0]);
                    setRemoteIpAtIndex(1, u8Data[1]);
                    setRemoteIpAtIndex(2, u8Data[2]);
                    setRemoteIpAtIndex(3, u8Data[3]);
                    setLocalPort((WORD)u8Data[4] | ((WORD)u8Data[5] << 8));
                    setRemotePort((WORD)u8Data[6] | ((WORD)u8Data[7] << 8));

                    if(!hidHandleBusy())
                    {
                        ToSendHidDataBuffer[0] = 0xF6;
                        ToSendHidDataBuffer[1] = getRemoteIpAtIndex(0);
                        ToSendHidDataBuffer[2] = getRemoteIpAtIndex(1);
                        ToSendHidDataBuffer[3] = getRemoteIpAtIndex(2);
                        ToSendHidDataBuffer[4] = getRemoteIpAtIndex(3);
                        ToSendHidDataBuffer[5] = (BYTE)((getLocalPort() >> 0) & 0x00FF);
                        ToSendHidDataBuffer[6] = (BYTE)((getLocalPort() >> 8) & 0x00FF);
                        ToSendHidDataBuffer[7] = (BYTE)((getRemotePort() >> 0) & 0x00FF);
                        ToSendHidDataBuffer[8] = (BYTE)((getRemotePort() >> 8) & 0x00FF);
                        ToSendHidDataBuffer[9] = 0x00;

                        hidTxOnePacket();
                    }
                }
                break;
#endif//test
            case 0xF7:
                break;
        }
    }

    return mrb_nil_value();
}

mrb_value mrb_send_note(mrb_state* mrb, mrb_value self)
{
    mrb_int num, vel, ch;

    mrb_get_args(mrb, "iii", &num, &vel, &ch);

    midiData.Val = 0;
    midiData.CableNumber = 0;
    midiData.CodeIndexNumber = MIDI_CIN_NOTE_ON;
    midiData.DATA_0 = 0x90 + ch;
    midiData.DATA_1 = num;
    midiData.DATA_2 = vel;

    if(!midiHandleBusy())
        midiTxOnePacket();
    //delayUs(2);

    return mrb_nil_value();
}

mrb_value mrb_send_cc(mrb_state* mrb, mrb_value self)
{
    mrb_int num, val, ch;

    mrb_get_args(mrb, "iii", &num, &val, &ch);

    midiData.Val = 0;
    midiData.CableNumber = 0;
    midiData.CodeIndexNumber = MIDI_CIN_CONTROL_CHANGE;
    midiData.DATA_0 = 0xB0 + ch;

    midiData.DATA_1 = num;
    midiData.DATA_2 = val;

    if(!midiHandleBusy())
        midiTxOnePacket();
    //delayUs(20);

    return mrb_nil_value();
}

mrb_value mrb_receive_midi_datas(mrb_state* mrb, mrb_value self)
{
    int i;
    BYTE ch, num, val;

    if(checkUSBState())
        return mrb_nil_value();

    if(!midiHandleBusy())
    {
        midiRxOnePacket();

        //MIDI Note
        i = 1;
        while((getRcvMidiDataBuffer(i) & 0xF0) == 0x90)
        {
            ch = getRcvMidiDataBuffer(i) & 0x0F;
            num = getRcvMidiDataBuffer(i + 1);
            val = getRcvMidiDataBuffer(i + 2);

            setRcvMidiDataBuffer(i, getRcvMidiDataBuffer(i) & 0x00);
            i += 4;
            continue;
        }

        //MIDI CC
        i = 1;
        while((getRcvMidiDataBuffer(i) & 0xF0) == 0xB0)
        {
            ch = getRcvMidiDataBuffer(i) & 0x0F;
            num = getRcvMidiDataBuffer(i + 1);
            val = getRcvMidiDataBuffer(i + 2);

            setRcvMidiDataBuffer(i, getRcvMidiDataBuffer(i) & 0x00);
            i += 4;
            continue;
        }
    }

    return mrb_nil_value();
}

mrb_value mrb_usb_host_init(mrb_state* mrb, mrb_value self)
{
    mrb_int bUsbHostInitialized;

    bUsbHostInitialized = USBHostInit(0);

    return mrb_fixnum_value(bUsbHostInitialized);
}

mrb_value mrb_usb_host_tasks(mrb_state* mrb, mrb_value self)
{
    USBTasks();

    return mrb_nil_value();
}

mrb_value mrb_convert_midi_to_osc(mrb_state* mrb, mrb_value self)
{
    HOST_MIDI_PACKET hostMidiPacket;

    hostMidiPacket = getHostMIDIPacket();

    if(getInitSendFlag() && hostMidiPacket.rcvFlag)
    {
        switch(hostMidiPacket.type)
        {
        case 0x80:// Note off
        case 0x90:// Note on
            sendOSCMessage(midiPrefix, msgNote, "iii", hostMidiPacket.ch, hostMidiPacket.num, hostMidiPacket.val);
            break;
        case 0xA0:// Key Pressure
            sendOSCMessage(midiPrefix, msgKp, "iii", hostMidiPacket.ch, hostMidiPacket.num, hostMidiPacket.val);
            break;
        case 0xB0:// Control Change
            sendOSCMessage(midiPrefix, msgCc, "iii", hostMidiPacket.ch, hostMidiPacket.num, hostMidiPacket.val);
            break;
        case 0xC0:// Program Change
            sendOSCMessage(midiPrefix, msgPc, "i", hostMidiPacket.num);
            break;
        case 0xD0:// Channel Pressure
            sendOSCMessage(midiPrefix, msgCp, "ii", hostMidiPacket.ch, hostMidiPacket.num);
            break;
        case 0xE0:// Pitch Bend
            sendOSCMessage(midiPrefix, msgPb, "iii", hostMidiPacket.ch, hostMidiPacket.num, hostMidiPacket.val);
            break;
        default:
            break;
        }
    }
    return mrb_nil_value();
}

#if defined(USB_USE_CDC)
void USBHostCDC_Clear_Out_DATA_Array(void)
{
    BYTE i;

    for(i = 0; i < MAX_NO_OF_OUT_BYTES; i++)
        USB_CDC_OUT_Data_Array[i] = 0;
}

mrb_value mrb_usb_cdc_rxtx_handler(mrb_state* mrb, mrb_value self)
{
    BYTE i;

    if(!USBHostCDC_ApiDeviceDetect()) /* TRUE if device is enumerated without error */
    {
        setApplCDCState(DEVICE_NOT_CONNECTED);
    }

    switch(getApplCDCState())
    {
        case DEVICE_NOT_CONNECTED:
            USBTasks();
            if(USBHostCDC_ApiDeviceDetect()) /* TRUE if device is enumerated without error */
            {
                setApplCDCState(DEVICE_CONNECTED);
            }
            break;
        case DEVICE_CONNECTED:
            setApplCDCState(READY_TO_TX_RX);
            break;
        case GET_IN_DATA:
            if(USBHostCDC_Api_Get_IN_Data(MAX_NO_OF_IN_BYTES, USB_CDC_IN_Data_Array))
            {
                setApplCDCState(GET_IN_DATA_WAIT);
            }
            else
            {
                setApplCDCState(READY_TO_TX_RX);
            }
            break;
        case GET_IN_DATA_WAIT:
            if(USBHostCDC_ApiTransferIsComplete(&ErrorDriver, &NumOfBytesRcvd))
            {
                if(!ErrorDriver)
                {
                    if(NumOfBytesRcvd > 0)
                    {
                        for(i = 0; i < NumOfBytesRcvd; i++)
                        {
                            sendOSCMessage(cdcPrefix, msgData, "iii", i, NumOfBytesRcvd, USB_CDC_IN_Data_Array[i]);
                        }
                    }
                    setApplCDCState(READY_TO_TX_RX);
                }
                else
                {
                    setApplCDCState(READY_TO_TX_RX);
                }
            }
            break;
        case SEND_OUT_DATA:
            if(USBHostCDC_Api_Send_OUT_Data(cdcOutDataLength, USB_CDC_OUT_Data_Array))
            {
                setApplCDCState(SEND_OUT_DATA_WAIT);
            }
            else
            {
                setApplCDCState(READY_TO_TX_RX);
            }
            break;
        case SEND_OUT_DATA_WAIT:
            if(USBHostCDC_ApiTransferIsComplete(&ErrorDriver, &NumOfBytesRcvd))
            {
                USBHostCDC_Clear_Out_DATA_Array();
                setApplCDCState(READY_TO_TX_RX);

                cdcSendFlag = FALSE;
            }
            break;
        case READY_TO_TX_RX:

            if(cdcSendFlag)
            {
                setApplCDCState(SEND_OUT_DATA);
                //cdcSendFlag = FALSE;
            }
            else
            {
                setApplCDCState(READY_TO_TX_RX);
            }
            break;
        default :
            break;
    }

    if(cdcReceiveInterval < 16384)
        cdcReceiveInterval++;
    else if(cdcReceiveInterval == 16384)
    {
        if(getApplCDCState() == READY_TO_TX_RX)
            setApplCDCState(GET_IN_DATA);
        cdcReceiveInterval = 0;
    }

    return mrb_nil_value();
}
#endif
