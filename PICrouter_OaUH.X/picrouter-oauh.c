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
 * picrouter-oauh.c,v.0.3 2012/08/23
 */

#include "picrouter-oauh.h"

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
	int value;
    static DWORD t = 0;

    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

    setupPitch();
#if 1 //A/D Auto Scan 
#ifdef PITCH
    AD1PCFG = 0x0000FFFC;// 0000 0000 0000 0000 1111 1111 1111 1100
    AD1CON2 = 0x0000041C;// 0000 0000 0000 0000 0000 0100 0001 1100
    AD1CSSL = 0x00000003;// 0000 0000 0000 0000 0000 0000 0000 0011
#endif
#ifdef OPT_DRUM
    AD1PCFG = 0x0000FC00;// 0000 0000 0000 0000 1111 1100 0000 0000
    AD1CON2 = 0x0000043C;// 0000 0000 0000 0000 0000 0100 0010 0100
    AD1CSSL = 0x000003FF;// 0000 0000 0000 0000 0000 0011 1111 1111
#endif
    AD1CON1 = 0x000010E6;// 0000 0000 0000 0000 1000 0000 0110 0110
    AD1CHS  = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
    AD1CON3 = 0x00001FFF;// 0000 0000 0000 0000 0000 1111 0000 1000
    AD1CON1bits.ON = 1;
#else // Manual A/D Conv.
    #define CONFIG_1    ADC_MODULE_OFF | ADC_FORMAT_INTG16 | ADC_CLK_MANUAL | ADC_AUTO_SAMPLING_ON | ADC_SAMP_ON
    #define CONFIG_2    ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF | ADC_SAMPLES_PER_INT_2 | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_ON
    #define CONFIG_3    ADC_SAMPLE_TIME_31 | ADC_CONV_CLK_SYSTEM | ADC_CONV_CLK_32Tcy
    #define CONFIG_PORT ENABLE_AN0_ANA | ENABLE_AN1_ANA
    #define CONFIG_SKIP SKIP_SCAN_ALL
    #define CONFIG_MULT ADC_CH0_POS_SAMPLEA_AN0 | ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN1 | ADC_CH0_NEG_SAMPLEA_NVREF
    OpenADC10(CONFIG_1, CONFIG_2, CONFIG_3, CONFIG_PORT, CONFIG_SKIP);
    SetChanADC10(CONFIG_MULT);
    EnableADC10();
#endif

    initSW();
    initLEDs();
    buttonInit();

    stateFlag = FALSE;
    prevState = 1;
    currentState = 1;

    LED_1_On();
    LED_2_On();

    //value = SYSTEMConfigWaitStatesAndPB( GetSystemClock() );

    // Enable the cache for the best performance
    CheKseg0CacheOn();

    // Enable optimal performance
    SYSTEMConfigPerformance(GetSystemClock());
    mOSCSetPBDIV(OSC_PB_DIV_1);				// Use 1:1 CPU Core:Peripheral clocks

    //test INTEnableInterrupts();

    prefix = "/pic";
    hostName = (char *)calloc(strlen(DEFAULT_HOST_NAME), sizeof(char));
    memcpy(hostName, DEFAULT_HOST_NAME, strlen(DEFAULT_HOST_NAME));
    TickInit();
    InitAppConfig();
    StackInit();
    ZeroconfLLInitialize();
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

    //USB Initialization
    #if defined(USE_USB_BUS_SENSE_IO)
    	tris_usb_bus_sense = 1;
    #endif

    #if defined(USE_SELF_POWER_SENSE_IO)
    	tris_self_power = 1;
    #endif

    USBHostInit(0);

    LED_1_Off();
    LED_2_Off();

    while(1)
    {
#if 1
        if(TickGet() - t >= TICK_SECOND / 2ul)
        {
            t = TickGet();
            //LED_1_Toggle();
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
        ZeroconfLLProcess();
        mDNSProcess();

#ifdef PITCH
        //if(usbState == 5)
#endif
        {
            UDPControlTask();
        }
        UDPSendTask();

        USBHostTasks();
        convertMidiToOsc();

        prevState = currentState;
    }

    return (EXIT_SUCCESS);
}

