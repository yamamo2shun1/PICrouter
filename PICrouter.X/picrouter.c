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
 * picrouter.c,v.1.9.0 2013/08/28
 */

#include "picrouter.h"

static BYTE sendOSCTaskIndex = 0;
static BYTE swState0 = 0;
static BYTE swState1 = 0;

void _general_exception_handler(unsigned cause, unsigned status)
{
    Nop();
    Nop();
}

int main(int argc, char** argv) {
    int i;
    BYTE eth_state = 0;

    // Enable optimal performance
    SYSTEMConfigPerformance(GetSystemClock());
    mOSCSetPBDIV(OSC_PB_DIV_1); // Use 1:1 CPU Core:Peripheral clocks

    // Enable the cache for the best performance
    CheKseg0CacheOn();

    DelayMs(100);
    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

    OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_8, TIMER4_COUNT);
    ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_6);

    OpenTimer5(T5_ON | T5_SOURCE_INT | T5_PS_1_8, TIMER5_COUNT);
    ConfigIntTimer5(T5_INT_ON | T5_INT_PRIOR_5);

    // Port Initialization
    initIOPorts();
    initAnalogVariables();
    initEncoderVariables();
    
    initSW();
    initLEDs();
    buttonInit();

    stateFlag = FALSE;
    prevState = 1;
    currentState = 1;

    LED_1_On();
    LED_2_On();
    DelayMs(200);

    // PWM
    freq = 10000; // 10kHz
    width = GetSystemClock() / freq;
    for(i = 0; i < PWM_NUM; i++)
        duty[i] = 50;

    setOSCPrefix("/std");
    setOSCHostName(DEFAULT_HOST_NAME);

    // USB Initialization
    #if defined(USE_USB_BUS_SENSE_IO)
        tris_usb_bus_sense = 1;
    #endif

    #if defined(USE_SELF_POWER_SENSE_IO)
        tris_self_power = 1;
    #endif

    TickInit();
    InitAppConfig();
    StackInit();
    ZeroconfLLInitialize();
    mDNSInitialize(DEFAULT_HOST_NAME);
    mDNSServiceRegister((const char *)DEFAULT_HOST_NAME, // base name of the service
                        "_oscit._udp.local",       // type of the service
                        8080,                      // TCP or UDP port, at which this service is available
                        ((const BYTE *)""),        // TXT info
                        1,                         // auto rename the service when if needed
                        NULL,                      // no callback function
                        NULL                       // no application context
                        );
    mDNSMulticastFilterRegister();

    // Enable multi-vectored interrupts
    INTEnableSystemMultiVectoredInt();

    LED_1_Off();
    LED_2_Off();
    DelayMs(200);

    while(1)
    {
        currentState = SW_State();

        if(prevState != currentState)
        {
            stateFlag = TRUE;
            stateFlag2 = TRUE;
        }

        switch(device_mode)
        {
            case MODE_DEVICE:
                for(i = 0; i < 3; i++)
                {
                    LED_1_On();
                    DelayMs(200);
                    LED_1_Off();
                    DelayMs(200);
                }

                USBDeviceInit();

                while(device_mode == MODE_DEVICE)
                {
                    // Ethernet Tasks
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
                        case 1:// 2:
                            mDNSProcess();
                            eth_state = 2;// 3;
                            break;
                        case 2:// 3:
                            DHCPServerTask();
                            eth_state = 0;
                            break;
                    }
                    if(dwLastIP != AppConfig.MyIPAddr.Val)
                    {
                        dwLastIP = AppConfig.MyIPAddr.Val;
                        mDNSFillHostRecord();
                    }
                    
                    receiveOSCTask();

                    // USB Tasks
                    USBDeviceTasks();
                    USBControlTask();

                    if(U1OTGSTATbits.SESVD == 0)
                    {
                        USBSoftDetach();
                        //test device_mode = MODE_UNKNOWN;
                    }
                }
                break;
            case MODE_HOST:
                for(i = 0; i < 3; i++)
                {
                    LED_2_On();
                    DelayMs(200);
                    LED_2_Off();
                    DelayMs(200);
                }

                while(device_mode == MODE_HOST)
                {
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
                        case 1:// 2:
                            mDNSProcess();
                            eth_state = 2;// 3;
                            break;
                        case 2:// 3:
                            DHCPServerTask();
                            eth_state = 0;
                            break;
                    }
                    if(dwLastIP != AppConfig.MyIPAddr.Val)
                    {
                        dwLastIP = AppConfig.MyIPAddr.Val;
                        mDNSFillHostRecord();
                    }

                    receiveOSCTask();

                    if(bUsbHostInitialized)
                    {
                        USBTasks();

                        convertMidiToOsc();

#if defined(USB_USE_HID)
                        convertHidToOsc();
#elif defined(USB_USE_CDC)
                        USB_CDC_RxTxHandler();

                        if(cdcReceiveInterval < 16384)
                            cdcReceiveInterval++;
                        else if(cdcReceiveInterval == 16384)
                        {
                            if(APPL_CDC_State == READY_TO_TX_RX)
                                APPL_CDC_State = GET_IN_DATA;
                            cdcReceiveInterval = 0;
                        }
#endif
                    }
                    else
                    {
                        bUsbHostInitialized = USBHostInit(0);
                    }
                }
                break;
            default:
                break;
        }        
    }
    return (EXIT_SUCCESS);
}

