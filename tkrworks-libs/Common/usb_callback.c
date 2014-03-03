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
 * usb_callback.c,v.0.3.0 2014/02/26
 */

#include "usb_callback.h"

USB_VOLATILE BYTE msCounter;

// for USB_MIDI
USB_HANDLE midiTxHandle = NULL;
USB_HANDLE midiRxHandle  = NULL;

unsigned char rcvMidiDataBuffer[64];
unsigned char sndMidiDataBuffer[64];

HOST_MIDI_PACKET rcvMidiPacket;

// for USB_HID
USB_HANDLE hidRxHandle = NULL;
USB_HANDLE hidTxHandle = NULL;

unsigned char rcvHidDataBuffer[64];
unsigned char sndHidDataBuffer[64];

#if defined(USB_USE_CDC)
    volatile APPL_STATE  APPL_CDC_State;
#endif

unsigned char getRcvMidiDataBuffer(BYTE index)
{
    if(index > 63)
        return 0;
    return rcvMidiDataBuffer[index];
}
void setRcvMidiDataBuffer(BYTE index, unsigned char value)
{
    if(index > 63)
        return;
    rcvMidiDataBuffer[index] = value;
}

unsigned char getRcvHidDataBuffer(BYTE index)
{
    if(index > 63)
        return 0;
    return rcvHidDataBuffer[index];
}
void setSndHidDataBuffer(BYTE index, unsigned char value)
{
    if(index > 63)
        return;
    sndHidDataBuffer[index] = value;
}

APPL_STATE getApplCDCState(void)
{
    return APPL_CDC_State;
}
void setApplCDCState(APPL_STATE state)
{
    APPL_CDC_State = state;
}

BOOL checkUSBState(void)
{
    BOOL flag = TRUE;

    if(USBDeviceState < CONFIGURED_STATE || USBSuspendControl == 1)
        flag = FALSE;

    return flag;
}

BOOL midiRxHandleBusy(void)
{
    return USBHandleBusy(midiRxHandle);
}
BOOL midiTxHandleBusy(void)
{
    return USBHandleBusy(midiTxHandle);
}

void midiRxOnePacket(void)
{
    midiRxHandle = USBRxOnePacket(MIDI_EP, (BYTE*)&rcvMidiDataBuffer[0], 64);
}

void midiTxOnePacket(BYTE *packet, BYTE len)
{
    midiTxHandle = USBTxOnePacket(MIDI_EP, packet, len);
}

void sendNote(BYTE ch, BYTE num, BYTE vel)
{
    USB_AUDIO_MIDI_EVENT_PACKET midiData;

    midiData.Val = 0;
    midiData.CableNumber = 0;
    midiData.CodeIndexNumber = MIDI_CIN_NOTE_ON;
//    if(vel > 0)
        midiData.DATA_0 = 0x90 + ch;
//    else
//        midiData.DATA_0 = 0x80 + ch;
    midiData.DATA_1 = num;
    midiData.DATA_2 = vel;
    
    if(!midiTxHandleBusy())
        midiTxOnePacket((BYTE *)&midiData, 4);
    
    //Delay10us(1);
    //delayUs(2);
}

void appendNote(BYTE ch, BYTE num, BYTE vel, BYTE pos)
{
    //USB_AUDIO_MIDI_EVENT_PACKET midiData;

    sndMidiDataBuffer[pos + 0] = 0;// midiData.Val = 0;
    sndMidiDataBuffer[pos + 0] = 0;// midiData.CableNumber = 0;
    sndMidiDataBuffer[pos + 0] = MIDI_CIN_NOTE_ON;// midiData.CodeIndexNumber = MIDI_CIN_NOTE_ON;

    sndMidiDataBuffer[pos + 1] = 0x90 + ch;// midiData.DATA_0 = 0x90 + ch;
    sndMidiDataBuffer[pos + 2] = num;// midiData.DATA_1 = num;
    sndMidiDataBuffer[pos + 3] = vel;// midiData.DATA_2 = vel;
    
    //Delay10us(1);
    //delayUs(2);
}

