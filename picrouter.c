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
 * picrouter.c,v.0.2 2012/07/29
 */

#include "picrouter.h"

// Initialization of IP address and others.
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

    memcpypgm2ram(AppConfig.NetBIOSName, (ROM void*)MY_DEFAULT_HOST_NAME, 16);
    FormatNetBIOSName(AppConfig.NetBIOSName);
}

void _general_exception_handler(unsigned cause, unsigned status)
{
    Nop();
    Nop();
}

int main(int argc, char** argv) {
	int value;
    static DWORD t = 0;
    WORD pCount = 0;
    WORD rCount = 0;

	mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

    setupPitch();
    AD1PCFG = 0x0000FFFC;// 0000 0000 0000 0000 1111 1111 1111 1100
    AD1CON1 = 0x000010E6;// 0000 0000 0000 0000 1000 0000 0110 0110
    AD1CON2 = 0x0000040C;// 0000 0000 0000 0000 0000 0100 0000 1100
    AD1CHS  = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
    AD1CSSL = 0x00000003;// 0000 0000 0000 0000 0000 0000 0000 0011
    AD1CON3 = 0x00000F08;// 0000 0000 0000 0000 0000 1111 0000 1000
    AD1CON1bits.ON = 1;

    initSW();
    initLEDs();
    buttonInit();
    //setupPitch();

    stateFlag = FALSE;
    prevState = 1;
    currentState = 1;

    LED_1_On();
    LED_2_On();

    // Enable the cache for the best performance
    CheKseg0CacheOn();

    // Enable optimal performance
    SYSTEMConfigPerformance(GetSystemClock());
    mOSCSetPBDIV(OSC_PB_DIV_1);				// Use 1:1 CPU Core:Peripheral clocks

    InitAppConfig();
    TickInit();
    StackInit();
    ZeroconfLLInitialize();
    mDNSInitialize(MY_DEFAULT_HOST_NAME);
    mDNSServiceRegister((const char *)"PICrouter-OaUD", // base name of the service
                        "_oscit._udp.local",       // type of the service
                        8000,                      // TCP or UDP port, at which this service is available
                        ((const BYTE *)""),        // TXT info
                        0,                         // auto rename the service when if needed
                        NULL,                      // no callback function
                        NULL                       // no application context
                        );
    mDNSMulticastFilterRegister();

    // Enable multi-vectored interrupts
    INTEnableSystemMultiVectoredInt();

    //USB Initialization
    #if defined(USE_USB_BUS_SENSE_IO)
    	tris_usb_bus_sense = 1;
    #endif

    #if defined(USE_SELF_POWER_SENSE_IO)
    	tris_self_power = 1;
    #endif

    USBDeviceInit();

    //for USB_MIDI
    MIDITxHandle = NULL;
    MIDIRxHandle = NULL;

    //for USB_HID
    HIDTxHandle = NULL;
    HIDRxHandle = NULL;

    #if defined(USB_INTERRUPT)
    	USBDeviceAttach();
    #endif
    usbAttachFlag = TRUE;

    LED_1_Off();
    LED_2_Off();

    while(1)
    {
#if 0//test
        if(TickGet() - t >= TICK_SECOND / 2ul)
        {
            t = TickGet();
            //LED_1_Toggle();
            LED_2_Toggle();
        }
#endif//test

		currentState = SW_State();

        if(prevState != currentState)
        {
            stateFlag = TRUE;
            stateFlag2 = TRUE;
        }

        StackTask();
        NBNSTask();
        ZeroconfLLProcess();
        mDNSProcess();

        if(initialCount > 32768)
            USBControlTask();
        else
            initialCount++;

        UDPControlTask();
        UDPSendTask();

        #if defined(USB_POLLING)
        	USBDeviceTasks();
        #endif

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
    }
    return (EXIT_SUCCESS);
}