/**********************************************
*  OSC Generic I/O Processing Part
**********************************************/
/*******************************************************************************
  Function:
    void initIOPorts(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void initIOPorts(void)
{
    int i = 0;

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
}

/*******************************************************************************
  Function:
    void receiveOSCTask(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void receiveOSCTask(void)
{
    BYTE index;
    int i, j, k;
    //debug static BYTE testNum = 0;

    //mT4IntEnable(0);
    //mT5IntEnable(0);

    getOSCPacket();

    if(processOSCPacket())
    {
        //debug LED_2_On();

        if(compareOSCPrefix(getOSCPrefix()))
        {
            if(compareOSCAddress(msgOnboardLed))
            {
                if(!(compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) || !compareTypeTagAtIndex(1, 's'))
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgOnboardLed, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                    type = getStringArgumentAtIndex(1);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetPortIO, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetPortIO, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                    state = getStringArgumentAtIndex(1);
                    if(strcmp(state, "high") && strcmp(state, "low"))
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetPortOut, ": wrong_argument_string");
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetPortOut, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetPortIn, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                    state = getStringArgumentAtIndex(1);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcEnable, ": wrong_argument_type");
                    return;
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
                    return;
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
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcEnable, ": wrong_argument_type");
                    return;
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
                    return;
                }

                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
                {
                    id = getIntArgumentAtIndex(0);

                    if(id > AN_NUM - 1)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcType, ": out_of_value_range");
                        return;
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
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcType, ": wrong_argument_type");
                    return;
                }
            }
            else if(compareOSCAddress(msgGetAdcType))
            {
                BYTE id;

                if(getArgumentsLength() != 1)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcType, ": must_be_1_argument");
                    return;
                }

                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
                {
                    id = getIntArgumentAtIndex(0);
                    if(id > AN_NUM - 1)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcType, ": out_of_value_range");
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcType, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                    type = getStringArgumentAtIndex(1);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcDio, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcDio, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                    state = getStringArgumentAtIndex(1);
                    if(strcmp(state, "high") && strcmp(state, "low"))
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcDo, ": wrong_argument_string");
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetAdcDo, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetAdcDi, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                    state = getStringArgumentAtIndex(1);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmEnable, ": wrong_argument_type");
                    return;
                }

                if(strcmp(state, "on") == 0)
                {
                    LONG w = (LONG)(((float)duty[index] / 100.0) * (float)width);
                    if(!onTimer23)
                    {
                        OpenTimer23(T23_ON | T23_SOURCE_INT | T23_PS_1_1, width);
                        onTimer23 = TRUE;
                    }
                    switch(index)
                    {
                        case 0:
                            OpenOC1(OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                            break;
                        case 1:
                            OpenOC3(OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                            break;
                        case 2:
                            OpenOC4(OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                            break;
                        case 3:
                            OpenOC5(OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                            break;
                    }
                    onSquare[index] = TRUE;
                }
                else if(strcmp(state, "off") == 0)
                {
                    onSquare[index] = FALSE;
                    if(onTimer23 && (!onSquare[0] && !onSquare[1] && !onSquare[2] && !onSquare[3]))
                    {
                        CloseTimer23();
                        onTimer23 = FALSE;
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
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetPwmEnable, ": wrong_argument_type");
                    return;
                }

                sendOSCMessage(getOSCPrefix(), msgPwmEnable, "is", index, onSquare[index] ? "on" : "off");
            }
            else if(compareOSCAddress(msgSetPwmFreq))
            {
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
                    freq = getIntArgumentAtIndex(0);
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetPwmEnable, ": wrong_argument_type");
                    return;
                }

                if(freq < 20)
                {
                    freq = 20;
                }
                else if(freq > 44100)
                {
                    freq = 44100;
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
                        return;
                    }

                    duty[index] = getIntArgumentAtIndex(1);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDuty, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetPwmDuty, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                    type = getStringArgumentAtIndex(1);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDio, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetPwmDio, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                    state = getStringArgumentAtIndex(1);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDo, ": wrong_argument_type");
                    return;
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
                        return;
                    }

                    state = inputPwmPort(id);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetPwmDi, ": wrong_argument_type");
                    return;
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
                        return;
                    }

                    type = getStringArgumentAtIndex(1);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetDigitalDio, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetDigitalDio, ": wrong_argument_type");
                    return;
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
                    return;
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
                        return;
                    }

                    state = inputDigitalPort(id);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetDigitalDi, ": wrong_argument_type");
                    return;
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
                            return;
                        }
                    }
                    else
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiConfig, ": 1:out_of_value_range");
                        return;
                    }

                    if(getArgumentsLength() < 3)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiConfig, ": need_3_arguments_at_leaset");
                        return;
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
                        return;
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
                    return;
                }

                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's') && compareTypeTagAtIndex(2, 's') && (compareTypeTagAtIndex(3, 'i') || compareTypeTagAtIndex(3, 'f')))
                {
                    id = getIntArgumentAtIndex(0);
                    if(id != 2 && id != 4)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiData, ": 0:wrong_argument_value");
                        return;
                    }

                    load_port = getStringArgumentAtIndex(1);
                    if(!comparePortNameAtIndex(load_port))
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiData, ": 1:wrong_argument_string");
                        return;
                    }

                    active_state = getStringArgumentAtIndex(2);
                    if(strcmp(active_state, "HL") && strcmp(active_state, "LH"))
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiData, ": 2:wrong_argument_string");
                        return;
                    }

                    byte_num = getIntArgumentAtIndex(3);
                    if(byte_num != 1 && byte_num != 2 && byte_num != 4)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiData, ": 3:wrong_argument_value");
                        return;
                    }

                    if(getArgumentsLength() < byte_num + 4)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiData, ": too_few_arguments");
                        return;
                    }

                    for(i = 0; i < byte_num; i++)
                    {
                        if(!(compareTypeTagAtIndex(i + 4, 'i') || compareTypeTagAtIndex(i + 4, 'f')))
                        {
                            sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiData, ": 4:wrong_argument_type");
                            return;
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
                    return;
                }

                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's') && compareTypeTagAtIndex(2, 's') && (compareTypeTagAtIndex(3, 'i') || compareTypeTagAtIndex(3, 'f')))
                {
                    id = getIntArgumentAtIndex(0);
                    if(id != 2 && id != 4)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiData, ": 0:wrong_argument_value");
                        return;
                    }

                    load_port = getStringArgumentAtIndex(1);
                    if(!comparePortNameAtIndex(load_port))
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiData, ": 1:wrong_argument_string");
                        return;
                    }

                    active_state = getStringArgumentAtIndex(2);
                    if(strcmp(active_state, "HL") && strcmp(active_state, "LH"))
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiData, ": 2:wrong_argument_string");
                        return;
                    }

                    byte_num = getIntArgumentAtIndex(3);
                    if(byte_num != 1 && byte_num != 2 && byte_num != 4)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiData, ": 3:wrong_argument_value");
                        return;
                    }

                    if((compareTypeTagAtIndex(4, 'i') || compareTypeTagAtIndex(4, 'f')))
                    {
                        data[0] = getIntArgumentAtIndex(4);
                    }
                    else
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiData, ": 4:wrong_argument_type");
                        return;
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
                        return;
                    }
                    type = getStringArgumentAtIndex(1);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiDio, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiDio, ": wrong_argument_type");
                    return;
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
                        return;
                    }

                    state = getStringArgumentAtIndex(1);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiDo, ": wrong_argument_type");
                    return;
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
                        return;
                    }

                    state = inputSpiPort(name);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetSpiDi, ": wrong_argument_type");
                    return;
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
                        return;
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
                        return;
                    }

                    for(i = 1; i < getArgumentsLength(); i++)
                    {
                        char* flag;
                        if(compareTypeTagAtIndex(i, 's'))
                            flag = getStringArgumentAtIndex(i);
                        else
                        {
                            sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cConfig, ": wrong_argument_type");
                            return;
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
                            return;
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
                        return;
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
                            return;
                    }
                    if((actualFreq - i2cFreq) > i2cFreq / 10)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cConfig, ": bad_frequency");
                        return;
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
                        return;
                    }

                    for(i = 3; i < getArgumentsLength(); i++)
                    {
                        char* flag;
                        if(compareTypeTagAtIndex(i, 's'))
                            flag = getStringArgumentAtIndex(i);
                        else
                        {
                            sendOSCMessage(sysPrefix, msgError, "ss", msgSetI2cSlaveAddress, ": wrong_argument_type");
                            return;
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
                            return;
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
                    return;
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
                                return;
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
                                return;
                            }
                            initLcd(mode_num, rsname, rwname, ename, db0name, db1name, db2name, db3name, db4name, db5name, db6name, db7name);
                            break;
#endif
                        default:
                            sendOSCMessage(sysPrefix, msgError, "ss", msgSetLcdConfig, ": support_only_4_bit_mode");
                            return;
                            break;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLcdConfig, ": wrong_argument_type");
                    return;
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
                    return;
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
                        return;
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
                            return;
                        }
                    }
                    writeLineLcd(line_num, line_text);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLcdText, ": wrong_argument_type");
                    return;
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
                    return;
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
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgClearLcdText, ": wrong_argument_type");
                    return;
                }
            }
            else if(compareOSCAddress(msgLatticePadPinSelect))
            {
                if(getArgumentsLength() < 4)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticePadPinSelect, ": too_few_arguments");
                    return;
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
                        return;
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
                    return;
                }
            }
            else if(compareOSCAddress(msgLatticeLedDrvPinSelect))
            {
                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeLedDrvPinSelect, ": too_few_arguments");
                    return;
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
                {
                    BYTE num = getIntArgumentAtIndex(0);
                    char* name_load = getStringArgumentAtIndex(1);

                    if(strlen(name_load) > 3)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeLedDrvPinSelect, ": too_long_string_length");
                        return;
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
                            return;
                            break;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeLedDrvPinSelect, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                    setNumConnectedLatticePad(num);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticePadConnectedNum, ": wrong_argument_type");
                    return;
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
                    return;
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
                        return;
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
                            return;
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
                    return;
                }
            }
            else if(compareOSCAddress(msgSetLatticeLedColumn))
            {
                if(getArgumentsLength() < 3)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedColumn, ": too_few_arguments");
                    return;
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
                        return;
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
                        return;
                    }

                    if(getLatticeLed(index) != 0)
                        setLatticeLedOn(index, TRUE);
                    else
                        setLatticeLedOn(index, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedColumn, ": wrong_argument_type");
                    return;
                }
            }
            else if(compareOSCAddress(msgSetLatticeLedRow))
            {
                if(getArgumentsLength() < 3)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedColumn, ": too_few_arguments");
                    return;
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
                        return;
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
                        return;
                    }

                    if(getLatticeLed(index) != 0)
                        setLatticeLedOn(index, TRUE);
                    else
                        setLatticeLedOn(index, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedRow, ": wrong_argument_type");
                    return;
                }
            }
            else if(compareOSCAddress(msgSetLatticeLedAll))
            {
                if(getArgumentsLength() < 5)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedAll, ": too_few_arguments");
                    return;
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
                            return;
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
                        return;
                    }

                    if(getLatticeLed(index) != 0)
                        setLatticeLedOn(index, TRUE);
                    else
                        setLatticeLedOn(index, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeLedColumn, ": wrong_argument_type");
                    return;
                }
            }
            else if(compareOSCAddress(msgLatticeLedClear))
            {
                if(getArgumentsLength() < 1)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeLedClear, ": too_few_arguments");
                    return;
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
                    return;
                }
            }
            else if(compareOSCAddress(msgGetLatticeLedIntensity))
            {
                if(getArgumentsLength() < 3)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeLedIntensity, ": too_few_arguments");
                    return;
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
                        return;
                    }

                    sendOSCMessage(getOSCPrefix(), msgLatticeLedIntensity, "iiii", index, x, y, getLatticeIntensity(index, y + (x * 4)));
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeLedIntensity, ": wrong_argument_type");
                    return;
                }
            }
            else if(compareOSCAddress(msgGetLatticeLedIntensityAll))
            {
                if(getArgumentsLength() < 1)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeLedIntensity, ": too_few_arguments");
                    return;
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
                    return;
                }
            }
            else if(compareOSCAddress(msgLatticePadPinSelect))
            {
                if(getArgumentsLength() < 4)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticePadPinSelect, ": too_few_arguments");
                    return;
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
                        return;
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
                    return;
                }
            }
            else if(compareOSCAddress(msgLatticeRgbDrvPinSelect))
            {
                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeRgbDrvPinSelect, ": too_few_arguments");
                    return;
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
                {
                    BYTE num = getIntArgumentAtIndex(0);
                    char* name_load = getStringArgumentAtIndex(1);

                    if(strlen(name_load) > 3)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeRgbDrvPinSelect, ": too_long_string_length");
                        return;
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
                            return;
                            break;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeRgbDrvPinSelect, ": wrong_argument_type");
                    return;
                }
            }
            else if(compareOSCAddress(msgSetLatticeRgbConnectedNum))
            {
                if(getArgumentsLength() < 1)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbConnectedNum, ": too_few_arguments");
                    return;
                }
                if(compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f'))
                {
                    int num = getIntArgumentAtIndex(0);
                    if(num < 1)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbConnectedNum, ": out_of_range_value");
                        return;
                    }
                    setNumConnectedLatticeRgb(num);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbConnectedNum, ": wrong_argument_type");
                    return;
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
                    return;
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')))
                {
                    BYTE index = getIntArgumentAtIndex(0);
                    BYTE size = getIntArgumentAtIndex(1);

                    if(index > 3 || !(size == 8 || size == 16))
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeRgbDrvPinSelect, ": too_long_string_length");
                        return;
                    }

                    setLatticeRgbLedNumber(index, size);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbSize, ": wrong_argument_type");
                    return;
                }
            }
            else if(compareOSCAddress(msgGetLatticeRgbSize))
            {
                if(getArgumentsLength() < 1)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbSize, ": too_few_arguments");
                    return;
                }
                if(compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f'))
                {
                    BYTE index = getIntArgumentAtIndex(0);

                    if(index > 3)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeRgbDrvPinSelect, ": too_long_string_length");
                        return;
                    }

                    sendOSCMessage(getOSCPrefix(), msgLatticeRgbSize, "ii", index, getLatticeRgbLedNumber(index));
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbSize, ": wrong_argument_type");
                    return;
                }
            }
            else if(compareOSCAddress(msgSetLatticeRgb))
            {
                if(getArgumentsLength() < 5)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgb, ": too_few_arguments");
                    return;
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
                        return;
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
                            return;
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
                    return;
                }
            }
            else if(compareOSCAddress(msgSetLatticeRgbColumn))
            {
                if(getArgumentsLength() < 4)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbColumn, ": too_few_arguments");
                    return;
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
                        return;
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
                        return;
                    }

                    if(getLatticeRgb(index, layer) != 0)
                        setLatticeRgbOn(index, layer, TRUE);
                    else
                        setLatticeRgbOn(index, layer, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbColumn, ": wrong_argument_type");
                    return;
                }
            }
            else if(compareOSCAddress(msgSetLatticeRgbRow))
            {
                if(getArgumentsLength() < 4)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbRow, ": too_few_arguments");
                    return;
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
                        return;
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
                        return;
                    }

                    if(getLatticeRgb(index, layer) != 0)
                        setLatticeRgbOn(index, layer, TRUE);
                    else
                        setLatticeRgbOn(index, layer, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbRow, ": wrong_argument_type");
                    return;
                }
            }
            else if(compareOSCAddress(msgSetLatticeRgbAll))
            {
                if(getArgumentsLength() < 6)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbAll, ": too_few_arguments");
                    return;
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
                            return;
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
                        return;
                    }

                    if(getLatticeRgb(index, layer) != 0)
                        setLatticeRgbOn(index, layer, TRUE);
                    else
                        setLatticeRgbOn(index, layer, FALSE);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetLatticeRgbAll, ": wrong_argument_type");
                    return;
                }
            }
            else if(compareOSCAddress(msgLatticeRgbClear))
            {
                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgLatticeRgbClear, ": too_few_arguments");
                    return;
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
                    return;
                }
            }
            else if(compareOSCAddress(msgGetLatticeRgbIntensity))
            {
                if(getArgumentsLength() < 4)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeRgbIntensity, ": too_few_arguments");
                    return;
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
                        return;
                    }

                    sendOSCMessage(getOSCPrefix(), msgLatticeRgbIntensity, "iiii", index, x, y, getLatticeRgbIntensity(index, layer, y + (x * 4)));
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeRgbIntensity, ": wrong_argument_type");
                    return;
                }
            }
            else if(compareOSCAddress(msgGetLatticeRgbIntensityAll))
            {
                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetLatticeRgbIntensity, ": too_few_arguments");
                    return;
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
                    return;
                }
            }
            else if(compareOSCAddress(msgRotaryIncEncPinSelect))
            {
                if(getArgumentsLength() < 4)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryIncEncPinSelect, ": too_few_arguments");
                    return;
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
                        return;
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
                    return;
                }
            }
            else if(compareOSCAddress(msgRotaryAbsEncPinSelect))
            {
                if(getArgumentsLength() < 3)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryAbsEncPinSelect, ": too_few_arguments");
                    return;
                }
                if(compareTypeTagAtIndex(0, 's') && compareTypeTagAtIndex(1, 's') && compareTypeTagAtIndex(2, 's'))
                {
                    char* name_cs = getStringArgumentAtIndex(0);
                    char* name_clk = getStringArgumentAtIndex(1);
                    char* name_do = getStringArgumentAtIndex(2);

                    if(strlen(name_cs) > 3 || strlen(name_clk) > 3 || strlen(name_do) > 3)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryAbsEncPinSelect, ": too_long_string_length");
                        return;
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
                    return;
                }
            }
            else if(compareOSCAddress(msgRotaryLedDrvPinSelect))
            {
                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryLedDrvPinSelect, ": too_few_arguments");
                    return;
                }
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && compareTypeTagAtIndex(1, 's'))
                {
                    BYTE num = getIntArgumentAtIndex(0);
                    char* name_ss = getStringArgumentAtIndex(1);

                    if(strlen(name_ss) > 3)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryLedDrvPinSelect, ": too_long_string_length");
                        return;
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
                            return;
                            break;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgRotaryLedDrvPinSelect, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                    setNumConnectedAbsEnc(num);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryAbsEncConnectedNum, ": wrong_argument_type");
                    return;
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
                    return;
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
                            return;
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
                    return;
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
                    return;
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
                                return;
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
                                return;
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
                    return;
                }
            }
            else if(compareOSCAddress(msgSetRotaryLedIntensity))
            {
                BYTE index, position, intensity;

                if(getArgumentsLength() < 3)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedIntensity, ": too_few_arguments");
                    return;
                }

                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')) && (compareTypeTagAtIndex(2, 'i') || compareTypeTagAtIndex(2, 'f')))
                {
                    index = getIntArgumentAtIndex(0);
                    if(getInitAbsEncFlag() && index >= getNumConnectedAbsEnc())
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedIntensity, ": out_of_range_value");
                        return;
                    }

                    position = getIntArgumentAtIndex(1) + 16;
                    if(position > 31)
                        position -= 32;
                    if(position > 31)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedIntensity, ": out_of_range_value");
                        return;
                    }

                    intensity = getIntArgumentAtIndex(2);
                    setIntensity(index, position, intensity);
                    if(intensity > 100)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedIntensity, ": out_of_range_value");
                        return;
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
                    return;
                }
            }
            else if(compareOSCAddress(msgGetRotaryLedIntensity))
            {
                BYTE index, position0, position1, intensity;

                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetRotaryLedIntensity, ": too_few_arguments");
                    return;
                }

                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')) && (compareTypeTagAtIndex(1, 'i') || compareTypeTagAtIndex(1, 'f')))
                {
                    index = getIntArgumentAtIndex(0);
                    if(getInitAbsEncFlag() && index >= getNumConnectedAbsEnc())
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgGetRotaryLedIntensity, ": out_of_range_value");
                        return;
                    }

                    position0 = getIntArgumentAtIndex(1);
                    position1 = position0 + 16;
                    if(position1 > 31)
                        position1 -= 32;
                    
                    if(position1 > 31)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedIntensity, ": out_of_range_value");
                        return;
                    }

                    intensity = getIntensity(index, position1);

                    sendOSCMessage(getOSCPrefix(), msgRotaryLedIntensity, "iii", index, position0, intensity);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetRotaryLedIntensity, ": wrong_argument_type");
                    return;
                }
            }
            else if(compareOSCAddress(msgSetRotaryLedIntensityAll))
            {
                BYTE index;
                DWORD all_intensity;

                if(getArgumentsLength() < 2)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRotaryLedIntensityAll, ": too_few_arguments");
                    return;
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
                    return;
                }
            }
        }
        else if(compareOSCPrefix(toscPrefix))
        {
            // touchOSC
            if(compareOSCAddress(msgOnboardLed1) && compareTypeTagAtIndex(0, 'f'))
            {
                switch(getIntArgumentAtIndex(0))
                {
                    case 0:
                        LED_1_Off();
                        break;
                    case 1:
                        LED_1_On();
                        break;
                }
            }
            else if(compareOSCAddress(msgOnboardLed2) && compareTypeTagAtIndex(0, 'f'))
            {
                switch(getIntArgumentAtIndex(0))
                {
                    case 0:
                        LED_2_Off();
                        break;
                    case 1:
                        LED_2_On();
                        break;
                }
            }
            else if(compareOSCAddress(msgSetPwmEnable1) && compareTypeTagAtIndex(0, 'f'))
            {
                if(getIntArgumentAtIndex(0))
                {
                    LONG w = (LONG)(((float)duty[index] / 100.0) * (float)width);
                    if(!onTimer23)
                    {
                        OpenTimer23(T23_ON | T23_SOURCE_INT | T23_PS_1_1, width);
                        onTimer23 = TRUE;
                    }
                    OpenOC1(OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                    onSquare[0] = TRUE;
                }
                else
                {
                    onSquare[0] = FALSE;
                    if(onTimer23 && (!onSquare[0] && !onSquare[1] && !onSquare[2] && !onSquare[3]))
                    {
                        CloseTimer23();
                        onTimer23 = FALSE;
                    }
                    CloseOC1();
                }
            }
            else if(compareOSCAddress(msgSetPwmEnable2) && compareTypeTagAtIndex(0, 'f'))
            {
                if(getIntArgumentAtIndex(0))
                {
                    LONG w = (LONG)(((float)duty[index] / 100.0) * (float)width);
                    if(!onTimer23)
                    {
                        OpenTimer23(T23_ON | T23_SOURCE_INT | T23_PS_1_1, width);
                        onTimer23 = TRUE;
                    }
                    OpenOC3(OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                    onSquare[1] = TRUE;
                }
                else
                {
                    onSquare[1] = FALSE;
                    if(onTimer23 && (!onSquare[0] && !onSquare[1] && !onSquare[2] && !onSquare[3]))
                    {
                        CloseTimer23();
                        onTimer23 = FALSE;
                    }
                    CloseOC3();
                }
            }
            else if(compareOSCAddress(msgSetPwmEnable3) && compareTypeTagAtIndex(0, 'f'))
            {
                if(getIntArgumentAtIndex(0))
                {
                    LONG w = (LONG)(((float)duty[index] / 100.0) * (float)width);
                    if(!onTimer23)
                    {
                        OpenTimer23(T23_ON | T23_SOURCE_INT | T23_PS_1_1, width);
                        onTimer23 = TRUE;
                    }
                    OpenOC4(OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                    onSquare[2] = TRUE;
                }
                else
                {
                    onSquare[2] = FALSE;
                    if(onTimer23 && (!onSquare[0] && !onSquare[1] && !onSquare[2] && !onSquare[3]))
                    {
                        CloseTimer23();
                        onTimer23 = FALSE;
                    }
                    CloseOC4();
                }
            }
            else if(compareOSCAddress(msgSetPwmEnable4) && compareTypeTagAtIndex(0, 'f'))
            {
                if(getIntArgumentAtIndex(0))
                {
                    LONG w = (LONG)(((float)duty[index] / 100.0) * (float)width);
                    if(!onTimer23)
                    {
                        OpenTimer23(T23_ON | T23_SOURCE_INT | T23_PS_1_1, width);
                        onTimer23 = TRUE;
                    }
                    OpenOC5(OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                    onSquare[3] = TRUE;
                }
                else
                {
                    onSquare[3] = FALSE;
                    if(onTimer23 && (!onSquare[0] && !onSquare[1] && !onSquare[2] && !onSquare[3]))
                    {
                        CloseTimer23();
                        onTimer23 = FALSE;
                    }
                    CloseOC5();
                }
            }
            else if(compareOSCAddress(msgSetPwmFreq))
            {
                if(compareTypeTagAtIndex(0, 'f'))
                    freq = getIntArgumentAtIndex(0);
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetPwmEnable, ": wrong_argument_type");
                    return;
                }

                if(freq < 20)
                {
                    freq = 20;
                }
                else if(freq > 44100)
                {
                    freq = 44100;
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
            else if(compareOSCAddress(msgSetPwmDuty1))
            {
                if(compareTypeTagAtIndex(0, 'f'))
                {
                    index = 0;
                    if(index > PWM_NUM - 1)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDuty, ": out_of_value_range");
                        return;
                    }

                    duty[index] = getIntArgumentAtIndex(0);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDuty, ": wrong_argument_type");
                    return;
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
            else if(compareOSCAddress(msgSetPwmDuty2))
            {
                if(compareTypeTagAtIndex(0, 'f'))
                {
                    index = 1;
                    if(index > PWM_NUM - 1)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDuty, ": out_of_value_range");
                        return;
                    }

                    duty[index] = getIntArgumentAtIndex(0);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDuty, ": wrong_argument_type");
                    return;
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
            else if(compareOSCAddress(msgSetPwmDuty3))
            {
                if(compareTypeTagAtIndex(0, 'f'))
                {
                    index = 2;
                    if(index > PWM_NUM - 1)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDuty, ": out_of_value_range");
                        return;
                    }

                    duty[index] = getIntArgumentAtIndex(0);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDuty, ": wrong_argument_type");
                    return;
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
            else if(compareOSCAddress(msgSetPwmDuty4))
            {
                if(compareTypeTagAtIndex(0, 'f'))
                {
                    index = 3;
                    if(index > PWM_NUM - 1)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDuty, ": out_of_value_range");
                        return;
                    }

                    duty[index] = getIntArgumentAtIndex(0);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetPwmDuty, ": wrong_argument_type");
                    return;
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
        }
        // MIDI
        else if(compareOSCPrefix(midiPrefix))
        {
            if(compareOSCAddress(msgSetNote)) // Note On/Off
            {
                BYTE currentEndpoint;

                BYTE ch = getIntArgumentAtIndex(0);
                BYTE num = getIntArgumentAtIndex(1);
                BYTE vel = getIntArgumentAtIndex(2);

                OSCTranslatedToUSB.Val = 0;
                OSCTranslatedToUSB.CableNumber = 0;
                OSCTranslatedToUSB.CodeIndexNumber = MIDI_CIN_NOTE_ON;
                OSCTranslatedToUSB.DATA_0 = 0x90 + ch;
                OSCTranslatedToUSB.DATA_1 = num;
                OSCTranslatedToUSB.DATA_2 = vel;

                for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
                {
                    endpointBuffers[currentEndpoint].pBufWriteLocation->Val = OSCTranslatedToUSB.Val;
                    endpointBuffers[currentEndpoint].pBufWriteLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;

                    if(endpointBuffers[currentEndpoint].pBufWriteLocation - endpointBuffers[currentEndpoint].bufferStart >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                    {
                        endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
                    }
                    break;
                }
            }
            else if(compareOSCAddress(msgSetCc)) // Control Change
            {
                BYTE currentEndpoint;

                BYTE ch = getIntArgumentAtIndex(0);
                BYTE num = getIntArgumentAtIndex(1);
                BYTE val = getIntArgumentAtIndex(2);

                OSCTranslatedToUSB.Val = 0;
                OSCTranslatedToUSB.CableNumber = 0;
                OSCTranslatedToUSB.CodeIndexNumber = MIDI_CIN_CONTROL_CHANGE;
                OSCTranslatedToUSB.DATA_0 = 0xB0 + ch;
                OSCTranslatedToUSB.DATA_1 = num;
                OSCTranslatedToUSB.DATA_2 = val;

                for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
                {
                    endpointBuffers[currentEndpoint].pBufWriteLocation->Val = OSCTranslatedToUSB.Val;
                    endpointBuffers[currentEndpoint].pBufWriteLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;

                    if(endpointBuffers[currentEndpoint].pBufWriteLocation - endpointBuffers[currentEndpoint].bufferStart >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                    {
                        endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
                    }
                    break;
                }
            }
            else if(compareOSCAddress(msgSetKp)) // Polyphonic Key Pressure
            {
                BYTE currentEndpoint;

                BYTE ch = getIntArgumentAtIndex(0);
                BYTE num = getIntArgumentAtIndex(1);
                BYTE prs = getIntArgumentAtIndex(2);

                OSCTranslatedToUSB.Val = 0;
                OSCTranslatedToUSB.CableNumber = 0;
                OSCTranslatedToUSB.CodeIndexNumber = MIDI_CIN_POLY_KEY_PRESS;
                OSCTranslatedToUSB.DATA_0 = 0xA0 + ch;
                OSCTranslatedToUSB.DATA_1 = num;
                OSCTranslatedToUSB.DATA_2 = prs;

                for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
                {
                    endpointBuffers[currentEndpoint].pBufWriteLocation->Val = OSCTranslatedToUSB.Val;
                    endpointBuffers[currentEndpoint].pBufWriteLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;

                    if(endpointBuffers[currentEndpoint].pBufWriteLocation - endpointBuffers[currentEndpoint].bufferStart >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                    {
                        endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
                    }
                    break;
                }
            }
            else if(compareOSCAddress(msgSetPc)) // Program Change
            {
                BYTE currentEndpoint;

                BYTE ch = getIntArgumentAtIndex(0);
                BYTE pnum = getIntArgumentAtIndex(1);

                OSCTranslatedToUSB.Val = 0;
                OSCTranslatedToUSB.CableNumber = 0;
                OSCTranslatedToUSB.CodeIndexNumber = MIDI_CIN_PROGRAM_CHANGE;
                OSCTranslatedToUSB.DATA_0 = 0xC0 + ch;
                OSCTranslatedToUSB.DATA_1 = pnum;

                for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
                {
                    endpointBuffers[currentEndpoint].pBufWriteLocation->Val = OSCTranslatedToUSB.Val;
                    endpointBuffers[currentEndpoint].pBufWriteLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;

                    if(endpointBuffers[currentEndpoint].pBufWriteLocation - endpointBuffers[currentEndpoint].bufferStart >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                    {
                        endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
                    }
                    break;
                }
            }
            else if(compareOSCAddress(msgSetCp)) // Channel Pressure
            {
                BYTE currentEndpoint;

                BYTE ch = getIntArgumentAtIndex(0);
                BYTE prs = getIntArgumentAtIndex(1);

                OSCTranslatedToUSB.Val = 0;
                OSCTranslatedToUSB.CableNumber = 0;
                OSCTranslatedToUSB.CodeIndexNumber = MIDI_CIN_CHANNEL_PREASURE;
                OSCTranslatedToUSB.DATA_0 = 0xD0 + ch;
                OSCTranslatedToUSB.DATA_1 = prs;

                for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
                {
                    endpointBuffers[currentEndpoint].pBufWriteLocation->Val = OSCTranslatedToUSB.Val;
                    endpointBuffers[currentEndpoint].pBufWriteLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;

                    if(endpointBuffers[currentEndpoint].pBufWriteLocation - endpointBuffers[currentEndpoint].bufferStart >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                    {
                        endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
                    }
                    break;
                }
            }
            else if(compareOSCAddress(msgSetPb)) // Pitch Bend
            {
                BYTE currentEndpoint;

                BYTE ch = getIntArgumentAtIndex(0);
                BYTE msb = getIntArgumentAtIndex(1);
                BYTE lsb = getIntArgumentAtIndex(2);

                OSCTranslatedToUSB.Val = 0;
                OSCTranslatedToUSB.CableNumber = 0;
                OSCTranslatedToUSB.CodeIndexNumber = MIDI_CIN_PITCH_BEND_CHANGE;
                OSCTranslatedToUSB.DATA_0 = 0xE0 + ch;
                OSCTranslatedToUSB.DATA_1 = msb;
                OSCTranslatedToUSB.DATA_2 = lsb;

                for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
                {
                    endpointBuffers[currentEndpoint].pBufWriteLocation->Val = OSCTranslatedToUSB.Val;
                    endpointBuffers[currentEndpoint].pBufWriteLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;

                    if(endpointBuffers[currentEndpoint].pBufWriteLocation - endpointBuffers[currentEndpoint].bufferStart >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                    {
                        endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
                    }
                    break;
                }
            }
        }
        // CDC
#if defined(USB_USE_CDC)
        else if(compareOSCPrefix(cdcPrefix))
        {
            if(compareOSCAddress(msgSetData))
            {
                WORD data = 0;
                cdcOutDataLength = getArgumentsLength();
                if(cdcOutDataLength >= MAX_NO_OF_OUT_BYTES)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetData, ": too_many_arguments");
                    return;
                }

                for(i = 0; i < cdcOutDataLength; i++)
                {
                    data = getIntArgumentAtIndex(i);
                    if(data > 255)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetData, ": out_of_range_value");
                        return;
                    }
                    USB_CDC_OUT_Data_Array[i] = (BYTE)data;
                }

                cdcSendFlag = TRUE;
                APPL_CDC_State = READY_TO_TX_RX;

                //_USBHostCDC_TerminateTransfer(USB_SUCCESS);
            }
        }
#endif
        else if(compareOSCPrefix(sysPrefix))
        {
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
                    return;
                }

                ip[0] = atoi(strtok(rip, "."));
                if(rip == NULL)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRemoteIp, ": wrong_argument_string");
                    return;
                }
                if(ip[0] > 255)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRemoteIp, ": wrong_argument_string");
                    return;
                }
                ip[1] = atoi(strtok(NULL, "."));
                if(rip == NULL)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRemoteIp, ": wrong_argument_string");
                    return;
                }
                if(ip[1] > 255)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRemoteIp, ": wrong_argument_string");
                    return;
                }
                ip[2] = atoi(strtok(NULL, "."));
                if(rip == NULL)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRemoteIp, ": wrong_argument_string");
                    return;
                }
                if(ip[2] > 255)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRemoteIp, ": wrong_argument_string");
                    return;
                }
                ip[3] = atoi(strtok(NULL, "."));
                if(ip[3] > 255)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetRemoteIp, ": wrong_argument_string");
                    return;
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
                        return;
                    }
                    setRemotePort(getIntArgumentAtIndex(0));
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetRemoteIp, ": wrong_argument_type");
                    return;
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
                        return;
                    }

                    //len = np - srcName;
                    //if(len >= 32)
                    if(strlen(srcName) >= 32)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetHostName, ": too_long_string");
                        return;
                    }

                    mDNSServiceDeRegister();
                    clearOSCHostName();
                    setOSCHostName(srcName);

                    //debug sendOSCMessage(sysPrefix, msgError, "ss", srcName, hostName);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetHostName, ": wrong_argument_type");
                    return;
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
                char macaddr[17] = {0};
                sprintf(macaddr, "%X:%X:%X:%X:%X:%X", AppConfig.MyMACAddr.v[0], AppConfig.MyMACAddr.v[1], AppConfig.MyMACAddr.v[2], AppConfig.MyMACAddr.v[3], AppConfig.MyMACAddr.v[4], AppConfig.MyMACAddr.v[5]);
                sendOSCMessage(sysPrefix, msgHostMac, "s", macaddr);
            }
            else if(compareOSCAddress(msgSetHostPort))
            {
                if((compareTypeTagAtIndex(0, 'i') || compareTypeTagAtIndex(0, 'f')))
                {
                    if(getIntArgumentAtIndex(0) < 0)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetHostPort, ": out_of_value_range");
                        return;
                    }
                    setLocalPort(getIntArgumentAtIndex(0));
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetHostPort, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                    clearOSCPrefix();
                    setOSCPrefix(getStringArgumentAtIndex(0));
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetPrefix, ": wrong_argument_type");
                    return;
                }
                sendOSCMessage(sysPrefix, msgConfiguration, "s", "succeeded");
            }
            else if(compareOSCAddress(msgGetPrefix))
            {
                sendOSCMessage(sysPrefix, msgPrefix, "s", getOSCPrefix());
            }
            else if(compareOSCAddress(msgSwitchUsbMode))
            {
                char* dm;
                if(compareTypeTagAtIndex(0, 's'))
                    dm = getStringArgumentAtIndex(0);
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSwitchUsbMode, ": wrong_argument_type");
                    return;
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
                    return;
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
                    return;
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
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgWriteNvmData, ": wrong_argument_type");
                    return;
                }
            }
            else if(compareOSCAddress(msgReadNvmData))
            {
                if(getArgumentsLength() < 1)
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgReadNvmData, ": too_few_arguments");
                    return;
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
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgReadNvmData, ": wrong_argument_type");
                    return;
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
                        return;
                    }
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSoftReset, ": wrong_argument_type");
                    return;
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
        }
        //debug LED_2_Off();
    }
    //mT4IntEnable(1);
    //mT5IntEnable(1);
}

/*******************************************************************************
  Function:
    void __ISR(_TIMER_4_VECTOR, ipl6) ledHandle(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void __ISR(_TIMER_4_VECTOR, ipl6) ledHandle(void)
{
    static BYTE led_state = 0;
    
    switch(led_state)
    {
        case 0:
            if(getInitLedDrvFlag())
                annularLedHandle();
            
            led_state = 1;
            break;
        case 1:
            if(getInitLatticeLedDrvFlag())
                latticeLedHandle();
            
            led_state = 2;
            break;
        case 2:
            if(getInitLatticeRgbDrvFlag())
                latticeRgbHandle();

            led_state = 0;
            break;
        default:
            led_state = 0;
            break;
    }

    mT4ClearIntFlag();
}

/*******************************************************************************
  Function:
    void __ISR(_TIMER_5_VECTOR, IPL5) sendOSCTask(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void __ISR(_TIMER_5_VECTOR, IPL5) sendOSCTask(void)
{
    int i, j;

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
            default:
                sendOSCTaskIndex = 0;
                break;
        }
    }
    mT5ClearIntFlag();
}

/*******************************************************************************
  Function:
    void USBControlTask()

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void USBControlTask()
{
    HIDControlTask();
    //sendNote();
    //sendControlChange();
    //receiveMIDIDatas();
}

/*******************************************************************************
  Function:
    void HIDControlTask(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void HIDControlTask(void)
{
    BYTE u8Data[128] = {0};

    // User Application USB tasks
    if(USBDeviceState < CONFIGURED_STATE || USBSuspendControl == 1)
        return;

    if(!USBHandleBusy(HIDRxHandle))
    {
        HIDRxHandle = USBRxOnePacket(HID_EP, (BYTE*)&ReceivedHidDataBuffer[0], 64);
        MIDIRxHandle = USBRxOnePacket(MIDI_EP, (BYTE*)&ReceivedHidDataBuffer[0], 64);
        if(ReceivedHidDataBuffer[0] == 0x80)
        {
            //LED_1_Toggle();
        }
        switch(ReceivedHidDataBuffer[0])
        {
            case 0x80:// ADC Enable
                if(ReceivedHidDataBuffer[1])
                {
                    AD1CON1bits.ON = 0;

                    int i;
                    WORD anum = 0;
                    WORD id = ReceivedHidDataBuffer[2];
                    BYTE state = ReceivedHidDataBuffer[3];

                    if(id > 13 || state > 1)
                        return;

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
                    BYTE id = ReceivedHidDataBuffer[2];

                    if(id > 13)
                        return;

                    ToSendHidDataBuffer[0] = 0x80;
                    ToSendHidDataBuffer[1] = 3;
                    ToSendHidDataBuffer[2] = ReceivedHidDataBuffer[2];
                    ToSendHidDataBuffer[3] = getAnalogEnable(ReceivedHidDataBuffer[2]);
                    ToSendHidDataBuffer[4] = getAnalogByte(ReceivedHidDataBuffer[2], BYTE_ORIGINAL);
                    ToSendHidDataBuffer[5] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                break;
            case 0x81:
                if(ReceivedHidDataBuffer[1])
                {
                    if(ReceivedHidDataBuffer[3] == 0)
                    {
                        setAnPortDioType(ReceivedHidDataBuffer[2], IO_IN);
                    }
                    else if(ReceivedHidDataBuffer[3] == 1)
                    {
                        setAnPortDioType(ReceivedHidDataBuffer[2], IO_OUT);
                    }
                }
                else
                {
                    BYTE id = ReceivedHidDataBuffer[2];

                    if(id > 13)
                        return;

                    ToSendHidDataBuffer[0] = 0x81;
                    ToSendHidDataBuffer[1] = 2;
                    ToSendHidDataBuffer[2] = ReceivedHidDataBuffer[2];
                    ToSendHidDataBuffer[3] = ioAnPort[ReceivedHidDataBuffer[2]];
                    ToSendHidDataBuffer[5] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                break;
            case 0x82:
                if(ReceivedHidDataBuffer[1] == 1)
                {
                    outputAnPort(ReceivedHidDataBuffer[2], ReceivedHidDataBuffer[3]);
                }
                else if(ReceivedHidDataBuffer[1] == 0)
                {
                    ToSendHidDataBuffer[0] = 0x82;
                    ToSendHidDataBuffer[1] = 2;
                    ToSendHidDataBuffer[2] = ReceivedHidDataBuffer[2];
                    ToSendHidDataBuffer[3] = inputAnPort(ReceivedHidDataBuffer[2]);
                    ToSendHidDataBuffer[4] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                break;
            case 0x83:
                if(ReceivedHidDataBuffer[1] == 1)
                {
                    BYTE index = ReceivedHidDataBuffer[2];
                    if(index > 4)
                        return;

                    if(ReceivedHidDataBuffer[3] == 1)
                    {
                        LONG w = (LONG)(((float)duty[index] / 100.0) * (float)width);
                        if(!onTimer23)
                        {
                            OpenTimer23(T23_ON | T23_SOURCE_INT | T23_PS_1_1, width);
                            onTimer23 = TRUE;
                        }
                        switch(index)
                        {
                            case 0:
                                OpenOC1(OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                                break;
                            case 1:
                                OpenOC3(OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                                break;
                            case 2:
                                OpenOC4(OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                                break;
                            case 3:
                                OpenOC5(OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width / 2, w);
                                break;
                        }
                        onSquare[index] = TRUE;
                    }
                    else if(ReceivedHidDataBuffer[3] == 0)
                    {
                        onSquare[index] = FALSE;
                        if(onTimer23 && (!onSquare[0] && !onSquare[1] && !onSquare[2] && !onSquare[3]))
                        {
                            CloseTimer23();
                            onTimer23 = FALSE;
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
                else if(ReceivedHidDataBuffer[1] == 0)
                {
                    BYTE index = ReceivedHidDataBuffer[2];
                    
                    ToSendHidDataBuffer[0] = 0x83;
                    ToSendHidDataBuffer[1] = 2;
                    ToSendHidDataBuffer[2] = index;
                    ToSendHidDataBuffer[3] = onSquare[index];
                    ToSendHidDataBuffer[4] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                break;
            case 0x84:
                if(ReceivedHidDataBuffer[1] == 1)
                {
                    freq = (ReceivedHidDataBuffer[2] << 8) | ReceivedHidDataBuffer[3];
                    if(freq < 20)
                    {
                        freq = 20;
                    }
                    else if(freq > 44100)
                    {
                        freq = 44100;
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
                else if(ReceivedHidDataBuffer[1] == 0)
                {
                    ToSendHidDataBuffer[0] = 0x84;
                    ToSendHidDataBuffer[1] = 2;
                    ToSendHidDataBuffer[2] = (BYTE)(freq >> 8);
                    ToSendHidDataBuffer[3] = (BYTE)freq;
                    ToSendHidDataBuffer[4] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                break;
            case 0x85:
                if(ReceivedHidDataBuffer[1] == 1)
                {
                    BYTE index = ReceivedHidDataBuffer[2];
                    duty[index] = ReceivedHidDataBuffer[3];
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
                else if(ReceivedHidDataBuffer[1] == 0)
                {
                    BYTE index = ReceivedHidDataBuffer[2];

                    ToSendHidDataBuffer[0] = 0x85;
                    ToSendHidDataBuffer[1] = 2;
                    ToSendHidDataBuffer[2] = index;
                    ToSendHidDataBuffer[3] = duty[index];
                    ToSendHidDataBuffer[4] = 0x00;
                    
                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                break;
            case 0x86:
                if(ReceivedHidDataBuffer[1] == 1)
                {
                    BYTE id = ReceivedHidDataBuffer[2];
                    BYTE type = ReceivedHidDataBuffer[3];

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
                    BYTE id = ReceivedHidDataBuffer[2];

                    if(id > 3)
                        return;

                    ToSendHidDataBuffer[0] = 0x86;
                    ToSendHidDataBuffer[1] = 2;
                    ToSendHidDataBuffer[2] = ReceivedHidDataBuffer[2];
                    ToSendHidDataBuffer[3] = ioPwmPort[ReceivedHidDataBuffer[2]];
                    ToSendHidDataBuffer[5] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                break;
            case 0x87:
                if(ReceivedHidDataBuffer[1] == 1)
                {
                    BYTE id = ReceivedHidDataBuffer[2];
                    BYTE state = ReceivedHidDataBuffer[3];
                    outputPwmPort(id, state);
                }
                else if(ReceivedHidDataBuffer[1] == 0)
                {
                    BYTE id = ReceivedHidDataBuffer[2];
                    BYTE state = inputPwmPort(id);

                    ToSendHidDataBuffer[0] = 0x87;
                    ToSendHidDataBuffer[1] = 2;
                    ToSendHidDataBuffer[2] = id;
                    ToSendHidDataBuffer[3] = state;
                    ToSendHidDataBuffer[4] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                break;
            case 0x88:
                if(ReceivedHidDataBuffer[1] == 1)
                {
                    BYTE id = ReceivedHidDataBuffer[2];
                    BYTE type = ReceivedHidDataBuffer[3];

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
                    BYTE id = ReceivedHidDataBuffer[2];

                    if(id > 3)
                        return;

                    ToSendHidDataBuffer[0] = 0x88;
                    ToSendHidDataBuffer[1] = 2;
                    ToSendHidDataBuffer[2] = ReceivedHidDataBuffer[2];
                    ToSendHidDataBuffer[3] = ioDPort[ReceivedHidDataBuffer[2]];
                    ToSendHidDataBuffer[5] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                break;
            case 0x89:
                if(ReceivedHidDataBuffer[1] == 1)
                {
                    BYTE id = ReceivedHidDataBuffer[2];
                    BYTE state = ReceivedHidDataBuffer[3];
                    outputDigitalPort(id, state);
                }
                else if(ReceivedHidDataBuffer[1] == 0)
                {
                    BYTE id = ReceivedHidDataBuffer[2];
                    BYTE state = inputDigitalPort(id);

                    ToSendHidDataBuffer[0] = 0x89;
                    ToSendHidDataBuffer[1] = 2;
                    ToSendHidDataBuffer[2] = id;
                    ToSendHidDataBuffer[3] = state;
                    ToSendHidDataBuffer[4] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                break;
            case 0x8A:
                if(ReceivedHidDataBuffer[1] == 1)
                {
                    BYTE id = ReceivedHidDataBuffer[2];
                    BYTE type = ReceivedHidDataBuffer[3];

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
                    BYTE id = ReceivedHidDataBuffer[2];

                    if(id > 5)
                        return;

                    ToSendHidDataBuffer[0] = 0x8A;
                    ToSendHidDataBuffer[1] = 2;
                    ToSendHidDataBuffer[2] = ReceivedHidDataBuffer[2];
                    ToSendHidDataBuffer[3] = ioSpiPort[ReceivedHidDataBuffer[2]];
                    ToSendHidDataBuffer[5] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                break;
            case 0x8B:
                if(ReceivedHidDataBuffer[1] == 1)
                {
                    BYTE id = ReceivedHidDataBuffer[2];
                    BYTE state = ReceivedHidDataBuffer[3];
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
                else if(ReceivedHidDataBuffer[1] == 0)
                {
                    BYTE id = ReceivedHidDataBuffer[2];
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

                    ToSendHidDataBuffer[0] = 0x8B;
                    ToSendHidDataBuffer[1] = 2;
                    ToSendHidDataBuffer[2] = id;
                    ToSendHidDataBuffer[3] = state;
                    ToSendHidDataBuffer[4] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                break;
            case 0xF0:// Host IP
                if(!ReceivedHidDataBuffer[1])
                {
                    ToSendHidDataBuffer[0] = 0xF0;
                    ToSendHidDataBuffer[1] = 4;
                    ToSendHidDataBuffer[2] = (BYTE)((AppConfig.MyIPAddr.Val >> 0) & 0x000000FF);
                    ToSendHidDataBuffer[3] = (BYTE)((AppConfig.MyIPAddr.Val >> 8) & 0x000000FF);
                    ToSendHidDataBuffer[4] = (BYTE)((AppConfig.MyIPAddr.Val >> 16) & 0x000000FF);
                    ToSendHidDataBuffer[5] = (BYTE)((AppConfig.MyIPAddr.Val >> 24) & 0x000000FF);
                    ToSendHidDataBuffer[6] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                break;
            case 0xF1:// Remote IP
                if(!ReceivedHidDataBuffer[1])
                {
                    ToSendHidDataBuffer[0] = 0xF1;
                    ToSendHidDataBuffer[1] = 4;
                    ToSendHidDataBuffer[2] = getRemoteIpAtIndex(0);
                    ToSendHidDataBuffer[3] = getRemoteIpAtIndex(1);
                    ToSendHidDataBuffer[4] = getRemoteIpAtIndex(2);
                    ToSendHidDataBuffer[5] = getRemoteIpAtIndex(3);
                    ToSendHidDataBuffer[6] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                else
                {
                    setRemoteIpAtIndex(0, ReceivedHidDataBuffer[2]);
                    setRemoteIpAtIndex(1, ReceivedHidDataBuffer[3]);
                    setRemoteIpAtIndex(2, ReceivedHidDataBuffer[4]);
                    setRemoteIpAtIndex(3, ReceivedHidDataBuffer[5]);
                    setInitSendFlag(FALSE);
                    closeOSCSendPort();
                }
                break;
            case 0xF2:// Local Port
                if(!ReceivedHidDataBuffer[1])
                {
                    ToSendHidDataBuffer[0] = 0xF2;
                    ToSendHidDataBuffer[1] = 2;
                    ToSendHidDataBuffer[2] = (BYTE)((getLocalPort() >> 0) & 0x00FF);
                    ToSendHidDataBuffer[3] = (BYTE)((getLocalPort() >> 8) & 0x00FF);
                    ToSendHidDataBuffer[4] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                else
                {
                    setLocalPort(ReceivedHidDataBuffer[2] | (ReceivedHidDataBuffer[3] << 8));
                    setInitReceiveFlag(FALSE);
                    closeOSCReceivePort();
                }
                break;
            case 0xF3:// Remote Port
                if(!ReceivedHidDataBuffer[1])
                {
                    ToSendHidDataBuffer[0] = 0xF3;
                    ToSendHidDataBuffer[1] = 2;
                    ToSendHidDataBuffer[2] = (BYTE)((getRemotePort() >> 0) & 0x00FF);
                    ToSendHidDataBuffer[3] = (BYTE)((getRemotePort() >> 8) & 0x00FF);
                    ToSendHidDataBuffer[4] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                else
                {
                    setRemotePort(ReceivedHidDataBuffer[2] | (ReceivedHidDataBuffer[3] << 8));
                    setInitSendFlag(FALSE);
                    closeOSCSendPort();
                }
                break;
            case 0xF4:// Mac Address
                if(!ReceivedHidDataBuffer[1])
                {
                    ToSendHidDataBuffer[0] = 0xF4;
                    ToSendHidDataBuffer[1] = 6;
                    ToSendHidDataBuffer[2] = AppConfig.MyMACAddr.v[0];
                    ToSendHidDataBuffer[3] = AppConfig.MyMACAddr.v[1];
                    ToSendHidDataBuffer[4] = AppConfig.MyMACAddr.v[2];
                    ToSendHidDataBuffer[5] = AppConfig.MyMACAddr.v[3];
                    ToSendHidDataBuffer[6] = AppConfig.MyMACAddr.v[4];
                    ToSendHidDataBuffer[7] = AppConfig.MyMACAddr.v[5];
                    ToSendHidDataBuffer[8] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                break;
            case 0xF5:// Soft Reset
                if(ReceivedHidDataBuffer[1])
                {
                    NVMErasePage((void*)NVM_DATA);
                    NVMWriteWord((void*)(NVM_DATA), (unsigned int)0x01);
                }
                SoftReset();
                break;
            case 0xF6:// Save Current Settings
                if(ReceivedHidDataBuffer[1])
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

                    if(!USBHandleBusy(HIDTxHandle))
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

                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                break;
            case 0xF7:
                break;
        }
    }
}

/*******************************************************************************
  Function:
    void sendNote(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
#if 0
void sendNote(void)
{
    int i, j;
    for(i = 0; i < MAX_BTN_ROW; i++)
        btnLast[i] = btnCurrent[i];

    for(i = 0; i < MAX_BTN_ROW; i++)
    {
        for(j = 0; j < MAX_BTN_COL; j++)
        {
            if(buttonCheck(i, j))
            {
                midiData.Val = 0;
                midiData.CableNumber = 0;
                midiData.CodeIndexNumber = MIDI_CIN_NOTE_ON;
                midiData.DATA_0 = 0x90;
                midiData.DATA_1 = i * MAX_BTN_COL + j;
                midiData.DATA_2 = btnCurrent[i] >> j & 0x01;

                if(!USBHandleBusy(MIDITxHandle))
                    MIDITxHandle = USBTxOnePacket(MIDI_EP, (BYTE*)&midiData, 4);
            }
        }
    }
    //delayUs(2);
}
#endif

/*******************************************************************************
  Function:
    void sendControlChange(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void sendControlChange(void)
{
    int i;
    BYTE value;

    for(i = 0; i < AN_NUM; i++)
    {
        if(getAnalogEnable(i))
        {
            analogInHandle(i, (LONG)ReadADC10(i));

            if(getAnalogFlag(i))
            {
                midiData.Val = 0;
                midiData.CableNumber = 0;
                midiData.CodeIndexNumber = MIDI_CIN_CONTROL_CHANGE;
                midiData.DATA_0 = 0xB0;

                value = getAnalogByte(i, BYTE_ORIGINAL);
                
                midiData.DATA_1 = i;
                midiData.DATA_2 = value;
           
                if(!USBHandleBusy(MIDITxHandle))
                {
                    MIDITxHandle = USBTxOnePacket(MIDI_EP, (BYTE*)&midiData, 4);
                    resetAnalogFlag(i);
                }
                //delayUs(20);
            }
        }
    }
}

/*******************************************************************************
  Function:
    void receiveMIDIDatas(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void receiveMIDIDatas(void)
{
    int i;
    BYTE ch, num, val;

    if((USBDeviceState < CONFIGURED_STATE) || (USBSuspendControl == 1))
        return;

    if(!USBHandleBusy(MIDIRxHandle))
    {
        MIDIRxHandle = USBRxOnePacket(MIDI_EP, (BYTE*)&ReceivedMidiDataBuffer, 64);

        //MIDI Note
        i = 1;
        while((ReceivedMidiDataBuffer[i] & 0xF0) == 0x90)
        {
            ch = ReceivedMidiDataBuffer[i] & 0x0F;
            num = ReceivedMidiDataBuffer[i + 1];
            val = ReceivedMidiDataBuffer[i + 2];

            ReceivedMidiDataBuffer[i] &= 0x00;
            i += 4;
            continue;
        }

        //MIDI CC
        i = 1;
        while((ReceivedMidiDataBuffer[i] & 0xF0) == 0xB0)
        {
            ch = ReceivedMidiDataBuffer[i] & 0x0F;
            num = ReceivedMidiDataBuffer[i + 1];
            val = ReceivedMidiDataBuffer[i + 2];

            ReceivedMidiDataBuffer[i] &= 0x00;
            i += 4;
            continue;
        }
    }
}

/*******************************************************************************
  Function:
    void convertMidiToOsc(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void convertMidiToOsc(void)
{
    BYTE currentEndpoint;

    switch(ProcState)
    {
        case STATE_INITIALIZE:
            ProcState = STATE_IDLE;
            break;
        case STATE_IDLE:
            break;
        case STATE_READY:
            for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
            {                
                switch(endpointBuffers[currentEndpoint].TransferState)
                {
                    case RX_DATA:
                        if(USBHostMIDIRead(deviceHandle, currentEndpoint, endpointBuffers[currentEndpoint].pBufWriteLocation, endpointBuffers[currentEndpoint].numOfMIDIPackets * sizeof(USB_AUDIO_MIDI_PACKET)) == USB_SUCCESS)
                        {
                            endpointBuffers[currentEndpoint].TransferState = RX_DATA_WAIT;
                        }
                        break;
                    case RX_DATA_WAIT:
                        if(!USBHostMIDITransferIsBusy(deviceHandle, currentEndpoint))
                        {
                            midiType = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_0 & 0xF0;
                            midiCh = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_0 & 0x0F;
                            midiNum = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_1;
                            midiVal = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_2;
                            
                            if(getInitSendFlag())
                            {
                                switch(midiType)
                                {
                                    case 0x80:// Note off
                                    case 0x90:// Note on
                                        sendOSCMessage(midiPrefix, msgNote, "iii", midiCh, midiNum, midiVal);
                                        break;
                                    case 0xA0:// Key Pressure
                                        sendOSCMessage(midiPrefix, msgKp, "iii", midiCh, midiNum, midiVal);
                                        break;
                                    case 0xB0:// Control Change
                                        sendOSCMessage(midiPrefix, msgCc, "iii", midiCh, midiNum, midiVal);
                                        break;
                                    case 0xC0:// Program Change
                                        sendOSCMessage(midiPrefix, msgPc, "i", midiNum);
                                        break;
                                    case 0xD0:// Channel Pressure
                                        sendOSCMessage(midiPrefix, msgCp, "ii", midiCh, midiNum);
                                        break;
                                    case 0xE0:// Pitch Bend
                                        sendOSCMessage(midiPrefix, msgPb, "iii", midiCh, midiNum, midiVal);
                                        break;
                                    default:
                                        break;
                                }
                                //naze StackTask();
                            }

                            endpointBuffers[currentEndpoint].TransferState = RX_DATA;
                            endpointBuffers[currentEndpoint].pBufWriteLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;
                            if(endpointBuffers[currentEndpoint].pBufWriteLocation - endpointBuffers[currentEndpoint].bufferStart >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                            {
                                endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
                            }
                        }
                        break;
                    case TX_DATA:
                        if(endpointBuffers[currentEndpoint].pBufReadLocation != endpointBuffers[currentEndpoint].pBufWriteLocation)
                        {
                            if(USBHostMIDIWrite(deviceHandle, currentEndpoint, endpointBuffers[currentEndpoint].pBufReadLocation, endpointBuffers[currentEndpoint].numOfMIDIPackets * sizeof(USB_AUDIO_MIDI_PACKET)) == USB_SUCCESS)
                            {
                                endpointBuffers[currentEndpoint].TransferState = TX_DATA_WAIT;
                            }
                        }    
                        break;
                    case TX_DATA_WAIT:
                        if(!USBHostMIDITransferIsBusy(deviceHandle, currentEndpoint))
                        {
                            endpointBuffers[currentEndpoint].TransferState = TX_DATA;
                            endpointBuffers[currentEndpoint].pBufReadLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;
                                
                            if(endpointBuffers[currentEndpoint].pBufReadLocation - endpointBuffers[currentEndpoint].bufferStart >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                            {
                                endpointBuffers[currentEndpoint].pBufReadLocation = endpointBuffers[currentEndpoint].bufferStart;
                            }
                        }        
                        break;
                }
                //delayUs(2);
            }
            break;
        case STATE_ERROR:
            LED_2_On();
            break;
        default:
            ProcState = STATE_INITIALIZE;
            break;
    }
}

#if defined(USB_USE_HID) // hid
/*******************************************************************************
  Function:
    void convertHidToOsc(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void convertHidToOsc(void)
{
    //App_Detect_Device();
    switch(App_State_Mouse)
    {
        case DEVICE_NOT_CONNECTED:
            USBTasks();
            //if(USBHostHID_ApiDeviceDetect()) /* True if report descriptor is parsed with no error */
            if(USBHostHIDDeviceDetect(1))
            {
                App_State_Mouse = DEVICE_CONNECTED;
            }
            break;
        case DEVICE_CONNECTED:
            App_State_Mouse = READY_TO_TX_RX_REPORT;
            break;
        case READY_TO_TX_RX_REPORT:
            if(!USBHostHID_ApiDeviceDetect())
            {
                App_State_Mouse = DEVICE_NOT_CONNECTED;
            }
            else
            {
                App_State_Mouse = GET_INPUT_REPORT;
            }
            break;
        case GET_INPUT_REPORT:
            if(USBHostHID_ApiGetReport(Appl_raw_report_buffer.Report_ID,0,
                                       Appl_raw_report_buffer.ReportSize, Appl_raw_report_buffer.ReportData))
            {
                /* Host may be busy/error -- keep trying */
            }
            else
            {
                App_State_Mouse = INPUT_REPORT_PENDING;
            }
            USBTasks();
            break;
        case INPUT_REPORT_PENDING:
            if(USBHostHID_ApiTransferIsComplete(&ErrorDriver,&NumOfBytesRcvd))
            {
                if(ErrorDriver ||(NumOfBytesRcvd != Appl_raw_report_buffer.ReportSize ))
                {
                    ErrorCounter++ ; 
                    if(MAX_ERROR_COUNTER <= ErrorDriver)
                        App_State_Mouse = ERROR_REPORTED;
                    else
                        App_State_Mouse = READY_TO_TX_RX_REPORT;
                }
                else
                {
                    ErrorCounter = 0; 
                    ReportBufferUpdated = TRUE;
                    App_State_Mouse = READY_TO_TX_RX_REPORT;
                    
                    {
                        BYTE i;
                        for(i=0;i<Appl_raw_report_buffer.ReportSize;i++)
                        {
                            if(Appl_raw_report_buffer.ReportData[i] != 0)
                            {
                            }
                        }
                    }
                    App_ProcessInputReport();
                }
            }
            break;
        case ERROR_REPORTED:
            break;
        default:
            break;       
    }
}
#endif

