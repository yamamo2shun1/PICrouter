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
 * picrouter-oauh.c,v.0.73 2012/09/24
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
	int value, i;
    static DWORD t = 0;

    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

#if 1 //A/D Auto Scan 
  #ifdef USE_PITCH
    setupPitch();
    AD1PCFG = 0x0000FFFC;// 0000 0000 0000 0000 1111 1111 1111 1100
    AD1CON2 = 0x0000041C;// 0000 0000 0000 0000 0000 0100 0001 1100
    AD1CSSL = 0x00000003;// 0000 0000 0000 0000 0000 0000 0000 0011
  #endif
  #ifdef USE_OPT_DRUM
    setupOptDrum();
    AD1PCFG = 0x0000FC00;// 0000 0000 0000 0000 1111 1100 0000 0000
    AD1CON2 = 0x0000043C;// 0000 0000 0000 0000 0000 0100 0010 0100
    AD1CSSL = 0x000003FF;// 0000 0000 0000 0000 0000 0011 1111 1111
  #endif
  #ifdef USE_LED_ENC
    setupLedEnc();
    AD1PCFG = 0x0000FFFF;// 0000 0000 0000 0000 1111 1111 1111 1111
    AD1CON2 = 0x0000043C;// 0000 0000 0000 0000 0000 0100 0010 0100
    AD1CSSL = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
  #endif
    AD1CON1 = 0x000010E6;// 0000 0000 0000 0000 1000 0000 0110 0110
    AD1CHS  = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
    AD1CON3 = 0x00001F08;// 0000 0000 0000 0000 0000 1111 0000 1000
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

//#ifdef USE_LED_PAD_16
#ifdef USE_LED_ENC
    SpiChnOpen(2, SPICON_MSTEN | SPICON_SMP | SPICON_MODE16 | SPICON_ON, 4);
    //test OpenSPI2(ENABLE_SDO_PIN | SPI_MODE16_ON | SPI_SMP_OFF | SPI_CKE_ON | MASTER_ENABLE_ON | CLK_POL_ACTIVE_HIGH | SEC_PRESCAL_2_1 | PRI_PRESCAL_1_1, SPI_ENABLE | SPI_IDLE_CON);
#endif

    initSW();
    initLEDs();
    buttonInit();

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

    //PWM
    for(i = 0; i < PWM_NUM; i++)
    {
        freq[i] = 10000; // 10kHz
        width[i] = GetSystemClock() / freq[i];
        duty[i] = 50;
    }

    prefix = (char *)calloc(strlen("/pic"), sizeof(char));
    memcpy(prefix, "/pic", strlen("/pic"));
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

#ifdef USE_LED_ENC
    for(i = 0; i < 32; i++)
    {
        intensity[i] = ledIntensity;
    }

    LD_LOAD(0);
    putcSPI2(0x0000);
    putcSPI2(0x0000);
    delayUs(1);
    LD_LOAD(1);
#endif

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

#ifdef USE_LED_ENC
        reA[1] = RE_A();
        reB[1] = RE_B();

        if((reA[1] != reA[0]) || (reB[1] != reB[0]))
        {
            reD = (((reA[0] << 1) + reB[0]) << 1) + ((reA[1] << 1) + reB[1]);
            reA[0] = reA[1];
            reB[0] = reB[1];

            if(initSendFlag)
                sendOSCMessage(TxSocket, prefix, msgRotaryEnc, "iii", ((reD & 0x02) >> 1) ? 1 : 0, reStep, encVelocityCount);
            if((reD & 0x02) >> 1)
            {
                reStep--;
                if(reStep < 0)
                    reStep = 255;
            }
            else
            {
                reStep++;
                if(reStep > 255)
                    reStep = 0;
            }
            encVelocityCount = 0;
        }
        else
        {
            if(encVelocityCount < 100000)
                encVelocityCount++;
        }
#endif

        StackTask();
        NBNSTask();
        ZeroconfLLProcess();
        mDNSProcess();

#ifdef USE_PITCH
        //if(usbState == 5)
#endif
        {
            UDPControlTask();
        }
        UDPSendTask();

        USBHostTasks();
        convertMidiToOsc();

