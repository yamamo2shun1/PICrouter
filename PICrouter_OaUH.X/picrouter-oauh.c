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
 * picrouter-oauh.c,v.1.02 2013/02/07
 */

#include "picrouter-oauh.h"

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
        configAnPort(i, IO_OUT);
        outputAnPort(i, LOW);
    }
    for(i = 0; i < 4; i++)
    {
        configPwmPort(i, IO_OUT);
        outputPwmPort(i, LOW);
    }
    for(i = 0; i < 4; i++)
    {
        configDPort(i, IO_OUT);
        outputDPort(i, LOW);
    }
    configSpiPort("sck2", IO_OUT);
    configSpiPort("sdi2", IO_OUT);
    configSpiPort("sdo2", IO_OUT);
    configSpiPort("sck4", IO_OUT);
    configSpiPort("sdi4", IO_OUT);
    configSpiPort("sdo4", IO_OUT);
    outputSpiPort("sck2", LOW);
    outputSpiPort("sdi2", LOW);
    outputSpiPort("sdo2", LOW);
    outputSpiPort("sck4", LOW);
    outputSpiPort("sdi4", LOW);
    outputSpiPort("sdo4", LOW);
}

int main(int argc, char** argv) {
    int value, i;
    static DWORD t = 0;

    // Enable optimal performance
    SYSTEMConfigPerformance(GetSystemClock());
    mOSCSetPBDIV(OSC_PB_DIV_1);             // Use 1:1 CPU Core:Peripheral clocks

    // Enable the cache for the best performance
    CheKseg0CacheOn();

    delayMs(100);
    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

    //Port Initialization
    initIOPorts();
    initAnalogVariables();
    initEncoderVariables();

    initSW();
    initLEDs();
    buttonInit();

    LED_1_On();
    LED_2_On();

    //PWM
    freq = 10000; // 10kHz
    width = GetSystemClock() / freq;
    for(i = 0; i < PWM_NUM; i++)
        duty[i] = 50;

    setOSCPrefix("/pic");
    setOSCHostName(DEFAULT_HOST_NAME);

#if 0
    SpiChnOpen(SPI_CHANNEL4, SPICON_MSTEN | SPICON_SMP | SPICON_MODE16 | SPICON_ON, 2);
    SpiChnOpen(SPI_CHANNEL2, SPICON_MSTEN | SPICON_SMP | SPICON_MODE16 | SPICON_ON, 4);
#endif

    TickInit();
    InitAppConfig();
    StackInit();
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

    // Enable multi-vectored interrupts
    INTEnableSystemMultiVectoredInt();

    //USB Initialization
    bUsbHostInitialized = USBHostInit(0);

    LED_1_Off();
    LED_2_Off();

    while(1)
    {
        if(TickGet() - t >= TICK_SECOND / 2ul)
        {
            t = TickGet();
            LED_2_Toggle();
        }

        StackTask();
        NBNSTask();
        //ZeroconfLLProcess();
        mDNSProcess();

        receiveOSCTask();
        sendOSCTask();

        if(bUsbHostInitialized)
        {
            USBHostTasks();
            convertMidiToOsc();

            USBHostHIDTasks();
            convertHidToOsc();
        }
        else
            bUsbHostInitialized = USBHostInit(0);
    }
    return (EXIT_SUCCESS);
}

#if 1
void sendSpiOneWord(WORD msb, DWORD usec, BYTE spi_id)
{
    // Set LOAD_PIN to low
    //
    switch(spi_id)
    {
        case SPI_2:
            putcSPI2(msb);
            break;
        case SPI_4:
            putcSPI4(msb);
            break;
    }
    delayUs(usec);
    // Set LOAD_PIN to high
    //
}

void sendSpiTwoWord(WORD msb, WORD lsb, DWORD usec, BYTE spi_id)
{
    // Set LOAD_PIN to low
    //
    switch(spi_id)
    {
        case SPI_2:
            putcSPI2(lsb);
            putcSPI2(msb);
            break;
        case SPI_4:
            putcSPI4(lsb);
            putcSPI4(msb);
            break;
    }
    delayUs(usec);
    // Set LOAD_PIN to low
    //
}

void sendSpiFourWord(WORD msb0, WORD lsb0, WORD msb1, WORD lsb1, DWORD usec, BYTE spi_id)
{
    // Set LOAD_PIN to low
    //
    switch(spi_id)
    {
        case SPI_2:
            putcSPI2(lsb1);
            putcSPI2(msb1);
            putcSPI2(lsb0);
            putcSPI2(msb0);
            break;
        case SPI_4:
            putcSPI4(lsb1);
            putcSPI4(msb1);
            putcSPI4(lsb0);
            putcSPI4(msb0);
            break;
    }
    delayUs(usec);
    // Set LOAD_PIN to low
    //
}
#endif

