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
 * osc.c,v.0.4 2012/08/22
 */

#include "osc.h"

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

#if 0
void sendOSCMessage(UDP_SOCKET sndSocket, char* prefix, char* command, char* type, ...)
{
	UINT16 i;
	va_list list;
	char* str;
	char* p;
	INT32 strSize, testSize, zeroSize, testSize1, zeroSize1, totalSize;
	INT32 prefixSize = strchr(prefix, 0) - prefix;
	INT32 commandSize = strchr(command, 0) - command;
	INT32 typeSize = strchr(type, 0) - type;

	//sprintf(str, "%s%s", prefix, command);
	strSize = prefixSize + commandSize;
	testSize = strSize;
	zeroSize = 0;
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
	while(*p != '\0')
	{
		if(*p == 'i' || *p == 'f')
			totalSize += 4;
		p++;
	}

	//str = (char *)malloc(sizeof(char) * totalSize);
	str = (char *)calloc(totalSize, sizeof(char));
	sprintf(str, "%s%s", prefix, command);

	//printf("combined string = %s : %ld %ld\n", str, strSize, zeroSize);
	//printf("type string = %s : %ld %ld\n", type, typeSize + 1, zeroSize1);

	va_start(list, type);

	sprintf((str + (prefixSize + commandSize + zeroSize)), ",%s", type);

	int index = 0;
	int ivalue;
	float fvalue;
	char* fchar;
	while(*type != '\0')
	{
		switch(*type)
		{
			case 'i':
				ivalue = va_arg(list, int);
				//printf("int=%d:0x%X 0x%X 0x%X 0x%X\n", ivalue, (ivalue  >> 24) & 0xFF, (ivalue >> 16) & 0xFF, (ivalue >> 8) & 0xFF, ivalue & 0xFF);
				*(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 0)) = (ivalue >> 24) & 0xFF;
				*(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 1)) = (ivalue >> 16) & 0xFF;
				*(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 2)) = (ivalue >> 8) & 0xFF;
				*(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 3)) = (ivalue >> 0) & 0xFF;
				break;
			case 'f':
				fvalue = (float)va_arg(list, double);
				fchar = (char *)&fvalue;
				//printf("float=%f:0x%X 0x%X 0x%X 0x%X\n", fvalue, fchar[3] & 0xFF, fchar[2] & 0xFF, fchar[1] & 0xFF, fchar[0] & 0xFF);
				*(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 0)) = fchar[3] & 0xFF;
				*(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 1)) = fchar[2] & 0xFF;
				*(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 2)) = fchar[1] & 0xFF;
				*(str + ((prefixSize + commandSize + zeroSize) + (typeSize + 1 + zeroSize1) + index + 3)) = fchar[0] & 0xFF;
				break;
		}
		index += 4;
		type++;
	}

        if(UDPIsPutReady(sndSocket))
        {
            UDPDiscard();
            UDPPutArray((BYTE *)str, totalSize);
            UDPFlush();
        }

	va_end(list);
	free(str);
}
#else
void sendOSCMessage(UDP_SOCKET sndSocket, char* prefix, char* command, char* type, ...)
{
	UINT16 i, j;
	va_list list;
	char* str;
	char *p;
	INT32 strSize, testSize, zeroSize, testSize1, zeroSize1, totalSize;
	INT32 prefixSize = strchr(prefix, 0) - prefix;
	INT32 commandSize = strchr(command, 0) - command;
	INT32 typeSize = strchr(type, 0) - type;

	strSize = prefixSize + commandSize;
	testSize = strSize;
	zeroSize = 0;
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

	if(UDPIsPutReady(sndSocket))
    {
        UDPDiscard();
        UDPPutArray((BYTE *)str, totalSize);
        UDPFlush();
    }

	va_end(list);
	free(str);
}
#endif

BOOL isEqualToAddress(char* str, char* address)
{
	UINT16 i = 0, j;
	char* msg;
        BOOL flag;

	while(str[i] != 0x00)
	{
		i++;
		if(str[i] == 0x00)
		{
			msg = (char *)calloc(i, sizeof(char));
			memcpy(msg, str, i);
		}
	}

        for(j = 0; j < i; j++)
        {
            if(msg[j] == address[j])
            {
                flag = TRUE;
            }
            else
            {
                flag = FALSE;
                break;
            }
        }
        free(msg);

        return flag;
}