#ifdef USE_LED_ENC
        if(ledOn)
        {
            WORD msb, lsb;
            msb = (WORD)((dwLedSequence[ledIntensityIndex] >> 16) & 0x0000FFFF);
            lsb = (WORD)(dwLedSequence[ledIntensityIndex] & 0x0000FFFF);
            LD_LOAD(0);
            putcSPI2(msb);
            putcSPI2(lsb);
            delayUs(8);
            LD_LOAD(1);
            ledIntensityIndex++;
            if(ledIntensityIndex == 100)
                ledIntensityIndex = 0;
        }
#endif
        prevState = currentState;
    }

    return (EXIT_SUCCESS);
}

#ifdef USE_PITCH
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

    AD_PORT2_OUT(0);
    AD_PORT3_OUT(0);
    AD_PORT4_OUT(0);
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

    BTN_LED_00(0);
    BTN_LED_01(0);
    BTN_LED_10(0);
    BTN_LED_11(0);
}
#endif

#ifdef USE_OPT_DRUM
void setupOptDrum(void)
{
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
}
#endif

#ifdef USE_LED_PAD_16
void setupLedPad16(void)
{
    AD_PORT0_IO(0);
    AD_PORT1_IO(0);
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
    AD_PORT10_OUT(0);
    AD_PORT11_OUT(0);
    AD_PORT12_OUT(0);
    AD_PORT13_OUT(0);

    D_PORT0_IO(0);
    D_PORT1_IO(1);
    D_PORT2_IO(1);
    D_PORT3_IO(1);

}
#endif

#ifdef USE_LED_ENC
void setupLedEnc(void)
{
    AD_PORT0_IO(0);
    AD_PORT1_IO(0);
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
    AD_PORT10_OUT(0);
    AD_PORT11_OUT(0);
    AD_PORT12_OUT(0);
    AD_PORT13_OUT(0);

    D_PORT0_IO(0);
    D_PORT1_IO(1);
    D_PORT2_IO(1);
    D_PORT3_IO(1);
}
#endif