void setupPitch(void)
{
    AD_PORT0_IO(1);
    AD_PORT1_IO(1);
    AD_PORT2_IO(0);
    AD_PORT3_IO(0);
    AD_PORT4_IO(0);
    AD_PORT5_IO(0);
    AD_PORT6_IO(0);
    AD_PORT7_IO(0);
    AD_PORT8_IO(0);
    AD_PORT9_IO(0);
    AD_PORT10_IO(0);
    AD_PORT11_IO(0);
    AD_PORT12_IO(0);
    AD_PORT13_IO(0);

    //AD_PORT0_OUT(0);
    //AD_PORT1_OUT(0);
    AD_PORT2_OUT(0);
    AD_PORT3_OUT(0);
    AD_PORT4_OUT(0);
    AD_PORT5_OUT(0);
    AD_PORT6_OUT(0);
    AD_PORT7_OUT(0);
    AD_PORT8_OUT(0);
    AD_PORT9_OUT(0);
    AD_PORT10_OUT(0);
    AD_PORT11_OUT(0);
    AD_PORT12_OUT(0);
    AD_PORT13_OUT(0);

    TRISFbits.TRISF5  = 1;
    TRISGbits.TRISG7  = 1;
    TRISFbits.TRISF0  = 1;
    TRISBbits.TRISB14 = 1;

    TRISGbits.TRISG6 = 0;
    TRISGbits.TRISG8 = 0;
    TRISFbits.TRISF1 = 0;
    TRISFbits.TRISF4 = 0;

    LATBbits.LATB2  = 0;
    LATBbits.LATB3  = 0;
    LATBbits.LATB4  = 0;
    LATBbits.LATB5  = 0;
    LATBbits.LATB6  = 0;
    LATBbits.LATB7  = 0;
    LATBbits.LATB8  = 0;
    LATBbits.LATB9  = 0;
    LATBbits.LATB10 = 0;
    LATBbits.LATB11 = 0;
    LATBbits.LATB12 = 0;

    BTN_LED_00(0);
    BTN_LED_01(0);
    BTN_LED_10(0);
    BTN_LED_11(0);
}

/**********************************************
*  UDP汎用I/O処理部
**********************************************/
void UDPControlTask(void)
{
	unsigned int RcvLen;
	static BYTE	buffer[40];

	if(!initReceiveFlag)
	{
		RxSocket = UDPOpen(localPort, NULL, 0);
		if(RxSocket != INVALID_UDP_SOCKET)
			initReceiveFlag = TRUE;
	}

	if(initReceiveFlag && UDPIsGetReady(RxSocket))
	{
		RcvLen = UDPGetArray(buffer, sizeof(buffer));
		UDPDiscard();
		if(buffer[0] == '/' && buffer[1] == 'p' && buffer[2] == 'i' && buffer[3] == 'c' &&
		   buffer[4] == '/' && buffer[5] == 'l' && buffer[6] == 'e' && buffer[7] == 'd' &&
		   buffer[8] == 0   && buffer[9] == 0   && buffer[10] == 0  && buffer[11] == 0)
		{
			if(buffer[12] == ',' && buffer[13] == 'i' && buffer[14] == 'i' && buffer[15] == 0)
			{
				if(buffer[16] == 0 && buffer[17] == 0 && buffer[18] == 0 && buffer[19] == 0)
				{
                    if(buffer[20] == 0 && buffer[21] == 0 && buffer[22] == 0 && buffer[23] == 1)
                    {
                        LED_1_On();
                    }
                    else if(buffer[20] == 0 && buffer[21] == 0 && buffer[22] == 0 && buffer[23] == 0)
                    {
                        LED_1_Off();
                    }
				}
				else if(buffer[16] == 0 && buffer[17] == 0 && buffer[18] == 0 && buffer[19] == 1)
				{
                    if(buffer[20] == 0 && buffer[21] == 0 && buffer[22] == 0 && buffer[23] == 1)
                    {
                        LED_2_On();
                    }
                    else if(buffer[20] == 0 && buffer[21] == 0 && buffer[22] == 0 && buffer[23] == 0)
                    {
                        LED_2_Off();
                    }
				}
			}
		}
	}
}

void UDPSendTask()
{
    BYTE i;

	if(!initSendFlag)
	{
        TxSocket = UDPOpenEx((remoteIP[0] | (remoteIP[1] << 8) | (remoteIP[2] << 16) | (remoteIP[3] << 24)), UDP_OPEN_IP_ADDRESS, 0, remotePort);
		if(TxSocket == INVALID_UDP_SOCKET)
			return;
		initSendFlag = TRUE;
	}

	if(initSendFlag && UDPIsPutReady(TxSocket))
	{
		if(stateFlag)
		{
			stateFlag = FALSE;
            UDPDiscard();
            UDPPutString((BYTE *)prefix);
            UDPPutString((BYTE *)msgSw);
            UDPPutArray(zero, 1);
            UDPPut(',');
            UDPPut('i');
            UDPPutArray(zero, 2);
            UDPPut(0);
            UDPPut(0);
            UDPPut(0);
            UDPPut(currentState ? 0 : 1);
            //test UDPPutArray(zero, 4);
            UDPFlush();
		}

        for(i = 0; i < 2; i++)
        {
            if(initReceiveFlag && oscSendFlag[i])
            {
                UDPDiscard();
                UDPPutString((BYTE *)prefix);
                UDPPutString((BYTE *)msgAdc);
                UDPPutArray(zero, 4);
                UDPPut(',');
                UDPPut('i');
                UDPPut('i');
                UDPPutArray(zero, 1);
                UDPPut(0);
                UDPPut(0);
                UDPPut(0);
                UDPPut(i);
                UDPPut(0);
                UDPPut(0);
                UDPPut((BYTE)((getAnalogWord(i, i) >> 8) & 0x00FF));
                UDPPut((BYTE)(getAnalogWord(i, i) & 0x00FF));
                UDPPutArray(zero, 4);
                UDPFlush();

                oscSendFlag[i] = FALSE;
            }
        }
	}
}

