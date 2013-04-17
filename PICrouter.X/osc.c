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
 * osc.c,v.1.0.0 2013/04/17
 */

#include "osc.h"

// Network
APP_CONFIG AppConfig;
static UDP_SOCKET RxSocket;
static UDP_SOCKET TxSocket;
BOOL initReceiveFlag = FALSE;
BOOL initSendFlag = FALSE;
BOOL chCompletedFlag = FALSE;
char* hostName = NULL;
char* stdPrefix = NULL;

// Remote IP Address Initialization
BYTE remoteIP[] = {224ul, 0ul, 0ul, 1ul};

// Port Number Initialization
WORD remotePort = 8000;
WORD localPort  = 8080;

// MAC address Initialization
static ROM BYTE SerializedMACAddress[6] = {MY_DEFAULT_MAC_BYTE1,
                                           MY_DEFAULT_MAC_BYTE2,
                                           MY_DEFAULT_MAC_BYTE3,
                                           MY_DEFAULT_MAC_BYTE4,
                                           MY_DEFAULT_MAC_BYTE5,
                                           MY_DEFAULT_MAC_BYTE6};

// for LED_PAD_16 or LED_PAD_64 (6)
const char msgLatticePad[]       = "/lattice/pad";
const char msgLatticeLed[]       = "/lattice/led";
const char msgLatticeLedColumn[] = "/lattice/led/col";
const char msgLatticeLedRow[]    = "/lattice/led/row";
const char msgLatticeLedAll[]    = "/lattice/led/all";
const char msgLatticeLedClear[]  = "/lattice/led/clear";

// for LED_ENC_32 or LED_ENC_ABS_32 (6)
const char msgRotaryLedStep[]      = "/rotary/led/step";
const char msgRotaryLedBits[]      = "/rotary/led/bits";
const char msgRotaryLedIntensity[] = "/rotary/led/intensity";
const char msgRotaryLedAllInt[]    = "/rotary/led/allint";
const char msgRotaryEnc[]          = "/rotary/enc";
const char msgSetRotaryEncStep[]   = "/rotary/enc/step/set";
// for ONLY LED_ENC_32 (1)
const char msgRotaryEncSwitch[]    = "/rotary/enc/switch";

//Standard OSC Messages
// for Onboard (2)
const char msgOnboardLed[] = "/onboard/led";
const char msgOnboardSw1[] = "/onboard/sw1";

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
const char msgAdcDio[]       = "/adc/dio";
const char msgSetAdcDio[]    = "/adc/dio/set";
const char msgGetAdcDio[]    = "/adc/dio/get";
const char msgSetAdcDo[]     = "/adc/dout/set";
const char msgAdcDi[]        = "/adc/din";
const char msgGetAdcDi[]     = "/adc/din/get";
// for PWM (10)
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
// for DIO (4)
const char msgDigitalDio[]    = "/digital/dio";
const char msgSetDigitalDio[] = "/digital/dio/set";
const char msgGetDigitalDio[] = "/digital/dio/get";
const char msgSetDigitalDo[]  = "/digital/dout/set";
const char msgDigitalDi[]     = "/digital/din";
const char msgGetDigitalDi[]  = "/digital/din/get";
// for SPI (7)
const char msgSetSpiConfig[] = "/spi/config/set";
const char msgSpiData[]      = "/spi/data";
const char msgSetSpiData[]   = "/spi/data/set";
const char msgGetSpiData[]   = "/spi/data/get";
const char msgSpiDio[]       = "/spi/dio";
const char msgSetSpiDio[]    = "/spi/dio/set";
const char msgGetSpiDio[]    = "/spi/dio/get";
const char msgSetSpiDo[]     = "/spi/dout/set";
const char msgSpiDi[]        = "/spi/din";
const char msgGetSpiDi[]     = "/spi/din/get";

//OSC Messages converted from MIDI Message (15)
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