#if defined(USB_USE_CDC)
/*******************************************************************************
  Function:
    void USBHostCDC_Clear_Out_DATA_Array(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void USBHostCDC_Clear_Out_DATA_Array(void)
{
    BYTE i;

    for(i = 0; i < MAX_NO_OF_OUT_BYTES; i++)
        USB_CDC_OUT_Data_Array[i] = 0;
}

/*******************************************************************************
  Function:
    void USB_CDC_RxTxHandler(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void USB_CDC_RxTxHandler(void)
{
    BYTE i;

    if(!USBHostCDC_ApiDeviceDetect()) /* TRUE if device is enumerated without error */
    {
       APPL_CDC_State = DEVICE_NOT_CONNECTED;
    }

    switch(APPL_CDC_State)
    {
        case DEVICE_NOT_CONNECTED:
            USBTasks();
            if(USBHostCDC_ApiDeviceDetect()) /* TRUE if device is enumerated without error */
            {
                APPL_CDC_State = DEVICE_CONNECTED;
            }
            break;
        case DEVICE_CONNECTED:
            APPL_CDC_State = READY_TO_TX_RX;
            break;
        case GET_IN_DATA:
            if(USBHostCDC_Api_Get_IN_Data(MAX_NO_OF_IN_BYTES, USB_CDC_IN_Data_Array))
            {
                APPL_CDC_State = GET_IN_DATA_WAIT;
            }
            else
            {
                APPL_CDC_State = READY_TO_TX_RX;
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
                    APPL_CDC_State = READY_TO_TX_RX;
                }
                else
                {
                    APPL_CDC_State = READY_TO_TX_RX;
                }
            }
            break;
        case SEND_OUT_DATA:
            if(USBHostCDC_Api_Send_OUT_Data(cdcOutDataLength, USB_CDC_OUT_Data_Array))
            {
                APPL_CDC_State = SEND_OUT_DATA_WAIT;
            }
            else
            {
                APPL_CDC_State = READY_TO_TX_RX;
            }
            break;
        case SEND_OUT_DATA_WAIT:
            if(USBHostCDC_ApiTransferIsComplete(&ErrorDriver, &NumOfBytesRcvd))
            {
                USBHostCDC_Clear_Out_DATA_Array();
                APPL_CDC_State = READY_TO_TX_RX;

                cdcSendFlag = FALSE;
            }
            break;
        case READY_TO_TX_RX:

            if(cdcSendFlag)
            {
                APPL_CDC_State = SEND_OUT_DATA;
                //cdcSendFlag = FALSE;
            }
            else
            {
                APPL_CDC_State = READY_TO_TX_RX;
            }
            break;
        default :
            break;
    }
}
#endif