/**********************************************
*  OSC Generic I/O Processing Part
**********************************************/
void receiveOSCTask(void)
{
    WORD rcvLen;
	BYTE buffer[1024] = {0};
    BYTE index, i, j, k;

    if(!initReceiveFlag)
        initReceiveFlag = openOSCReceivePort(localPort);

    if(initReceiveFlag && isOSCGetReady(rcvLen))
    {
        getOSCArray(buffer, sizeof(buffer));
        if(compareOSCAddress(stdPrefix, msgOnboardLed))
        {
            if(getIntArgumentAtIndex(buffer, prefix, msgOnboardLed, 0) == 0)
            {
                if(getIntArgumentAtIndex(buffer, prefix, msgOnboardLed, 1) == 1)
                {
                    LED_1_On();
                }
                else if(getIntArgumentAtIndex(buffer, prefix, msgOnboardLed, 1) == 0)
                {
                    LED_1_Off();
                }
            }
            else if(getIntArgumentAtIndex(buffer, prefix, msgOnboardLed, 0) == 1)
            {
                if(getIntArgumentAtIndex(buffer, prefix, msgOnboardLed, 1) == 1)
                {
                    LED_2_On();
                }
                else if(getIntArgumentAtIndex(buffer, prefix, msgOnboardLed, 1) == 0)
                {
                    LED_2_Off();
                }
            }
        }
        // A/D
        else if(compareOSCAddress(stdPrefix, msgSetAdcEnable))
        {
            AD1CON1bits.ON = 0;

            BYTE i;
            WORD anum = 0;
            WORD id = getIntArgumentAtIndex(buffer, stdPrefix, msgSetAdcEnable, 0);
            BYTE state = getIntArgumentAtIndex(buffer, stdPrefix, msgSetAdcEnable, 1);

            if(id > 13 || state > 1)
                return;

            if(state == 1)
                analogEnable[id] = TRUE;
            else if(state == 0)
                analogEnable[id] = FALSE;

            for(i = 0; i < USE_ADC_NUM; i++)
            {
                if(analogEnable[i])
                    anum++;
            }

            configAnPort(id, IO_IN);

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
                AD1CON2 = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
                AD1CSSL = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
                AD1CON1 = 0x00000000;// 0000 0000 0000 0000 1000 0000 0000 0000
                AD1CHS  = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
                AD1CON3 = 0x00001F08;// 0000 0000 0000 0000 0000 1111 0000 1000
            }
        }
        else if(compareOSCAddress(stdPrefix, msgGetAdcEnable))
        {
            BYTE id = getIntArgumentAtIndex(buffer, stdPrefix, msgGetAdcEnable, 0);

            if(id > 13)
                return;

            sendOSCMessage(stdPrefix, msgGetAdcEnable, "ii", id, analogEnable[id]);
        }
        else if(compareOSCAddress(stdPrefix, msgConfigAdc))
        {
            BYTE id = getIntArgumentAtIndex(buffer, stdPrefix, msgConfigAdc, 0);
            char* type = getStringArgumentAtIndex(buffer, stdPrefix, msgConfigAdc, 1);
            if(!strcmp(type, "din"))
            {
                configAnPort(id, IO_IN);
            }
            else if(!strcmp(type, "dout"))
            {
                configAnPort(id, IO_OUT);
            }
        }
        else if(compareOSCAddress(stdPrefix, msgSetAdcDO))
        {
            BYTE id = getIntArgumentAtIndex(buffer, stdPrefix, msgSetAdcDO, 0);
            BYTE state = getIntArgumentAtIndex(buffer, stdPrefix, msgSetAdcDO, 1);
            outputAnPort(id, state);
        }
        else if(compareOSCAddress(stdPrefix, msgGetAdcDI))
        {
            BYTE id = getIntArgumentAtIndex(buffer, stdPrefix, msgGetAdcDI, 0);
            BYTE state = inputAnPort(id);
            sendOSCMessage(stdPrefix, msgGetAdcDI, "ii", id, state);
        }
        // PWM
        else if(compareOSCAddress(stdPrefix, msgSetPwmState))
        {
            index = getIntArgumentAtIndex(buffer, stdPrefix, msgSetPwmState, 0);
            if(index > 4)
                return;

            if(strcmp(getStringArgumentAtIndex(buffer, stdPrefix, msgSetPwmState, 1), "on") == 0)
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
            else if(strcmp(getStringArgumentAtIndex(buffer, stdPrefix, msgSetPwmState, 1), "off") == 0)
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
        else if(compareOSCAddress(stdPrefix, msgGetPwmState))
        {
            index = getIntArgumentAtIndex(buffer, stdPrefix, msgGetPwmState, 0);
            sendOSCMessage(stdPrefix, msgGetPwmState, "is", index, onSquare[index] ? "on" : "off");
        }
        else if(compareOSCAddress(stdPrefix, msgSetPwmFreq))
        {
            freq = getIntArgumentAtIndex(buffer, stdPrefix, msgSetPwmFreq, 0);
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
            sendOSCMessage(stdPrefix, msgGetPwmFreq, "i", freq);
        }
        else if(compareOSCAddress(stdPrefix, msgSetPwmDuty))
        {
            index = getIntArgumentAtIndex(buffer, stdPrefix, msgSetPwmDuty, 0);
            duty[index] = getIntArgumentAtIndex(buffer, stdPrefix, msgSetPwmDuty, 1);
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
            index = getIntArgumentAtIndex(buffer, stdPrefix, msgGetPwmDuty, 0);
            sendOSCMessage(stdPrefix, msgGetPwmDuty, "ii", index, duty[index]);
        }
        else if(compareOSCAddress(stdPrefix, msgConfigPwm))
        {
            BYTE id = getIntArgumentAtIndex(buffer, stdPrefix, msgConfigPwm, 0);
            char* type = getStringArgumentAtIndex(buffer, stdPrefix, msgConfigPwm, 1);
            if(!strcmp(type, "din"))
            {
                configPwmPort(id, IO_IN);
            }
            else if(!strcmp(type, "dout"))
            {
                configPwmPort(id, IO_OUT);
            }
        }
        else if(compareOSCAddress(stdPrefix, msgSetPwmDO))
        {
            BYTE id = getIntArgumentAtIndex(buffer, stdPrefix, msgSetPwmDO, 0);
            BYTE state = getIntArgumentAtIndex(buffer, stdPrefix, msgSetPwmDO, 1);
            outputPwmPort(id, state);
        }
        else if(compareOSCAddress(stdPrefix, msgGetPwmDI))
        {
            BYTE id = getIntArgumentAtIndex(buffer, stdPrefix, msgGetPwmDI, 0);
            BYTE state = inputPwmPort(id);
            sendOSCMessage(stdPrefix, msgGetPwmDI, "ii", id, state);
        }
        // D/IO
        else if(compareOSCAddress(stdPrefix, msgConfigDIO))
        {
            BYTE id = getIntArgumentAtIndex(buffer, stdPrefix, msgConfigDIO, 0);
            char* type = getStringArgumentAtIndex(buffer, stdPrefix, msgConfigDIO, 1);
            if(!strcmp(type, "din"))
            {
                configDPort(id, IO_IN);
            }
            else if(!strcmp(type, "dout"))
            {
                configDPort(id, IO_OUT);
            }
        }
        else if(compareOSCAddress(stdPrefix, msgSetDO))
        {
            BYTE id = getIntArgumentAtIndex(buffer, stdPrefix, msgSetDO, 0);
            BYTE state = getIntArgumentAtIndex(buffer, stdPrefix, msgSetDO, 1);
            outputDPort(id, state);
        }
        else if(compareOSCAddress(stdPrefix, msgGetDI))
        {
            BYTE id = getIntArgumentAtIndex(buffer, stdPrefix, msgGetDI, 0);
            BYTE state = inputDPort(id);
            sendOSCMessage(stdPrefix, msgGetDI, "ii", id, state);
        }
        // SPI
        else if(compareOSCAddress(stdPrefix, msgConfigSpi))
        {
            char* name = getStringArgumentAtIndex(buffer, stdPrefix, msgConfigSpi, 0);
            char* type = getStringArgumentAtIndex(buffer, stdPrefix, msgConfigSpi, 1);
            if(!strcmp(type, "din"))
            {
                configSpiPort(name, IO_IN);
            }
            else if(!strcmp(type, "dout"))
            {
                configSpiPort(name, IO_OUT);
            }
        }
        else if(compareOSCAddress(stdPrefix, msgSetSpiDO))
        {
            char *name = getStringArgumentAtIndex(buffer, stdPrefix, msgSetSpiDO, 0);
            BYTE state = getIntArgumentAtIndex(buffer, stdPrefix, msgSetSpiDO, 1);
            outputSpiPort(name, state);
        }
        else if(compareOSCAddress(stdPrefix, msgGetSpiDI))
        {
            char* name = getStringArgumentAtIndex(buffer, stdPrefix, msgGetSpiDI, 0);
            BYTE state = inputSpiPort(name);
            sendOSCMessage(stdPrefix, msgGetSpiDI, "si", name, state);
        }
        // USB_MIDI
        else if(compareOSCAddress(midiPrefix, msgSetNote)) // Note On/Off
        {
            BYTE currentEndpoint;
            
            BYTE ch = getIntArgumentAtIndex(buffer, midiPrefix, msgSetNote, 0);
            BYTE num = getIntArgumentAtIndex(buffer, midiPrefix, msgSetNote, 1);
            BYTE vel = getIntArgumentAtIndex(buffer, midiPrefix, msgSetNote, 2);

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
        else if(compareOSCAddress(midiPrefix, msgSetCc)) // Control Change
        {
            BYTE currentEndpoint;
            
            BYTE ch = getIntArgumentAtIndex(buffer, midiPrefix, msgSetNote, 0);
            BYTE num = getIntArgumentAtIndex(buffer, midiPrefix, msgSetNote, 1);
            BYTE val = getIntArgumentAtIndex(buffer, midiPrefix, msgSetNote, 2);

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
        else if(compareOSCAddress(midiPrefix, msgSetKp)) // Polyphonic Key Pressure
        {
            BYTE currentEndpoint;
            
            BYTE ch = getIntArgumentAtIndex(buffer, midiPrefix, msgSetKp, 0);
            BYTE num = getIntArgumentAtIndex(buffer, midiPrefix, msgSetKp, 1);
            BYTE prs = getIntArgumentAtIndex(buffer, midiPrefix, msgSetKp, 2);

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
        else if(compareOSCAddress(midiPrefix, msgSetPc)) // Program Change
        {
            BYTE currentEndpoint;
            
            BYTE ch = getIntArgumentAtIndex(buffer, midiPrefix, msgSetPc, 0);
            BYTE pnum = getIntArgumentAtIndex(buffer, midiPrefix, msgSetPc, 1);

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
        else if(compareOSCAddress(midiPrefix, msgSetCp)) // Channel Pressure
        {
            BYTE currentEndpoint;
            
            BYTE ch = getIntArgumentAtIndex(buffer, midiPrefix, msgSetCp, 0);
            BYTE prs = getIntArgumentAtIndex(buffer, midiPrefix, msgSetCp, 1);

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
        else if(compareOSCAddress(midiPrefix, msgSetPb)) // Pitch Bend
        {
            BYTE currentEndpoint;
            
            BYTE ch = getIntArgumentAtIndex(buffer, midiPrefix, msgSetPb, 0);
            BYTE msb = getIntArgumentAtIndex(buffer, midiPrefix, msgSetPb, 1);
            BYTE lsb = getIntArgumentAtIndex(buffer, midiPrefix, msgSetPb, 2);

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
        // System Setting
        else if(compareOSCAddress(sysPrefix, msgSetRemoteIp))
        {
            char* rip = getStringArgumentAtIndex(buffer, sysPrefix, msgSetRemoteIp, 0);
            remoteIP[0] = atoi(strtok(rip, "."));
            remoteIP[1] = atoi(strtok(NULL, "."));
            remoteIP[2] = atoi(strtok(NULL, "."));
            remoteIP[3] = atoi(strtok(NULL, "."));
            initSendFlag = FALSE;
            closeOSCSendPort();
        }
        else if(compareOSCAddress(sysPrefix, msgGetRemoteIp))
        {
            char* rip = (char *)calloc(15, sizeof(char));;
            sprintf(rip, "%d.%d.%d.%d", remoteIP[0], remoteIP[1], remoteIP[2], remoteIP[3]);
            sendOSCMessage(sysPrefix, msgRemoteIp, "s", rip);
        }
        else if(compareOSCAddress(sysPrefix, msgSetRemotePort))
        {
            remotePort = getIntArgumentAtIndex(buffer, sysPrefix, msgSetRemotePort, 0);
            closeOSCSendPort();
            initSendFlag = FALSE;
        }
        else if(compareOSCAddress(sysPrefix, msgGetRemotePort))
        {
            sendOSCMessage(sysPrefix, msgRemotePort, "i", remotePort);
        }
        else if(compareOSCAddress(sysPrefix, msgSetHostName))
        {
            mDNSServiceDeRegister();
            free(hostName);
            hostName = NULL;
            hostName = getStringArgumentAtIndex(buffer, sysPrefix, msgSetHostName, 0);
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
            sendOSCMessage(sysPrefix, msgHostName, "s", hostName);
        }
        else if(compareOSCAddress(sysPrefix, msgGetHostName))
        {
            sendOSCMessage(sysPrefix, msgHostName, "s", hostName);
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
            localPort = getIntArgumentAtIndex(buffer, sysPrefix, msgSetHostPort, 0);
            closeOSCReceivePort();
            initReceiveFlag = FALSE;
        }
        else if(compareOSCAddress(sysPrefix, msgGetHostPort))
        {
            sendOSCMessage(sysPrefix, msgHostPort, "i", localPort);
        }
        else if(compareOSCAddress(sysPrefix, msgSetPrefix))
        {
            free(prefix);
            prefix = NULL;
            prefix = getStringArgumentAtIndex(buffer, sysPrefix, msgSetPrefix, 0);
        }
        else if(compareOSCAddress(sysPrefix, msgGetPrefix))
        {
            sendOSCMessage(sysPrefix, msgPrefix, "s", prefix);
        }
        else if(compareOSCAddress(sysPrefix, msgSoftReset))
        {
            BOOL chFlag = getIntArgumentAtIndex(buffer, sysPrefix, msgSoftReset, 0);
            if(chFlag)
            {
                NVMErasePage((void*)NVM_DATA);
                NVMWriteWord((void*)(NVM_DATA), (unsigned int)0x01);
            }
            SoftReset();
        }
        else if(compareOSCAddress(sysPrefix, msgDebug))
        {
            sendOSCMessage(stdPrefix, "/debug", "i", *(unsigned int *)(NVM_DATA));
        }
        LED_2_Off();
    } 
}

void sendOSCTask(void)
{
    BYTE i;
    static BYTE swState0 = 0;
    static BYTE swState1 = 0;

    if(!initSendFlag)
        initSendFlag = openOSCSendPort(remoteIP, remotePort);

    if(initSendFlag)
    {
        swState1 = SW_State();
        if(swState1 != swState0)
        {
            sendOSCMessage(stdPrefix, msgOnboardSw1, "i", swState1);
        }
        swState0 = swState1;

        if(BusyADC10())
        {
            for(i = 0; i < USE_ADC_NUM; i++)
            {
                if(analogEnable[i])
                    analogInHandle(i, (LONG)ReadADC10(i));
            }
        }
        sendAdc();
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
                                        sendOSCMessage(midiPrefix, msgGetNote, "iii", midiCh, midiNum, midiVal);
                                        break;
                                    case 0xA0:// Key Pressure
                                        sendOSCMessage(midiPrefix, msgGetKp, "iii", midiCh, midiNum, midiVal);
                                        break;
                                    case 0xB0:// Control Change
                                        sendOSCMessage(midiPrefix, msgGetCc, "iii", midiCh, midiNum, midiVal);
                                        break;
                                    case 0xC0:// Program Change
                                        sendOSCMessage(midiPrefix, msgGetPc, "i", midiNum);
                                        break;
                                    case 0xD0:// Channel Pressure
                                        sendOSCMessage(midiPrefix, msgGetCp, "ii", midiCh, midiNum);
                                        break;
                                    case 0xE0:// Pitch Bend
                                        sendOSCMessage(midiPrefix, msgGetPb, "iii", midiCh, midiNum, midiVal);
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
                delayUs(2);
            }
            break;
        case STATE_ERROR:
            break;
        default:
            ProcState = STATE_INITIALIZE;
            break;
    }
}

void convertHidToOsc(void)
{
    App_Detect_Device();
    switch(App_State_Mouse)
    {
        case DEVICE_NOT_CONNECTED:
            USBTasks();
            if(USBHostHID_ApiDeviceDetect()) /* True if report descriptor is parsed with no error */
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

//******************************************************************************
//******************************************************************************
// USB Support Functions
//******************************************************************************
//******************************************************************************

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
void App_Detect_Device(void)
{
  if(!USBHostHID_ApiDeviceDetect())
  {
     App_State_Mouse = DEVICE_NOT_CONNECTED;
  }
}

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
       if((reportItem->reportType==hidReportInput) && (reportItem->dataModes == (HIDData_Variable|HIDData_Relative))&&
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
        case EVENT_HID_RPT_DESC_PARSED:
             #ifdef APPL_COLLECT_PARSED_DATA
                 return(APPL_COLLECT_PARSED_DATA());
             #else
                 return TRUE;
             #endif
            break;
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
