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
 * prb-mini.c,v.0.10 2012/10/21
 */

#include "prb-mini.h"

// IPアドレスその他の初期設定
static void InitAppConfig(void)
{
    AppConfig.Flags.bIsDHCPEnabled = TRUE;
    AppConfig.Flags.bInConfigMode = TRUE;
    memcpypgm2ram((void*)&AppConfig.MyMACAddr, (ROM void*)SerializedMACAddress, sizeof(AppConfig.MyMACAddr));
    AppConfig.MyIPAddr.Val = MY_DEFAULT_IP_ADDR_BYTE1 | MY_DEFAULT_IP_ADDR_BYTE2<<8ul | MY_DEFAULT_IP_ADDR_BYTE3<<16ul | MY_DEFAULT_IP_ADDR_BYTE4<<24ul;
    AppConfig.DefaultIPAddr.Val = AppConfig.MyIPAddr.Val;
    AppConfig.MyMask.Val = MY_DEFAULT_MASK_BYTE1 | MY_DEFAULT_MASK_BYTE2<<8ul | MY_DEFAULT_MASK_BYTE3<<16ul | MY_DEFAULT_MASK_BYTE4<<24ul;
    AppConfig.DefaultMask.Val = AppConfig.MyMask.Val;
    AppConfig.MyGateway.Val = MY_DEFAULT_GATE_BYTE1 | MY_DEFAULT_GATE_BYTE2<<8ul | MY_DEFAULT_GATE_BYTE3<<16ul | MY_DEFAULT_GATE_BYTE4<<24ul;
    AppConfig.PrimaryDNSServer.Val = MY_DEFAULT_PRIMARY_DNS_BYTE1 | MY_DEFAULT_PRIMARY_DNS_BYTE2<<8ul | MY_DEFAULT_PRIMARY_DNS_BYTE3<<16ul  | MY_DEFAULT_PRIMARY_DNS_BYTE4<<24ul;
    AppConfig.SecondaryDNSServer.Val = MY_DEFAULT_SECONDARY_DNS_BYTE1 | MY_DEFAULT_SECONDARY_DNS_BYTE2<<8ul | MY_DEFAULT_SECONDARY_DNS_BYTE3<<16ul  | MY_DEFAULT_SECONDARY_DNS_BYTE4<<24ul;
}

void _general_exception_handler(unsigned cause, unsigned status)
{
    Nop();
    Nop();
}

int main(int argc, char** argv) {
	int value, i, state = 0;
    static DWORD t = 0;

    // Enable optimal performance
    SYSTEMConfigPerformance(GetSystemClock());
    mOSCSetPBDIV(OSC_PB_DIV_1);             // Use 1:1 CPU Core:Peripheral clocks

    // Enable the cache for the best performance
    CheKseg0CacheOn();

    delayMs(100);
    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

    //Port Initialization
    setupLedEnc();
  
    initSW();
    initLEDs();
    buttonInit();
    encoderInit();

    LED_1_On();
    LED_2_On();

    //PWM
    for(i = 0; i < PWM_NUM; i++)
    {
        freq = 10000; // 10kHz
        width = GetSystemClock() / freq;
        duty[i] = 50;
    }

    prefix = (char *)calloc(strlen("/prb"), sizeof(char));
    memcpy(prefix, "/prb", strlen("/prb"));
    hostName = (char *)calloc(strlen(DEFAULT_HOST_NAME), sizeof(char));
    memcpy(hostName, DEFAULT_HOST_NAME, strlen(DEFAULT_HOST_NAME));

     //A/D Auto Scan
#if 0// Infinium
    AD1PCFG = 0x0000FFF3;// 0000 0000 0000 0000 1111 1111 1111 0011
    AD1CON2 = 0x0000041C;// 0000 0000 0000 0000 0000 0100 0001 1100
    AD1CSSL = 0x0000000C;// 0000 0000 0000 0000 0000 0000 0000 1100
    AD1CON1 = 0x000010E6;// 0000 0000 0000 0000 1000 0000 1110 0110
    AD1CHS  = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
    AD1CON3 = 0x00001F08;// 0000 0000 0000 0000 0000 1111 0000 1000
#else// CF-X2
    AD1PCFG = 0x0000FF03;// 0000 0000 0000 0000 1111 1111 0000 0011
    AD1CON2 = 0x0000042C;// 0000 0000 0000 0000 0000 0100 0010 1100
    AD1CSSL = 0x000000FC;// 0000 0000 0000 0000 0000 0000 1111 1100
    AD1CON1 = 0x000010E6;// 0000 0000 0000 0000 1000 0000 1110 0110
    AD1CHS  = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
    AD1CON3 = 0x00001F08;// 0000 0000 0000 0000 0000 1111 0000 1000
#endif
    AD1CON1bits.ON = 1;

    SpiChnOpen(SPI_CHANNEL4, SPICON_MSTEN | SPICON_SMP | SPICON_MODE16 | SPICON_ON, 2);
    SpiChnOpen(SPI_CHANNEL2, SPICON_MSTEN | SPICON_SMP | SPICON_MODE16 | SPICON_ON, 4);

#if 0
    IEC0 = 0;
    IEC1 = 0;
    IEC2 = 0;
    EthEventsEnableClr(ETH_EV_ALL);
    IEC0 = 0;
    IEC1 = 0;
    IEC2 = 0;
    EthEventsEnableClr(ETH_EV_ALL);
    IEC0 = 0;
    IEC1 = 0;
    IEC2 = 0;
    EthEventsEnableClr(ETH_EV_ALL);
    IEC0 = 0;
    IEC1 = 0;
    IEC2 = 0;
    EthEventsEnableClr(ETH_EV_ALL);
    IEC0 = 0;
    IEC1 = 0;
    IEC2 = 0;
#endif

#if 1
    OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_8, TIMER_COUNT);
    ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_1);
