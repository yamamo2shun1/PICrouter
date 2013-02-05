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
 * osc.c,v.0.87 2012/11/30
 */

#include "osc.h"

const char msgOnboardLed[]         = "/onboard/led";
const char msgVolumeLed[]          = "/volume/led";
const char msgLatticeLed[]         = "/lattice/led";
const char msgLatticeLedColumn[]   = "/lattice/led_col";
const char msgLatticeLedRow[]      = "/lattice/led_row";
const char msgRotaryLedStep[]      = "/rotary/led/step";
const char msgRotaryLedBits[]      = "/rotary/led/bits";
const char msgRotaryLedIntensity[] = "/rotary/led/intensity";
const char msgRotaryLedAllInt[]    = "/rotary/led/allint";
const char msgRotaryEnc[]          = "/rotary/enc";
const char msgSetRotaryEncStep[]   = "/rotary/enc/step/set";
const char msgRotaryEncSwitch[]    = "/rotary/enc/switch";

//Standard OSC Messages
const char stdPrefix[]      = "/std";
const char msgSetPwmState[] = "/pwm/state/set";
const char msgGetPwmState[] = "/pwm/state/get";
const char msgSetPwmFreq[]  = "/pwm/freq/set";
const char msgGetPwmFreq[]  = "/pwm/freq/get";
const char msgSetPwmDuty[]  = "/pwm/duty/set";
const char msgGetPwmDuty[]  = "/pwm/duty/get";

//OSC Messages converted from MIDI Message
const char midiPrefix[] = "/midi";
const char msgNote[]    = "/note";
const char msgPp[]      = "/pp";
const char msgCc[]      = "/cc";
const char msgPc[]      = "/pc";
const char msgKp[]      = "/kp";
const char msgCp[]      = "/cp";
const char msgPb[]      = "/pb";
const char msgSetNote[]    = "/note/set";
const char msgSetPp[]      = "/pp/set";
const char msgSetCc[]      = "/cc/set";
const char msgSetPc[]      = "/pc/set";
const char msgSetKp[]      = "/kp/set";
const char msgSetCp[]      = "/cp/set";
const char msgSetPb[]      = "/pb/set";

//System OSC Messages for Network Settings
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

UINT16 rcvAddressLength;
UINT16 rcvTypesStartIndex;
INT16 rcvArgumentsLength;
char rcvArgsTypeArray[128] = {0};
UINT16 rcvArgumentsStartIndex[128] = {0};

BOOL openOSCSendPort(UDP_SOCKET sndSocket, BYTE* remoteIP, WORD remotePort)
{
    BOOL flag = FALSE;
    sndSocket = UDPOpenEx((remoteIP[0] | (remoteIP[1] << 8) | (remoteIP[2] << 16) | (remoteIP[3] << 24)), UDP_OPEN_IP_ADDRESS, 0, remotePort);
    if(sndSocket == INVALID_UDP_SOCKET)
	flag = FALSE;
    else
        flag = TRUE;
    return flag;
}

BOOL openOSCReceivePort(UDP_SOCKET rcvSocket, WORD localPort)
{
    BOOL flag = FALSE;
    rcvSocket = UDPOpen(localPort, NULL, 0);
    if(rcvSocket != INVALID_UDP_SOCKET)
        flag = TRUE;
    return flag;
}

void closeOSCSendPort(UDP_SOCKET sndSocket)
{
    UDPClose(sndSocket);
}

void closeOSCReceivePort(UDP_SOCKET rcvSocket)
{
    UDPClose(rcvSocket);
}

