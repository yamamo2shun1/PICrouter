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
 * osc.c,v.1.9.0 2014/06/17
 */

#include "osc.h"

// Network
static UDP_SOCKET RxSocket;
static UDP_SOCKET TxSocket;
static UDP_SOCKET DiscoverSocket;
static BOOL initReceiveFlag = FALSE;
static BOOL initSendFlag = FALSE;
static BOOL initDiscoverFlag = FALSE;
static BOOL chCompletedFlag = FALSE;
static char hostName[17] = "picrouter";
static char* stdPrefix = NULL;

static INT32 oscTotalSize;
static char sndOSCData[MAX_MESSAGE_LEN];

// Remote IP Address Initialization
static BYTE remoteIP[] = {224ul, 0ul, 0ul, 1ul};
//static BYTE remoteIP[] = {192ul, 168ul, 111ul, 21ul};

// Port Number Initialization
static WORD remotePort = 8000;
static WORD localPort  = 8080;

// MAC address Initialization
static ROM BYTE SerializedMACAddress[6] = {MY_DEFAULT_MAC_BYTE1,
                                           MY_DEFAULT_MAC_BYTE2,
                                           MY_DEFAULT_MAC_BYTE3,
                                           MY_DEFAULT_MAC_BYTE4,
                                           MY_DEFAULT_MAC_BYTE5,
                                           MY_DEFAULT_MAC_BYTE6};

// for LED_PAD_16 or LED_PAD_64 (15)
const char msgLatticePadConnectedNum[]    = "/lattice/pad/num";
const char msgSetLatticePadConnectedNum[] = "/lattice/pad/num/set";
const char msgGetLatticePadConnectedNum[] = "/lattice/pad/num/get";
const char msgLatticePadPinSelect[]       = "/lattice/pad/pin/select";
const char msgLatticePad[]                = "/lattice/pad";
const char msgLatticeLedDrvPinSelect[]    = "/lattice/led/driver/pin/select";
const char msgSetLatticeLed[]             = "/lattice/led/set";
const char msgSetLatticeLedColumn[]       = "/lattice/led/col/set";
const char msgSetLatticeLedRow[]          = "/lattice/led/row/set";
const char msgSetLatticeLedAll[]          = "/lattice/led/all/set";
const char msgLatticeLedClear[]           = "/lattice/led/clear";
const char msgLatticeLedIntensity[]       = "/lattice/led/intensity";
const char msgGetLatticeLedIntensity[]    = "/lattice/led/intensity/get";
const char msgLatticeLedIntensityAll[]    = "/lattice/led/intensity/all";
const char msgGetLatticeLedIntensityAll[] = "/lattice/led/intensity/all/get";

// for RGB_PAD_16/RGB_PAD_8/RGB_PAD_8L
const char msgLatticeRgbDrvPinSelect[]    = "/lattice/rgb/driver/pin/select";
const char msgLatticeRgbConnectedNum[]    = "/lattice/rgb/num";
const char msgSetLatticeRgbConnectedNum[] = "/lattice/rgb/num/set";
const char msgGetLatticeRgbConnectedNum[] = "/lattice/rgb/num/get";
const char msgLatticeRgbSize[]            = "/lattice/rgb/size";
const char msgSetLatticeRgbSize[]         = "/lattice/rgb/size/set";
const char msgGetLatticeRgbSize[]         = "/lattice/rgb/size/get";
const char msgSetLatticeRgb[]             = "/lattice/rgb/set";
const char msgSetLatticeRgbColumn[]       = "/lattice/rgb/col/set";
const char msgSetLatticeRgbRow[]          = "/lattice/rgb/row/set";
const char msgSetLatticeRgbAll[]          = "/lattice/rgb/all/set";
const char msgLatticeRgbClear[]           = "/lattice/rgb/clear";
const char msgLatticeRgbIntensity[]       = "/lattice/rgb/intensity";
const char msgGetLatticeRgbIntensity[]    = "/lattice/rgb/intensity/get";
const char msgLatticeRgbIntensityAll[]    = "/lattice/rgb/intensity/all";
const char msgGetLatticeRgbIntensityAll[] = "/lattice/rgb/intensity/all/get";

// for LED_ENC_32 or LED_ENC_ABS_32 (6)
const char msgRotaryLedDrvPinSelect[]    = "/rotary/led/driver/pin/select";
const char msgSetRotaryLedStep[]         = "/rotary/led/step/set";
const char msgSetRotaryLedBits[]         = "/rotary/led/bits/set";
const char msgRotaryLedIntensity[]       = "/rotary/led/intensity";
const char msgSetRotaryLedIntensity[]    = "/rotary/led/intensity/set";
const char msgGetRotaryLedIntensity[]    = "/rotary/led/intensity/get";
const char msgSetRotaryLedIntensityAll[] = "/rotary/led/intensity/all/set";
// for LED_ENC_32 (3)
const char msgRotaryIncEncPinSelect[] = "/rotary/inc/enc/pin/select";
const char msgRotaryIncEnc[]          = "/rotary/inc/enc";
const char msgRotaryIncEncSwitch[]    = "/rotary/inc/enc/switch";
// for LED_ENC_ABS_32 (6)
const char msgRotaryAbsEncPinSelect[]       = "/rotary/abs/enc/pin/select";
const char msgRotaryAbsEncConnectedNum[]    = "/rotary/abs/enc/num";
const char msgSetRotaryAbsEncConnectedNum[] = "/rotary/abs/enc/num/set";
const char msgGetRotaryAbsEncConnectedNum[] = "/rotary/abs/enc/num/get";
const char msgRotaryAbsEnc[]                = "/rotary/abs/enc";

