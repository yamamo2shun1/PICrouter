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
 * osc.h,v.0.83 2012/09/24
 */

#ifndef OSC_H
#define	OSC_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "TCPIP Stack/TCPIP.h"

extern const char msgOnboardLed[];
extern const char msgVolumeLed[];
extern const char msgLatticeLed[];
extern const char msgRotaryLedStep[];
extern const char msgRotaryLedBits[];
extern const char msgRotaryLedIntensity[];
extern const char msgRotaryLedAllInt[];
extern const char msgRotaryEnc[];
extern const char msgRotaryEncSwitch[];

//Standard OSC Messages
extern const char stdPrefix[];
extern const char msgSetPwmState[];
extern const char msgGetPwmState[];
extern const char msgSetPwmFreq[];
extern const char msgGetPwmFreq[];
extern const char msgSetPwmDuty[];
extern const char msgGetPwmDuty[];

//OSC Messages converted from MIDI Message
extern const char midiPrefix[];
extern const char msgNote[];
extern const char msgPp[];
extern const char msgCc[];
extern const char msgPc[];
extern const char msgKp[];
extern const char msgCp[];
extern const char msgPb[];
extern const char msgSetNote[];
extern const char msgSetPp[];
extern const char msgSetCc[];
extern const char msgSetPc[];
extern const char msgSetKp[];
extern const char msgSetCp[];
extern const char msgSetPb[];

//System OSC Messages for Network Settings
extern const char sysPrefix[];
extern const char msgPrefix[];
extern const char msgSetPrefix[];
extern const char msgGetPrefix[];
extern const char msgRemoteIp[];
extern const char msgSetRemoteIp[];
extern const char msgGetRemoteIp[];
extern const char msgRemotePort[];
extern const char msgSetRemotePort[];
extern const char msgGetRemotePort[];
extern const char msgHostName[];
extern const char msgSetHostName[];
extern const char msgGetHostName[];
extern const char msgHostIp[];
extern const char msgGetHostIp[];
extern const char msgHostMac[];
extern const char msgGetHostMac[];
extern const char msgHostPort[];
extern const char msgSetHostPort[];
extern const char msgGetHostPort[];

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