void sendOSCMessage(UDP_SOCKET sndSocket, const char* prefix, const char* command, const char* type, ...)
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

	if(UDPIsPutReady(sndSocket))
	{
		do
		{
			if(testSize <= 8)
			{
				zeroSize = (8 - testSize);
				testSize -= 8;
			}
			else
				testSize -= 8;
		} while(testSize > 0);

		if(zeroSize == 0)
			zeroSize = 4;
		else if(zeroSize > 4 && zeroSize < 8)
			zeroSize -= 4;

		testSize1 = typeSize + 1;
		zeroSize1 = 0;
		do
		{
			if(testSize1 <= 4)
			{
				zeroSize1 = (4 - testSize1);
				testSize1 -= 4;
			}
			else
				testSize1 -= 4;
		} while(testSize1 > 0);

		if(zeroSize1 == 0)
			zeroSize1 = 4;

		totalSize = (prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1);// + (typeSize * 4);
		p = type;

		va_start(list, type);

		int ivalue;
		float fvalue;
		char* fchar;
		char* cstr;
		while(*p != '\0')
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
				j = 0;
				do
				{
					if(i < 4)
					{
						i = 0;
						j += 4;
					}
					else if(i == 4)
					{
						i -= 4;
						j += 8;
					}
					else
					{
						i -= 4;
						j += 4;
					}
				} while(i > 0);
				totalSize += j;
			}
			p++;
		}
		va_end(list);

		str = (char *)calloc(totalSize, sizeof(char));
		sprintf(str, "%s%s", prefix, command);

		va_start(list, type);

		sprintf((str + (prefixSize + commandSize + zeroSize)), ",%s", type);

		int index = 0;
		while(*type != '\0')
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
					j = 0;
					do
					{
						if(i < 4)
						{
							i = 0;
							j += 4;
						}
						else if(i == 4)
						{
							i -= 4;
							j += 8;
						}
						else
						{
							i -= 4;
							j += 4;
						}
					} while(i > 0);
					i = 0;
					while(cstr[i] != '\0')
					{
						*(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + i)) = cstr[i] & 0xFF;
						i++;
					}
					index += j;
					break;
			}
			type++;
		}

        UDPPutArray((BYTE *)str, totalSize);
        UDPFlush();

        free(str);
		str = NULL;
		va_end(list);
    }
}

BOOL isEqualToAddress(const char* str, const char* prefix, const char* address)
{
	INT16 i = 0, j, k, n = 0, u = 0, v = 0, length = 0;
	INT16 m = 0;
	char* msg;
    BOOL flag = TRUE;
    BOOL mflag = FALSE;

	while(str[i] != 0x00)
	{
		i++;
		if(i > strlen(str))
		{
			flag = FALSE;
		}
		if(str[i] == 0x00)
		{
			msg = (char *)calloc(i, sizeof(char));
			memcpy(msg, str, i);
			mflag = TRUE;
		}
	}

    if(!flag)
    {
    	if(mflag)
    	{
    		free(msg);
    		msg = NULL;
    	}
    	return FALSE;
    }

	rcvAddressLength = i;

	for(j = 0; j < rcvAddressLength; j++)
    {
        if(j < strlen(prefix))
        {
        	if(msg[j] == prefix[j])
            	flag = TRUE;
        	else
        	{
            	flag = FALSE;
            	break;
        	}
        }
        else if(j >= strlen(prefix))
        {
        	if(msg[j] == address[j - strlen(prefix)])
            	flag = TRUE;
        	else
        	{
            	flag = FALSE;
            	break;
        	}
        }
        else if(j >= strlen(prefix) + strlen(address))
        {
        	flag = FALSE;
        	break;
        }
    }
    if(mflag)
    {
    	free(msg);
    	msg = NULL;
    }
    if(!flag)
    	return FALSE;

	while(str[i] != 0x2C)
		i++;
	j = i;
	rcvTypesStartIndex = j;

	while(str[i] != 0x00)
		i++;
	for(k = 0; k < i - j - 1; k++)
		rcvArgsTypeArray[k] = str[k + j + 1];
	rcvArgumentsLength = i - j;

	m = rcvArgumentsLength;
	do
	{
		n += 4;
		m -= 4;
	} while(m >= 0);

	for(k = 0; k < rcvArgumentsLength - 1; k++)
	{
		rcvArgumentsStartIndex[k] = rcvTypesStartIndex + length + n;
		switch(rcvArgsTypeArray[k])
		{
			case 'i':
			case 'f':
				length += 4;
				break;
			case 's':
				u = 0;
				while(str[rcvTypesStartIndex + n + length + u] != '\0')
					u++;
				v = 0;
				do
				{
					if(u < 8)
					{
						u = 0;
						v += 8;
					}
					else
					{
						u -= 8;
						v += 8;
					}
				} while(u > 0);
				length += v;
				break;
		}
	}
    return TRUE;
}