// for DAC+ADC_SHIELD
const char msgSetExternalDac[] = "/external/dac/set";
const char msgInitExternalAdc[] = "/external/adc/init";
const char msgConvertExternalAdc[] = "/external/adc/convert";
const char msgReadExternalAdc[] = "/external/adc/read";
const char msgExternalAdc[] = "/external/adc";

// for ADG1414
const char msgSetADG1414[] = "/adg1414/set";

// for Gyro
const char msgSetGyroEnable[] = "/gyro/enable/set";
const char msgSetGyroParameters[] = "/gyro/parameters/set";

//Standard OSC Messages
// for Onboard (2)
const char msgOnboardLed[] = "/onboard/led";
const char msgOnboardSw1[] = "/onboard/sw1";

// for I/O Port (6)
const char msgPortIO[]     = "/port/io";
const char msgSetPortIO[]  = "/port/io/set";
const char msgGetPortIO[]  = "/port/io/get";
const char msgSetPortOut[] = "/port/out/set";
const char msgPortIn[]     = "/port/in";
const char msgGetPortIn[]  = "/port/in/get";

// for A/D (7)
const char msgAdc[]          = "/adc";
const char msgAdcEnable[]    = "/adc/enable";
const char msgSetAdcEnable[] = "/adc/enable/set";
const char msgGetAdcEnable[] = "/adc/enable/get";
const char msgAdcType[]      = "/adc/type";
const char msgSetAdcType[]   = "/adc/type/set";
const char msgGetAdcType[]   = "/adc/type/get";
const char msgAdcCurve[]     = "/adc/curve";
const char msgSetAdcCurve[]  = "/adc/curve/set";
const char msgGetAdcCurve[]  = "/adc/curve/get";
const char msgAdcDio[]       = "/adc/dio";
const char msgSetAdcDio[]    = "/adc/dio/set";
const char msgGetAdcDio[]    = "/adc/dio/get";
const char msgSetAdcDo[]     = "/adc/dout/set";
const char msgAdcDi[]        = "/adc/din";
const char msgGetAdcDi[]     = "/adc/din/get";
// for PWM (15)
const char msgPwmEnable[]    = "/pwm/enable";
const char msgSetPwmEnable[] = "/pwm/enable/set";
const char msgGetPwmEnable[] = "/pwm/enable/get";
const char msgPwmFreq[]      = "/pwm/freq";
const char msgSetPwmFreq[]   = "/pwm/freq/set";
const char msgGetPwmFreq[]   = "/pwm/freq/get";
const char msgPwmDuty[]      = "/pwm/duty";
const char msgSetPwmDuty[]   = "/pwm/duty/set";
const char msgGetPwmDuty[]   = "/pwm/duty/get";
const char msgPwmDio[]       = "/pwm/dio";
const char msgSetPwmDio[]    = "/pwm/dio/set";
const char msgGetPwmDio[]    = "/pwm/dio/get";
const char msgSetPwmDo[]     = "/pwm/dout/set";
const char msgPwmDi[]        = "/pwm/din";
const char msgGetPwmDi[]     = "/pwm/din/get";
// for DIO (6)
const char msgDigitalDio[]    = "/digital/dio";
const char msgSetDigitalDio[] = "/digital/dio/set";
const char msgGetDigitalDio[] = "/digital/dio/get";
const char msgSetDigitalDo[]  = "/digital/dout/set";
const char msgDigitalDi[]     = "/digital/din";
const char msgGetDigitalDi[]  = "/digital/din/get";
// for SPI (11)
const char msgSetSpiConfig[] = "/spi/config/set";
const char msgDisableSpi[]   = "/spi/disable";
const char msgSpiData[]      = "/spi/data";
const char msgSetSpiData[]   = "/spi/data/set";
const char msgGetSpiData[]   = "/spi/data/get";
const char msgSpiDio[]       = "/spi/dio";
const char msgSetSpiDio[]    = "/spi/dio/set";
const char msgGetSpiDio[]    = "/spi/dio/get";
const char msgSetSpiDo[]     = "/spi/dout/set";
const char msgSpiDi[]        = "/spi/din";
const char msgGetSpiDi[]     = "/spi/din/get";
// for I2C (7)
const char msgEnableI2c[]          = "/i2c/enable";
const char msgSetI2cConfig[]       = "/i2c/config/set";
const char msgSetI2cFreq[]         = "/i2c/freq/set";
const char msgSetI2cSlaveAddress[] = "/i2c/slave/address/set";
const char msgI2cData[]            = "/i2c/data";
const char msgSetI2cData[]         = "/i2c/data/set";
const char msgGetI2cData[]         = "/i2c/data/get";

const char msgSetLcdConfig[] = "/lcd/config/set";
const char msgSetLcdText[]   = "/lcd/text/set";
const char msgClearLcdText[] = "/lcd/text/clear";

//OSC Messages converted from MIDI Messages (15)
const char midiPrefix[] = "/midi";
const char msgSetNote[] = "/note/set";
const char msgNote[]    = "/note";
const char msgSetPp[]   = "/pp/set";
const char msgPp[]      = "/pp";
const char msgSetCc[]   = "/cc/set";
const char msgCc[]      = "/cc";
const char msgSetPc[]   = "/pc/set";
const char msgPc[]      = "/pc";
const char msgSetKp[]   = "/kp/set";
const char msgKp[]      = "/kp";
const char msgSetCp[]   = "/cp/set";
const char msgCp[]      = "/cp";
const char msgSetPb[]   = "/pb/set";
const char msgPb[]      = "/pb";

// OSC Messages converted from CDC Messages (2)
const char cdcPrefix[]  = "/cdc";
const char msgSetData[] = "/data/set";
const char msgData[]    = "/data";

