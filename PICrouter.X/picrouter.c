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
 * picrouter.c,v.1.4.8 2013/04/02
 */

#include "picrouter.h"

void _general_exception_handler(unsigned cause, unsigned status)
{
    Nop();
    Nop();
}

void initIOPorts(void)
{
    BYTE i = 0;

    for(i = 0; i < 14; i++)
    {
        //configAnPort(i, IO_OUT);
        //outputAnPort(i, LOW);
        setAnPortDioType(i, IO_IN);
    }

    for(i = 0; i < 4; i++)
    {
        setPwmPortDioType(i, IO_OUT);
        outputPwmPort(i, LOW);
    }

    for(i = 0; i < 4; i++)
    {
        setDigitalPortDioType(i, IO_OUT);
        if(i == 1)
            outputDigitalPort(i, HIGH);
        else
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

int main(int argc, char** argv) {
    int i;
    static DWORD t = 0;

    // Enable optimal performance
    SYSTEMConfigPerformance(GetSystemClock());
    mOSCSetPBDIV(OSC_PB_DIV_1); // Use 1:1 CPU Core:Peripheral clocks

    // Enable the cache for the best performance
    CheKseg0CacheOn();

    DelayMs(100);
    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

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

    mDNSInitialize(DEFAULT_HOST_NAME);
    mDNSServiceRegister((const char *)DEFAULT_HOST_NAME, // base name of the service
                        "_oscit._udp.local",       // type of the service
                        8080,                      // TCP or UDP port, at which this service is available
                        ((const BYTE *)""),        // TXT info
                        0,                         // auto rename the service when if needed
                        NULL,                      // no callback function
                        NULL                       // no application context
                        );
    mDNSMulticastFilterRegister();

    // Enable multi-vectored interrupts
    INTEnableSystemMultiVectoredInt();

    LED_1_Off();
    LED_2_Off();
    DelayMs(500);

    while(1)
    {
#if 0
        if(TickGet() - t >= TICK_SECOND / 2ul)
        {
            t = TickGet();
            LED_2_Toggle();
        }
#endif
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
                    mDNSProcess();
                    DHCPServerTask();

                    receiveOSCTask();
                    sendOSCTask();

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
                    mDNSProcess();
                    DHCPServerTask();

                    receiveOSCTask();
                    sendOSCTask();

                    if(bUsbHostInitialized)
                    {
                        USBHostTasks();
                        convertMidiToOsc();

                        //hid USBHostHIDTasks();
                        //hid convertHidToOsc();
                    }
                    else
                        bUsbHostInitialized = USBHostInit(0);
                }
                break;
            default:
                break;
        }
        
#if 0
    	if(stateFlag2)
        {

        	midiData.Val = 0;
  			midiData.CableNumber = 0;
  			midiData.CodeIndexNumber = MIDI_CIN_NOTE_ON;
  			
  			midiData.DATA_0 = 0x90;
      		midiData.DATA_1 = 30;
      		midiData.DATA_2 = (1 - currentState) * 127;

      		if(!USBHandleBusy(MIDITxHandle))
      		{
          		MIDITxHandle = USBTxOnePacket(MIDI_EP, (BYTE*)&midiData, 4);
          		stateFlag2 = FALSE;
          	}
        }
        prevState = currentState;
#endif
    }
    return (EXIT_SUCCESS);
}