// ******************************************************************************************************
// ************** USB Callback Functions ****************************************************************
// ******************************************************************************************************
// The USB firmware stack will call the callback functions USBCBxxx() in response to certain USB related
// events.  For example, if the host PC is powering down, it will stop sending out Start of Frame (SOF)
// packets to your device.  In response to this, all USB devices are supposed to decrease their power
// consumption from the USB Vbus to <2.5mA each.  The USB module detects this condition (which according
// to the USB specifications is 3+ms of no bus activity/SOF packets) and then calls the USBCBSuspend()
// function.  You should modify these callback functions to take appropriate actions for each of these
// conditions.  For example, in the USBCBSuspend(), you may wish to add code that will decrease power
// consumption from Vbus to <2.5mA (such as by clock switching, turning off LEDs, putting the
// microcontroller to sleep, etc.).  Then, in the USBCBWakeFromSuspend() function, you may then wish to
// add code that undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack will not automatically call this
// function.  This function is meant to be called from the application firmware instead.  See the
// additional comments near the function.

/******************************************************************************
 * Function:        void USBCBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Call back that is invoked when a USB suspend is detected
 *
 * Note:            None
 *****************************************************************************/
void USBCBSuspend(void)
{
	//Example power saving code.  Insert appropriate code here for the desired
	//application behavior.  If the microcontroller will be put to sleep, a
	//process similar to that shown below may be used:

	//ConfigureIOPinsForLowPower();
	//SaveStateOfAllInterruptEnableBits();
	//DisableAllInterruptEnableBits();
	//EnableOnlyTheInterruptsWhichWillBeUsedToWakeTheMicro();	//should enable at least USBActivityIF as a wake source
	//Sleep();
	//RestoreStateOfAllPreviouslySavedInterruptEnableBits();	//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.
	//RestoreIOPinsToNormal();									//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.

	//IMPORTANT NOTE: Do not clear the USBActivityIF (ACTVIF) bit here.  This bit is
	//cleared inside the usb_device.c file.  Clearing USBActivityIF here will cause
	//things to not work as intended.


    #if defined(__C30__)
    #if 0
        U1EIR = 0xFFFF;
        U1IR = 0xFFFF;
        U1OTGIR = 0xFFFF;
        IFS5bits.USB1IF = 0;
        IEC5bits.USB1IE = 1;
        U1OTGIEbits.ACTVIE = 1;
        U1OTGIRbits.ACTVIF = 1;
        Sleep();
    #endif
    #endif
}


