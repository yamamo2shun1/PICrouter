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
 * osc.c,v.0.9.23 2013/03/30
 */

#include "osc.h"

// Network
APP_CONFIG AppConfig;
UDP_SOCKET RxSocket;
UDP_SOCKET TxSocket;
BOOL initReceiveFlag = FALSE;
BOOL initSendFlag = FALSE;
BOOL chCompletedFlag = FALSE;
char* hostName = NULL;
char* stdPrefix = NULL;

// Remote IP Address Initialization
//BYTE remoteIP[] = {192ul, 168ul, 1ul, 255ul};
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

BYTE oscPacket[MAX_PACKET_SIZE] = {0};
char rcvAddressStrings[128] = {0};
UINT16 rcvAddressLength;
UINT16 rcvTypesStartIndex;
INT16 rcvArgumentsLength;
char rcvArgsTypeArray[128] = {0};
UINT16 rcvArgumentsStartIndex[128] = {0};
UINT16 rcvArgumentsIndexLength[128] = {0};

// Network Setting Initialization(IP Address, MAC Address and so on)
//static void InitAppConfig(void)
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

BOOL isOSCGetReady(WORD len)
{
    len = UDPIsGetReady(RxSocket);
    if(!len)
        return FALSE;

    return TRUE;
}

BOOL isOSCPutReady(void)
{
    WORD puttablePacketSize = 0;
    puttablePacketSize = UDPIsPutReady(TxSocket);
    if(!puttablePacketSize)
        return FALSE;

    return TRUE;
}

void getOSCPacket(void)
{
    INT16 i = 0, j = 0, n = 0, u = 0, v = 0, length = 0;
    WORD r_size = 0;

    r_size = UDPGetArray(oscPacket, MAX_PACKET_SIZE);
    if(!r_size && *(oscPacket + 0) != '/')
        return;

    memset(rcvAddressStrings, 0, sizeof(rcvAddressStrings));

    while(*(oscPacket + i))
    {
        i++;
        if(i >= MAX_PACKET_SIZE)
            return;
    }
    memcpy(rcvAddressStrings, oscPacket, i);

    rcvAddressLength = i;

    while(*(oscPacket + i) != ',')
        i++;
    rcvTypesStartIndex = i;

    i++;
    while(*(oscPacket + i))
    {
        j++;
        i++;
    }
    memcpy(rcvArgsTypeArray, oscPacket + rcvTypesStartIndex + 1, j);

    rcvArgumentsLength = i - rcvTypesStartIndex;
    n = ((rcvArgumentsLength / 4) + 1) * 4;

    for(j = 0; j < rcvArgumentsLength - 1; j++)
    {
        *(rcvArgumentsStartIndex + j) = rcvTypesStartIndex + length + n;
        switch(*(rcvArgsTypeArray + j))
        {
            case 'i':
            case 'f':
                length += 4;
                *(rcvArgumentsIndexLength + j) = 4;
                break;
            case 's':
                u = 0;
                while(*(oscPacket + (rcvTypesStartIndex + n + length + u)))
                    u++;

                if((u % 4) == 0)
                    v = (u / 4) * 8;
                else
                    v = ((u / 4) + 1) * 4;

                length += v;
                *(rcvArgumentsIndexLength + j) = v;
                break;
            case 'T':
            case 'F':
            case 'N':
            case 'I':
                break;
            default:
                break;
        }
    }
    UDPDiscard();
}