#endif

    TickInit();
    InitAppConfig();
    StackInit();

    ZeroconfLLInitialize();
    mDNSInitialize(hostName);
    mDNSServiceRegister((const char *)hostName, // base name of the service
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

    //USB Initialization
    bUsbHostInitialized = USBHostInit(0);
#if 0//test
    #if defined(USE_USB_BUS_SENSE_IO)
    	tris_usb_bus_sense = 1;
    #endif

    #if defined(USE_SELF_POWER_SENSE_IO)
    	tris_self_power = 1;
    #endif
#endif

    for(i = 0; i < 32; i++)
        intensity[i] = ledIntensity;
    sendSpiTwoWord(0x0000, 0x0000, 8);
    sendSpiOneWord(0x0000, 8);

    LED_1_Off();
    LED_2_Off();

    while(1)
    {
        if(TickGet() - t >= TICK_SECOND / 2ul)
        {
            t = TickGet();
            LED_1_Toggle();
        }

#if 0 // debug
        LED_2_Toggle();
        delayMs(100);
#endif

        StackTask();
        NBNSTask();
        ZeroconfLLProcess();
        mDNSProcess();

        UDPControlTask();
        UDPSendTask();
        if(bUsbHostInitialized)
        {
            USBHostTasks();
            convertMidiToOsc();
        }
        else
            bUsbHostInitialized = USBHostInit(0);

#if 0
        currentState = SW_State();
        if(prevState != currentState)
        {
            stateFlag = TRUE;
            prevState = currentState;
        }
#endif
    }
    return (EXIT_SUCCESS);
}

void sendSpiOneWord(WORD msb, DWORD usec)
{
    LD16_LOAD(0);
    putcSPI4(msb);
    delayUs(usec);
    LD16_LOAD(1);
}
void sendSpiTwoWord(WORD msb, WORD lsb, DWORD usec)
{
    LD32_LOAD(0);
    putcSPI2(lsb);
    putcSPI2(msb);
    delayUs(usec);
    LD32_LOAD(1);
}

void setupLedEnc(void)
{
    AD_PORT0_IO(0);
    AD_PORT1_IO(0);
    AD_PORT2_IO(1);
    AD_PORT3_IO(1);
#if 0// Infinium
    AD_PORT4_IO(0);
    AD_PORT5_IO(0);
    AD_PORT6_IO(0);
    AD_PORT7_IO(0);
#else// CF-X2
    AD_PORT4_IO(1);
    AD_PORT5_IO(1);
    AD_PORT6_IO(1);
    AD_PORT7_IO(1);
#endif
    AD_PORT8_IO(0);
    AD_PORT9_IO(0);

    AD_PORT10_IO(1);
    AD_PORT11_IO(0);
    AD_PORT12_IO(0);
    AD_PORT13_IO(0);


    AD_PORT0_OUT(0);
    AD_PORT1_OUT(0);
    AD_PORT2_OUT(0);
    AD_PORT3_OUT(0);
    AD_PORT4_OUT(0);
    AD_PORT5_OUT(0);
    AD_PORT6_OUT(0);
    AD_PORT7_OUT(0);
    AD_PORT8_OUT(0);
    AD_PORT9_OUT(0);

    D_PORT0_IO(0);
    //test D_PORT1_IO(1);
    D_PORT1_IO(0);//test for EMS22A50
    //test D_PORT2_IO(1);
    D_PORT2_IO(0);//test for EMS22A50
    D_PORT3_IO(1);

    RE_CS(0);
    RE_CLK(0);
}

