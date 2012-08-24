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
 * osc.h,v.0.7 2012/08/24
 */

#ifndef OSC_H
#define	OSC_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "TCPIP Stack/TCPIP.h"

//System OSC Messages for Network Settings
extern char sysPrefix[];
extern char msgPrefix[];
extern char msgSetPrefix[];
extern char msgGetPrefix[];
extern char msgRemoteIp[];
extern char msgSetRemoteIp[];
extern char msgGetRemoteIp[];
extern char msgRemotePort[];
extern char msgSetRemotePort[];
extern char msgGetRemotePort[];
extern char msgHostName[];
extern char msgSetHostName[];
extern char msgGetHostName[];
extern char msgHostIp[];
extern char msgGetHostIp[];
extern char msgHostMac[];
extern char msgGetHostMac[];
extern char msgHostPort[];
extern char msgSetHostPort[];
extern char msgGetHostPort[];

BOOL openOSCSendPort(UDP_SOCKET sndSocket, BYTE* remoteIp, WORD remotePort);
BOOL openOSCReceivePort(UDP_SOCKET rcvSocket, WORD localPort);
void closeOSCSendPort(UDP_SOCKET sndSocket);
void closeOSCReceivePort(UDP_SOCKET rcvSocket);
void sendOSCMessage(UDP_SOCKET sndSocket, const char* prefix, const char* command, const char* type, ...);

BOOL isEqualToAddress(const char* str, const char* prefix, const char* address);
INT32 getIntArgumentAtIndex(const char* str, const char* prefix, const char* address, const UINT16 index);
float getFloatArgumentAtIndex(const char* str, const char* prefix, const char* address, const UINT16 index);
char* getStringArgumentAtIndex(const char* str, const char* prefix, const char* address, const UINT16 index);

#endif	/* OSC_H */