/**********************************************
*  OSC Generic I/O Processing Part
**********************************************/
void receiveOSCTask(void)
{
    WORD rcvLen;
    BYTE index, i, j, k;

    if(!initReceiveFlag)
        initReceiveFlag = openOSCReceivePort(localPort);

    if(initReceiveFlag && isOSCGetReady(rcvLen))
        getOSCPacket();

    if(processOSCPacket())
    {
        //debug LED_2_On();

        if(compareOSCPrefix(stdPrefix))
        {
            if(compareOSCAddress(stdPrefix, msgOnboardLed))
            {
                if(!compareTypeTagAtIndex(0, 'i') || !compareTypeTagAtIndex(1, 's'))
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgOnboardLed, ": wrong_argument_type");
                    //sendOSCMessage(sysPrefix, msgError, "si", msgOnboardLed, 314);
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
            else if(compareOSCAddress(stdPrefix, msgSetPortIO))
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
            else if(compareOSCAddress(stdPrefix, msgGetPortIO))
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
                    sendOSCMessage(stdPrefix, msgPortIO, "ss", port_name, "in");
                else
                    sendOSCMessage(stdPrefix, msgPortIO, "ss", port_name, "out");
            }
            else if(compareOSCAddress(stdPrefix, msgSetPortOut))
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
            else if(compareOSCAddress(stdPrefix, msgGetPortIn))
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
                    sendOSCMessage(stdPrefix, msgAdcDi, "ss", port_name, "high");
                else
                    sendOSCMessage(stdPrefix, msgAdcDi, "ss", port_name, "low");
            }
            // A/D
            else if(compareOSCAddress(stdPrefix, msgSetAdcEnable))
            {
                AD1CON1bits.ON = 0;

                BYTE id, anum;
                char* state;

                if(compareTypeTagAtIndex(0, 'i') && compareTypeTagAtIndex(1, 's'))
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
                    analogEnable[id] = TRUE;
                    setAnPortDioType(id, IO_IN);

                    AD1PCFG &= ~(0x0001 << id);
                    AD1CSSL |= (0x0001 << id);
                }
                else if(!strcmp(state, "off"))
                {
                    analogEnable[id] = FALSE;
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
                    if(analogEnable[i])
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
            else if(compareOSCAddress(stdPrefix, msgGetAdcEnable))
            {
                BYTE id;

                if(compareTypeTagAtIndex(0, 'i'))
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

                if(analogEnable[id])
                    sendOSCMessage(stdPrefix, msgAdcEnable, "is", id, "on");
                else
                    sendOSCMessage(stdPrefix, msgAdcEnable, "is", id, "off");
            }
            else if(compareOSCAddress(stdPrefix, msgSetAdcDio))
            {
                BYTE id;
                char* type;
 
                if(compareTypeTagAtIndex(0, 'i') && compareTypeTagAtIndex(1, 's'))
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
            else if(compareOSCAddress(stdPrefix, msgGetAdcDio))
            {
                BYTE id;
                if(compareTypeTagAtIndex(0, 'i'))
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
                    sendOSCMessage(stdPrefix, msgAdcDio, "is", id, "in");
                else
                    sendOSCMessage(stdPrefix, msgAdcDio, "is", id, "out");
            }
            else if(compareOSCAddress(stdPrefix, msgSetAdcDo))
            {
                BYTE id;
                char* state;

                if(compareTypeTagAtIndex(0, 'i') && compareTypeTagAtIndex(1, 's'))
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
            else if(compareOSCAddress(stdPrefix, msgGetAdcDi))
            {
                BYTE id;

                if(compareTypeTagAtIndex(0, 'i'))
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
                    sendOSCMessage(stdPrefix, msgAdcDi, "is", id, "high");
                else
                    sendOSCMessage(stdPrefix, msgAdcDi, "is", id, "low");
            }
            // PWM
            else if(compareOSCAddress(stdPrefix, msgSetPwmEnable))
            {
                char* state;
                if(compareTypeTagAtIndex(0, 'i') && compareTypeTagAtIndex(1, 's'))
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
            else if(compareOSCAddress(stdPrefix, msgGetPwmEnable))
            {
                if(compareTypeTagAtIndex(0, 'i'))
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

                sendOSCMessage(stdPrefix, msgPwmEnable, "is", index, onSquare[index] ? "on" : "off");
            }
            else if(compareOSCAddress(stdPrefix, msgSetPwmFreq))
            {
                if(compareTypeTagAtIndex(0, 'i'))
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
            else if(compareOSCAddress(stdPrefix, msgGetPwmFreq))
            {
                sendOSCMessage(stdPrefix, msgPwmFreq, "i", freq);
            }
            else if(compareOSCAddress(stdPrefix, msgSetPwmDuty))
            {
                if(compareTypeTagAtIndex(0, 'i') && compareTypeTagAtIndex(1, 'i'))
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
            else if(compareOSCAddress(stdPrefix, msgGetPwmDuty))
            {
                if(compareTypeTagAtIndex(0, 'i'))
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

                sendOSCMessage(stdPrefix, msgPwmDuty, "ii", index, duty[index]);
            }
            else if(compareOSCAddress(stdPrefix, msgSetPwmDio))
            {
                BYTE id;
                char* type;

                if(compareTypeTagAtIndex(0, 'i') && compareTypeTagAtIndex(1, 's'))
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
            else if(compareOSCAddress(stdPrefix, msgGetPwmDio))
            {
                BYTE id;
                if(compareTypeTagAtIndex(0, 'i'))
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
                    sendOSCMessage(stdPrefix, msgPwmDio, "is", id, "in");
                else
                    sendOSCMessage(stdPrefix, msgPwmDio, "is", id, "out");
            }
            else if(compareOSCAddress(stdPrefix, msgSetPwmDo))
            {
                BYTE id;
                char* state;

                if(compareTypeTagAtIndex(0, 'i') && compareTypeTagAtIndex(1, 's'))
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
            else if(compareOSCAddress(stdPrefix, msgGetPwmDi))
            {
                BYTE id;
                BYTE state;

                if(compareTypeTagAtIndex(0, 'i'))
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
                    sendOSCMessage(stdPrefix, msgPwmDi, "is", id, "high");
                else
                    sendOSCMessage(stdPrefix, msgPwmDi, "is", id, "low");
            }
            // D/IO
            else if(compareOSCAddress(stdPrefix, msgSetDigitalDio))
            {
                BYTE id;
                char* type;

                if(compareTypeTagAtIndex(0, 'i') && compareTypeTagAtIndex(1, 's'))
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
            else if(compareOSCAddress(stdPrefix, msgGetDigitalDio))
            {
                BYTE id;
                if(compareTypeTagAtIndex(0, 'i'))
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
                    sendOSCMessage(stdPrefix, msgDigitalDio, "is", id, "in");
                else
                    sendOSCMessage(stdPrefix, msgDigitalDio, "is", id, "out");
            }
            else if(compareOSCAddress(stdPrefix, msgSetDigitalDo))
            {
                BYTE id;
                char* state;

                if(compareTypeTagAtIndex(0, 'i') && compareTypeTagAtIndex(1, 's'))
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
            else if(compareOSCAddress(stdPrefix, msgGetDigitalDi))
            {
                BYTE id;
                BYTE state;

                if(compareTypeTagAtIndex(0, 'i'))
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
                    sendOSCMessage(stdPrefix, msgDigitalDi, "is", id, "high");
                else
                    sendOSCMessage(stdPrefix, msgDigitalDi, "is", id, "low");
            }
            // SPI
            else if(compareOSCAddress(stdPrefix, msgSetSpiConfig))
            {
                BYTE id;
                DWORD bitrate0 = 0;
                WORD bitrate = 0;
                DWORD spiFlags = 0;

                if(compareTypeTagAtIndex(0, 'i') && compareTypeTagAtIndex(1, 'i'))
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
            else if(compareOSCAddress(stdPrefix, msgSetSpiData))
            {
                BYTE id = 0;
                char* load_port;
                char* active_state;
                BYTE byte_num = 0;
                WORD data[4] = {0};
                //DWORD usec = 0;

                if(getArgumentsLength() < 5)
                {
                    sendOSCMessage(sysPrefix, msgError, "si", "need_4_arguments_at_leaset", getArgumentsLength());
                    return;
                }

                if(compareTypeTagAtIndex(0, 'i') && compareTypeTagAtIndex(1, 's') && compareTypeTagAtIndex(2, 's') && compareTypeTagAtIndex(3, 'i'))
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
                        if(!compareTypeTagAtIndex(i + 4, 'i'))
                        {
                            sendOSCMessage(sysPrefix, msgError, "ss", msgSetSpiData, ": 4:wrong_argument_type");
                            return;
                        }

                        data[i] = getIntArgumentAtIndex(i + 4);
                    }

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
            else if(compareOSCAddress(stdPrefix, msgGetSpiData))
            {
                BYTE id = 0;
                char* load_port;
                char* active_state;
                BYTE byte_num = 0;
                WORD data[4] = {0};
                //DWORD usec = 0;

                if(getArgumentsLength() < 4)
                {
                    sendOSCMessage(sysPrefix, msgError, "si", "need_4_arguments_at_leaset", getArgumentsLength());
                    return;
                }

                if(compareTypeTagAtIndex(0, 'i') && compareTypeTagAtIndex(1, 's') && compareTypeTagAtIndex(2, 's') && compareTypeTagAtIndex(3, 'i'))
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

                    if(compareTypeTagAtIndex(4, 'i'))
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
                        sendOSCMessage(stdPrefix, msgSpiData, "ii", id, data[0]);
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
            else if(compareOSCAddress(stdPrefix, msgSetSpiDio))
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
            else if(compareOSCAddress(stdPrefix, msgGetSpiDio))
            {
                char* name;

                if(compareTypeTagAtIndex(0, 's') && compareTypeTagAtIndex(1, 's'))
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
                    sendOSCMessage(stdPrefix, msgDigitalDio, "is", name, "in");
                else
                    sendOSCMessage(stdPrefix, msgDigitalDio, "is", name, "out");
            }
            else if(compareOSCAddress(stdPrefix, msgSetSpiDo))
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
            else if(compareOSCAddress(stdPrefix, msgGetSpiDi))
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
                    sendOSCMessage(stdPrefix, msgSpiDi, "ss", name, "high");
                else
                    sendOSCMessage(stdPrefix, msgSpiDi, "ss", name, "low");
            }
        }
        else if(compareOSCPrefix(sysPrefix))
        {
            // System Setting
            if(compareOSCAddress(sysPrefix, msgSetRemoteIp))
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
                remoteIP[0] = (BYTE)ip[0];
                remoteIP[1] = (BYTE)ip[1];
                remoteIP[2] = (BYTE)ip[2];
                remoteIP[3] = (BYTE)ip[3];
                initSendFlag = FALSE;
                chCompletedFlag = TRUE;
                closeOSCSendPort();
            }
            else if(compareOSCAddress(sysPrefix, msgGetRemoteIp))
            {
                char rip[15];
                sprintf(rip, "%d.%d.%d.%d", remoteIP[0], remoteIP[1], remoteIP[2], remoteIP[3]);
                sendOSCMessage(sysPrefix, msgRemoteIp, "s", rip);
            }
            else if(compareOSCAddress(sysPrefix, msgSetRemotePort))
            {
                if(compareTypeTagAtIndex(0, 'i'))
                {
                    if(getIntArgumentAtIndex(0) < 0)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgGetRemoteIp, ": out_of_value_range");
                        return;
                    }
                    remotePort = getIntArgumentAtIndex(0);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgGetRemoteIp, ": wrong_argument_type");
                    return;
                }

                closeOSCSendPort();
                initSendFlag = FALSE;
                chCompletedFlag = TRUE;
            }
            else if(compareOSCAddress(sysPrefix, msgGetRemotePort))
            {
                sendOSCMessage(sysPrefix, msgRemotePort, "i", remotePort);
            }
            else if(compareOSCAddress(sysPrefix, msgSetHostName))
            {
                char* srcName = NULL;
                char* np = NULL;
                int len = 0;

                if(compareTypeTagAtIndex(0, 's'))
                {
                    srcName = getStringArgumentAtIndex(0);
                    np = strstr(srcName, ".local");
                    if(np == NULL)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetHostName, ": wrong_argument_string");
                        return;
                    }

                    len = np - srcName;
                    if(len >= 32)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetHostName, ": too_long_string");
                        return;
                    }

                    mDNSServiceDeRegister();
                    free(hostName);
                    hostName = NULL;
                    hostName = (char *)calloc(len + 1, sizeof(char));
                    strncpy(hostName, srcName, len);

                    //debug sendOSCMessage(sysPrefix, msgError, "ss", srcName, hostName);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetHostName, ": wrong_argument_type");
                    return;
                }

                mDNSInitialize(hostName);
                mDNSServiceRegister((const char *)hostName, // base name of the service
                                    "_oscit._udp.local",    // type of the service
                                    8080,                   // TCP or UDP port, at which this service is available
                                    ((const BYTE *)""),     // TXT info
                                    0,                      // auto rename the service when if needed
                                    NULL,                   // no callback function
                                    NULL                    // no application context
                    );
                mDNSMulticastFilterRegister();
                sendOSCMessage(sysPrefix, msgConfiguration, "s", "succeeded");
            }
            else if(compareOSCAddress(sysPrefix, msgGetHostName))
            {
                char fullHostName[32] = {0};
                sprintf(fullHostName, "%s.local", hostName);
                sendOSCMessage(sysPrefix, msgHostName, "s", fullHostName);
            }
            else if(compareOSCAddress(sysPrefix, msgGetHostIp))
            {
                char hip[15] = {0};
                BYTE hip0 = AppConfig.MyIPAddr.Val & 0xFF;
                BYTE hip1 = (AppConfig.MyIPAddr.Val >> 8) & 0xFF;
                BYTE hip2 = (AppConfig.MyIPAddr.Val >> 16) & 0xFF;
                BYTE hip3 = (AppConfig.MyIPAddr.Val >> 24) & 0xFF;
                sprintf(hip, "%d.%d.%d.%d", hip0, hip1, hip2, hip3);
                sendOSCMessage(sysPrefix, msgHostIp, "s", hip);
            }
            else if(compareOSCAddress(sysPrefix, msgGetHostMac))
            {
                char macaddr[17] = {0};
                sprintf(macaddr, "%X:%X:%X:%X:%X:%X", AppConfig.MyMACAddr.v[0], AppConfig.MyMACAddr.v[1], AppConfig.MyMACAddr.v[2], AppConfig.MyMACAddr.v[3], AppConfig.MyMACAddr.v[4], AppConfig.MyMACAddr.v[5]);
                sendOSCMessage(sysPrefix, msgHostMac, "s", macaddr);
            }
            else if(compareOSCAddress(sysPrefix, msgSetHostPort))
            {
                if(compareTypeTagAtIndex(0, 'i'))
                {
                    if(getIntArgumentAtIndex(0) < 0)
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetHostPort, ": out_of_value_range");
                        return;
                    }
                    localPort = getIntArgumentAtIndex(0);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetHostPort, ": wrong_argument_type");
                    return;
                }

                closeOSCReceivePort();
                initReceiveFlag = FALSE;
                chCompletedFlag = TRUE;
            }
            else if(compareOSCAddress(sysPrefix, msgGetHostPort))
            {
                sendOSCMessage(sysPrefix, msgHostPort, "i", localPort);
            }
            else if(compareOSCAddress(sysPrefix, msgSetPrefix))
            {
                if(compareTypeTagAtIndex(0, 's'))
                {
                    char* str = getStringArgumentAtIndex(0);
                    if(str[0] != '/')
                    {
                        sendOSCMessage(sysPrefix, msgError, "ss", msgSetPrefix, ": must_begin_slash");
                        return;
                    }
                    free(stdPrefix);
                    stdPrefix = NULL;
                    stdPrefix = getStringArgumentAtIndex(0);
                }
                else
                {
                    sendOSCMessage(sysPrefix, msgError, "ss", msgSetPrefix, ": wrong_argument_type");
                    return;
                }
                sendOSCMessage(sysPrefix, msgConfiguration, "s", "succeeded");
            }
            else if(compareOSCAddress(sysPrefix, msgGetPrefix))
            {
                sendOSCMessage(sysPrefix, msgPrefix, "s", stdPrefix);
            }
            else if(compareOSCAddress(sysPrefix, msgSwitchUsbMode))
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
            else if(compareOSCAddress(sysPrefix, msgGetUsbMode))
            {
                if(device_mode == MODE_DEVICE)
                    sendOSCMessage(sysPrefix, msgUsbMode, "s", "device");
                else if(device_mode == MODE_HOST)
                    sendOSCMessage(sysPrefix, msgUsbMode, "s", "host");
            }
            else if(compareOSCAddress(sysPrefix, msgSoftReset))
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
            else if(compareOSCAddress(sysPrefix, msgDebug))
            {
                for(i = 0; i < getArgumentsLength(); i++)
                {
                    if(compareTypeTagAtIndex(i, 'i'))
                        sendOSCMessage(sysPrefix, msgDebug, "iii", i, getIntArgumentAtIndex(i));
                    else if(compareTypeTagAtIndex(i, 'f'))
                        sendOSCMessage(sysPrefix, msgDebug, "if", i, getFloatArgumentAtIndex(i));
                    else if(compareTypeTagAtIndex(i, 's'))
                        sendOSCMessage(sysPrefix, msgDebug, "is", i, getStringArgumentAtIndex(i));
                    else if(compareTypeTagAtIndex(i, 'T'))
                        sendOSCMessage(sysPrefix, msgDebug, "iT", i);
                    else if(compareTypeTagAtIndex(i, 'F'))
                        sendOSCMessage(sysPrefix, msgDebug, "iF", i);
                    else if(compareTypeTagAtIndex(i, 'N'))
                        sendOSCMessage(sysPrefix, msgDebug, "iN", i);
                    else if(compareTypeTagAtIndex(i, 'I'))
                        sendOSCMessage(sysPrefix, msgDebug, "iI", i);
                }
            }
            else if(compareOSCAddress(sysPrefix, msgGetVersion))
            {
                sendOSCMessage(sysPrefix, msgVersion, "s", CURRENT_VERSION);
            }
        }
        //debug LED_2_Off();
    }
}

