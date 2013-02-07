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
 * picrouter.c,v.0.7 2013/02/07
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
    int i;
    static DWORD t = 0;

    // Enable optimal performance
    SYSTEMConfigPerformance(GetSystemClock());
    mOSCSetPBDIV(OSC_PB_DIV_1); // Use 1:1 CPU Core:Peripheral clocks

    // Enable the cache for the best performance
    CheKseg0CacheOn();

    delayMs(100);
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

    setOSCPrefix("/pic");
    setOSCHostName(DEFAULT_HOST_NAME);

    // USB Initialization
    #if defined(USE_USB_BUS_SENSE_IO)
        tris_usb_bus_sense = 1;
    #endif

    #if defined(USE_SELF_POWER_SENSE_IO)
        tris_self_power = 1;
    #endif
    USBDeviceInit();

    TickInit();
    InitAppConfig();
    StackInit();
    mDNSInitialize(hostName);
    mDNSServiceRegister((const char *)hostName, // base name of the service
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

        StackTask();
        NBNSTask();
        mDNSProcess();

        receiveOSCTask();
        sendOSCTask();

        USBDeviceTasks();
        USBControlTask();
        
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
	static BYTE buffer[1024] = {0};
    BYTE index, i, j, k;

    if(!initReceiveFlag)
        initReceiveFlag = openOSCReceivePort(localPort);

    if(initReceiveFlag && isOSCGetReady(rcvLen))
    {
        LED_2_On();
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
            char* name = getStringArgumentAtIndex(buffer, stdPrefix, msgSetSpiDO, 0);
            BYTE state = getIntArgumentAtIndex(buffer, stdPrefix, msgSetSpiDO, 1);
            outputSpiPort(name, state);
        }
        else if(compareOSCAddress(stdPrefix, msgGetSpiDI))
        {
            char* name = getStringArgumentAtIndex(buffer, stdPrefix, msgGetSpiDI, 0);
            BYTE state = inputSpiPort(name);
            sendOSCMessage(stdPrefix, msgGetSpiDI, "si", name, state);
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
            //free(rip);
            //rip = NULL;
        }
        else if(compareOSCAddress(sysPrefix, msgGetRemoteIp))
        {
            char* rip = (char *)calloc(15, sizeof(char));;
            sprintf(rip, "%d.%d.%d.%d", remoteIP[0], remoteIP[1], remoteIP[2], remoteIP[3]);
            sendOSCMessage(sysPrefix, msgRemoteIp, "s", rip);
            //free(rip);
            //rip = NULL;
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
    if(!initSendFlag)
        initSendFlag = openOSCSendPort(remoteIP, remotePort);

    if(initSendFlag)
    {
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

void USBControlTask()
{
    static BYTE usbState = 0;
    switch(usbState)
    {
        case 0:
            HIDControlTask();
            usbState = 1;
            break;
        case 1:
            //while(BusyADC10());
            //analogInHandle(0, (LONG)((ADC1BUF0 + ADC1BUF2 + ADC1BUF4 + ADC1BUF6) / 4));
            //analogInHandle(1, (LONG)((ADC1BUF1 + ADC1BUF3 + ADC1BUF5 + ADC1BUF7) / 4));
            usbState = 2;
            break;
        case 2:
            //sendNote();
            //sendControlChange();
            //receiveMIDIDatas();
            usbState = 0;
            break;
    }
}

void HIDControlTask(void)
{
    BYTE u8Data[128];

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
            case 0xF0:// DHCP Enable
                break;
            case 0xF1:// Local IP
                if(!ReceivedHidDataBuffer[1])
                {
                    ToSendHidDataBuffer[0] = 0xF1;
                    ToSendHidDataBuffer[1] = (BYTE)((AppConfig.MyIPAddr.Val >> 0) & 0x000000FF);
                    ToSendHidDataBuffer[2] = (BYTE)((AppConfig.MyIPAddr.Val >> 8) & 0x000000FF);
                    ToSendHidDataBuffer[3] = (BYTE)((AppConfig.MyIPAddr.Val >> 16) & 0x000000FF);
                    ToSendHidDataBuffer[4] = (BYTE)((AppConfig.MyIPAddr.Val >> 24) & 0x000000FF);
                    ToSendHidDataBuffer[5] = 0x00;

                    if(!USBHandleBusy(HIDTxHandle))
                    {
                        HIDTxHandle = USBTxOnePacket(HID_EP, (BYTE*)ToSendHidDataBuffer, 64);
                    }
                }
                else
                {
                    if(!AppConfig.Flags.bIsDHCPEnabled)
                        AppConfig.MyIPAddr.Val = (ReceivedHidDataBuffer[2] | (ReceivedHidDataBuffer[3] << 8) | (ReceivedHidDataBuffer[4] << 16) | (ReceivedHidDataBuffer[5] << 24));
                }
                break;
            case 0xF2:// Remote IP
                if(!ReceivedHidDataBuffer[1])
                {
                    ToSendHidDataBuffer[0] = 0xF2;
                    ToSendHidDataBuffer[1] = remoteIP[0];
                    ToSendHidDataBuffer[2] = remoteIP[1];
                    ToSendHidDataBuffer[3] = remoteIP[2];
                    ToSendHidDataBuffer[4] = remoteIP[3];
                    ToSendHidDataBuffer[5] = 0x00;

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
            case 0xF3:// Local Port
                if(!ReceivedHidDataBuffer[1])
                {
                    ToSendHidDataBuffer[0] = 0xF3;
                    ToSendHidDataBuffer[1] = (BYTE)((localPort >> 0) & 0x00FF);
                    ToSendHidDataBuffer[2] = (BYTE)((localPort >> 8) & 0x00FF);
                    ToSendHidDataBuffer[3] = 0x00;

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
            case 0xF4:// Remote Port
                if(!ReceivedHidDataBuffer[1])
                {
                    ToSendHidDataBuffer[0] = 0xF4;
                    ToSendHidDataBuffer[1] = (BYTE)((remotePort >> 0) & 0x00FF);
                    ToSendHidDataBuffer[2] = (BYTE)((remotePort >> 8) & 0x00FF);
                    ToSendHidDataBuffer[3] = 0x00;

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
            case 0xF5:// Mac Address
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
                midiData.DATA_1 = i + 2 * j;

                if(btnCurrent[i] & (1 << j))
                    midiData.DATA_2 = 127;
                else
                    midiData.DATA_2 = 0;

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

    for(i = 0; i < 2; i++)
    {
        if(getAnalogFlag(i))
        {
            midiData.Val = 0;
            midiData.CableNumber = 0;
            midiData.CodeIndexNumber = MIDI_CIN_CONTROL_CHANGE;
            midiData.DATA_0 = 0xB0;

            if(i == 0)
            {
                value = getAnalogByte(i, MIDI_ORIGINAL);
            }
            else if(i == 1)
            {
                value = getAnalogByte(i, MIDI_VOLUME);
            }
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