/**********************************************
*  UDP Generic I/O Processing Part
*********************************************/
void UDPControlTask(void)
{
	unsigned int RcvLen;
	static BYTE	buffer[1024];
    BYTE index, i, j, k;

	if(!initReceiveFlag)
	{
        RxSocket = UDPOpen(localPort, NULL, 0);
		if(RxSocket != INVALID_UDP_SOCKET)
        {
			initReceiveFlag = TRUE;
        }
	}
    if(initReceiveFlag && UDPIsGetReady(RxSocket))
    {
        LED_2_On();
        //if(initSendFlag)
        //    sendOSCMessage(TxSocket, stdPrefix, "/debug", "i", 0);
        RcvLen = UDPGetArray(buffer, sizeof(buffer));
        //if(initSendFlag)
        //    sendOSCMessage(TxSocket, stdPrefix, "/debug", "iii", 1, RcvLen, buffer[0]);
        //UDPDiscard();
        if(isEqualToAddress(buffer, prefix, msgOnboardLed))
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
        else if(isEqualToAddress(buffer, prefix, msgLatticeLed))
        {
            BYTE x = getIntArgumentAtIndex(buffer, prefix, msgLatticeLed, 0);
            BYTE y = getIntArgumentAtIndex(buffer, prefix, msgLatticeLed, 1);
            BYTE state = getIntArgumentAtIndex(buffer, prefix, msgLatticeLed, 2);

            if(state)
                ledState |= (1 << y) << (x * 4);
            else
                ledState &= ~((1 << y) << (x * 4));

            sendSpiOneWord(0x0000, 8);
            sendSpiOneWord(ledState, 8);
            //sendOSCMessage(TxSocket, stdPrefix, "/debug", "i", ledState);
        }
        else if(isEqualToAddress(buffer, prefix, msgLatticeLedColumn))
        {
            BYTE column = getIntArgumentAtIndex(buffer, prefix, msgLatticeLedColumn, 0);
            BYTE data = getIntArgumentAtIndex(buffer, prefix, msgLatticeLedColumn, 1);

            if(column > 3 || data > 15)
                return;

            ledState &= ~(0x000F << (column * 4));
            ledState |= data << (column * 4);

            sendSpiOneWord(0x0000, 8);
            sendSpiOneWord(ledState, 8);
            //sendOSCMessage(TxSocket, stdPrefix, "/debug", "i", ledState);
        }
        else if(isEqualToAddress(buffer, prefix, msgLatticeLedRow))
        {
            BYTE row = getIntArgumentAtIndex(buffer, prefix, msgLatticeLedRow, 0);
            BYTE data = getIntArgumentAtIndex(buffer, prefix, msgLatticeLedRow, 1);

            if(row > 3 || data > 15)
                return;

            WORD data1 = 0;
            for(i = 0; i < 4; i++)
            {
                if((data >> i) & 0x01)
                    data1 |= (1 << (i * 4));
            }
            ledState &= ~(0x1111 << row);
            ledState |= data1 << row;

            sendSpiOneWord(0x0000, 8);
            sendSpiOneWord(ledState, 8);
            //sendOSCMessage(TxSocket, stdPrefix, "/debug", "i", ledState);
        }
        else if(isEqualToAddress(buffer, prefix, msgSetRotaryEncStep))
        {
            reStep = getIntArgumentAtIndex(buffer, prefix, msgSetRotaryEncStep, 0);
        }
        else if(isEqualToAddress(buffer, prefix, msgRotaryLedStep))
        {
            INT16 pos0 = getIntArgumentAtIndex(buffer, prefix, msgRotaryLedStep, 0);
            INT16 pos;
            if(24 - pos0 >= 0)
                pos = 24 - pos0;
            else
                pos = (24 - pos0) + 32;
            BYTE len = getIntArgumentAtIndex(buffer, prefix, msgRotaryLedStep, 1);
            dwLedData = 0;

            BYTE argLen = rcvArgumentsLength - 3;

            if(argLen >= len)
            {
                for(i = 0; i < len; i++)
                {
                    //debug sendOSCMessage(TxSocket, stdPrefix, "/debug0", "iiii", argLen, len, pos, i);
                    if(pos >= i)
                        intensity[pos - i] = getIntArgumentAtIndex(buffer, prefix, msgRotaryLedStep, i + 2);
                    else
                        intensity[32 + pos - i] = getIntArgumentAtIndex(buffer, prefix, msgRotaryLedStep, i + 2);
                }
            }
            else if(argLen < len)
            {
                for(i = 0; i < argLen; i++)
                {
                    //debug sendOSCMessage(TxSocket, stdPrefix, "/debug1", "iiii", argLen, len, pos, i);
                    if(pos >= i)
                        intensity[pos - i] = getIntArgumentAtIndex(buffer, prefix, msgRotaryLedStep, i + 2);
                    else
                        intensity[32 + pos - i] = getIntArgumentAtIndex(buffer, prefix, msgRotaryLedStep, i + 2);
                }
            }

            for(i = 0; i < len; i++)
            {
                if(pos >= len)
                    dwLedData |= (1 << (pos - i));
                else
                    dwLedData |= (1 << (32 + pos - i));
            }

            for(j = 0; j < 100; j++)
            {
                dwLedSequence[j] = dwLedData;

                for(k = 0; k < 32; k++)
                {
                    if(j >= intensity[k])
                        dwLedSequence[j] &= ~(1 << k);
                }
            }

            if(dwLedData != 0)
            {
                //ledIntensityIndex = 0;
                ledOn = TRUE;
            }
            else
            {
                ledOn = FALSE;
                sendSpiTwoWord(0x0000, 0x0000, 8);
            }
        }
        else if(isEqualToAddress(buffer, prefix, msgRotaryLedBits))
        {
            dwLedData = getIntArgumentAtIndex(buffer, prefix, msgRotaryLedBits, 0);
            for(j = 0; j < 100; j++)
            {
                dwLedSequence[j] = dwLedData;

                for(k = 0; k < 32; k++)
                {
                    if(j >= intensity[k])
                        dwLedSequence[j] &= ~(1 << k);
                }
            }
            if(dwLedData != 0)
            {
                //ledIntensityIndex = 0;
                ledOn = TRUE;
            }
            else
            {
                ledOn = FALSE;
                sendSpiTwoWord(0x0000, 0x0000, 8);
            }
        }
        else if(isEqualToAddress(buffer, prefix, msgRotaryLedIntensity))
        {
            index = getIntArgumentAtIndex(buffer, prefix, msgRotaryLedIntensity, 0);
            intensity[index] = getIntArgumentAtIndex(buffer, prefix, msgRotaryLedIntensity, 1);
            if(intensity[index] > 90)
                intensity[index] = 90;

            for(j = 0; j < 100; j++)
            {
                dwLedSequence[j] = dwLedData;
                for(k = 0; k < 32; k++)
                {
                    if(j >= intensity[k])
                        dwLedSequence[j] &= ~(1 << k);
                }
            }
        }
        else if(isEqualToAddress(buffer, prefix, msgRotaryLedAllInt))
        {
            DWORD dwld = getIntArgumentAtIndex(buffer, prefix, msgRotaryLedAllInt, 0);
            if(dwld > 90)
                dwld = 90;

            for(j = 0; j < 100; j++)
            {
                dwLedSequence[j] = dwLedData;
                for(k = 0; k < 32; k++)
                {
                    intensity[k] = dwld;
                    if(j >= intensity[k])
                        dwLedSequence[j] &= ~(1 << k);
                }
            }
        }
        else if(isEqualToAddress(buffer, stdPrefix, msgSetPwmState))
        {
            index = getIntArgumentAtIndex(buffer, stdPrefix, msgSetPwmState, 0);
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
                if(onTimer23)
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
                onSquare[index] = FALSE;
            }
        }
        else if(isEqualToAddress(buffer, stdPrefix, msgGetPwmState))
        {
            index = getIntArgumentAtIndex(buffer, stdPrefix, msgGetPwmState, 0);
            sendOSCMessage(TxSocket, stdPrefix, msgGetPwmState, "is", index, onSquare[index] ? "on" : "off");
        }
        else if(isEqualToAddress(buffer, stdPrefix, msgSetPwmFreq))
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
        else if(isEqualToAddress(buffer, stdPrefix, msgGetPwmFreq))
        {
            sendOSCMessage(TxSocket, stdPrefix, msgGetPwmFreq, "i", freq);
        }
        else if(isEqualToAddress(buffer, stdPrefix, msgSetPwmDuty))
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
        else if(isEqualToAddress(buffer, stdPrefix, msgGetPwmDuty))
        {
            index = getIntArgumentAtIndex(buffer, stdPrefix, msgGetPwmDuty, 0);
            sendOSCMessage(TxSocket, stdPrefix, msgGetPwmDuty, "ii", index, duty[index]);
        }
#if 0
        else if(isEqualToAddress(buffer, midiPrefix, msgSetNote))
        {
            if(ProcState == STATE_READY)
            {
                BYTE currentEndpoint;
                ENDPOINT_DIRECTION direction;

                endpointBuffers = malloc(sizeof(ENDPOINT_BUFFER) * USBHostMIDINumberOfEndpoints(deviceHandle));

                for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
                {
                    direction = USBHostMIDIEndpointDirection(deviceHandle, currentEndpoint);
                    if(direction == OUT)
                    {
                        endpointBuffers[currentEndpoint].TransferState = TX_DATA;
                    }
                    data0 = getIntArgumentAtIndex(buffer, midiPrefix, msgSetNote, 0);
                    data1 = getIntArgumentAtIndex(buffer, midiPrefix, msgSetNote, 1);
                    data2 = getIntArgumentAtIndex(buffer, midiPrefix, msgSetNote, 2);
                    endpointBuffers[currentEndpoint].pBufReadLocation->DATA_0 = data0;
                    endpointBuffers[currentEndpoint].pBufReadLocation->DATA_1 = data1;
                    endpointBuffers[currentEndpoint].pBufReadLocation->DATA_2 = data2;
                    if(USBHostMIDIWrite(deviceHandle, currentEndpoint, endpointBuffers[currentEndpoint].pBufReadLocation, endpointBuffers[currentEndpoint].numOfMIDIPackets * sizeof(USB_AUDIO_MIDI_PACKET)) == USB_SUCCESS)
                    {
                        //test endpointBuffers[currentEndpoint].TransferState = TX_DATA_WAIT;
                        LED_1_Toggle();
                    }

                    if(!USBHostMIDITransferIsBusy(deviceHandle, currentEndpoint))
                    {
                        endpointBuffers[currentEndpoint].TransferState = TX_DATA;
                        endpointBuffers[currentEndpoint].pBufReadLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;
                        if(endpointBuffers[currentEndpoint].pBufReadLocation - endpointBuffers[currentEndpoint].bufferStart >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                        {
                            endpointBuffers[currentEndpoint].pBufReadLocation = endpointBuffers[currentEndpoint].bufferStart;
                        }
                    }
                }
            }

            //test BYTE currentEndpoint = 0;
            //test for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
            {
                
                //test LED_1_Toggle();
                //testUSBHostMIDIWrite(deviceHandle, currentEndpoint, endpointBuffers[currentEndpoint].pBufReadLocation, endpointBuffers[currentEndpoint].numOfMIDIPackets * sizeof(USB_AUDIO_MIDI_PACKET));
            }
        }
#endif
        else if(isEqualToAddress(buffer, sysPrefix, msgSetRemoteIp))
        {
            char* rip = getStringArgumentAtIndex(buffer, sysPrefix, msgSetRemoteIp, 0);
            remoteIP[0] = atoi(strtok(rip, "."));
            remoteIP[1] = atoi(strtok(NULL, "."));
            remoteIP[2] = atoi(strtok(NULL, "."));
            remoteIP[3] = atoi(strtok(NULL, "."));
            initSendFlag = FALSE;
            UDPClose(TxSocket);
            free(rip);
            rip = NULL;
        }
        else if(isEqualToAddress(buffer, sysPrefix, msgGetRemoteIp))
        {
            char* rip = (char *)calloc(15, sizeof(char));;
            sprintf(rip, "%d.%d.%d.%d", remoteIP[0], remoteIP[1], remoteIP[2], remoteIP[3]);
            sendOSCMessage(TxSocket, sysPrefix, msgRemoteIp, "s", rip);
            free(rip);
            rip = NULL;
        }
        else if(isEqualToAddress(buffer, sysPrefix, msgSetRemotePort))
        {
            remotePort = getIntArgumentAtIndex(buffer, sysPrefix, msgSetRemotePort, 0);
            UDPClose(TxSocket);
            initSendFlag = FALSE;
        }
        else if(isEqualToAddress(buffer, sysPrefix, msgGetRemotePort))
        {
            sendOSCMessage(TxSocket, sysPrefix, msgRemotePort, "i", remotePort);
        }
        else if(isEqualToAddress(buffer, sysPrefix, msgSetHostName))
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
            sendOSCMessage(TxSocket, sysPrefix, msgHostName, "s", hostName);
        }
        else if(isEqualToAddress(buffer, sysPrefix, msgGetHostName))
        {
            sendOSCMessage(TxSocket, sysPrefix, msgHostName, "s", hostName);
        }
        else if(isEqualToAddress(buffer, sysPrefix, msgGetHostIp))
        {
            char hip[15] = {0};
            BYTE hip0 = AppConfig.MyIPAddr.Val & 0xFF;
            BYTE hip1 = (AppConfig.MyIPAddr.Val >> 8) & 0xFF;
            BYTE hip2 = (AppConfig.MyIPAddr.Val >> 16) & 0xFF;
            BYTE hip3 = (AppConfig.MyIPAddr.Val >> 24) & 0xFF;
            sprintf(hip, "%d.%d.%d.%d", hip0, hip1, hip2, hip3);
            sendOSCMessage(TxSocket, sysPrefix, msgHostIp, "s", hip);
        }
        else if(isEqualToAddress(buffer, sysPrefix, msgGetHostMac))
        {
            char macaddr[17] = {0};
            sprintf(macaddr, "%X:%X:%X:%X:%X:%X", AppConfig.MyMACAddr.v[0], AppConfig.MyMACAddr.v[1], AppConfig.MyMACAddr.v[2], AppConfig.MyMACAddr.v[3], AppConfig.MyMACAddr.v[4], AppConfig.MyMACAddr.v[5]);
            sendOSCMessage(TxSocket, sysPrefix, msgHostMac, "s", macaddr);
        }
        else if(isEqualToAddress(buffer, sysPrefix, msgSetHostPort))
        {
            localPort = getIntArgumentAtIndex(buffer, sysPrefix, msgSetHostPort, 0);
            UDPClose(RxSocket);
            initReceiveFlag = FALSE;
        }
        else if(isEqualToAddress(buffer, sysPrefix, msgGetHostPort))
        {
            sendOSCMessage(TxSocket, sysPrefix, msgHostPort, "i", localPort);
        }
        else if(isEqualToAddress(buffer, sysPrefix, msgSetPrefix))
        {
            free(prefix);
            prefix = NULL;
            prefix = getStringArgumentAtIndex(buffer, sysPrefix, msgSetPrefix, 0);
        }
        else if(isEqualToAddress(buffer, sysPrefix, msgGetPrefix))
        {
            sendOSCMessage(TxSocket, sysPrefix, msgPrefix, "s", prefix);
        }
        else if(isEqualToAddress(buffer, sysPrefix, msgSoftReset))
        {
            //BYTE ps = getIntArgumentAtIndex(buffer, sysPrefix, msgSoftReset, 0);
            //TRISDbits.TRISD10 = 0;
            //LATDbits.LATD10 = ps;
            //delayMs(500);
            NVMErasePage((void*)NVM_DATA);
            NVMWriteWord((void*)(NVM_DATA), (unsigned int)0x01);

            SoftReset();
        }
        else if(isEqualToAddress(buffer, sysPrefix, msgDebug))
        {
            sendOSCMessage(TxSocket, stdPrefix, "/debug", "i", *(unsigned int *)(NVM_DATA));
        }
        LED_2_Off();
    }
}