/**********************************************
*  UDP Generic I/O Processing Part
**********************************************/
void UDPControlTask(void)
{
	unsigned int RcvLen;
	static BYTE	buffer[128];
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
        RcvLen = UDPGetArray(buffer, sizeof(buffer));
        UDPDiscard();
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
#ifdef USE_PITCH
        else if(isEqualToAddress(buffer, prefix, msgVolumeLed))
        {
            BYTE type = getIntArgumentAtIndex(buffer, prefix, msgVolumeLed, 0);
            BYTE val = getIntArgumentAtIndex(buffer, prefix, msgVolumeLed, 1);
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
        else if(isEqualToAddress(buffer, prefix, msgLatticeLed))
        {
            BYTE x = getIntArgumentAtIndex(buffer, prefix, msgLatticeLed, 0);
            BYTE y = getIntArgumentAtIndex(buffer, prefix, msgLatticeLed, 1);
            BYTE state = getIntArgumentAtIndex(buffer, prefix, msgLatticeLed, 2);

#ifdef USE_PITCH
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

#ifdef USE_LED_PAD_16
            if(state)
                ledState |= (1 << y) << (x * 4);
            else
                ledState &= ~((1 << y) << (x * 4));

            LD_LOAD(0);
            //putcSPI2(0x0000);
            putcSPI2(ledState);
            delayUs(2);
            LD_LOAD(1);
#endif
        }
#ifdef USE_LED_ENC
        else if(isEqualToAddress(buffer, prefix, msgRotaryLedStep))
        {
            BYTE pos = getIntArgumentAtIndex(buffer, prefix, msgRotaryLedStep, 0);
            BYTE len = getIntArgumentAtIndex(buffer, prefix, msgRotaryLedStep, 1);
            dwLedData = 0;

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
                LD_LOAD(0);
                putcSPI2(0);
                putcSPI2(0);
                delayUs(4);
                LD_LOAD(1);
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
                LD_LOAD(0);
                putcSPI2(0);
                putcSPI2(0);
                delayUs(4);
                LD_LOAD(1);
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
                intensity[j] = dwld;
                dwLedSequence[j] = dwLedData;
                for(k = 0; k < 32; k++)
                {
                    if(j >= intensity[k])
                        dwLedSequence[j] &= ~(1 << k);
                }
            }
        }
#endif
#ifdef USE_OPT_DRUM
        else if(isEqualToAddress(buffer, prefix, "/opt"))
        {
            LATBbits.LATB10 = getIntArgumentAtIndex(buffer, prefix, "/opt", 0);
        }
#endif
        else if(isEqualToAddress(buffer, stdPrefix, msgSetPwmState))
        {
            index = getIntArgumentAtIndex(buffer, prefix, msgSetPwmState, 0);
            if(strcmp(getStringArgumentAtIndex(buffer, prefix, msgSetPwmState, 1), "on") == 0)
            {
                LONG w = (LONG)(((float)duty[index] / 100.0) * (float)width[index]);
                if(!onTimer23)
                {
                    OpenTimer23(T23_ON | T23_SOURCE_INT | T23_PS_1_1, width[index]);
                    onTimer23 = TRUE;
                }
                switch(index)
                {
                    case 0:
                        OpenOC1(OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width[index] / 2, w);
                        break;
                    case 1:
                        OpenOC3(OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, width[index] / 2, w);
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                }
                onSquare[index] = TRUE;
            }
            else if(strcmp(getStringArgumentAtIndex(buffer, prefix, msgSetPwmState, 1), "off") == 0)
            {
                if(onTimer23)
                {
                    CloseTimer23();
                    onTimer23 = FALSE;
                }
                CloseOC1();
                onSquare[index] = FALSE;
            }
        }
        else if(isEqualToAddress(buffer, stdPrefix, msgGetPwmState))
        {
            index = getIntArgumentAtIndex(buffer, prefix, msgSetPwmState, 0);
            sendOSCMessage(TxSocket, prefix, msgGetPwmState, "is", index, onSquare[index] ? "on" : "off");
        }
        else if(isEqualToAddress(buffer, stdPrefix, msgSetPwmFreq))
        {
            index = getIntArgumentAtIndex(buffer, prefix, msgSetPwmState, 0);
            freq[index] = getIntArgumentAtIndex(buffer, stdPrefix, msgSetPwmFreq, 1);
            if(freq[index] < 20)
            {
                freq[index] = 20;
            }
            else if(freq[index] > 44100)
            {
                freq[index] = 44100;
            }
            T2CONbits.TON = 0;
            TMR2 = 0;
            OC1CONbits.OCM = 0b000;
            width[index] = GetSystemClock() / freq[index];
            PR2 = width[index];
            OC1RS = width[index] / 2;
            OC1CONbits.OCM = 0b110;
            T2CONbits.TON = 1;
        }
        else if(isEqualToAddress(buffer, stdPrefix, msgGetPwmFreq))
        {
            index = getIntArgumentAtIndex(buffer, prefix, msgSetPwmState, 0);
            sendOSCMessage(TxSocket, stdPrefix, msgGetPwmFreq, "ii", index, freq[index]);
        }
        else if(isEqualToAddress(buffer, stdPrefix, msgSetPwmDuty))
        {
            index = getIntArgumentAtIndex(buffer, prefix, msgSetPwmState, 0);
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
            OC1CONbits.OCM = 0b000;
            OC1RS = (LONG)(((float)duty[index] / 100.0) * (float)width[index]);
            OC1CONbits.OCM = 0b110;
            T2CONbits.TON = 1;
        }
        else if(isEqualToAddress(buffer, stdPrefix, msgGetPwmDuty))
        {
            index = getIntArgumentAtIndex(buffer, prefix, msgSetPwmState, 0);
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
            char* rip = (char *)calloc(15, sizeof(char));
            rip = getStringArgumentAtIndex(buffer, sysPrefix, msgSetRemoteIp, 0);
            remoteIP[0] = atoi(strtok(rip, "."));
            remoteIP[1] = atoi(strtok(NULL, "."));
            remoteIP[2] = atoi(strtok(NULL, "."));
            remoteIP[3] = atoi(strtok(NULL, "."));
            initSendFlag = FALSE;
            UDPClose(TxSocket);
            free(rip);
        }
        else if(isEqualToAddress(buffer, sysPrefix, msgGetRemoteIp))
        {
            char* rip = (char *)calloc(15, sizeof(char));;
            sprintf(rip, "%d.%d.%d.%d", remoteIP[0], remoteIP[1], remoteIP[2], remoteIP[3]);
            sendOSCMessage(TxSocket, sysPrefix, msgRemoteIp, "s", rip);
            free(rip);
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
            char* hn = getStringArgumentAtIndex(buffer, sysPrefix, msgSetHostName, 0);
            hostName = (char *)calloc(strlen(hn) + 1, sizeof(char));
            memcpy(hostName, hn, strlen(hn));
            hostName[strlen(hn)] = '\0';

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
            char* prfx = getStringArgumentAtIndex(buffer, sysPrefix, msgSetPrefix, 0);
            prefix = (char *)calloc(strlen(prfx) + 1, sizeof(char));
            memcpy(prefix, prfx, strlen(prfx));
            prefix[strlen(prfx)] = '\0';
        }
        else if(isEqualToAddress(buffer, sysPrefix, msgGetPrefix))
        {
            sendOSCMessage(TxSocket, sysPrefix, msgPrefix, "s", prefix);
        }
    } 
}