INT32 getIntArgumentAtIndex(const char* str, const char* prefix, const char* address, const UINT16 index)
{
	INT16 s = 0;
    INT16 m = rcvArgumentsLength;
	INT32 sign, exponent, mantissa;
	INT64 lvalue;
	float fvalue;
	float sum;

	if(index >= rcvArgumentsLength - 1)
		return 0;

	switch(rcvArgsTypeArray[index])
	{
		case 'i':
			lvalue = ((str[rcvArgumentsStartIndex[index] + 0] & 0xFF) << 24) |
				     ((str[rcvArgumentsStartIndex[index] + 1] & 0xFF) << 16) |
				     ((str[rcvArgumentsStartIndex[index] + 2] & 0xFF) << 8) |
				      (str[rcvArgumentsStartIndex[index] + 3] & 0xFF);
			break;
		case 'f':
			lvalue = ((str[rcvArgumentsStartIndex[index] + 0] & 0xFF) << 24) |
					 ((str[rcvArgumentsStartIndex[index] + 1] & 0xFF) << 16) |
					 ((str[rcvArgumentsStartIndex[index] + 2] & 0xFF) << 8) |
					 (str[rcvArgumentsStartIndex[index] + 3] & 0xFF);
			lvalue &= 0xffffffff;

			sign = ((lvalue >> 33) & 0x01) ? 1 : -1;
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

float getFloatArgumentAtIndex(const char* str, const char* prefix, const char* address, const UINT16 index)
{
	INT16 s = 0;
    INT16 m = rcvArgumentsLength;
	INT32 sign, exponent, mantissa;
	INT64 lvalue;
	float fvalue;
	float sum;

	if(index >= rcvArgumentsLength - 1)
		return 0.0;

	switch(rcvArgsTypeArray[index])
	{
		case 'i':
			lvalue = (str[rcvArgumentsStartIndex[index] + 0] << 24) |
					 (str[rcvArgumentsStartIndex[index] + 1] << 16) |
					 (str[rcvArgumentsStartIndex[index] + 2] << 8) |
					 str[rcvArgumentsStartIndex[index] + 3];
			fvalue = (float)lvalue;
			break;
		case 'f':
			lvalue = ((str[rcvArgumentsStartIndex[index] + 0] & 0xFF) << 24) |
					 ((str[rcvArgumentsStartIndex[index] + 1] & 0xFF) << 16) |
					 ((str[rcvArgumentsStartIndex[index] + 2] & 0xFF) << 8) |
					 (str[rcvArgumentsStartIndex[index] + 3] & 0xFF);
			lvalue &= 0xffffffff;

			sign = ((lvalue >> 33) & 0x01) ? 1 : -1;
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

char* getStringArgumentAtIndex(const char* str, const char* prefix, const char* address, const UINT16 index)
{
	INT16 k = 0, n = 0, s = 0, u = 0, v = 0, length = 0;
    INT16 m = rcvArgumentsLength;
	char* cstr;

	do
	{
		n += 4;
		m -= 4;
	} while(m >= 0);

	if(index >= rcvArgumentsLength - 1)
		return 0;

	length = 0;
	for(k = 0; k <= index; k++)
	{
		switch(rcvArgsTypeArray[k])
		{
			case 'i':
			case 'f':
				if(k != index)
					length += 4;
				break;
			case 's':
				u = 0;
				while(str[rcvTypesStartIndex + n + length + u] != '\0')
					u++;
				v = 0;
				do
				{
					if(u < 8)
					{
						u = 0;
						v += 8;
					}
					else
					{
						u -= 8;
						v += 8;
					}
				} while(u > 0);
				if(k != index)
					length += v;
				break;
		}
	}

	switch(rcvArgsTypeArray[index])
	{
		case 'i':
		case 'f':
			cstr = (char *)calloc(4, sizeof(char));
			memcpy(cstr, str + rcvTypesStartIndex + n + length, 4);
			break;
		case 's':
			cstr = (char *)calloc(v, sizeof(char));
			memcpy(cstr, str + rcvTypesStartIndex + n + length, v);
			break;
	}
	return cstr;
}