void sendOSCTask(void)
{
    BYTE i, j;
    static BYTE swState0 = 0;
    static BYTE swState1 = 0;

    if(!initSendFlag)
    {
        initSendFlag = openOSCSendPort(remoteIP, remotePort);
        if(chCompletedFlag)
        {
            sendOSCMessage(sysPrefix, msgConfiguration, "s", "succeeded");
            chCompletedFlag = FALSE;
        }
    }

    if(initSendFlag)
    {
        swState1 = SW_State();
        if(swState1 != swState0)
        {
            if(swState1)
                sendOSCMessage(stdPrefix, msgOnboardSw1, "s", "off");
            else
                sendOSCMessage(stdPrefix, msgOnboardSw1, "s", "on");
        }
        swState0 = swState1;

        j = 0;
        for(i = 0; i < AN_NUM; i++)
        {
            if(analogEnable[i])
            {
                analogInHandle(i, (LONG)ReadADC10(j));
                j++;
            }
        }
        sendAdc();
    }
}

void USBControlTask()
{
    HIDControlTask();
    //sendNote();
    //sendControlChange();
    //receiveMIDIDatas();
}

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

                    BYTE i;
                    WORD anum = 0;
                    WORD id = ReceivedHidDataBuffer[2];
                    BYTE state = ReceivedHidDataBuffer[3];

                    if(id > 13 || state > 1)
                        return;

                    if(state == 1)
                        analogEnable[id] = TRUE;
                    else if(state == 0)
                        analogEnable[id] = FALSE;

                    for(i = 0; i < AN_NUM; i++)
                    {
                        if(analogEnable[i])
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
                    ToSendHidDataBuffer[3] = analogEnable[ReceivedHidDataBuffer[2]];
                    ToSendHidDataBuffer[4] = getAnalogByte(ReceivedHidDataBuffer[2], MIDI_ORIGINAL);
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
                    ToSendHidDataBuffer[2] = remoteIP[0];
                    ToSendHidDataBuffer[3] = remoteIP[1];
                    ToSendHidDataBuffer[4] = remoteIP[2];
                    ToSendHidDataBuffer[5] = remoteIP[3];
                    ToSendHidDataBuffer[6] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                else
                {
                    remoteIP[0] = ReceivedHidDataBuffer[2];
                    remoteIP[1] = ReceivedHidDataBuffer[3];
                    remoteIP[2] = ReceivedHidDataBuffer[4];
                    remoteIP[3] = ReceivedHidDataBuffer[5];
                    initSendFlag = FALSE;
                    closeOSCSendPort();
                }
                break;
            case 0xF2:// Local Port
                if(!ReceivedHidDataBuffer[1])
                {
                    ToSendHidDataBuffer[0] = 0xF2;
                    ToSendHidDataBuffer[1] = 2;
                    ToSendHidDataBuffer[2] = (BYTE)((localPort >> 0) & 0x00FF);
                    ToSendHidDataBuffer[3] = (BYTE)((localPort >> 8) & 0x00FF);
                    ToSendHidDataBuffer[4] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                else
                {
                    localPort = ReceivedHidDataBuffer[2] | (ReceivedHidDataBuffer[3] << 8);
                    initReceiveFlag = FALSE;
                    closeOSCReceivePort();
                }
                break;
            case 0xF3:// Remote Port
                if(!ReceivedHidDataBuffer[1])
                {
                    ToSendHidDataBuffer[0] = 0xF3;
                    ToSendHidDataBuffer[1] = 2;
                    ToSendHidDataBuffer[2] = (BYTE)((remotePort >> 0) & 0x00FF);
                    ToSendHidDataBuffer[3] = (BYTE)((remotePort >> 8) & 0x00FF);
                    ToSendHidDataBuffer[4] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                else
                {
                    remotePort = ReceivedHidDataBuffer[2] | (ReceivedHidDataBuffer[3] << 8);
                    initSendFlag = FALSE;
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
                    u8Data[0] = remoteIP[0];
                    u8Data[1] = remoteIP[1];
                    u8Data[2] = remoteIP[2];
                    u8Data[3] = remoteIP[3];
                    u8Data[4] = (BYTE)((localPort >> 0) & 0x00FF);
                    u8Data[5] = (BYTE)((localPort >> 8) & 0x00FF);
                    u8Data[6] = (BYTE)((remotePort >> 0) & 0x00FF);
                    u8Data[7] = (BYTE)((remotePort >> 8) & 0x00FF);

                    //NVMWriteRow((void *)NVM_PROGRAM_PAGE, (void *)u8Data);
                }
                else
                {
                    //memcpy(u8Data, (void *)NVM_PROGRAM_PAGE, 8);

                    remoteIP[0] = u8Data[0];
                    remoteIP[1] = u8Data[1];
                    remoteIP[2] = u8Data[2];
                    remoteIP[3] = u8Data[3];
                    localPort = (WORD)u8Data[4] | ((WORD)u8Data[5] << 8);
                    remotePort = (WORD)u8Data[6] | ((WORD)u8Data[7] << 8);

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        ToSendHidDataBuffer[0] = 0xF6;
                        ToSendHidDataBuffer[1] = remoteIP[0];
                        ToSendHidDataBuffer[2] = remoteIP[1];
                        ToSendHidDataBuffer[3] = remoteIP[2];
                        ToSendHidDataBuffer[4] = remoteIP[3];
                        ToSendHidDataBuffer[5] = (BYTE)((localPort >> 0) & 0x00FF);
                        ToSendHidDataBuffer[6] = (BYTE)((localPort >> 8) & 0x00FF);
                        ToSendHidDataBuffer[7] = (BYTE)((remotePort >> 0) & 0x00FF);
                        ToSendHidDataBuffer[8] = (BYTE)((remotePort >> 8) & 0x00FF);
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

void sendNote(void)
{
    BYTE i, j;
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

void sendControlChange(void)
{
    BYTE i, value;

    for(i = 0; i < AN_NUM; i++)
    {
        if(analogEnable[i])
        {
            analogInHandle(i, (LONG)ReadADC10(i));

            if(getAnalogFlag(i))
            {
                midiData.Val = 0;
                midiData.CableNumber = 0;
                midiData.CodeIndexNumber = MIDI_CIN_CONTROL_CHANGE;
                midiData.DATA_0 = 0xB0;

                value = getAnalogByte(i, MIDI_ORIGINAL);
                
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

void receiveMIDIDatas(void)
{
    BYTE i, ch, num, val;

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
                            
                            if(initSendFlag)
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
                                StackTask();
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
            break;
        default:
            ProcState = STATE_INITIALIZE;
            break;
    }
}

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
//hid        case EVENT_HID_RPT_DESC_PARSED:
//hid             #ifdef APPL_COLLECT_PARSED_DATA
//hid                 return(APPL_COLLECT_PARSED_DATA());
//hid             #else
//hid                 return TRUE;
//hid             #endif
//hid            break;
        case EVENT_MIDI_TRANSFER_DONE:  // The main state machine will poll the driver.
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
        default:
            break;
    }
    return FALSE;
} // USB_ApplicationEventHandler