void sendControlChange(BYTE ch, BYTE num, BYTE val)
{
    USB_AUDIO_MIDI_EVENT_PACKET midiData;

    midiData.Val = 0;
    midiData.CableNumber = 0;
    midiData.CodeIndexNumber = MIDI_CIN_CONTROL_CHANGE;
    midiData.DATA_0 = 0xB0 + ch;

    midiData.DATA_1 = num;
    midiData.DATA_2 = val;

    if(!midiTxHandleBusy())
        midiTxOnePacket((BYTE *)&midiData, 4);
    
    //Delay10us(1);
    //delayUs(20);
}

void appendControlChange(BYTE ch, BYTE num, BYTE val, BYTE pos)
{
    //USB_AUDIO_MIDI_EVENT_PACKET midiData;

    sndMidiDataBuffer[pos + 0] = 0;//midiData.Val = 0;
    sndMidiDataBuffer[pos + 0] = 0;//midiData.CableNumber = 0;
    sndMidiDataBuffer[pos + 0] = MIDI_CIN_CONTROL_CHANGE;//midiData.CodeIndexNumber = MIDI_CIN_CONTROL_CHANGE;
    sndMidiDataBuffer[pos + 1] = 0xB0 + ch;//midiData.DATA_0 = 0xB0 + ch;

    sndMidiDataBuffer[pos + 2] = num;//midiData.DATA_1 = num;
    sndMidiDataBuffer[pos + 3] = val;//midiData.DATA_2 = val;

    //Delay10us(1);
    //delayUs(20);
}

void sendMIDI(BYTE len)
{
    if(!midiTxHandleBusy())
        midiTxOnePacket((BYTE *)&sndMidiDataBuffer, len);
}

BOOL hidHandleBusy(void)
{
    return USBHandleBusy(hidRxHandle);
}

void hidRxOnePacket(void)
{
    hidRxHandle = USBRxOnePacket(HID_EP, (BYTE*)&rcvHidDataBuffer[0], 64);
}

void hidTxOnePacket(void)
{
    hidTxHandle = USBTxOnePacket(HID_EP, (BYTE*)sndHidDataBuffer, 64);
}

void putHostMIDIPacket(USB_AUDIO_MIDI_PACKET packet)
{
    BYTE currentEndpoint;
    for(currentEndpoint = 0; currentEndpoint < USBHostMIDINumberOfEndpoints(deviceHandle); currentEndpoint++)
    {
        endpointBuffers[currentEndpoint].pBufWriteLocation->Val = packet.Val;
        endpointBuffers[currentEndpoint].pBufWriteLocation += endpointBuffers[currentEndpoint].numOfMIDIPackets;
        
        if(endpointBuffers[currentEndpoint].pBufWriteLocation - endpointBuffers[currentEndpoint].bufferStart >= endpointBuffers[currentEndpoint].numOfMIDIPackets * MIDI_USB_BUFFER_SIZE)
        {
            endpointBuffers[currentEndpoint].pBufWriteLocation = endpointBuffers[currentEndpoint].bufferStart;
        }
        break;
    }
}