/******************************************************************************
 * Function:        void _USB1Interrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the USB interrupt bit is set
 *					In this example the interrupt is only used when the device
 *					goes to sleep when it receives a USB suspend command
 *
 * Note:            None
 *****************************************************************************/
#if 0
void __attribute__ ((interrupt)) _USB1Interrupt(void)
{
    #if !defined(self_powered)
        if(U1OTGIRbits.ACTVIF)
        {
            IEC5bits.USB1IE = 0;
            U1OTGIEbits.ACTVIE = 0;
            IFS5bits.USB1IF = 0;

            //USBClearInterruptFlag(USBActivityIFReg,USBActivityIFBitNum);
            USBClearInterruptFlag(USBIdleIFReg,USBIdleIFBitNum);
            //USBSuspendControl = 0;
        }
    #endif
}
#endif

/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The host may put USB peripheral devices in low power
 *					suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *					mode, the host may wake the device back up by sending non-
 *					idle state signalling.
 *
 *					This call back is invoked when a wakeup from USB suspend
 *					is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
	// If clock switching or other power savings measures were taken when
	// executing the USBCBSuspend() function, now would be a good time to
	// switch back to normal full power run mode conditions.  The host allows
	// a few milliseconds of wakeup time, after which the device must be
	// fully back to normal, and capable of receiving and processing USB
	// packets.  In order to do this, the USB module must receive proper
	// clocking (IE: 48MHz clock must be available to SIE for full speed USB
	// operation).
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void)
{
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.

    if(msCounter != 0)
    {
        msCounter--;
    }
}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.

	// Typically, user firmware does not need to do anything special
	// if a USB error occurs.  For example, if the host sends an OUT
	// packet to your device, but the packet gets corrupted (ex:
	// because of a bad connection, or the user unplugs the
	// USB cable during the transmission) this will typically set
	// one or more USB error interrupt flags.  Nothing specific
	// needs to be done however, since the SIE will automatically
	// send a "NAK" packet to the host.  In response to this, the
	// host will normally retry to send the packet again, and no
	// data loss occurs.  The system will typically recover
	// automatically, without the need for application firmware
	// intervention.

	// Nevertheless, this callback function is provided, such as
	// for debugging purposes.
}