//System OSC Messages for Network Settings (25)
const char sysPrefix[]           = "/sys";
const char msgPrefix[]           = "/prefix";
const char msgSetPrefix[]        = "/prefix/set";
const char msgGetPrefix[]        = "/prefix/get";
const char msgRemoteIp[]         = "/remote/ip";
const char msgSetRemoteIp[]      = "/remote/ip/set";
const char msgGetRemoteIp[]      = "/remote/ip/get";
const char msgRemotePort[]       = "/remote/port";
const char msgSetRemotePort[]    = "/remote/port/set";
const char msgGetRemotePort[]    = "/remote/port/get";
const char msgHostName[]         = "/host/name";
const char msgSetHostName[]      = "/host/name/set";
const char msgGetHostName[]      = "/host/name/get";
const char msgHostIp[]           = "/host/ip";
const char msgGetHostIp[]        = "/host/ip/get";
const char msgHostMac[]          = "/host/mac";
const char msgGetHostMac[]       = "/host/mac/get";
const char msgHostPort[]         = "/host/port";
const char msgSetHostPort[]      = "/host/port/set";
const char msgGetHostPort[]      = "/host/port/get";
const char msgUsbMode[]          = "/usb/mode";
const char msgSwitchUsbMode[]    = "/usb/mode/switch";
const char msgGetUsbMode[]       = "/usb/mode/get";
const char msgNvmData[]          = "/nvm/data";
const char msgWriteNvmData[]     = "/nvm/data/write";
const char msgReadNvmData[]      = "/nvm/data/read";
const char msgClearNvmData[]     = "/nvm/data/clear";
const char msgSoftReset[]        = "/soft/reset";
const char msgConfiguration[]    = "/configuration";
const char msgDiscoverDevices[]  = "/discover/devices";
const char msgDiscoveredDevice[] = "/discovered/device";
const char msgDebug[]            = "/debug";
const char msgError[]            = "/error";
const char msgVersion[]          = "/version";
const char msgGetVersion[]       = "/version/get";

// for touchOSC
const char toscPrefix[] = "/tosc";

const char msgOnboardLed1[] = "/onboard/led/1";
const char msgOnboardLed2[] = "/onboard/led/2";

const char msgSetPwmEnable1[] = "/pwm/enable/set/1";
const char msgSetPwmEnable2[] = "/pwm/enable/set/2";
const char msgSetPwmEnable3[] = "/pwm/enable/set/3";
const char msgSetPwmEnable4[] = "/pwm/enable/set/4";
const char msgSetPwmDuty1[]   = "/pwm/duty/set/1";
const char msgSetPwmDuty2[]   = "/pwm/duty/set/2";
const char msgSetPwmDuty3[]   = "/pwm/duty/set/3";
const char msgSetPwmDuty4[]   = "/pwm/duty/set/4";

// Variables
static BYTE indexA = 0;
static BYTE indexA0 = 0;
static BYTE ringBufIndex = 0;
static BYTE ringProcessIndex = 0;
#ifdef USE_SPI_SRAM
static BYTE udpOnePacket[MAX_PACKET_SIZE] = {0};
#else
static BYTE udpPacket[MAX_BUF_SIZE][MAX_PACKET_SIZE] = {0};
#endif
static BYTE oscPacket[MAX_PACKET_SIZE] = {0};
static char rcvAddressStrings[MAX_ADDRESS_LEN] = {0};
static UINT16 rcvPrefixLength = 0;
static UINT16 rcvAddressLength = 0;
static UINT16 rcvTypesStartIndex = 0;
static INT16 rcvArgumentsLength = 0;
static char rcvArgsTypeArray[MAX_ARGS_LEN] = {0};
static UINT16 rcvArgumentsStartIndex[MAX_ARGS_LEN] = {0};
static UINT16 rcvArgumentsIndexLength[MAX_ARGS_LEN] = {0};
static BYTE bundleData[512] = {0};
static UINT16 bundleAppendIndex = 0;

#if defined(USE_RN131)
static volatile BOOL uartInterruptFlag = FALSE;
#endif

// Static Functions
static BOOL copyOSCPacketFromUDPPacket();
static BOOL extractAddressFromOSCPacket();
static BOOL extractTypeTagFromOSCPacket();
static BOOL extractArgumentsFromOSCPacket();

#ifdef USE_SPI_SRAM
static BYTE getUdpPacket(WORD bufferIndex, WORD packetIndex)
{
    return readByteFromSRAM((bufferIndex * MAX_PACKET_SIZE) + packetIndex);
}
static void setUdpPacket(WORD bufferIndex, WORD packetIndex, BYTE value)
{
    writeByteToSRAM((bufferIndex * MAX_PACKET_SIZE) + packetIndex, value);
}
static BYTE* getUdpPackets(WORD bufferIndex)
{
    memset(udpOnePacket, 0, MAX_PACKET_SIZE);
    readBytesFromSRAM(bufferIndex * MAX_PACKET_SIZE, udpOnePacket, MAX_PACKET_SIZE);
    return udpOnePacket;
}
static void setUdpPackets(WORD bufferIndex, BYTE* data)
{
    writeBytesToSRAM(bufferIndex * MAX_PACKET_SIZE, data, MAX_PACKET_SIZE);
}
static void clearUdpPackets(WORD bufferIndex)
{
    clearSRAM(bufferIndex * MAX_PACKET_SIZE, MAX_PACKET_SIZE);
}
#endif

void InitAppConfig(void)
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

    //memcpypgm2ram(AppConfig.NetBIOSName, (ROM void*)DEFAULT_HOST_NAME, 16);
    //FormatNetBIOSName(AppConfig.NetBIOSName);
}

void setInitReceiveFlag(BOOL flag)
{
    initReceiveFlag = flag;
}

