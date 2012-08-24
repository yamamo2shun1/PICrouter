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
 * osc.h,v.0.5 2012/08/24
 */

#ifndef OSC_H
#define	OSC_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "TCPIP Stack/TCPIP.h"

BOOL openOSCSendPort(UDP_SOCKET sndSocket, BYTE* remoteIp, WORD remotePort);
BOOL openOSCReceivePort(UDP_SOCKET rcvSocket, WORD localPort);
void closeOSCSendPort(UDP_SOCKET sndSocket);
void closeOSCReceivePort(UDP_SOCKET rcvSocket);
void sendOSCMessage(UDP_SOCKET sndSocket, char* prefix, char* command, char* type, ...);

BOOL isEqualToAddress(char* str, char* prefix, char* address);
INT32 getIntArgumentAtIndex(char* str, char* prefix, char* address, UINT16 index);
float getFloatArgumentAtIndex(char* str, char* prefix, char* address, UINT16 index);
char* getStringArgumentAtIndex(char* str, char* prefix, char* address, UINT16 index);

#endif	/* OSC_H */