void sendPad(void)
{
    BYTE i, j, k, l;

    SR_SL(0);
    delayUs(1);
    SR_SL(1);

    for(i = 0; i < MAX_BTN_ROW; i++)
    {
        btnLast[i] = btnCurrent[i];
    }

    for(i = 0; i < MAX_BTN_ROW; i++)
    {
        switch(i)
        {
            case 0:
                l = 1;
                break;
            case 1:
                l = 0;
                break;
            case 2:
                l = 3;
                break;
            case 3:
                l = 2;
                break;
        }
        for(j = 0; j < MAX_BTN_COL; j++)
        {
            k = 3 - j;

            if(SR_QH())
            {
                btnCurrent[l] &= ~(1 << k);
            }
            else
            {
                btnCurrent[l] |= (1 << k);
            }
            SR_CLK(1);
            delayUs(1);
            SR_CLK(0);

            if(buttonCheck(l, k))
            {
                sendOSCMessage(TxSocket, prefix, msgPress, "iii", l, k, (btnCurrent[l] & (1 << k)) ? 1 : 0);
                StackTask();
            }
            delayUs(5);
        }
    }
}

void sendAdc(void)
{
    BYTE i;
#if 0// Infinium
    for(i = 0; i < USE_ADC_NUM; i++)
    {
        if(getAnalogFlag(i))
        {
            flag = TRUE;
            currentValue[i] = getAnalogByte(i, TYPE_MIDI_ORIGINAL);
            if(i == 1)
            {
                BYTE faderValue = 0;
#if 0
                if(currentValue[0] < 127)
                    faderValue = 255 - currentValue[1];
                else
                    faderValue = currentValue[0];
#else
                if(currentValue[0] <= 127 && currentValue[1] > 127)
                {
                    faderValue = currentValue[0];
                    sendOSCMessage(TxSocket, prefix, msgAdc, "iiii", i + 10, faderValue, currentValue[0], currentValue[1]);
                    StackTask();
                }
                else if(currentValue[0] > 127 && currentValue[1] <= 127)
                {
                    faderValue = 255 - currentValue[1];
                    sendOSCMessage(TxSocket, prefix, msgAdc, "iiii", i + 10, faderValue, currentValue[0], currentValue[1]);
                    StackTask();
                }
#endif
            }
            resetAnalogFlag(i);
            //delayUs(5);
        }
    }
#endif
#if 1// CF-X2
    BOOL flag = FALSE;
    int b_min = 120;
    int b_max = 136;

    for(i = 0; i < USE_ADC_NUM; i++)
    {
        if(getAnalogFlag(i))
        {
            //currentValue[i] = getAnalogWord(i, TYPE_LONG_ORIGINAL);
            currentValue[i] = getAnalogByte(i, TYPE_MIDI_ORIGINAL);
            flag = TRUE;

            resetAnalogFlag(i);
        }
    }

    if(initSendFlag && flag)
    {
        if((currentValue[0] >= b_min && currentValue[0] <= b_max) &&
           (currentValue[1] >= b_min && currentValue[1] <= b_max) &&
           (currentValue[2] >= b_min && currentValue[2] <= b_max) &&
           (currentValue[3] >= b_min && currentValue[3] <= b_max))
        {
            currentSection = 0;
            currentPosition = 152 - currentValue[5 - currentSection];
            
            if(currentPosition > currentPosition1 && prevDirection == -1)
                currentPosition = currentPosition1;
        }
        else if((currentValue[2] >= b_min && currentValue[2] <= b_max) &&
                (currentValue[3] >= b_min && currentValue[3] <= b_max) &&
                (currentValue[4] >= b_min && currentValue[4] <= b_max) &&
                (currentValue[5] >= b_min && currentValue[5] <= b_max))
        {
            currentSection = 5;
            currentPosition = currentValue[5 - currentSection] - 88;
        }
        else if((currentValue[0] >= b_min && currentValue[0] <= b_max) &&
                (currentValue[1] >= b_min && currentValue[1] <= b_max) &&
                (currentValue[2] >= b_min && currentValue[2] <= b_max))
        {
            currentSection = 1;

            if(currentSection != prevSection)
            {
                boundaryValue[5 - currentSection] = currentValue[5 - currentSection];
                boundaryPosition = currentPosition;
            }
            currentPosition = (boundaryValue[5 - currentSection] - currentValue[5 - currentSection]) + boundaryPosition;
        }
        else if((currentValue[3] >= b_min && currentValue[3] <= b_max) &&
                (currentValue[4] >= b_min && currentValue[4] <= b_max) &&
                (currentValue[5] >= b_min && currentValue[5] <= b_max))
        {
            currentSection = 4;

            if(currentSection != prevSection)
            {
                boundaryValue[5 - currentSection] = currentValue[5 - currentSection];
                boundaryPosition = currentPosition;
            }
            currentPosition = currentValue[5 - currentSection] - boundaryValue[5 - currentSection] + boundaryPosition;
        }
        else if((currentValue[0] >= b_min && currentValue[0] <= b_max) &&
                (currentValue[1] >= b_min && currentValue[1] <= b_max) &&
                (currentValue[5] >= b_min && currentValue[5] <= b_max))
        {
            currentSection = 2;

            if(currentSection != prevSection)
            {
                boundaryValue[5 - currentSection] = currentValue[5 - currentSection];
                boundaryPosition = currentPosition;
            }
            currentPosition = (boundaryValue[5 - currentSection] - currentValue[5 - currentSection]) + boundaryPosition;

            if(prevSection == 3)
                centerPosition = currentPosition;
        }
        else if((currentValue[0] >= b_min && currentValue[0] <= b_max) &&
                (currentValue[4] >= b_min && currentValue[4] <= b_max) &&
                (currentValue[5] >= b_min && currentValue[5] <= b_max))
        {
            currentSection = 3;

            if(currentSection != prevSection)
            {
                boundaryValue[5 - currentSection] = currentValue[5 - currentSection];
                boundaryPosition = currentPosition;
            }
            currentPosition = currentValue[5 - currentSection] - boundaryValue[5 - currentSection] + boundaryPosition;

            if(prevSection == 2)
                centerPosition = currentPosition;
        }
#if 0
        if((currentValue[0] >= b_min && currentValue[0] <= b_max) &&
           (currentValue[1] >= b_min && currentValue[1] <= b_max) &&
           (currentValue[4] >= b_min && currentValue[4] <= b_max) &&
           (currentValue[5] >= b_min && currentValue[5] <= b_max))
        {
            currentPosition = 150;
        }
#endif
        if((currentPosition >> 1) != (currentPosition1 >> 1))
            prevPosition = currentPosition1;

        if(currentPosition > 255)
            sendOSCMessage(TxSocket, prefix, msgAdc, "iiii", 127, prevPosition >> 1, 0, currentSection);
        else
        {
            if(currentSection < 3)
            {
                if((currentPosition >> 1) > (prevPosition >> 1))
                    currentDirection = 1;
                else if((currentPosition >> 1) < (prevPosition >> 1))
                    currentDirection = -1;
                sendOSCMessage(TxSocket, prefix, msgAdc, "iiii", (currentPosition >> 1), (prevPosition >> 1), currentDirection, currentSection);
            }
            else
            {
                if((currentPosition >> 1) > prevPosition)
                    currentDirection = -1;
                else if((currentPosition >> 1) < prevPosition)
                    currentDirection = 1;
                sendOSCMessage(TxSocket, prefix, msgAdc, "iiii", 255 - (currentPosition >> 1), (prevPosition >> 1), currentDirection, currentSection);
            }
        }

        StackTask();

        if(currentPosition > currentPosition1 && prevDirection == -1 && currentSection)
            prevDirection = -1;
        else
        {
            if(currentPosition <= 1)
                prevDirection = 0;
            else
                prevDirection = currentDirection;
        }
        prevSection = currentSection;
        currentPosition1 = currentPosition;
        
    }
#endif
}