/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        When SETUP packets arrive from the host, some
 * 					firmware must process the request and respond
 *					appropriately to fulfill the request.  Some of
 *					the SETUP packets will be for standard
 *					USB "chapter 9" (as in, fulfilling chapter 9 of
 *					the official USB specifications) requests, while
 *					others may be specific to the USB device class
 *					that is being implemented.  For example, a HID
 *					class device needs to be able to respond to
 *					"GET REPORT" type of requests.  This
 *					is not a standard USB chapter 9 request, and
 *					therefore not handled by usb_device.c.  Instead
 *					this request should be handled by class specific
 *					firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *******************************************************************/
void USBCBCheckOtherReq(void)
{
	USBCheckHIDRequest();
}//end


/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is
 *					called when a SETUP, bRequest: SET_DESCRIPTOR request
 *					arrives.  Typically SET_DESCRIPTOR requests are
 *					not used in most applications, and it is
 *					optional to support this type of request.
 *
 * Note:            None
 *******************************************************************/
void USBCBStdSetDscHandler(void)
{
    // Must claim session ownership if supporting this request
}//end


/*******************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This
 *					callback function should initialize the endpoints
 *					for the device's usage according to the current
 *					configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
	//enable the HID endpoint
    USBEnableEndpoint(HID_EP, USB_IN_ENABLED | USB_OUT_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);
    //Re-arm the OUT endpoint for the next packet
    //USBOutHandle = HIDRxPacket(HID_EP | MIDI_EP,(BYTE*)&ReceivedHidDataBuffer,64);
    HIDRxHandle = USBRxOnePacket(HID_EP, (BYTE*)&ReceivedHidDataBuffer, 64);

    //enable the HID endpoint
    USBEnableEndpoint(MIDI_EP, USB_OUT_ENABLED | USB_IN_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);
    //Re-arm the OUT endpoint for the next packet
    MIDIRxHandle = USBRxOnePacket(MIDI_EP, (BYTE*)&ReceivedMidiDataBuffer, 64);
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB specifications allow some types of USB
 * 					peripheral devices to wake up a host PC (such
 *					as if it is in a low power suspend to RAM state).
 *					This can be a very useful feature in some
 *					USB applications, such as an Infrared remote
 *					control	receiver.  If a user presses the "power"
 *					button on a remote control, it is nice that the
 *					IR receiver can detect this signalling, and then
 *					send a USB "command" to the PC to wake up.
 *					
 *					The USBCBSendResume() "callback" function is used
 *					to send this special USB signalling which wakes 
 *					up the PC.  This function may be called by
 *					application firmware to wake up the PC.  This
 *					function will only be able to wake up the host if
 *                  all of the below are true:
 *					
 *					1.  The USB driver used on the host PC supports
 *						the remote wakeup capability.
 *					2.  The USB configuration descriptor indicates
 *						the device is remote wakeup capable in the
 *						bmAttributes field.
 *					3.  The USB host PC is currently sleeping,
 *						and has previously sent your device a SET 
 *						FEATURE setup packet which "armed" the
 *						remote wakeup capability.   
 *
 *                  If the host has not armed the device to perform remote wakeup,
 *                  then this function will return without actually performing a
 *                  remote wakeup sequence.  This is the required behavior, 
 *                  as a USB device that has not been armed to perform remote 
 *                  wakeup must not drive remote wakeup signalling onto the bus;
 *                  doing so will cause USB compliance testing failure.
 *                  
 *					This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            This function does nothing and returns quickly, if the USB
 *                  bus and host are not in a suspended condition, or are 
 *                  otherwise not in a remote wakeup ready state.  Therefore, it
 *                  is safe to optionally call this function regularly, ex: 
 *                  anytime application stimulus occurs, as the function will
 *                  have no effect, until the bus really is in a state ready
 *                  to accept remote wakeup. 
 *
 *                  When this function executes, it may perform clock switching,
 *                  depending upon the application specific code in 
 *                  USBCBWakeFromSuspend().  This is needed, since the USB
 *                  bus will no longer be suspended by the time this function
 *                  returns.  Therefore, the USB module will need to be ready
 *                  to receive traffic from the host.
 *
 *                  The modifiable section in this routine may be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of ~3-15 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at least 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
void USBCBSendResume(void)
{
    static WORD delay_count;
    
    //First verify that the host has armed us to perform remote wakeup.
    //It does this by sending a SET_FEATURE request to enable remote wakeup,
    //usually just before the host goes to standby mode (note: it will only
    //send this SET_FEATURE request if the configuration descriptor declares
    //the device as remote wakeup capable, AND, if the feature is enabled
    //on the host (ex: on Windows based hosts, in the device manager 
    //properties page for the USB device, power management tab, the 
    //"Allow this device to bring the computer out of standby." checkbox 
    //should be checked).
    if(USBGetRemoteWakeupStatus() == TRUE) 
    {
        //Verify that the USB bus is in fact suspended, before we send
        //remote wakeup signalling.
        if(USBIsBusSuspended() == TRUE)
        {
            USBMaskInterrupts();
            
            //Clock switch to settings consistent with normal USB operation.
            USBCBWakeFromSuspend();
            USBSuspendControl = 0; 
            USBBusIsSuspended = FALSE;  //So we don't execute this code again, 
                                        //until a new suspend condition is detected.

            //Section 7.1.7.7 of the USB 2.0 specifications indicates a USB
            //device must continuously see 5ms+ of idle on the bus, before it sends
            //remote wakeup signalling.  One way to be certain that this parameter
            //gets met, is to add a 2ms+ blocking delay here (2ms plus at 
            //least 3ms from bus idle to USBIsBusSuspended() == TRUE, yeilds
            //5ms+ total delay since start of idle).
            delay_count = 3600U;        
            do
            {
                delay_count--;
            }while(delay_count);
            
            //Now drive the resume K-state signalling onto the USB bus.
            USBResumeControl = 1;       // Start RESUME signaling
            delay_count = 1800U;        // Set RESUME line for 1-13 ms
            do
            {
                delay_count--;
            }while(delay_count);
            USBResumeControl = 0;       //Finished driving resume signalling

            USBUnmaskInterrupts();
        }
    }
}


/****************************************************************************
  Function:
    void App_Detect_Device(void)

  Description:
    This function monitors the status of device connected/disconnected

  Precondition:
    None

  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
***************************************************************************/
#if defined(USB_USE_HID)
void App_Detect_Device(void)
{
  if(!USBHostHID_ApiDeviceDetect())
  {
     App_State_Mouse = DEVICE_NOT_CONNECTED;
  }
}
#endif