void sendOSCMessage(const char* prefix, const char* command, const char* type, ...)
{
    INT16 i, j;
    va_list list;
    char* str;
    const char *p;
    INT32 strSize, testSize, zeroSize, testSize1, zeroSize1, totalSize;
    INT32 prefixSize = strchr(prefix, 0) - prefix;
    INT32 commandSize = strchr(command, 0) - command;
    INT32 typeSize = strchr(type, 0) - type;

    strSize = prefixSize + commandSize;
    testSize = strSize;
    zeroSize = 0;

    if(isOSCPutReady())
    {
        //debug LED_1_On();
        zeroSize = (testSize % 8) == 0 ? 0 : 8 - (testSize % 8);

        if(zeroSize == 0)
            zeroSize = 4;
        else if(zeroSize > 4 && zeroSize < 8)
            zeroSize -= 4;

        testSize1 = typeSize + 1;
        zeroSize1 = (testSize1 % 4) == 0 ? 0 : 4 - (testSize1 % 4);

        if(zeroSize1 == 0)
            zeroSize1 = 4;

        totalSize = (prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1);
        p = type;

        va_start(list, type);

        int ivalue;
        float fvalue;
        char* fchar;
        char* cstr;
        while(*p)
        {
            if(*p == 'i')
            {
                ivalue = va_arg(list, int);
                totalSize += 4;
            }
            else if(*p == 'f')
            {
                fvalue = va_arg(list, double);
                totalSize += 4;
            }
            else if(*p == 's')
            {
                cstr = va_arg(list, char*);
                i = 0;
                while(cstr[i] != '\0')
                    i++;

                if((i % 4) == 0)
                    j = (i / 4) * 8;
                else
                    j = ((i / 4) + 1) * 4;

                totalSize += j;
            }
            else if(*p == 'T')
            {
            }
            else if(*p == 'F')
            {
            }
            p++;
        }
        va_end(list);

        str = (char *)calloc(totalSize, sizeof(char));
        sprintf(str, "%s%s", prefix, command);

        va_start(list, type);

        sprintf((str + (prefixSize + commandSize + zeroSize)), ",%s", type);

        int index = 0;
        while(*type)
        {
            switch(*type)
            {
                case 'i':
                    ivalue = va_arg(list, int);
                    *(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 0)) = (ivalue >> 24) & 0xFF;
                    *(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 1)) = (ivalue >> 16) & 0xFF;
                    *(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 2)) = (ivalue >> 8) & 0xFF;
                    *(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 3)) = (ivalue >> 0) & 0xFF;
                    index += 4;
                    break;
                case 'f':
                    fvalue = (float)va_arg(list, double);
                    fchar = (char *)&fvalue;
                    *(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 0)) = fchar[3] & 0xFF;
                    *(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 1)) = fchar[2] & 0xFF;
                    *(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 2)) = fchar[1] & 0xFF;
                    *(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 3)) = fchar[0] & 0xFF;
                    index += 4;
                    break;
                case 's':
                    cstr = va_arg(list, char*);
                    i = 0;
                    while(cstr[i] != '\0')
                        i++;

                    if((i % 4) == 0)
                        j = (i / 4) * 8;
                    else
                        j = ((i / 4) + 1) * 4;

                    i = 0;
                    while(cstr[i] != '\0')
                    {
                        *(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + i)) = cstr[i] & 0xFF;
                        i++;
                    }
                    index += j;
                    break;
                case 'T':
                case 'F':
                case 'N':
                case 'I':
                    break;
                default:
                    break;
            }
            type++;
        }

        UDPPutArray((BYTE *)str, totalSize);
        UDPFlush();

        free(str);
        str = NULL;
        va_end(list);

        //debug LED_1_Off();
    }
}

BOOL compareOSCPrefix(const char* prefix)
{
    char* str = rcvAddressStrings;
    if(strlen(str) < strlen(prefix))
        return FALSE;
    while(*prefix != NULL)
    {
        if(*str++ != *prefix++)
            return FALSE;
    }
    return TRUE;
}

BOOL compareOSCAddress(const char* prefix, const char* address)
{
    char* str = rcvAddressStrings;

    if(strlen(str) > strlen(prefix) + strlen(address))
        return FALSE;

    str += strlen(prefix);

    while(*address != NULL)
    {
        if(*str++ != *address++)
            return FALSE;
    }
    return TRUE;
}

BOOL compareTypeTagAtIndex(const UINT16 index, const char typetag)
{
    if(index >= rcvArgumentsLength - 1 || *(rcvArgsTypeArray + index) != typetag ||
       (*(rcvArgsTypeArray + index) != 'i' && *(rcvArgsTypeArray + index) != 'f' && *(rcvArgsTypeArray + index) != 's' &&
        *(rcvArgsTypeArray + index) != 'T' && *(rcvArgsTypeArray + index) != 'F' && *(rcvArgsTypeArray + index) != 'N' &&
        *(rcvArgsTypeArray + index) != 'I'))
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
    INT32 sign, exponent, mantissa;
    INT64 lvalue;
    float fvalue;
    float sum;

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

            sum = 0.0;
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
    INT32 sign, exponent, mantissa;
    INT64 lvalue;
    float fvalue;
    float sum;

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

            //sign = ((lvalue >> 33) & 0x01) ? 1 : -1;
            sign = ((lvalue >> 31) & 0x01) ? -1 : 1;
            exponent = ((lvalue >> 23) & 0xFF) - 127;
            mantissa = lvalue & 0x7FFFFF;

            sum = 0.0;
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
    char* cstr;

    if(index >= rcvArgumentsLength - 1)
        return "error";

    switch(*(rcvArgsTypeArray + index))
    {
        case 'i':
        case 'f':
            return "error";
            break;
        case 's':
            cstr = (char *)calloc(*(rcvArgumentsIndexLength + index), sizeof(char));
            memcpy(cstr, oscPacket + *(rcvArgumentsStartIndex + index), *(rcvArgumentsIndexLength + index));
            break;
    }
    return cstr;
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