void sendEnc(void)
{
#if 0
    currentSwitch = RE_SW();
    if(prevSwitch != currentSwitch)
    {
        switchFlag = TRUE;
        prevSwitch = currentSwitch;
    }

    if(initSendFlag && sendEncFlag)
    {
        if(reFlag[0])
        {
            sendOSCMessage(TxSocket, prefix, msgRotaryEnc, "iiffi", direction, reStep, omega[0], alpha[0], (1 - currentSwitch));
        }
        else if(reFlag[1])
        {
            sendOSCMessage(TxSocket, prefix, msgRotaryEnc, "iiffi", direction, reStep, omega[1], alpha[1], (1 - currentSwitch));
        }
        sendEncFlag = FALSE;
    }

    if(initSendFlag && switchFlag)
    {
        sendOSCMessage(TxSocket, prefix, msgRotaryEncSwitch, "i", 1 - currentSwitch);
        switchFlag = FALSE;
    }
#else// test for EMS22A50
    if(initSendFlag && sendEncFlag)
    {
        //debug sendOSCMessage(TxSocket, prefix, msgRotaryEnc, "fiiiiiiiii", reAbsAnglePos, reMatchCount, rePosData[0], rePosData[1], rePosData[2], rePosData[3], rePosData[4], rePosData[5], rePosData[6], rePosData[7]);
        if(reAbsAnglePos > reAbsAnglePosLast)
        {
            if(reAbsAnglePos - reAbsAnglePosLast > 1000)
                reDirection = -1;
            else
                reDirection = 1;
        }
        else if(reAbsAnglePos < reAbsAnglePosLast)
        {
            if(reAbsAnglePos - reAbsAnglePosLast < -1000)
                reDirection = 1;
            else
                reDirection = -1;
        }
        else
            reDirection = 0;

        //if(reCounted != 0)
        //    reVelocity = (reAbsAnglePos - reAbsAnglePosLast) / (reCounted * ((float)TIMER_COUNT / 10.0)) * 1000000.0;
        sendOSCMessage(TxSocket, prefix, msgRotaryEnc, "fiff", reAbsAnglePos, reDirection, reVelocity, reCounted);
        sendEncFlag = FALSE;
        reMatchCount = 0;

        reAbsAnglePosLast = reAbsAnglePos;

        StackTask();
    }
#endif
}