BOOL getInitReceiveFlag(void)
{
    return initReceiveFlag;
}

void setInitSendFlag(BOOL flag)
{
    initSendFlag = flag;
}

BOOL getInitSendFlag(void)
{
    return initSendFlag;
}

void setInitDiscoverFlag(BOOL flag)
{
    initDiscoverFlag = flag;
}

BOOL getInitDiscoverFlag(void)
{
    return initDiscoverFlag;
}

void setChCompletedFlag(BOOL flag)
{
    chCompletedFlag = flag;
}

BOOL getChCompletedFlag(void)
{
    return chCompletedFlag;
}

void setRemoteIpAtIndex(BYTE index, BYTE number)
{
    remoteIP[index] = number;
}

BYTE getRemoteIpAtIndex(BYTE index)
{
    return remoteIP[index];
}

BYTE* getRemoteIp(void)
{
    return remoteIP;
}

void setRemotePort(WORD number)
{
    remotePort = number;
}

WORD getRemotePort(void)
{
    return remotePort;
}

void setLocalPort(WORD number)
{
    localPort = number;
}

WORD getLocalPort(void)
{
    return localPort;
}

void setOSCPrefix(char* prefix_string)
{
    stdPrefix = (char *)calloc(strlen(prefix_string), sizeof(char));
    memcpy(stdPrefix, prefix_string, strlen(prefix_string));
}

char* getOSCPrefix(void)
{
    return stdPrefix;
}

void clearOSCPrefix(void)
{
    free(stdPrefix);
    stdPrefix = NULL;
}

void setOSCHostName(char* host_name)
{
    BYTE hnlen = strlen(host_name);
    //hostName = (char *)calloc(strlen(host_name), sizeof(char));
    memset(hostName, 0, MAX_HOST_NAME_LEN + 1);
    if(hnlen <= MAX_HOST_NAME_LEN)
        memcpy(hostName, host_name, hnlen);
    else
        memcpy(hostName, host_name, MAX_HOST_NAME_LEN);
}

char* getOSCHostName(void)
{
    return hostName;
}

void clearOSCHostName(void)
{
    memset(hostName, 0, MAX_HOST_NAME_LEN + 1);
    //free(hostName);
    //hostName = NULL;
}

BOOL openOSCSendPort(BYTE* ip_address, WORD port_number)
{
    if(ip_address[0] == 224 && ip_address[1] == 0 && ip_address[2] == 0)
    {
        NODE_INFO mcRemote;

        mcRemote.IPAddr.v[0] = ip_address[0];
        mcRemote.IPAddr.v[1] = ip_address[1];
        mcRemote.IPAddr.v[2] = ip_address[2];
        mcRemote.IPAddr.v[3] = ip_address[3];

        mcRemote.MACAddr.v[0] = 0x01;
        mcRemote.MACAddr.v[1] = 0x00;
        mcRemote.MACAddr.v[2] = 0x5E;
        mcRemote.MACAddr.v[3] = ip_address[1] & 0x7F;
        mcRemote.MACAddr.v[4] = ip_address[2];
        mcRemote.MACAddr.v[5] = ip_address[3];

        TxSocket = UDPOpenEx((DWORD)(PTR_BASE)&mcRemote, UDP_OPEN_NODE_INFO, 0, port_number);
    }
    else
    {
        TxSocket = UDPOpenEx((ip_address[0] | (ip_address[1] << 8) | (ip_address[2] << 16) | (ip_address[3] << 24)), UDP_OPEN_IP_ADDRESS, 0, port_number);
    }

    if(TxSocket == INVALID_UDP_SOCKET)
        return FALSE;

    return TRUE;
}

BOOL openOSCReceivePort(WORD port_number)
{
    RxSocket = UDPOpen(port_number, NULL, 0);

    if(RxSocket == INVALID_UDP_SOCKET)
        return FALSE;

    return TRUE;
}

BOOL openDiscoverPort(void)
{
    DiscoverSocket = UDPOpenEx(0, UDP_OPEN_SERVER, 2860, 30303);
    if(DiscoverSocket == INVALID_UDP_SOCKET)
        return FALSE;

    return TRUE;
}

BOOL isOSCSendPortOpened(void)
{
    return UDPIsOpened(TxSocket);
}

BOOL isOSCReceivePortOpened(void)
{
    return UDPIsOpened(RxSocket);
}

BOOL isDiscoverPortOpened(void)
{
    return UDPIsOpened(DiscoverSocket);
}

void closeOSCSendPort(void)
{
    initSendFlag = FALSE;
    UDPClose(TxSocket);
    TxSocket = 0;
}

void closeOSCReceivePort(void)
{
    initReceiveFlag = FALSE;
    UDPClose(RxSocket);
    RxSocket = 0;
}

void closeDiscoverPort(void)
{
    initDiscoverFlag = FALSE;
    UDPClose(DiscoverSocket);
    DiscoverSocket = 0;
}

BOOL isOSCGetReady(void)
{
    if(!UDPIsGetReady(RxSocket))
        return FALSE;

    return TRUE;
}

BOOL isOSCPutReady(void)
{
    if(!UDPIsPutReady(TxSocket))
        return FALSE;

    return TRUE;
}

BOOL isDiscoverPutReady(void)
{
    if(!UDPIsPutReady(DiscoverSocket))
        return FALSE;

    return TRUE;
}

#if defined(USE_RN131)
void setOSCPacketFromRN134(BYTE index, BYTE value)
{
    udpPacket[ringBufIndex][index] = value;
}

void incRingBufIndex(void)
{
    ringBufIndex++;
    ringBufIndex &= (MAX_BUF_SIZE - 1);
}