/****************************************************************************
  Function:
    void App_ProcessInputReport(void)

  Description:
    This function processes input report received from HID device.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
***************************************************************************/
#if defined(USB_USE_HID)
void App_ProcessInputReport(void)
{
    BYTE  data;
   /* process input report received from device */
    USBHostHID_ApiImportData(Appl_raw_report_buffer.ReportData, Appl_raw_report_buffer.ReportSize
                          ,Appl_Button_report_buffer, &Appl_Mouse_Buttons_Details);
    USBHostHID_ApiImportData(Appl_raw_report_buffer.ReportData, Appl_raw_report_buffer.ReportSize
                          ,Appl_XY_report_buffer, &Appl_XY_Axis_Details);

 // X-axis
    data = (Appl_XY_report_buffer[0] & 0xF0) >> 4;
    data = (Appl_XY_report_buffer[0] & 0x0F);

 // Y-axis
    data = (Appl_XY_report_buffer[1] & 0xF0) >> 4;
    data = (Appl_XY_report_buffer[1] & 0x0F);
    
    sendOSCMessage("/hid", "/get", "iiii", Appl_XY_report_buffer[0], Appl_XY_report_buffer[1], Appl_Button_report_buffer[0], Appl_Button_report_buffer[1]);
    
    if(Appl_Button_report_buffer[0] == 1)
    {
    }
    if(Appl_Button_report_buffer[1] == 1)
    {
    }

}
#endif