#if 1
void __ISR(_TIMER_4_VECTOR, ipl1) T4Handler(void)
{
#if 0
    encoderCheck(RE_A(), RE_B());
    if(ledOn)
    {
        WORD msb, lsb;
        msb = (WORD)((dwLedSequence[ledIntensityIndex] >> 16) & 0x0000FFFF);
        lsb = (WORD)(dwLedSequence[ledIntensityIndex] & 0x0000FFFF);
        sendSpiTwoWord(msb, lsb, 8);
        ledIntensityIndex++;
        if(ledIntensityIndex == 100)
            ledIntensityIndex = 0;
    }

    if(encCount[0] < 100000)
        encCount[0]++;
    if(encCount[1] < 100000)
        encCount[1]++;
    if(dt < 100000)
        dt++;
#else//test for EMS22A50
    int i;
    WORD currentPos;

    if(ledOn)
    {
        WORD msb, lsb;
        msb = (WORD)((dwLedSequence[ledIntensityIndex] >> 16) & 0x0000FFFF);
        lsb = (WORD)(dwLedSequence[ledIntensityIndex] & 0x0000FFFF);
        sendSpiTwoWord(msb, lsb, 8);
        ledIntensityIndex++;
        if(ledIntensityIndex == 100)
            ledIntensityIndex = 0;
    }

    RE_CS(1);
    delayUs(1);
    RE_CS(0);
    delayUs(1);

    for(reDataIndex = 0; reDataIndex < 16; reDataIndex++)
    {
        RE_CLK(0);
        delayUs(1);
        RE_CLK(1);
        delayUs(1);

        reState[reDataIndex] = RE_DO();
    }

    currentPos = 0;
    for(i = 0; i < 10; i++)
        currentPos |= (reState[9 - i] << i);

    reAvgIndex++;
    if(reMatchCount == 0)
    {
        for(i = 0; i < AVG_NUM; i++)
            rePosData[i] = 0;

        rePosData[0] = currentPos;
        reMatchCount = 1;
    }
    else
    {
        BOOL matchFlag = FALSE;
        for(i = 0; i < reMatchCount; i++)
        {
            if(currentPos != rePosData[i])
                matchFlag = TRUE;
            else
            {
                matchFlag = FALSE;
                break;
            }
        }
        if(matchFlag)
        {
            rePosData[reMatchCount] = currentPos;
            reMatchCount++;
        }
        
    }

    if(reAvgIndex == AVG_NUM)
    {
        if(reMatchCount > 3)
        {
            BYTE startBoundaryCount = 0;
            BYTE endBoundaryCount = 0;
            if(reMatchCount > 8)
                reMatchCount = 8;

            float test = reAbsAnglePos;
            reAbsAnglePos = 0.0;
            for(i = 0; i < reMatchCount; i++)
            {
                if(rePosData[i] <= 10)
                    startBoundaryCount++;
                if(rePosData[i] >= 1013)
                    endBoundaryCount++;
            }
            if(startBoundaryCount == 0 || endBoundaryCount == 0)
            {
                for(i = 0; i < reMatchCount; i++)
                {
                    reAbsAnglePos += (float)rePosData[i];
                }
                reAbsAnglePos /= (float)reMatchCount;
            }
            else
            {
                if(startBoundaryCount > endBoundaryCount)
                {
                    for(i = 0; i < reMatchCount; i++)
                    {
                        if(rePosData[i] <= 10)
                            reAbsAnglePos += (float)rePosData[i];
                    }
                    reAbsAnglePos /= (float)startBoundaryCount;
                }
                else
                {
                    for(i = 0; i < reMatchCount; i++)
                    {
                        if(rePosData[i] >= 1013)
                            reAbsAnglePos += (float)rePosData[i];
                    }
                    reAbsAnglePos /= (float)endBoundaryCount;
                }
            }
            sendEncFlag = TRUE;
            if(reCounting != 0)
            {
                //reCounted = reCounting;
                reAvgCounted[reVelAvgIndex] = reCounting;
                reCounted = 0.0;
                for(i = 0; i < 8; i++)
                {
                    //reCounted += (float)reAvgCounted[i];
                    if(i <= reVelAvgIndex)
                        reCounted += (7 - (reVelAvgIndex - i)) * reAvgCounted[i];
                    else
                        reCounted += (7 - (reVelAvgIndex - i) - 8) * reAvgCounted[i];
                }
                //reCounted /= 8.0;
                reCounted /= ((8.0 * 9.0) / 2.0);
                reVelAvgIndex++;
                if(reVelAvgIndex >= 8)
                    reVelAvgIndex = 0;

                //reVelocity = (reAbsAnglePos - test) / (reCounted * ((float)TIMER_COUNT / 10.0)) * 1000000.0;
                float diff = 0;
                if(fabs(reAbsAnglePos - test) > 1000.0)
                {
                    if(reAbsAnglePos - test < 1000.0)
                        diff = (reAbsAnglePos + 1023.0) - test;
                    else if(reAbsAnglePos - test > 1000.0)
                        diff = reAbsAnglePos - (test + 1023.0);
                }
                else
                    diff = reAbsAnglePos - test;

                reAvgVelocity[reVelAvgIndex2] = diff / (reCounted * ((float)TIMER_COUNT / 10.0)) * 1000000.0;
                reVelocity = 0.0;
                for(i = 0; i < 16; i++)
                {
                    //reVelocity += reAvgVelocity[i];
                    if(i <= reVelAvgIndex2)
                        reVelocity += (15 - (reVelAvgIndex2 - i)) * reAvgVelocity[i];
                    else
                        reVelocity += (15 - (reVelAvgIndex2 - i) - 16) * reAvgVelocity[i];
                }
                //reVelocity /= 16.0;
                reVelocity /= ((17.0 * 16.0) / 2.0);
                reVelAvgIndex2++;
                if(reVelAvgIndex2 >= 16)
                    reVelAvgIndex2 = 0;
            }
            reCounting = 0;
        }
        else
        {
            if(reCounting < 100000)
                reCounting++;
        }
        reAvgIndex = 0;
    }
#endif
    mT4ClearIntFlag();
}
#endif