BYTE getRingBufIndex(void)
{
    return ringBufIndex;
}
BYTE getRingProcessIndex(void)
{
    return ringProcessIndex;
}

BOOL checkEmptyUdpPacket(void)
{
    if(!udpPacket[ringBufIndex][0] && ringProcessIndex != ringBufIndex)
        return TRUE;
    else
        return FALSE;
}

void setUartInterruptFlag(BOOL flag)
{
    uartInterruptFlag = flag;
}
#endif

void getOSCPacket(void)
{
    if(!initReceiveFlag)
        initReceiveFlag = openOSCReceivePort(localPort);
    
    if(initReceiveFlag && isOSCGetReady())
    {
#ifdef USE_SPI_SRAM
        memset(udpOnePacket, 0, MAX_PACKET_SIZE);
        UDPGetArray(udpOnePacket, MAX_PACKET_SIZE);
        if(!udpPacket[ringBufIndex][0])
        {
            setUdpPackets(ringBufIndex, udpOnePacket);
            ringBufIndex++;
        }
#else
        if(!udpPacket[ringBufIndex][0])
            UDPGetArray(udpPacket[ringBufIndex++], MAX_PACKET_SIZE);
#endif
        ringBufIndex &= (MAX_BUF_SIZE - 1);
        UDPDiscard();
    }
}

BOOL processOSCPacket(void)
{
    static BYTE state_index = 0;
    switch(state_index)
    {
        case 0:
            if(!copyOSCPacketFromUDPPacket())
                return FALSE;
            state_index = 1;
            break;
        case 1:
            if(!extractAddressFromOSCPacket())
            {
                state_index = 0;
                return FALSE;
            }
            state_index = 2;
            break;
        case 2:
            if(!extractTypeTagFromOSCPacket())
            {
                state_index = 0;
                return FALSE;
            }
            state_index = 3;
            break;
        case 3:
            if(!extractArgumentsFromOSCPacket())
            {
                state_index = 0;
                return FALSE;
            }
            state_index = 4;
            break;
        case 4:
            state_index = 0;
            return TRUE;
            break;
        default:
            state_index = 0;
            break;
    }
    return FALSE;
}

static BOOL copyOSCPacketFromUDPPacket()
{
    int i, j;
    DWORD len = 0;

#if 0//defined(USE_RN131)
    if(uartInterruptFlag)
        return FALSE;
#endif

#ifdef USE_SPI_SRAM
    if(!strcmp(getUdpPackets(ringProcessIndex), "#bundle"))
#else
    if(!strcmp(udpPacket[ringProcessIndex], "#bundle"))
#endif
    {
        j = 16;
        while(TRUE)
        {
            len = 0;
#ifdef USE_SPI_SRAM
            len += (DWORD)getUdpPacket(ringProcessIndex, j) << 24;
            len += (DWORD)getUdpPacket(ringProcessIndex, j + 1) << 16;
            len += (DWORD)getUdpPacket(ringProcessIndex, j + 2) << 8;
            len += (DWORD)getUdpPacket(ringProcessIndex, j + 3);
#else
            len += (DWORD)udpPacket[ringProcessIndex][j] << 24;
            len += (DWORD)udpPacket[ringProcessIndex][j + 1] << 16;
            len += (DWORD)udpPacket[ringProcessIndex][j + 2] << 8;
            len += (DWORD)udpPacket[ringProcessIndex][j + 3];
#endif

            if(len == 0)
                break;

            for(i = 0; i < len; i++)
#ifdef USE_SPI_SRAM
                setUdpPacket(ringBufIndex, i, getUdpPacket(ringProcessIndex, j + 4 + i));
#else
                udpPacket[ringBufIndex][i] = udpPacket[ringProcessIndex][j + 4 + i];
#endif

            j += (4 + len);

            ringBufIndex = (ringBufIndex + 1) & (MAX_BUF_SIZE - 1);
        }

#ifdef USE_SPI_SRAM
        clearUdpPackets(ringProcessIndex);
        ringProcessIndex++;
#else
        memset(udpPacket[ringProcessIndex++], 0, MAX_PACKET_SIZE);
#endif
        ringProcessIndex &= (MAX_BUF_SIZE - 1);
    }

#ifdef USE_SPI_SRAM
    if(getUdpPacket(ringProcessIndex, 0) != '/')
#else
    if(udpPacket[ringProcessIndex][0] != '/')
#endif
    {
        if(udpPacket[ringProcessIndex][0] != NULL)
            memset(udpPacket[ringProcessIndex], 0, MAX_PACKET_SIZE);
        
        ringProcessIndex++;
        ringProcessIndex &= (MAX_BUF_SIZE - 1);
        
        return FALSE;
    }

#ifdef USE_SPI_SRAM
    memcpy(oscPacket, getUdpPackets(ringProcessIndex), MAX_PACKET_SIZE);
    clearUdpPackets(ringProcessIndex);
    ringProcessIndex++;
#else
    memcpy(oscPacket, udpPacket[ringProcessIndex], MAX_PACKET_SIZE);
    memset(udpPacket[ringProcessIndex++], 0, MAX_PACKET_SIZE);
#endif

    ringProcessIndex &= (MAX_BUF_SIZE - 1);

    return TRUE;
}

static BOOL extractAddressFromOSCPacket()
{
    memset(rcvAddressStrings, 0, sizeof(rcvAddressStrings));

    indexA = 3;
    while(oscPacket[indexA])
    {
        indexA += 4;
        if(indexA >= MAX_PACKET_SIZE)
            return FALSE;
    }
    indexA0 = indexA;
    indexA--;
    while(!oscPacket[indexA])
        indexA--;
    indexA++;

    memcpy(rcvAddressStrings, oscPacket, indexA);

    rcvAddressLength = indexA;

    return TRUE;
}