/****************************************************************************
  Function:
    BOOL USB_HID_DataCollectionHandler(void)
  Description:
    This function is invoked by HID client , purpose is to collect the 
    details extracted from the report descriptor. HID client will store
    information extracted from the report descriptor in data structures.
    Application needs to create object for each report type it needs to 
    extract.
    For ex: HID_DATA_DETAILS Appl_ModifierKeysDetails;
    HID_DATA_DETAILS is defined in file usb_host_hid_appl_interface.h
    Each member of the structure must be initialized inside this function.
    Application interface layer provides functions :
    USBHostHID_ApiFindBit()
    USBHostHID_ApiFindValue()
    These functions can be used to fill in the details as shown in the demo
    code.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    TRUE    - If the report details are collected successfully.
    FALSE   - If the application does not find the the supported format.

  Remarks:
    This Function name should be entered in the USB configuration tool
    in the field "Parsed Data Collection handler".
    If the application does not define this function , then HID cient 
    assumes that Application is aware of report format of the attached
    device.
***************************************************************************/
#if defined(USB_USE_HID)
BOOL USB_HID_DataCollectionHandler(void)
{
  BYTE NumOfReportItem = 0;
  BYTE i;
  USB_HID_ITEM_LIST* pitemListPtrs;
  USB_HID_DEVICE_RPT_INFO* pDeviceRptinfo;
  HID_REPORTITEM *reportItem;
  HID_USAGEITEM *hidUsageItem;
  BYTE usageIndex;
  BYTE reportIndex;

  pDeviceRptinfo = USBHostHID_GetCurrentReportInfo(); // Get current Report Info pointer
  pitemListPtrs = USBHostHID_GetItemListPointers();   // Get pointer to list of item pointers

  BOOL status = FALSE;
   /* Find Report Item Index for Modifier Keys */
   /* Once report Item is located , extract information from data structures provided by the parser */
   NumOfReportItem = pDeviceRptinfo->reportItems;
   for(i=0;i<NumOfReportItem;i++)
    {
       reportItem = &pitemListPtrs->reportItemList[i];
       //if((reportItem->reportType==hidReportInput) && (reportItem->dataModes == (HIDData_Variable|HIDData_Relative))&&
       if((reportItem->reportType==hidReportInput) && (reportItem->dataModes == (HIDData_Variable))&&
           (reportItem->globals.usagePage==USAGE_PAGE_GEN_DESKTOP))
        {
           /* We now know report item points to modifier keys */
           /* Now make sure usage Min & Max are as per application */
            usageIndex = reportItem->firstUsageItem;
            hidUsageItem = &pitemListPtrs->usageItemList[usageIndex];

            reportIndex = reportItem->globals.reportIndex;
            Appl_XY_Axis_Details.reportLength = (pitemListPtrs->reportList[reportIndex].inputBits + 7)/8;
            Appl_XY_Axis_Details.reportID = (BYTE)reportItem->globals.reportID;
            Appl_XY_Axis_Details.bitOffset = (BYTE)reportItem->startBit;
            Appl_XY_Axis_Details.bitLength = (BYTE)reportItem->globals.reportsize;
            Appl_XY_Axis_Details.count=(BYTE)reportItem->globals.reportCount;
            Appl_XY_Axis_Details.interfaceNum= USBHostHID_ApiGetCurrentInterfaceNum();
        }
        else if((reportItem->reportType==hidReportInput) && (reportItem->dataModes == HIDData_Variable)&&
           (reportItem->globals.usagePage==USAGE_PAGE_BUTTONS))
        {
           /* We now know report item points to modifier keys */
           /* Now make sure usage Min & Max are as per application */
            usageIndex = reportItem->firstUsageItem;
            hidUsageItem = &pitemListPtrs->usageItemList[usageIndex];

            reportIndex = reportItem->globals.reportIndex;
            Appl_Mouse_Buttons_Details.reportLength = (pitemListPtrs->reportList[reportIndex].inputBits + 7)/8;
            Appl_Mouse_Buttons_Details.reportID = (BYTE)reportItem->globals.reportID;
            Appl_Mouse_Buttons_Details.bitOffset = (BYTE)reportItem->startBit;
            Appl_Mouse_Buttons_Details.bitLength = (BYTE)reportItem->globals.reportsize;
            Appl_Mouse_Buttons_Details.count=(BYTE)reportItem->globals.reportCount;
            Appl_Mouse_Buttons_Details.interfaceNum= USBHostHID_ApiGetCurrentInterfaceNum();
        }
    }

   if(pDeviceRptinfo->reports == 1)
    {
        Appl_raw_report_buffer.Report_ID = 0;
        Appl_raw_report_buffer.ReportSize = (pitemListPtrs->reportList[reportIndex].inputBits + 7)/8;
//        Appl_raw_report_buffer.ReportData = (BYTE*)malloc(Appl_raw_report_buffer.ReportSize);
        Appl_raw_report_buffer.ReportPollRate = pDeviceRptinfo->reportPollingRate;
        status = TRUE;
    }

    return(status);
}
#endif

/*******************************************************************
 * Function:        BOOL USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, WORD size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  WORD size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch( (INT)event )
    {
        case EVENT_TRANSFER:
            //Add application specific callback task or callback function here if desired.
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_CONFIGURED:
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        case EVENT_TRANSFER_TERMINATED:
            //Add application specific callback task or callback function here if desired.
            //The EVENT_TRANSFER_TERMINATED event occurs when the host performs a CLEAR
            //FEATURE (endpoint halt) request on an application endpoint which was 
            //previously armed (UOWN was = 1).  Here would be a good place to:
            //1.  Determine which endpoint the transaction that just got terminated was 
            //      on, by checking the handle value in the *pdata.
            //2.  Re-arm the endpoint if desired (typically would be the case for OUT 
            //      endpoints).
            break;
        default:
            break;
    }
    return TRUE;
}

/*************************************************************************
 * Function:        USB_ApplicationEventHandler
 *
 * Preconditions:   The USB must be initialized.
 *
 * Input:           event       Identifies the bus event that occured
 *
 *                  data        Pointer to event-specific data
 *
 *                  size        Size of the event-specific data
 *
 * Output:          deviceHandle (global)
 *                  Updates device handle pointer.
 *
 *                  endpointBuffers (global)
 *                  Allocates or Deallocates endpoint buffers.
 *
 *                  DemoState (global)
 *                  Updates the demo state as appropriate when events occur.
 *
 * Returns:         TRUE if the event was handled, FALSE if not
 *
 * Side Effects:    Event-specific actions have been taken.
 *
 * Overview:        This routine is called by the Host layer or client
 *                  driver to notify the application of events that occur.
 *                  If the event is recognized, it is handled and the
 *                  routine returns TRUE.  Otherwise, it is ignored (or
 *                  just "sniffed" and the routine returns FALSE.
 *************************************************************************/

BOOL USB_ApplicationEventHandler ( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    BYTE currentEndpoint;
    ENDPOINT_DIRECTION direction;

    // Handle specific events.
    switch ((INT)event)
    {
        case EVENT_VBUS_REQUEST_POWER:
        case EVENT_VBUS_RELEASE_POWER:
        case EVENT_HUB_ATTACH:
        case EVENT_UNSUPPORTED_DEVICE:
        case EVENT_CANNOT_ENUMERATE:
        case EVENT_CLIENT_INIT_ERROR:
        case EVENT_OUT_OF_MEMORY:
        case EVENT_UNSPECIFIED_ERROR:   // This should never be generated.
        case EVENT_SUSPEND:
        case EVENT_DETACH:
        case EVENT_RESUME:
        case EVENT_BUS_ERROR:
            return TRUE;
            break;
#if defined(USB_USE_HID)
        case EVENT_HID_RPT_DESC_PARSED:
            #ifdef APPL_COLLECT_PARSED_DATA
                return(APPL_COLLECT_PARSED_DATA());
            #else
                return TRUE;
            #endif
            break;
#elif defined(USB_USE_CDC)
        case EVENT_CDC_NONE:
        case EVENT_CDC_ATTACH:
        case EVENT_CDC_COMM_READ_DONE:
        case EVENT_CDC_COMM_WRITE_DONE:
        case EVENT_CDC_DATA_READ_DONE:
        case EVENT_CDC_DATA_WRITE_DONE:
        case EVENT_CDC_RESET:
            return TRUE;
            break;
        case EVENT_CDC_NAK_TIMEOUT:
                APPL_CDC_State = READY_TO_TX_RX;
                return TRUE;
            break;
#endif
        case EVENT_MIDI_ATTACH:
            deviceHandle = data;
            ProcState = STATE_READY;
            
            endpointBuffers = malloc(sizeof(ENDPOINT_BUFFER) * USBHostMIDINumberOfEndpoints(deviceHandle));
            
            for( currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++ )
            {
                direction = USBHostMIDIEndpointDirection(deviceHandle, currentEndpoint);
                // For OUT endpoints
                if(direction == OUT)
                {   
                    // We only want to send NUM_MIDI_PKTS_IN_USB_PKT MIDI packet per USB packet
                    endpointBuffers[currentEndpoint].numOfMIDIPackets = NUM_MIDI_PKTS_IN_USB_PKT;
                    
                    // And we want to start it off transmitting data
                    endpointBuffers[currentEndpoint].TransferState = TX_DATA;
                }
                // For IN endpoints
                else if (direction == IN)
                {
                    // We will accept however many will fit inside the maximum USB packet size
                    endpointBuffers[currentEndpoint].numOfMIDIPackets = USBHostMIDISizeOfEndpoint(deviceHandle, currentEndpoint) / sizeof(USB_AUDIO_MIDI_PACKET);
                    
                    // And we want to start it off trying to read data
                    endpointBuffers[currentEndpoint].TransferState = RX_DATA;
                }
                else
                {
                    continue;
                }
                
                // Allocate the 2D buffer, and keep track of the write and read locations
                endpointBuffers[currentEndpoint].bufferStart = malloc( sizeof(USB_AUDIO_MIDI_PACKET) * endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE );
                endpointBuffers[currentEndpoint].pBufReadLocation = endpointBuffers[currentEndpoint].bufferStart;
                endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
            }    

            return TRUE;
            break;
        case EVENT_MIDI_DETACH:
            for( currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++ )
            {
                free(endpointBuffers[currentEndpoint].bufferStart);
                endpointBuffers[currentEndpoint].bufferStart = NULL;
                endpointBuffers[currentEndpoint].pBufReadLocation = NULL;
                endpointBuffers[currentEndpoint].pBufWriteLocation = NULL;
            }
            
            free(endpointBuffers);
            endpointBuffers = NULL;
            
            deviceHandle = NULL;
            ProcState = STATE_INITIALIZE;
            return TRUE;
            break;
        case EVENT_MIDI_TRANSFER_DONE:  // The main state machine will poll the driver.
            return TRUE;
            break;
        default:
            break;
    }
    return FALSE;
} // USB_ApplicationEventHandler