void setupPitch(void)
{
#ifdef PITCH
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
#endif

#if OPT_DRUM
    AD_PORT0_IO(1);
    AD_PORT1_IO(1);
    AD_PORT2_IO(1);
    AD_PORT3_IO(1);
    AD_PORT4_IO(1);
    AD_PORT5_IO(1);
    AD_PORT6_IO(1);
    AD_PORT7_IO(1);
    AD_PORT8_IO(1);
    AD_PORT9_IO(1);
    AD_PORT10_IO(0);
    AD_PORT11_IO(0);
    AD_PORT12_IO(0);
    AD_PORT13_IO(0);

    AD_PORT10_OUT(0);
    AD_PORT11_OUT(0);
    AD_PORT12_OUT(0);
    AD_PORT13_OUT(0);
#endif

    TRISFbits.TRISF5  = 1;
    TRISGbits.TRISG7  = 1;
    TRISFbits.TRISF0  = 1;
    TRISBbits.TRISB14 = 1;

    TRISGbits.TRISG6 = 0;
    TRISGbits.TRISG8 = 0;
    TRISFbits.TRISF1 = 0;
    TRISFbits.TRISF4 = 0;

    BTN_LED_00(0);
    BTN_LED_01(0);
    BTN_LED_10(0);
    BTN_LED_11(0);
}

/**********************************************
*  UDP Generic I/O Processing Part
**********************************************/
void UDPControlTask(void)
{
	unsigned int RcvLen;
	static BYTE	buffer[128];

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
        if(isEqualToAddress(buffer, "/pic/pad/led"))
        {
#if 0
            if(getIntArgumentAtIndex(buffer, "/pic/led", 0) == 0)
            {
                if(getIntArgumentAtIndex(buffer, "/pic/led", 1) == 1)
                {
                    LED_1_On();
                }
                else if(getIntArgumentAtIndex(buffer, "/pic/led", 1) == 0)
                {
                    LED_1_Off();
                }
            }
            else if(getIntArgumentAtIndex(buffer, "/pic/led", 0) == 1)
            {
                if(getIntArgumentAtIndex(buffer, "/pic/led", 1) == 1)
                {
                    LED_2_On();
                }
                else if(getIntArgumentAtIndex(buffer, "/pic/led", 1) == 0)
                {
                    LED_2_Off();
                }
            }
#else
            BYTE x = getIntArgumentAtIndex(buffer, "/pic/pad/led", 0);
            BYTE y = getIntArgumentAtIndex(buffer, "/pic/pad/led", 1);
            BYTE state = getIntArgumentAtIndex(buffer, "/pic/pad/led", 2);
            if(x == 0 && y == 0)
            {
                BTN_LED_00((state > 0)?1:0);
            }
            else if(x == 1 && y == 0)
            {
                BTN_LED_01((state > 0)?1:0);
            }
            else if(x == 0 && y == 1)
            {
                BTN_LED_10((state > 0)?1:0);
            }
            else if(x == 1 && y == 1)
            {
                BTN_LED_11((state > 0)?1:0);
            }
#endif
        }
#ifdef PITCH
        else if(isEqualToAddress(buffer, "/pic/vol/led"))
        {
            BYTE type = getIntArgumentAtIndex(buffer, "/pic/vol/led", 0);
            BYTE val = getIntArgumentAtIndex(buffer, "/pic/vol/led", 1);
            if(type == 1)
            {
                //if(val < 1)
                if(val < 2)
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
                //else if(val < 4)
                else if(val < 8)
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
                //else if(val < 16)
                else if(val < 32)
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
                //else if(val < 28)
                else if(val < 56)
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
                //else if(val < 40)
                else if(val < 80)
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
                //else if(val < 52)
                else if(val < 104)
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
                //else if(val < 64)
                else if(val < 128)
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
                //else if(val < 76)
                else if(val < 152)
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
                //else if(val < 88)
                else if(val < 176)
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
                //else if(val < 100)
                else if(val < 200)
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
                //else if(val < 112)
                else if(val < 224)
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
                //else if(val < 124)
                else if(val < 248)
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
                //else if(val < 128)
                else if(val < 256)
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
        }
#endif
#ifdef OPT_DRUM
        else if(isEqualToAddress(buffer, "/pic/opt"))
        {
            LATBbits.LATB10 = getIntArgumentAtIndex(buffer, "/pic/opt", 0);
        }
#endif
        else if(isEqualToAddress(buffer, "/sys/remote/ip/set"))
        {
            char* rip = (char *)calloc(15, sizeof(char));
            rip = getStringArgumentAtIndex(buffer, "/sys/remote/ip/set", 0);
            remoteIP[0] = atoi(strtok(rip, "."));
            remoteIP[1] = atoi(strtok(NULL, "."));
            remoteIP[2] = atoi(strtok(NULL, "."));
            remoteIP[3] = atoi(strtok(NULL, "."));
            initSendFlag = FALSE;
            UDPClose(TxSocket);
            free(rip);
        }
        else if(isEqualToAddress(buffer, "/sys/remote/ip/get"))
        {
            char* rip = (char *)calloc(15, sizeof(char));;
            sprintf(rip, "%d.%d.%d.%d", remoteIP[0], remoteIP[1], remoteIP[2], remoteIP[3]);
            sendOSCMessage(TxSocket, sysPrefix, msgRemoteIp, "s", rip);
            free(rip);
        }
        else if(isEqualToAddress(buffer, "/sys/host/name/set"))
        {
            mDNSServiceDeRegister();
            free(hostName);
            char* hn = getStringArgumentAtIndex(buffer, "/sys/host/name/set", 0);
            hostName = (char *)calloc(strlen(hn), sizeof(char));
            memcpy(hostName, hn, strlen(hn));

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
        else if(isEqualToAddress(buffer, "/sys/host/name/get"))
        {
            sendOSCMessage(TxSocket, sysPrefix, msgHostName, "s", hostName);
        }
        else if(isEqualToAddress(buffer, "/sys/host/ip/get"))
        {
            char* hip = (char *)calloc(15, sizeof(char));
            BYTE hip0 = AppConfig.MyIPAddr.Val & 0xFF;
            BYTE hip1 = (AppConfig.MyIPAddr.Val >> 8) & 0xFF;
            BYTE hip2 = (AppConfig.MyIPAddr.Val >> 16) & 0xFF;
            BYTE hip3 = (AppConfig.MyIPAddr.Val >> 24) & 0xFF;
            sprintf(hip, "%d.%d.%d.%d", hip0, hip1, hip2, hip3);
            sendOSCMessage(TxSocket, sysPrefix, msgHostIp, "s", hip);
            free(hip);
        }
        else if(isEqualToAddress(buffer, "/sys/host/mac/get"))
        {
            char* macaddr = (char *)calloc(17, sizeof(char));
            sprintf(macaddr, "%X:%X:%X:%X:%X:%X", AppConfig.MyMACAddr.v[0], AppConfig.MyMACAddr.v[1], AppConfig.MyMACAddr.v[2], AppConfig.MyMACAddr.v[3], AppConfig.MyMACAddr.v[4], AppConfig.MyMACAddr.v[5]);
            sendOSCMessage(TxSocket, sysPrefix, msgHostMac, "s", macaddr);
            free(macaddr);
        }
        else if(isEqualToAddress(buffer, "/sys/prefix/set"))
        {
            prefix = getStringArgumentAtIndex(buffer, "/sys/prefx/set", 0);
        }
        else if(isEqualToAddress(buffer, "/sys/prefix/get"))
        {
            sendOSCMessage(TxSocket, sysPrefix, msgPrefix, "s", prefix);
        }
    } 
}

void sendPad(void)
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
                sendOSCMessage(TxSocket, prefix, msgPress, "iii", i, j, (btnCurrent[i] & (1 << j)) ? 1 : 0);
            delayUs(5);
        }
    }
}