INT32 getIntArgumentAtIndex(char* str, char* address, UINT16 index)
{
	UINT16 i = 0, j = 0, k = 0, n = 0, s = 0, u = 0, v = 0, length = 0;
        INT16 m = 0;
	INT32 sign, exponent, mantissa;
	INT64 lvalue;
	float fvalue;
	float sum;
	char* msg;
	char* types;

	while(str[i] != 0x00)
	{
		i++;
		if(str[i] == 0x00)
		{
			msg = (char *)calloc(i, sizeof(char));
			memcpy(msg, str, i);
		}
	}

	while(str[i] != 0x2C)
	{
		i++;
		if(str[i] == 0x2C)
			j = i;
	}

	while(str[i] != 0x00)
	{
		i++;
		if(str[i] == 0x00)
		{
			types = (char *)calloc(i - j - 1, sizeof(char));
			memcpy(types, str + j + 1, i - j - 1);
		}
	}

	m = i - j;
	n = 0;
	do
	{
		n += 4;
		m -= 4;
	} while(m >= 0);

	if(index >= (i - j - 1))
		return 0;

	length = 0;
	for(k = 0; k <= index; k++)
	{
		switch(types[k])
		{
			case 'i':
			case 'f':
				if(k != index)
					length += 4;
				break;
			case 's':
				u = 0;
				while(str[j + n + length + u] != '\0')
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

	switch(types[index])
	{
		case 'i':
			lvalue = (str[j + n + length] << 24) | (str[j + n + 1 + length] << 16) | (str[j + n + 2 + length] << 8) | str[j + n + 3 + length];
			break;
		case 'f':
			lvalue = ((str[j + n + 0 + length] & 0xFF) << 24) | ((str[j + n + 1 + length] & 0xFF) << 16) | ((str[j + n + 2 + length] & 0xFF) << 8) | (str[j + n + 3 + length] & 0xFF);
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
	free(msg);
	free(types);

	return lvalue;
}

float getFloatArgumentAtIndex(char* str, char* address, UINT16 index)
{
	UINT16 i = 0, j = 0, k = 0, n = 0, s = 0, u = 0, v = 0, length = 0;
        INT16 m = 0;
	INT32 sign, exponent, mantissa;
	INT64 lvalue;
	float fvalue;
	float sum;
	char* msg;
	char* types;

	while(str[i] != 0x00)
	{
		i++;
		if(str[i] == 0x00)
		{
			msg = (char *)calloc(i, sizeof(char));
			memcpy(msg, str, i);
		}
	}

	while(str[i] != 0x2C)
	{
		i++;
		if(str[i] == 0x2C)
			j = i;
	}

	while(str[i] != 0x00)
	{
		i++;
		if(str[i] == 0x00)
		{
			types = (char *)calloc(i - j - 1, sizeof(char));
			memcpy(types, str + j + 1, i - j - 1);
		}
	}

	m = i - j;
	n = 0;
	do
	{
		n += 4;
		m -= 4;
	} while(m >= 0);

	if(index >= (i - j - 1))
		return 0.0;

	length = 0;
	for(k = 0; k <= index; k++)
	{
		switch(types[k])
		{
			case 'i':
			case 'f':
				if(k != index)
					length += 4;
				break;
			case 's':
				u = 0;
				while(str[j + n + length + u] != '\0')
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

	switch(types[index])
	{
		case 'i':
			lvalue = (str[j + n + length] << 24) | (str[j + n + 1+ length] << 16) | (str[j + n + 2+ length] << 8) | str[j + n + 3+ length];
			fvalue = (float)lvalue;
			break;
		case 'f':
			lvalue = ((str[j + n + 0 + length] & 0xFF) << 24) | ((str[j + n + 1 + length] & 0xFF) << 16) | ((str[j + n + 2 + length] & 0xFF) << 8) | (str[j + n + 3 + length] & 0xFF);
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
	free(msg);
	free(types);

	return fvalue;
}

char* getStringArgumentAtIndex(char* str, char* address, UINT16 index)
{
	int i = 0, j = 0, k = 0, n = 0, m = 0, u = 0, v = 0, length = 0;
	char* cstr;
	char* msg;
	char* types;

	while(str[i] != 0x00)
	{
		i++;
		if(str[i] == 0x00)
		{
			msg = (char *)calloc(i, sizeof(char));
			memcpy(msg, str, i);
		}
	}

	while(str[i] != 0x2C)
	{
		i++;
		if(str[i] == 0x2C)
			j = i;
	}

	while(str[i] != 0x00)
	{
		i++;
		if(str[i] == 0x00)
		{
			types = (char *)calloc(i - j - 1, sizeof(char));
			memcpy(types, str + j + 1, i - j - 1);
		}
	}

	m = i - j;
	n = 0;
	do
	{
		n += 4;
		m -= 4;
	} while(m >= 0);

	if(index >= (i - j - 1))
		return 0;

	length = 0;
	for(k = 0; k <= index; k++)
	{
		switch(types[k])
		{
			case 'i':
			case 'f':
				if(k != index)
					length += 4;
				break;
			case 's':
				u = 0;
				while(str[j + n + length + u] != '\0')
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

	switch(types[index])
	{
		case 'i':
		case 'f':
			cstr = (char *)calloc(4, sizeof(char));
			memcpy(cstr, str + j + n + length, 4);
			break;
		case 's':
			cstr = (char *)calloc(v, sizeof(char));
			memcpy(cstr, str + j + n + length, v);
			break;
	}
	free(msg);
	free(types);

	return cstr;
}