static BOOL extractTypeTagFromOSCPacket()
{
    rcvTypesStartIndex = indexA0 + 1;
    indexA = indexA0 + 2;

    while(oscPacket[indexA])
    {
        indexA++;
        if(indexA >= MAX_PACKET_SIZE)
            return FALSE;
    }
    memcpy(rcvArgsTypeArray, oscPacket + rcvTypesStartIndex + 1, indexA - rcvTypesStartIndex);

    return TRUE;
}

static BOOL extractArgumentsFromOSCPacket()
{
    INT16 i = 0, n = 0, u = 0, length = 0;

    if(indexA == 0 || indexA >= MAX_PACKET_SIZE)
        return FALSE;

    rcvArgumentsLength = indexA - rcvTypesStartIndex;
    n = ((rcvArgumentsLength / 4) + 1) * 4;

    if(!rcvArgumentsLength)
        return FALSE;

    for(i = 0; i < rcvArgumentsLength - 1; i++)
    {
        rcvArgumentsStartIndex[i] = rcvTypesStartIndex + length + n;
        switch(rcvArgsTypeArray[i])
        {
            case 'i':
            case 'f':
                length += 4;
                rcvArgumentsIndexLength[i] = 4;
                break;
            case 's':
                u = 0;
                while(oscPacket[rcvArgumentsStartIndex[i] + u])
                {
                    u++;
                    if((rcvArgumentsStartIndex[i] + u) >= MAX_PACKET_SIZE)
                        return FALSE;
                }

                rcvArgumentsIndexLength[i] = ((u / 4) + 1) * 4;

                length += rcvArgumentsIndexLength[i];
                break;
            default: // T, F,N,I and others
                break;
        }
    }
    return TRUE;
}

void sendOSCMessage(const char* prefix, const char* command, const char* type, ...)
{
    va_list list;

    INT32 prefixSize = strchr(prefix, 0) - prefix;
    INT32 commandSize = strchr(command, 0) - command;
    INT32 addressSize = prefixSize + commandSize;
    INT32 typeSize = strchr(type, 0) - type;
    INT32 zeroSize = 0;
    INT32 totalSize = 0;
    
    int ivalue = 0;
    float fvalue = 0.0;
    char* fchar = NULL;
    char* cstr = NULL;
    int cstr_len = 0;
    char str[MAX_MESSAGE_LEN];
    memset(str, 0, MAX_MESSAGE_LEN);

    if(isOSCPutReady() || isDiscoverPutReady())
    {
        //debug LED_1_On();

        sprintf(str, "%s%s", prefix, command);

        zeroSize = (addressSize ^ ((addressSize >> 3) << 3)) == 0 ? 0 : 8 - (addressSize ^ ((addressSize >> 3) << 3));
        if(zeroSize == 0)
            zeroSize = 4;
        else if(zeroSize > 4 && zeroSize < 8)
            zeroSize -= 4;

        totalSize = (addressSize + zeroSize);
        sprintf((str + totalSize), ",%s", type);

        typeSize++;
        zeroSize = (typeSize ^ ((typeSize >> 2) << 2)) == 0 ? 0 : 4 - (typeSize ^ ((typeSize >> 2) << 2));
        if(zeroSize == 0)
            zeroSize = 4;
        
        totalSize += (typeSize + zeroSize);

        va_start(list, type);
        while(*type)
        {
            switch(*type)
            {
                case 'i':
                    ivalue = va_arg(list, int);
                    str[totalSize++] = (ivalue >> 24) & 0xFF;
                    str[totalSize++] = (ivalue >> 16) & 0xFF;
                    str[totalSize++] = (ivalue >> 8) & 0xFF;
                    str[totalSize++] = (ivalue >> 0) & 0xFF;
                    break;
                case 'f':
                    fvalue = (float)va_arg(list, double);
                    fchar = (char *)&fvalue;
                    str[totalSize++] = fchar[3] & 0xFF;
                    str[totalSize++] = fchar[2] & 0xFF;
                    str[totalSize++] = fchar[1] & 0xFF;
                    str[totalSize++] = fchar[0] & 0xFF;
                    break;
                case 's':
                    cstr = va_arg(list, char*);
                    cstr_len = 0;
                    while(*cstr)
                    {
                        str[totalSize + cstr_len] = *(cstr++) & 0xFF;
                        cstr_len++;
                    }
                    totalSize += ((cstr_len / 4) + 1) * 4;
                    break;
                default: // T, F, N, I and others
                    break;
            }
            type++;
        }
        va_end(list);

        UDPPutArray((BYTE *)str, totalSize);
        UDPFlush();

#if defined(USE_RN131)
        sendMessageToRN134((BYTE *)str, totalSize);
#endif

        //debug LED_1_Off();
    }
}

void setOSCAddress(const char* prefix, const char* command)
{
    INT32 prefixSize = strchr(prefix, 0) - prefix;
    INT32 commandSize = strchr(command, 0) - command;
    INT32 addressSize = prefixSize + commandSize;
    INT32 zeroSize = 0;

    memset(sndOSCData, 0, MAX_MESSAGE_LEN);
    oscTotalSize = 0;

    sprintf(sndOSCData, "%s%s", prefix, command);

    zeroSize = (addressSize ^ ((addressSize >> 3) << 3)) == 0 ? 0 : 8 - (addressSize ^ ((addressSize >> 3) << 3));
    if(zeroSize == 0)
        zeroSize = 4;
    else if(zeroSize > 4 && zeroSize < 8)
        zeroSize -= 4;

    oscTotalSize = (addressSize + zeroSize);
}