//System OSC Messages for Network Settings (25)
const char sysPrefix[]        = "/sys";
const char msgPrefix[]        = "/prefix";
const char msgSetPrefix[]     = "/prefix/set";
const char msgGetPrefix[]     = "/prefix/get";
const char msgRemoteIp[]      = "/remote/ip";
const char msgSetRemoteIp[]   = "/remote/ip/set";
const char msgGetRemoteIp[]   = "/remote/ip/get";
const char msgRemotePort[]    = "/remote/port";
const char msgSetRemotePort[] = "/remote/port/set";
const char msgGetRemotePort[] = "/remote/port/get";
const char msgHostName[]      = "/host/name";
const char msgSetHostName[]   = "/host/name/set";
const char msgGetHostName[]   = "/host/name/get";
const char msgHostIp[]        = "/host/ip";
const char msgGetHostIp[]     = "/host/ip/get";
const char msgHostMac[]       = "/host/mac";
const char msgGetHostMac[]    = "/host/mac/get";
const char msgHostPort[]      = "/host/port";
const char msgSetHostPort[]   = "/host/port/set";
const char msgGetHostPort[]   = "/host/port/get";
const char msgUsbMode[]       = "/usb/mode";
const char msgSwitchUsbMode[] = "/usb/mode/switch";
const char msgGetUsbMode[]    = "/usb/mode/get";
const char msgSoftReset[]     = "/soft/reset";
const char msgConfiguration[] = "/configuration";
const char msgDebug[]         = "/debug";
const char msgError[]         = "/error";
const char msgVersion[]       = "/version";
const char msgGetVersion[]    = "/version/get";

// for touchOSC
const char toscPrefix[]      = "/tosc";

const char msgOnboardLed1[]  = "/onboard/led/1";
const char msgOnboardLed2[]  = "/onboard/led/2";

const char msgSetPwmEnable1[] = "/pwm/enable/set/1";
const char msgSetPwmEnable2[] = "/pwm/enable/set/2";
const char msgSetPwmEnable3[] = "/pwm/enable/set/3";
const char msgSetPwmEnable4[] = "/pwm/enable/set/4";
const char msgSetPwmDuty1[]   = "/pwm/duty/set/1";
const char msgSetPwmDuty2[]   = "/pwm/duty/set/2";
const char msgSetPwmDuty3[]   = "/pwm/duty/set/3";
const char msgSetPwmDuty4[]   = "/pwm/duty/set/4";

// Static Variables
static volatile BYTE state_index = 0;
static volatile BYTE indexA = 0;
static volatile BYTE indexB = 0;
static volatile BYTE ringBufIndex = 0;
static volatile BYTE ringProcessIndex = 0;
static BYTE udpPacket[MAX_BUF_SIZE][MAX_PACKET_SIZE] = {0};
static BYTE oscPacket[MAX_PACKET_SIZE] = {0};
static char rcvAddressStrings[MAX_ADDRESS_LEN] = {0};
static UINT16 rcvAddressLength;
static UINT16 rcvTypesStartIndex;
static INT16 rcvArgumentsLength;
static char rcvArgsTypeArray[MAX_ARGS_LEN] = {0};
static UINT16 rcvArgumentsStartIndex[MAX_ARGS_LEN] = {0};
static UINT16 rcvArgumentsIndexLength[MAX_ARGS_LEN] = {0};
static BYTE bundleData[512] = {0};
static UINT16 bundleAppendIndex = 0;

// Static Functions
static BOOL copyOSCPacketFromUDPPacket();
static BOOL extractAddressFromOSCPacket();
static BOOL extractTypeTagFromOSCPacket();
static BOOL extractArgumentsFromOSCPacket();

// Network Setting Initialization(IP Address, MAC Address and so on)
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
}

void setOSCPrefix(char* prefix_string)
{
    stdPrefix = (char *)calloc(strlen(prefix_string), sizeof(char));
    memcpy(stdPrefix, prefix_string, strlen(prefix_string));
}
void setOSCHostName(char* host_name)
{
    hostName = (char *)calloc(strlen(host_name), sizeof(char));
    memcpy(hostName, host_name, strlen(host_name));
}

BOOL openOSCSendPort(BYTE* ip_address, WORD port_number)
{
#if 0
    TxSocket = UDPOpenEx((ip_address[0] | (ip_address[1] << 8) | (ip_address[2] << 16) | (ip_address[3] << 24)), UDP_OPEN_IP_ADDRESS, 0, port_number);
#else
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
#endif

    if(TxSocket == INVALID_UDP_SOCKET)
        return FALSE;

    return TRUE;
}