void sendPad(void)
{
    BYTE i, j, k, l;
#ifdef USE_LED_PAD_16
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
            }
            delayUs(5);
        }
    }
#else
    for(i = 0; i < MAX_BTN_ROW; i++)
    {
        btnLast[i] = btnCurrent[i];
    }
    btnCurrent[0] = BTN_PAD_00() | (BTN_PAD_01() << 1);
    btnCurrent[1] = BTN_PAD_10() | (BTN_PAD_11() << 1);

    for(i = 0; i < MAX_BTN_ROW; i++)
    {
        for(j = 0; j < MAX_BTN_COL; j++)
        {
            if(buttonCheck(i, j))
            {
                sendOSCMessage(TxSocket, prefix, msgPress, "iii", i, j, (btnCurrent[i] & (1 << j)) ? 1 : 0);
            }
            delayUs(5);
        }
    }
#endif
}

void sendAdc(void)
{
    BYTE i;

    for(i = 0; i < USE_ADC_NUM; i++)
    {
        if(getAnalogFlag(i))
        {
#ifdef USE_PITCH
            //currentValue[i] = getAnalogByte(i, 1);
            if(i == 0)
            {
                currentValue[i] = getAnalogByte(i, TYPE_MIDI_ORIGINAL);
            }
            if(i == 1)
            {
                currentValue[i] = getAnalogByte(i, TYPE_MIDI_VOLUME);
            }
            //currentValue[i] = getAnalogWord(i, TYPE_LONG_ORIGINAL);
#endif
#ifdef USE_OPT_DRUM
            currentValue[i] = getAnalogByte(i, TYPE_MIDI_ORIGINAL);
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
        TxSocket = UDPOpenEx((remoteIP[0] | (remoteIP[1] << 8) | (remoteIP[2] << 16) | (remoteIP[3] << 24)), UDP_OPEN_IP_ADDRESS, 0, remotePort);
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
#ifdef USE_PITCH
                analogInHandle(0, (LONG)((ADC1BUF0 + ADC1BUF2 + ADC1BUF4 + ADC1BUF6) / 4));
                analogInHandle(1, (LONG)((ADC1BUF1 + ADC1BUF3 + ADC1BUF5 + ADC1BUF7) / 4));
#endif
                usbState = 1;
                break;
#ifdef USE_PITCH
            case 1:
                //test sendAdc();
                usbState = 2;
                break;
            case 2:
                //test sendPad();
                usbState = 0;
                break;
#if 0
            case 3:
                convertOSCtoMIDI();
                usbState = 0;
                break;
#endif
#endif
#ifdef USE_OPT_DRUM
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
                switch(endpointBuffers[currentEndpoint].TransferState)
                {
                    case RX_DATA:
                        if(USBHostMIDIRead(deviceHandle, currentEndpoint, endpointBuffers[currentEndpoint].pBufWriteLocation, endpointBuffers[currentEndpoint].numOfMIDIPackets * sizeof(USB_AUDIO_MIDI_PACKET)) == USB_SUCCESS)
                        {
                            if(initSendFlag)
                            {
                                endpointBuffers[currentEndpoint].TransferState = RX_DATA_WAIT;
                                midiType = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_0 & 0xF0;
                                midiCh = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_0 & 0x0F;
                                midiNum = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_1;
                                midiVal = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_2;

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
                            }
                        }
                    case RX_DATA_WAIT:
                        if(!USBHostMIDITransferIsBusy(deviceHandle, currentEndpoint))
                        {
                            endpointBuffers[currentEndpoint].TransferState = RX_DATA;
                            endpointBuffers[currentEndpoint].pBufWriteLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;
                            if(endpointBuffers[currentEndpoint].pBufWriteLocation - endpointBuffers[currentEndpoint].bufferStart >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
                            {
                                endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
                            }
                        }
                        break;
                }
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