void setOSCTypeTag(const char* type)
{
    INT32 typeSize = strchr(type, 0) - type;
    INT32 zeroSize = 0;

    sprintf((sndOSCData + oscTotalSize), ",%s", type);

    typeSize++;
    zeroSize = (typeSize ^ ((typeSize >> 2) << 2)) == 0 ? 0 : 4 - (typeSize ^ ((typeSize >> 2) << 2));
    if(zeroSize == 0)
        zeroSize = 4;
        
    oscTotalSize += (typeSize + zeroSize);
}

void addOSCIntArgument(int value)
{
    sndOSCData[oscTotalSize++] = (value >> 24) & 0xFF;
    sndOSCData[oscTotalSize++] = (value >> 16) & 0xFF;
    sndOSCData[oscTotalSize++] = (value >> 8) & 0xFF;
    sndOSCData[oscTotalSize++] = (value >> 0) & 0xFF;
}

void addOSCFloatArgument(float value)
{
    char* fchar = NULL;
    fchar = (char *)&value;
    sndOSCData[oscTotalSize++] = fchar[3] & 0xFF;
    sndOSCData[oscTotalSize++] = fchar[2] & 0xFF;
    sndOSCData[oscTotalSize++] = fchar[1] & 0xFF;
    sndOSCData[oscTotalSize++] = fchar[0] & 0xFF;
}

void addOSCStringArgument(char* str)
{
    int cstr_len = 0;
    while(*str)
    {
        sndOSCData[oscTotalSize + cstr_len] = *(str++) & 0xFF;
        cstr_len++;
    }
    oscTotalSize += ((cstr_len / 4) + 1) * 4;
}

void clearOSCMessage(void)
{
    memset(sndOSCData, 0, MAX_MESSAGE_LEN);
    oscTotalSize = 0;
}

void flushOSCMessage(void)
{
    if(isOSCPutReady() || isDiscoverPutReady())
    {
        UDPPutArray((BYTE *)sndOSCData, oscTotalSize);
        UDPFlush();
    }
}

void clearOSCBundle(void)
{
    memset(bundleData, 0, sizeof(bundleData));
    bundleData[0] = '#';
    bundleData[1] = 'b';
    bundleData[2] = 'u';
    bundleData[3] = 'n';
    bundleData[4] = 'd';
    bundleData[5] = 'l';
    bundleData[6] = 'e';

    bundleAppendIndex = 16;
}

void appendOSCMessageToBundle(const char* prefix, const char* command, const char* type, ...)
{
    va_list list;

    INT32 prefixSize = strchr(prefix, 0) - prefix;
    INT32 commandSize = strchr(command, 0) - command;
    INT32 addressSize = prefixSize + commandSize;
    INT32 typeSize = strchr(type, 0) - type;
    INT32 zeroSize = 0;
    INT32 totalSize = 0;

    int ivalue = 0;
    float fvalue = 0.0;
    char* fchar = NULL;
    char* cstr = NULL;
    int cstr_len = 0;
    char str[MAX_MESSAGE_LEN];
    memset(str, 0, MAX_MESSAGE_LEN);

    sprintf(str, "%s%s", prefix, command);

    zeroSize = (addressSize ^ ((addressSize >> 3) << 3)) == 0 ? 0 : 8 - (addressSize ^ ((addressSize >> 3) << 3));
    if(zeroSize == 0)
        zeroSize = 4;
    else if(zeroSize > 4 && zeroSize < 8)
        zeroSize -= 4;

    totalSize = (addressSize + zeroSize);
    sprintf((str + totalSize), ",%s", type);

    typeSize++;
    zeroSize = (typeSize ^ ((typeSize >> 2) << 2)) == 0 ? 0 : 4 - (typeSize ^ ((typeSize >> 2) << 2));
    if(zeroSize == 0)
        zeroSize = 4;

    totalSize += (typeSize + zeroSize);

    va_start(list, type);
    while(*type)
    {
        switch(*type)
        {
            case 'i':
                ivalue = va_arg(list, int);
                str[totalSize++] = (ivalue >> 24) & 0xFF;
                str[totalSize++] = (ivalue >> 16) & 0xFF;
                str[totalSize++] = (ivalue >> 8) & 0xFF;
                str[totalSize++] = (ivalue >> 0) & 0xFF;
                break;
            case 'f':
                fvalue = (float)va_arg(list, double);
                fchar = (char *)&fvalue;
                str[totalSize++] = fchar[3] & 0xFF;
                str[totalSize++] = fchar[2] & 0xFF;
                str[totalSize++] = fchar[1] & 0xFF;
                str[totalSize++] = fchar[0] & 0xFF;
                break;
            case 's':
                cstr = va_arg(list, char*);
                cstr_len = 0;
                while(*cstr)
                {
                    str[totalSize + cstr_len] = *(cstr++) & 0xFF;
                    cstr_len++;
                }
                totalSize += ((cstr_len / 4) + 1) * 4;
                break;
            default: // T, F, N, I and others
                break;
        }
        type++;
    }
    va_end(list);

    bundleData[bundleAppendIndex++] = (totalSize >> 24) & 0xFF;
    bundleData[bundleAppendIndex++] = (totalSize >> 16) & 0xFF;
    bundleData[bundleAppendIndex++] = (totalSize >> 8) & 0xFF;
    bundleData[bundleAppendIndex++] = (totalSize) & 0xFF;
    memcpy(bundleData + bundleAppendIndex, str, totalSize);
    bundleAppendIndex += totalSize;
}

void sendOSCBundle(void)
{
    if(isOSCPutReady())
    {
        UDPPutArray(bundleData, bundleAppendIndex);
        UDPFlush();
    }
}

