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
 * picrouter-oauh.c,v.0.1 2012/08/14
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

    initSW();
    initLEDs();
    buttonInit();

    //setupPitch();
    AD1PCFG = 0x0000FFFC;// 0000 0000 0000 0000 1111 1111 1111 1100
    AD1CON1 = 0x000010E6;// 0000 0000 0000 0000 1000 0000 0110 0110
    AD1CON2 = 0x0000040C;// 0000 0000 0000 0000 0000 0100 0000 1100
    AD1CHS  = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
    AD1CSSL = 0x00000003;// 0000 0000 0000 0000 0000 0000 0000 0011
    AD1CON3 = 0x00000F08;// 0000 0000 0000 0000 0000 1111 0000 1000
    AD1CON1bits.ON = 1;

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

    TickInit();
    InitAppConfig();
    StackInit();
    ZeroconfLLInitialize();
    mDNSInitialize(MY_DEFAULT_HOST_NAME);
    mDNSServiceRegister((const char *)"PICrouter-OaUH", // base name of the service
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
        if(TickGet() - t >= TICK_SECOND / 2ul)
        {
            t = TickGet();
            //LED_1_Toggle();
            LED_2_Toggle();
        }

		currentState = SW_State();
		if(!currentState)
			LATBbits.LATB2 = 1;
		else
			LATBbits.LATB2 = 0;
        if(prevState != currentState)
        {
        	stateFlag = TRUE;
        	stateFlag2 = TRUE;
        }

        StackTask();
        NBNSTask();
        ZeroconfLLProcess();
        mDNSProcess();

        UDPControlTask();
        UDPSendTask();

        USBHostTasks();
        convertMidiToOsc();

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
*  UDP Generic I/O Processing Part
**********************************************/
void UDPControlTask(void)
{
	unsigned int RcvLen;
	static BYTE	buffer[40];

	if(!initReceiveFlag)
	{
		//MySocket = UDPOpen(localPort, NULL, remotePort);
        RxSocket = UDPOpen(localPort, NULL, 0);
		if(RxSocket != INVALID_UDP_SOCKET)
			initReceiveFlag = TRUE;
	}
    if(initReceiveFlag && UDPIsGetReady(RxSocket))
    {
        RcvLen = UDPGetArray(buffer, sizeof(buffer));
        UDPDiscard();
        if(isEqualToAddress(buffer, "/pic/led"))
        {
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
        }
        else if(isEqualToAddress(buffer, "/sys/remote/ip"))
        {
            remoteIP[0] = getIntArgumentAtIndex(buffer, "/sys/remote/ip", 0);
            remoteIP[1] = getIntArgumentAtIndex(buffer, "/sys/remote/ip", 1);
            remoteIP[2] = getIntArgumentAtIndex(buffer, "/sys/remote/ip", 2);
            remoteIP[3] = getIntArgumentAtIndex(buffer, "/sys/remote/ip", 3);
            initSendFlag = FALSE;
            UDPClose(TxSocket);
        }
    } 
}

void UDPSendTask()
{
    if(!initSendFlag)
    {
        //TxSocket = UDPOpenEx((192ul | (168ul << 8) | (1ul << 16) | (4ul << 24)), UDP_OPEN_IP_ADDRESS, 0, remotePort);
        TxSocket = UDPOpenEx((remoteIP[0] | (remoteIP[1] << 8) | (remoteIP[2] << 16) | (remoteIP[3] << 24)), UDP_OPEN_IP_ADDRESS, 0, remotePort);
        if(TxSocket == INVALID_UDP_SOCKET)
			return;
		initSendFlag = TRUE;
    }

    if(initSendFlag /*&& UDPIsPutReady(TxSocket)*/)
	{
        if(stateFlag)
        {
            stateFlag = FALSE;
            sendOSCMessage(TxSocket, prefix, msgSw, "i", (currentState ? 0 : 1));
        }
	}
	//UDPClose(MySocket);
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
                                    break;
                                case 0xB0:// Control Change
                                    sendOSCMessage(TxSocket, msgMidi, msgCc, "iii", ch, num, val);
                                    break;
                                case 0xC0:// Program Change
                                    break;
                                case 0xD0:// Channel Pressure
                                    break;
                                case 0xE0:// Pitch Bend
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