void USBControlTask()
{
    static BYTE usbState = 0;
    switch(usbState)
    {
        case 0:
            HIDControlTask();
            usbState++;
            break;
        case 1:
            while(BusyADC10());
            usbState++;
        case 2:
            analogInHandle(0, (DWORD)((ADC1BUF0 + ADC1BUF2) / 2));
            usbState++;
            break;
        case 3:
            analogInHandle(1, (DWORD)((ADC1BUF1 + ADC1BUF3) / 2));
            usbState++;
            break;
        case 4:
            sendNote();
            sendControlChange();
            receiveMIDIDatas();
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
                    UDPClose(TxSocket);
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
                    UDPClose(RxSocket);
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
                    UDPClose(TxSocket);
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

                    NVMWriteRow((void *)NVM_PROGRAM_PAGE, (void *)u8Data);
                }
                else
                {
                    memcpy(u8Data, (void *)NVM_PROGRAM_PAGE, 8);

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

    btnCurrent[0] = BTN_PAD_00 | (BTN_PAD_01 << 1);
    btnCurrent[1] = BTN_PAD_10 | (BTN_PAD_11 << 1);

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
    BYTE i;

    for(i = 0; i < 2; i++)
    {
        if(getAnalogFlag(i))
        {
            midiData.Val = 0;
            midiData.CableNumber = 0;
            midiData.CodeIndexNumber = MIDI_CIN_CONTROL_CHANGE;
            midiData.DATA_0 = 0xB0;
            midiData.DATA_1 = i;
            midiData.DATA_2 = getAnalogByte(i, i);
            if(!USBHandleBusy(MIDITxHandle))
            {
                MIDITxHandle = USBTxOnePacket(MIDI_EP, (BYTE*)&midiData, 4);
                resetAnalogFlag(i);
                oscSendFlag[i] = TRUE;
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

            switch(num)
            {
                case 0:
                    BTN_LED_00((val > 0)?1:0);
                    break;
                case 1:
                    BTN_LED_01((val > 0)?1:0);
                    break;
                case 2:
                    BTN_LED_10((val > 0)?1:0);
                    break;
                case 3:
                    BTN_LED_11((val > 0)?1:0);
                    break;
            }
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

            if(num == 1)
            {
                if(val < 1)
                {
                    LATBbits.LATB2  = 0;
                    LATBbits.LATB3  = 0;
                    LATBbits.LATB4  = 0;
                    LATBbits.LATB5  = 0;
                    LATBbits.LATB6  = 0;
                    LATBbits.LATB7  = 0;
                    LATBbits.LATB8  = 0;
                    LATBbits.LATB9  = 0;
                    LATBbits.LATB10 = 0;
                    LATBbits.LATB11 = 0;
                    LATBbits.LATB12 = 0;
                }
                else if(val < 4)
                {
                    LATBbits.LATB2  = 0;
                    LATBbits.LATB3  = 0;
                    LATBbits.LATB4  = 0;
                    LATBbits.LATB5  = 0;
                    LATBbits.LATB6  = 0;
                    LATBbits.LATB7  = 0;
                    LATBbits.LATB8  = 0;
                    LATBbits.LATB9  = 0;
                    LATBbits.LATB10 = 0;
                    LATBbits.LATB11 = 0;
                    LATBbits.LATB12 = 1;
                }
                else if(val < 16)
                {
                    LATBbits.LATB2  = 0;
                    LATBbits.LATB3  = 0;
                    LATBbits.LATB4  = 0;
                    LATBbits.LATB5  = 0;
                    LATBbits.LATB6  = 0;
                    LATBbits.LATB7  = 0;
                    LATBbits.LATB8  = 0;
                    LATBbits.LATB9  = 0;
                    LATBbits.LATB10 = 0;
                    LATBbits.LATB11 = 1;
                    LATBbits.LATB12 = 1;
                }
                else if(val < 28)
                {
                    LATBbits.LATB2  = 0;
                    LATBbits.LATB3  = 0;
                    LATBbits.LATB4  = 0;
                    LATBbits.LATB5  = 0;
                    LATBbits.LATB6  = 0;
                    LATBbits.LATB7  = 0;
                    LATBbits.LATB8  = 0;
                    LATBbits.LATB9  = 0;
                    LATBbits.LATB10 = 1;
                    LATBbits.LATB11 = 1;
                    LATBbits.LATB12 = 1;
                }
                else if(val < 40)
                {
                    LATBbits.LATB2  = 0;
                    LATBbits.LATB3  = 0;
                    LATBbits.LATB4  = 0;
                    LATBbits.LATB5  = 0;
                    LATBbits.LATB6  = 0;
                    LATBbits.LATB7  = 0;
                    LATBbits.LATB8  = 0;
                    LATBbits.LATB9  = 1;
                    LATBbits.LATB10 = 1;
                    LATBbits.LATB11 = 1;
                    LATBbits.LATB12 = 1;
                }
                else if(val < 52)
                {
                    LATBbits.LATB2  = 0;
                    LATBbits.LATB3  = 0;
                    LATBbits.LATB4  = 0;
                    LATBbits.LATB5  = 0;
                    LATBbits.LATB6  = 0;
                    LATBbits.LATB7  = 0;
                    LATBbits.LATB8  = 1;
                    LATBbits.LATB9  = 1;
                    LATBbits.LATB10 = 1;
                    LATBbits.LATB11 = 1;
                    LATBbits.LATB12 = 1;
                }
                else if(val < 64)
                {
                    LATBbits.LATB2  = 0;
                    LATBbits.LATB3  = 0;
                    LATBbits.LATB4  = 0;
                    LATBbits.LATB5  = 0;
                    LATBbits.LATB6  = 0;
                    LATBbits.LATB7  = 1;
                    LATBbits.LATB8  = 1;
                    LATBbits.LATB9  = 1;
                    LATBbits.LATB10 = 1;
                    LATBbits.LATB11 = 1;
                    LATBbits.LATB12 = 1;
                }
                else if(val < 76)
                {
                    LATBbits.LATB2  = 0;
                    LATBbits.LATB3  = 0;
                    LATBbits.LATB4  = 0;
                    LATBbits.LATB5  = 0;
                    LATBbits.LATB6  = 1;
                    LATBbits.LATB7  = 1;
                    LATBbits.LATB8  = 1;
                    LATBbits.LATB9  = 1;
                    LATBbits.LATB10 = 1;
                    LATBbits.LATB11 = 1;
                    LATBbits.LATB12 = 1;
                }
                else if(val < 88)
                {
                    LATBbits.LATB2  = 0;
                    LATBbits.LATB3  = 0;
                    LATBbits.LATB4  = 0;
                    LATBbits.LATB5  = 0;
                    LATBbits.LATB6  = 1;
                    LATBbits.LATB7  = 1;
                    LATBbits.LATB8  = 1;
                    LATBbits.LATB9  = 1;
                    LATBbits.LATB10 = 1;
                    LATBbits.LATB11 = 1;
                    LATBbits.LATB12 = 1;
                }
                else if(val < 100)
                {
                    LATBbits.LATB2  = 0;
                    LATBbits.LATB3  = 0;
                    LATBbits.LATB4  = 0;
                    LATBbits.LATB5  = 1;
                    LATBbits.LATB6  = 1;
                    LATBbits.LATB7  = 1;
                    LATBbits.LATB8  = 1;
                    LATBbits.LATB9  = 1;
                    LATBbits.LATB10 = 1;
                    LATBbits.LATB11 = 1;
                    LATBbits.LATB12 = 1;
                }
                else if(val < 112)
                {
                    LATBbits.LATB2  = 0;
                    LATBbits.LATB3  = 0;
                    LATBbits.LATB4  = 1;
                    LATBbits.LATB5  = 1;
                    LATBbits.LATB6  = 1;
                    LATBbits.LATB7  = 1;
                    LATBbits.LATB8  = 1;
                    LATBbits.LATB9  = 1;
                    LATBbits.LATB10 = 1;
                    LATBbits.LATB11 = 1;
                    LATBbits.LATB12 = 1;
                }
                else if(val < 124)
                {
                    LATBbits.LATB2  = 0;
                    LATBbits.LATB3  = 1;
                    LATBbits.LATB4  = 1;
                    LATBbits.LATB5  = 1;
                    LATBbits.LATB6  = 1;
                    LATBbits.LATB7  = 1;
                    LATBbits.LATB8  = 1;
                    LATBbits.LATB9  = 1;
                    LATBbits.LATB10 = 1;
                    LATBbits.LATB11 = 1;
                    LATBbits.LATB12 = 1;
                }
                else if(val < 128)
                {
                    LATBbits.LATB2  = 1;
                    LATBbits.LATB3  = 1;
                    LATBbits.LATB4  = 1;
                    LATBbits.LATB5  = 1;
                    LATBbits.LATB6  = 1;
                    LATBbits.LATB7  = 1;
                    LATBbits.LATB8  = 1;
                    LATBbits.LATB9  = 1;
                    LATBbits.LATB10 = 1;
                    LATBbits.LATB11 = 1;
                    LATBbits.LATB12 = 1;
                }
            }
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