void sendAdc(void)
{
    BYTE i;

    for(i = 0; i < USE_ADC_NUM; i++)
    {
        if(getAnalogFlag(i))
        {
#ifdef PITCH
            //currentValue[i] = getAnalogByte(i, 1);
            if(i == 0)
                currentValue[i] = getAnalogByte(i, TYPE_MIDI_ORIGINAL);
            if(i == 1)
                currentValue[i] = getAnalogByte(i, TYPE_MIDI_VOLUME);
            //currentValue[i] = getAnalogWord(i, TYPE_LONG_ORIGINAL);
#endif
#ifdef OPT_DRUM
            currentValue[i] = getAnalogByte(i, 0);
#endif
            //if(currentValue[i] != prevValue[0][i] && currentValue[i] != prevValue[1][i])
            {
                sendOSCMessage(TxSocket, prefix, msgAdc, "ii", i, currentValue[i]);
                //sendOSCMessage(TxSocket, prefix, msgAdc, "iiiiiiiii", i, getAnalogWord(i, 3), getAnalogWord(i, 4), getAnalogWord(i, 5), getAnalogWord(i, 6), getAnalogWord(i, 7), getAnalogWord(i, 8), getAnalogWord(i, 9), getAnalogWord(i, 10));
                //prevValue[1][i] = prevValue[0][i];
                //prevValue[0][i] = currentValue[i];
                resetAnalogFlag(i);
                delayUs(20);
            }
        }
    }
}