#if 0
void convertOscToMidi(BYTE buffer)
{
    if(isEqualToAddress(buffer, midiPrefix, msgSetNote))
    {
        BYTE ch = getIntArgumentAtIndex(buffer, midiPrefix, msgSetNote, 0);
        BYTE num = getIntArgumentAtIndex(buffer, midiPrefix, msgSetNote, 1);
        BYTE vel = getIntArgumentAtIndex(buffer, midiPrefix, msgSetNote, 2);
        endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_0 = 0x90 | ch;
        endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_1 = num;       // argument 1
        endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_2 = vel;       // argument 2
    
    }
    USBHostMIDIWrite(deviceHandle, currentEndpoint, endpointBuffers[currentEndpoint].pBufReadLocation, endpointBuffers[currentEndpoint].numOfMIDIPackets * sizeof(USB_AUDIO_MIDI_PACKET));
}
#endif

void UDPSendTask()
{
    if(!initSendFlag)
    {
        TxSocket = UDPOpenEx((remoteIP[0] | (remoteIP[1] << 8) | (remoteIP[2] << 16) |
                             (remoteIP[3] << 24)), UDP_OPEN_IP_ADDRESS, 0, remotePort);
        if(TxSocket == INVALID_UDP_SOCKET)
        {
			return;
        }
		initSendFlag = TRUE;
    }

    if(initSendFlag)
	{
        if(stateFlag)
        {
            stateFlag = FALSE;
            //sendOSCMessage(TxSocket, prefix, msgSw, "i", (currentState ? 0 : 1));
            if(!currentState)
                sendOSCMessage(TxSocket, sysPrefix, msgPrefix, "is", -2, prefix);
        }

        if(!BusyADC10())
        {
#if 0// Infinium
            analogInHandle(0, (LONG)((ADC1BUF0 + ADC1BUF2 + ADC1BUF4 + ADC1BUF6) / 4));
            analogInHandle(1, (LONG)((ADC1BUF1 + ADC1BUF3 + ADC1BUF5 + ADC1BUF7) / 4));
#else// CF-X2
            analogInHandle(0, (LONG)((ADC1BUF0 + ADC1BUF6) / 2));
            analogInHandle(1, (LONG)((ADC1BUF1 + ADC1BUF7) / 2));
            analogInHandle(2, (LONG)((ADC1BUF2 + ADC1BUF8) / 2));
            analogInHandle(3, (LONG)((ADC1BUF3 + ADC1BUF9) / 2));
            analogInHandle(4, (LONG)((ADC1BUF4 + ADC1BUFA) / 2));
            analogInHandle(5, (LONG)((ADC1BUF5 + ADC1BUFB) / 2));
#endif
            sendAdc();
        }

        switch(usbState)
        {
            case 0:
                sendPad();
                usbState = 1;
                break;
            case 1:
                sendEnc();
                usbState = 0;
                break;
#if 0
            case 3:
                convertOSCtoMIDI();
                usbState = 0;
                break;
#endif
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
                        //memset(endpointBuffers[currentEndpoint].pBufWriteLocation, 0x00, endpointBuffers[currentEndpoint].numOfMIDIPackets * sizeof(USB_AUDIO_MIDI_PACKET));
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
                                        sendOSCMessage(TxSocket, midiPrefix, msgNote, "iii", midiCh, midiNum, midiVal);
                                        break;
                                    case 0xA0:// Key Pressure
                                        sendOSCMessage(TxSocket, midiPrefix, msgKp, "iii", midiCh, midiNum, midiVal);
                                        break;
                                    case 0xB0:// Control Change
                                        sendOSCMessage(TxSocket, midiPrefix, msgCc, "iii", midiCh, midiNum, midiVal);
                                        break;
                                    case 0xC0:// Program Change
                                        sendOSCMessage(TxSocket, midiPrefix, msgPc, "i", midiNum);
                                        break;
                                    case 0xD0:// Channel Pressure
                                        sendOSCMessage(TxSocket, midiPrefix, msgCp, "ii", midiCh, midiNum);
                                        break;
                                    case 0xE0:// Pitch Bend
                                        sendOSCMessage(TxSocket, midiPrefix, msgPb, "iii", midiCh, midiNum, midiVal);
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

//******************************************************************************
//******************************************************************************
// USB Support Functions
//******************************************************************************
//******************************************************************************

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