BOOL compareOSCPrefix(const char* prefix)
{
    int i = 0;
    rcvPrefixLength = strlen(prefix);

    if(rcvAddressLength < rcvPrefixLength)
        return FALSE;

    while(prefix[i])
    {
        if(rcvAddressStrings[i] != prefix[i])
            return FALSE;

        i++;
        if(i > rcvPrefixLength)
            return FALSE;
    }
    return TRUE;
}

BOOL compareOSCAddress(const char* address)
{
    int i = rcvPrefixLength, j = 0;
    WORD address_len = strlen(address);

    if(rcvAddressLength > rcvPrefixLength + address_len)
        return FALSE;

    while(address[j])
    {
        if(rcvAddressStrings[i] != address[j])
            return FALSE;

        i++;
        j++;
        if((i > rcvPrefixLength + address_len) || (j > address_len))
            return FALSE;
    }
    return TRUE;
}

BOOL compareTypeTagAtIndex(const UINT16 index, const char typetag)
{
    if(index >= rcvArgumentsLength - 1 || rcvArgsTypeArray[index] != typetag)
        return FALSE;

    return TRUE;
}

WORD getArgumentsLength(void)
{
    return rcvArgumentsLength - 1;
}

INT32 getIntArgumentAtIndex(const UINT16 index)
{
    INT16 s = 0;
    INT32 sign = 0, exponent = 0, mantissa = 0;
    INT64 lvalue = 0;
    float fvalue = 0.0;
    float sum = 0.0;

    if(index >= rcvArgumentsLength - 1)
        return 0;

    switch(rcvArgsTypeArray[index])
    {
        case 'i':
            lvalue = ((oscPacket[rcvArgumentsStartIndex[index]] & 0xFF) << 24) |
                     ((oscPacket[rcvArgumentsStartIndex[index] + 1] & 0xFF) << 16) |
                     ((oscPacket[rcvArgumentsStartIndex[index] + 2] & 0xFF) << 8) |
                      (oscPacket[rcvArgumentsStartIndex[index] + 3] & 0xFF);
            break;
        case 'f':
            lvalue = ((oscPacket[rcvArgumentsStartIndex[index]] & 0xFF) << 24) |
                     ((oscPacket[rcvArgumentsStartIndex[index] + 1] & 0xFF) << 16) |
                     ((oscPacket[rcvArgumentsStartIndex[index] + 2] & 0xFF) << 8) |
                      (oscPacket[rcvArgumentsStartIndex[index] + 3] & 0xFF);
            lvalue &= 0xffffffff;

            sign = ((lvalue >> 31) & 0x01) ? -1 : 1;
            exponent = ((lvalue >> 23) & 0xFF) - 127;
            mantissa = lvalue & 0x7FFFFF;

            for(s = 0; s < 23; s++)
            {
                int onebit = (mantissa >> (22 - s)) & 0x1;
                sum += (float)onebit * (1.0 / (float)(1 << (s + 1)));
            }
            sum += 1.0;

            if(exponent >= 0)
                fvalue = sign * sum * (1 << exponent);
            else
                fvalue = sign * sum * (1.0 / (float)(1 << abs(exponent)));
            lvalue = (int)fvalue;
            break;
    }
    return lvalue;
}

float getFloatArgumentAtIndex(const UINT16 index)
{
    INT16 s = 0;
    INT32 sign = 0, exponent = 0, mantissa = 0;
    INT64 lvalue = 0;
    float fvalue = 0.0;
    float sum = 0.0;

    if(index >= rcvArgumentsLength - 1)
        return 0.0;

    switch(rcvArgsTypeArray[index])
    {
        case 'i':
            lvalue = (oscPacket[rcvArgumentsStartIndex[index]] << 24) |
                     (oscPacket[rcvArgumentsStartIndex[index] + 1] << 16) |
                     (oscPacket[rcvArgumentsStartIndex[index] + 2] << 8) |
                      oscPacket[rcvArgumentsStartIndex[index] + 3];
            fvalue = (float)lvalue;
            break;
        case 'f':
            lvalue = ((oscPacket[rcvArgumentsStartIndex[index]] & 0xFF) << 24) |
                     ((oscPacket[rcvArgumentsStartIndex[index] + 1] & 0xFF) << 16) |
                     ((oscPacket[rcvArgumentsStartIndex[index] + 2] & 0xFF) << 8) |
                      (oscPacket[rcvArgumentsStartIndex[index] + 3] & 0xFF);
            lvalue &= 0xffffffff;

            sign = ((lvalue >> 31) & 0x01) ? -1 : 1;
            exponent = ((lvalue >> 23) & 0xFF) - 127;
            mantissa = lvalue & 0x7FFFFF;

            for(s = 0; s < 23; s++)
            {
                int onebit = (mantissa >> (22 - s)) & 0x1;
                sum += (float)onebit * (1.0 / (float)(1 << (s + 1)));
            }
            sum += 1.0;

            if(exponent >= 0)
                fvalue = sign * sum * (1 << exponent);
            else
                fvalue = sign * sum * (1.0 / (float)(1 << abs(exponent)));
            break;
    }
    return fvalue;
}

char* getStringArgumentAtIndex(const UINT16 index)
{
    if(index >= rcvArgumentsLength - 1)
        return "error";

    switch(rcvArgsTypeArray[index])
    {
        case 'i':
        case 'f':
            return "error";
            break;
        case 's':
            return oscPacket + rcvArgumentsStartIndex[index];
            break;
    }
    return "error";
}

BOOL getBooleanArgumentAtIndex(const UINT16 index)
{
    BOOL flag = FALSE;

    if(index >= rcvArgumentsLength - 1)
        return flag;

    switch(rcvArgsTypeArray[index])
    {
        case 'T':
            flag = TRUE;
            break;
        case 'F':
            flag = FALSE;
            break;
    }
    return flag;
}