HOST_MIDI_PACKET getHostMIDIPacket(void)
{
    BYTE currentEndpoint;

    switch(ProcState)
    {
    case STATE_INITIALIZE:
        ProcState = STATE_IDLE;
        rcvMidiPacket.rcvFlag = FALSE;
        break;
    case STATE_IDLE:
        rcvMidiPacket.rcvFlag = FALSE;
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
                rcvMidiPacket.rcvFlag = FALSE;
                break;
            case RX_DATA_WAIT:
                if(!USBHostMIDITransferIsBusy(deviceHandle, currentEndpoint))
                {
                    rcvMidiPacket.rcvFlag = TRUE;
                    rcvMidiPacket.type = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_0 & 0xF0;
                    rcvMidiPacket.ch = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_0 & 0x0F;
                    rcvMidiPacket.num = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_1;
                    rcvMidiPacket.val = endpointBuffers[currentEndpoint].pBufWriteLocation->DATA_2;

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
                rcvMidiPacket.rcvFlag = FALSE;
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
                rcvMidiPacket.rcvFlag = FALSE;
                break;
            }
            //delayUs(2);
        }
        break;
    case STATE_ERROR:
        rcvMidiPacket.rcvFlag = FALSE;
        //LED_2_On();
        break;
    default:
        ProcState = STATE_INITIALIZE;
        rcvMidiPacket.rcvFlag = FALSE;
        break;
    }
    return rcvMidiPacket;
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
 *                  In this example the interrupt is only used when the device
 *                  goes to sleep when it receives a USB suspend command
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
 *                  suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *                  mode, the host may wake the device back up by sending non-
 *                  idle state signalling.
 *
 *                  This call back is invoked when a wakeup from USB suspend
 *                  is detected.
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
 *                  firmware must process the request and respond
 *                  appropriately to fulfill the request.  Some of
 *                  the SETUP packets will be for standard
 *                  USB "chapter 9" (as in, fulfilling chapter 9 of
 *                  the official USB specifications) requests, while
 *                  hers may be specific to the USB device class
 *                  that is being implemented.  For example, a HID
 *                  class device needs to be able to respond to
 *                  "GET REPORT" type of requests.  This
 *                  is not a standard USB chapter 9 request, and
 *                  therefore not handled by usb_device.c.  Instead
 *                  this request should be handled by class specific
 *                  irmware, such as that contained in usb_function_hid.c.
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
 *                  called when a SETUP, bRequest: SET_DESCRIPTOR request
 *                  arrives.  Typically SET_DESCRIPTOR requests are
 *                  not used in most applications, and it is
 *                  optional to support this type of request.
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
 *                  SET_CONFIGURATION (wValue not = 0) request.  This
 *                  callback function should initialize the endpoints
 *                  for the device's usage according to the current
 *                  configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
    //enable the HID endpoint
    USBEnableEndpoint(HID_EP, USB_IN_ENABLED | USB_OUT_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);
    //Re-arm the OUT endpoint for the next packet
    //USBOutHandle = HIDRxPacket(HID_EP | MIDI_EP,(BYTE*)&ReceivedHidDataBuffer,64);
    hidRxHandle = USBRxOnePacket(HID_EP, (BYTE*)&rcvHidDataBuffer, 64);

    //enable the HID endpoint
    USBEnableEndpoint(MIDI_EP, USB_OUT_ENABLED | USB_IN_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);
    //Re-arm the OUT endpoint for the next packet
    midiRxHandle = USBRxOnePacket(MIDI_EP, (BYTE*)&rcvMidiDataBuffer, 64);
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
 *                  peripheral devices to wake up a host PC (such
 *                  as if it is in a low power suspend to RAM state).
 *                  This can be a very useful feature in some
 *                  USB applications, such as an Infrared remote
 *                  control	receiver.  If a user presses the "power"
 *                  button on a remote control, it is nice that the
 *                  IR receiver can detect this signalling, and then
 *                  send a USB "command" to the PC to wake up.
 *
 *                  USBCBSendResume() "callback" function is used
 *                  to send this special USB signalling which wakes
 *                  up the PC.  This function may be called by
 *                  application firmware to wake up the PC.  This
 *                  function will only be able to wake up the host if
 *                  all of the below are true:
 *
 *                  1.  The USB driver used on the host PC supports
 *                      the remote wakeup capability.
 *                  2.  The USB configuration descriptor indicates
 *                      the device is remote wakeup capable in the
 *                      bmAttributes field.
 *                  3.  The USB host PC is currently sleeping,
 *                      and has previously sent your device a SET
 *                      FEATURE setup packet which "armed" the
 *                      remote wakeup capability.
 *
 *                  If the host has not armed the device to perform remote wakeup,
 *                  then this function will return without actually performing a
 *                  remote wakeup sequence.  This is the required behavior,
 *                  as a USB device that has not been armed to perform remote
 *                  wakeup must not drive remote wakeup signalling onto the bus;
 *                  doing so will cause USB compliance testing failure.
 *
 *                  This callback should send a RESUME signal that
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
#if defined(USB_USE_CDC)
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