void UDPSendTask()
{
    if(!initSendFlag)
    {
        TxSocket = UDPOpenEx((remoteIP[0] | (remoteIP[1] << 8) | (remoteIP[2] << 16) | (remoteIP[3] << 24)), UDP_OPEN_IP_ADDRESS, 0, remotePort);
        if(TxSocket == INVALID_UDP_SOCKET)
			return;
		initSendFlag = TRUE;
    }

    if(initSendFlag)
	{
        if(stateFlag)
        {
            stateFlag = FALSE;
            //sendOSCMessage(TxSocket, prefix, msgSw, "i", (currentState ? 0 : 1));
            if(!currentState)
            {
                sendOSCMessage(TxSocket, sysPrefix, msgPrefix, "is", -2, prefix);
            }
        }

        switch(usbState)
        {
            case 0:
                //test ConvertADC10();
                while(BusyADC10());
                //test analogInHandle(0, (LONG)ReadADC10(1));
                //test analogInHandle(1, (LONG)ReadADC10(0));
                analogInHandle(0, (LONG)((ADC1BUF0 + ADC1BUF2 + ADC1BUF4 + ADC1BUF6) / 4));
                analogInHandle(1, (LONG)((ADC1BUF1 + ADC1BUF3 + ADC1BUF5 + ADC1BUF7) / 4));
                usbState = 1;
                break;
#ifdef PITCH
            case 1:
                sendAdc();
                usbState = 2;
                break;
            case 2:
                sendPad();
                usbState = 0;
                break;
#endif
#ifdef OPT_DRUM
            case 1:
                analogInHandle(0, (LONG)(ADC1BUF0));
                usbState = 2;
                break;
            case 2:
                analogInHandle(1, (LONG)(ADC1BUF1));
                usbState = 3;
                break;
            case 3:
                analogInHandle(2, (LONG)(ADC1BUF2));
                usbState = 4;
                break;
            case 4:
                analogInHandle(3, (LONG)(ADC1BUF3));
                usbState = 5;
                break;
            case 5:
                analogInHandle(4, (LONG)(ADC1BUF4));
                usbState = 6;
                break;
            case 6:
                analogInHandle(5, (LONG)(ADC1BUF5));
                usbState = 7;
                break;
            case 7:
                analogInHandle(6, (LONG)(ADC1BUF6));
                usbState = 8;
                break;
            case 8:
                analogInHandle(7, (LONG)(ADC1BUF7));
                usbState = 9;
                break;
            case 9:
                analogInHandle(8, (LONG)(ADC1BUF8));
                usbState = 10;
                break;
            case 10:
                analogInHandle(9, (LONG)(ADC1BUF9));
                usbState = 11;
                break;
            case 11:
                sendAdc();
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
       			if(USBHostMIDIRead(deviceHandle, currentEndpoint, endpointBuffers[currentEndpoint].pBufWriteLocation, endpointBuffers[currentEndpoint].numOfMIDIPackets * sizeof(USB_AUDIO_MIDI_PACKET)) == USB_SUCCESS)
        		{
                    if(initSendFlag)
                    {
                        BYTE type = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_0 & 0xF0;
                        BYTE ch = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_0 & 0x0F;
                        BYTE num = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_1;
                        BYTE val = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_2;

                        if(type != midiType || num != midiNum || val != midiVal)
                        {
                            switch(type)
                            {
                                case 0x80:// Note off
                                case 0x90:// Note on
                                    sendOSCMessage(TxSocket, msgMidi, msgNote, "iii", ch, num, val);
                                    break;
                                case 0xA0:// Key Pressure
                                    sendOSCMessage(TxSocket, msgMidi, msgKp, "iii", ch, num, val);
                                    break;
                                case 0xB0:// Control Change
                                    sendOSCMessage(TxSocket, msgMidi, msgCc, "iii", ch, num, val);
                                    break;
                                case 0xC0:// Program Change
                                    //sendOSCMessage(TxSocket, msgMidi, msgPc, "iii", ch, num, val);
                                    sendOSCMessage(TxSocket, msgMidi, msgPc, "i", num);
                                    break;
                                case 0xD0:// Channel Pressure
                                    //sendOSCMessage(TxSocket, msgMidi, msgCp, "iii", ch, num, val);
                                    sendOSCMessage(TxSocket, msgMidi, msgCp, "ii", ch, num);
                                    break;
                                case 0xE0:// Pitch Bend
                                    sendOSCMessage(TxSocket, msgMidi, msgPb, "iii", ch, num, val);
                                    break;
                                default:
                                    break;
                            }
                        }

                        midiType = type;
                        midiNum = num;
                        midiVal = val;
                    }
#if 0//test
           			USBHostMIDIWrite(deviceHandle, currentEndpoint, &TxHostMidiDataBuffer, sizeof(RxHostMidiDataBuffer));
#endif//test
        		}
#if 1
        		if(!USBHostMIDITransferIsBusy(deviceHandle, currentEndpoint))
                {
                    //endpointBuffers[currentEndpoint].TransferState = RX_DATA;
                            
                    endpointBuffers[currentEndpoint].pBufWriteLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;
                            
                    if(endpointBuffers[currentEndpoint].pBufWriteLocation - endpointBuffers[currentEndpoint].bufferStart >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                    {
                        endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
                    }
                }
#endif
        		//UARTRealTimeToUSB.Val = UARTRealTimeToUSBBuffer.Val;
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