BOOL openOSCReceivePort(WORD port_number)
{
    RxSocket = UDPOpen(port_number, NULL, 0);
#if 0
    NODE_INFO mcRemote;

    mcRemote.IPAddr.v[0] = 224;
    mcRemote.IPAddr.v[1] = 0;
    mcRemote.IPAddr.v[2] = 0;
    mcRemote.IPAddr.v[3] = 1;

    mcRemote.MACAddr.v[0] = 0x01;
    mcRemote.MACAddr.v[1] = 0x00;
    mcRemote.MACAddr.v[2] = 0x5E;
    mcRemote.MACAddr.v[3] = 0x00;
    mcRemote.MACAddr.v[4] = 0x00;
    mcRemote.MACAddr.v[5] = 0x01;

    RxSocket = UDPOpen(port_number, (PTR_BASE)&mcRemote , 0);
#endif

    if(RxSocket == INVALID_UDP_SOCKET)
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

void closeOSCSendPort(void)
{
    initSendFlag = FALSE;
    UDPClose(TxSocket);
    TxSocket = NULL;
}

void closeOSCReceivePort(void)
{
    initReceiveFlag = FALSE;
    UDPClose(RxSocket);
    RxSocket = NULL;
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

void getOSCPacket(void)
{
    if(!initReceiveFlag)
        initReceiveFlag = openOSCReceivePort(localPort);
    
    if(initReceiveFlag && isOSCGetReady())
    {
        if(UDPGetArray(udpPacket[ringBufIndex], MAX_PACKET_SIZE) > 0)
        {
            ringBufIndex++;
            if(ringBufIndex >= MAX_BUF_SIZE)
                ringBufIndex = 0;
        }
        UDPDiscard();
    }
}

BOOL processOSCPacket(void)
{
    switch(state_index)
    {
        case 0:
            indexA = 0;
            indexB = 0;
            if(!copyOSCPacketFromUDPPacket())
                return FALSE;
            state_index = 1;
            break;
        case 1:
            if(!extractAddressFromOSCPacket())
                return FALSE;

            state_index = 2;
            break;
        case 2:
            if(!extractTypeTagFromOSCPacket())
                return FALSE;

            state_index = 3;
            break;
        case 3:
            if(!extractArgumentsFromOSCPacket())
                return FALSE;

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
    BYTE i, j;
    DWORD len = 0;

    if(!strcmp(udpPacket[ringProcessIndex], "#bundle"))
    {
        j = 16;
        while(TRUE)
        {
            len = 0;
            len += (DWORD)udpPacket[ringProcessIndex][j] << 24;
            len += (DWORD)udpPacket[ringProcessIndex][j + 1] << 16;
            len += (DWORD)udpPacket[ringProcessIndex][j + 2] << 8;
            len += (DWORD)udpPacket[ringProcessIndex][j + 3];

            if(len == 0)
                break;

            //memcpy(oscPacket, &udpPacket[ringProcessIndex][20], len);
            for(i = 0; i < len; i++)
            {
                //oscPacket[i] = udpPacket[ringProcessIndex][20 + i];
                udpPacket[ringBufIndex][i] = udpPacket[ringProcessIndex][j + 4 + i];
            }
            j += (4 + len);

            ringBufIndex++;
            if(ringBufIndex >= MAX_BUF_SIZE)
                ringBufIndex = 0;
        }

        memset(udpPacket[ringProcessIndex], 0, MAX_PACKET_SIZE);
        ringProcessIndex++;
        if(ringProcessIndex >= MAX_BUF_SIZE)
            ringProcessIndex = 0;

    }

    if(udpPacket[ringProcessIndex][0] != '/')
    {
        if(ringProcessIndex != ringBufIndex)
        {
            ringProcessIndex++;
            if(ringProcessIndex >= MAX_BUF_SIZE)
                ringProcessIndex = 0;
        }
        return FALSE;
    }

    memcpy(oscPacket, udpPacket[ringProcessIndex], MAX_PACKET_SIZE);
    memset(udpPacket[ringProcessIndex], 0, MAX_PACKET_SIZE);
    ringProcessIndex++;
    if(ringProcessIndex >= MAX_BUF_SIZE)
        ringProcessIndex = 0;

    return TRUE;
}

static BOOL extractAddressFromOSCPacket()
{
    memset(rcvAddressStrings, 0, sizeof(rcvAddressStrings));

    while(*(oscPacket + indexA))
    {
        indexA++;
        if(indexA >= MAX_PACKET_SIZE)
            return FALSE;
    }
    memcpy(rcvAddressStrings, oscPacket, indexA);

    rcvAddressLength = indexA;

    return TRUE;
}

static BOOL extractTypeTagFromOSCPacket()
{
    while(*(oscPacket + indexA) != ',')
    {
        indexA++;
        if(indexA >= MAX_PACKET_SIZE)
            return FALSE;
    }
    rcvTypesStartIndex = indexA;
    indexA++;

    while(*(oscPacket + indexA))
    {
        indexB++;
        indexA++;
        if(indexA >= MAX_PACKET_SIZE)
            return FALSE;
    }
    memcpy(rcvArgsTypeArray, oscPacket + rcvTypesStartIndex + 1, indexB);

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
        *(rcvArgumentsStartIndex + i) = rcvTypesStartIndex + length + n;
        switch(*(rcvArgsTypeArray + i))
        {
            case 'i':
            case 'f':
                length += 4;
                *(rcvArgumentsIndexLength + i) = 4;
                break;
            case 's':
                u = 0;
                while(*(oscPacket + (*(rcvArgumentsStartIndex + i) + u)))
                {
                    u++;
                    if((*(rcvArgumentsStartIndex + i) + u) >= MAX_PACKET_SIZE)
                        return FALSE;
                }

                *(rcvArgumentsIndexLength + i) = ((u / 4) + 1) * 4;

                length += *(rcvArgumentsIndexLength + i);
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
    char str[128];
    memset(str, 0, 128);

    if(isOSCPutReady())
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
                    *(str + (totalSize++)) = (ivalue >> 24) & 0xFF;
                    *(str + (totalSize++)) = (ivalue >> 16) & 0xFF;
                    *(str + (totalSize++)) = (ivalue >> 8) & 0xFF;
                    *(str + (totalSize++)) = (ivalue >> 0) & 0xFF;
                    break;
                case 'f':
                    fvalue = (float)va_arg(list, double);
                    fchar = (char *)&fvalue;
                    *(str + (totalSize++)) = *(fchar + 3) & 0xFF;
                    *(str + (totalSize++)) = *(fchar + 2) & 0xFF;
                    *(str + (totalSize++)) = *(fchar + 1) & 0xFF;
                    *(str + (totalSize++)) = *(fchar + 0) & 0xFF;
                    break;
                case 's':
                    cstr = va_arg(list, char*);
                    cstr_len = 0;
                    while(*cstr)
                    {
                        *(str + (totalSize + cstr_len)) = *(cstr++) & 0xFF;
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

        //debug LED_1_Off();
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
    char str[128];
    memset(str, 0, 128);

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
                *(str + (totalSize++)) = (ivalue >> 24) & 0xFF;
                *(str + (totalSize++)) = (ivalue >> 16) & 0xFF;
                *(str + (totalSize++)) = (ivalue >> 8) & 0xFF;
                *(str + (totalSize++)) = (ivalue >> 0) & 0xFF;
                break;
            case 'f':
                fvalue = (float)va_arg(list, double);
                fchar = (char *)&fvalue;
                *(str + (totalSize++)) = *(fchar + 3) & 0xFF;
                *(str + (totalSize++)) = *(fchar + 2) & 0xFF;
                *(str + (totalSize++)) = *(fchar + 1) & 0xFF;
                *(str + (totalSize++)) = *(fchar + 0) & 0xFF;
                break;
            case 's':
                cstr = va_arg(list, char*);
                cstr_len = 0;
                while(*cstr)
                {
                    *(str + (totalSize + cstr_len)) = *(cstr++) & 0xFF;
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

    *(bundleData + (bundleAppendIndex++)) = (totalSize >> 24) & 0xFF;
    *(bundleData + (bundleAppendIndex++)) = (totalSize >> 16) & 0xFF;
    *(bundleData + (bundleAppendIndex++)) = (totalSize >> 8) & 0xFF;
    *(bundleData + (bundleAppendIndex++)) = (totalSize >> 0) & 0xFF;
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
    BYTE i = 0;
    WORD prefix_len = strlen(prefix);

    if(rcvAddressLength < prefix_len)
        return FALSE;

    while(*(prefix + i))
    {
        if(*(rcvAddressStrings + i) != *(prefix + i))
            return FALSE;

        i++;
        if(i > prefix_len)
            return FALSE;
    }
    return TRUE;
}

BOOL compareOSCAddress(const char* prefix, const char* address)
{
    BYTE i = 0, j = 0;
    WORD prefix_len = strlen(prefix);
    WORD address_len = strlen(address);

    if(rcvAddressLength > prefix_len + address_len)
        return FALSE;

    i = prefix_len;

    while(*(address + j))
    {
        if(*(rcvAddressStrings + i) != *(address + j))
            return FALSE;

        i++;
        j++;
        if((i > prefix_len + address_len) || (j > address_len))
            return FALSE;
    }
    return TRUE;
}

BOOL compareTypeTagAtIndex(const UINT16 index, const char typetag)
{
    //if(index >= rcvArgumentsLength - 1 || *(rcvArgsTypeArray + index) != typetag ||
    //   (*(rcvArgsTypeArray + index) != 'i' && *(rcvArgsTypeArray + index) != 'f' && *(rcvArgsTypeArray + index) != 's' &&
    //    *(rcvArgsTypeArray + index) != 'T' && *(rcvArgsTypeArray + index) != 'F' && *(rcvArgsTypeArray + index) != 'N' &&
    //    *(rcvArgsTypeArray + index) != 'I'))
    if(index >= rcvArgumentsLength - 1 || *(rcvArgsTypeArray + index) != typetag)
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

    switch(*(rcvArgsTypeArray + index))
    {
        case 'i':
            lvalue = ((*(oscPacket + *(rcvArgumentsStartIndex + index) + 0) & 0xFF) << 24) |
                     ((*(oscPacket + *(rcvArgumentsStartIndex + index) + 1) & 0xFF) << 16) |
                     ((*(oscPacket + *(rcvArgumentsStartIndex + index) + 2) & 0xFF) << 8) |
                      (*(oscPacket + *(rcvArgumentsStartIndex + index) + 3) & 0xFF);
            break;
        case 'f':
            lvalue = ((*(oscPacket + *(rcvArgumentsStartIndex + index) + 0) & 0xFF) << 24) |
                     ((*(oscPacket + *(rcvArgumentsStartIndex + index) + 1) & 0xFF) << 16) |
                     ((*(oscPacket + *(rcvArgumentsStartIndex + index) + 2) & 0xFF) << 8) |
                      (*(oscPacket + *(rcvArgumentsStartIndex + index) + 3) & 0xFF);
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

    switch(*(rcvArgsTypeArray + index))
    {
        case 'i':
            lvalue = (*(oscPacket + *(rcvArgumentsStartIndex + index) + 0) << 24) |
                     (*(oscPacket + *(rcvArgumentsStartIndex + index) + 1) << 16) |
                     (*(oscPacket + *(rcvArgumentsStartIndex + index) + 2) << 8) |
                      *(oscPacket + *(rcvArgumentsStartIndex + index) + 3);
            fvalue = (float)lvalue;
            break;
        case 'f':
            lvalue = ((*(oscPacket + *(rcvArgumentsStartIndex + index) + 0) & 0xFF) << 24) |
                     ((*(oscPacket + *(rcvArgumentsStartIndex + index) + 1) & 0xFF) << 16) |
                     ((*(oscPacket + *(rcvArgumentsStartIndex + index) + 2) & 0xFF) << 8) |
                      (*(oscPacket + *(rcvArgumentsStartIndex + index) + 3) & 0xFF);
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

    switch(*(rcvArgsTypeArray + index))
    {
        case 'i':
        case 'f':
            return "error";
            break;
        case 's':
            return oscPacket + *(rcvArgumentsStartIndex + index);
            break;
    }
    return "error";
}

BOOL getBooleanArgumentAtIndex(const UINT16 index)
{
    BOOL flag = FALSE;

    if(index >= rcvArgumentsLength - 1)
        return flag;

    switch(*(rcvArgsTypeArray + index))
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