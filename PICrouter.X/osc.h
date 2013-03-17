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
 * osc.h,v.0.98 2013/03/17
 */

#ifndef OSC_H
#define	OSC_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "TCPIP Stack/TCPIP.h"
#include "HardwareProfile.h"

#define DEFAULT_HOST_NAME "PICrouter"

// Network
extern APP_CONFIG AppConfig;
extern UDP_SOCKET RxSocket;
extern UDP_SOCKET TxSocket;
extern BOOL initReceiveFlag;
extern BOOL initSendFlag;
extern char* hostName;
extern char* prefix;

// Remote IP Address Initialization
extern BYTE remoteIP[];

// Port Number Initialization
extern WORD remotePort;
extern WORD localPort;

// for LED_PAD_16 or LED_PAD_64
extern const char msgLatticePad[];
extern const char msgLatticeLed[];
extern const char msgLatticeLedColumn[];
extern const char msgLatticeLedRow[];
extern const char msgLatticeLedAll[];
extern const char msgLatticeLedClear[];

// for LED_ENC_32 or LED_ENC_ABS_32
extern const char msgRotaryLedStep[];
extern const char msgRotaryLedBits[];
extern const char msgRotaryLedIntensity[];
extern const char msgRotaryLedAllInt[];
extern const char msgRotaryEnc[];
extern const char msgSetRotaryEncStep[];
// for ONLY LED_ENC_32
extern const char msgRotaryEncSwitch[];

// Standard OSC Messages
extern const char stdPrefix[];
// for Onboard
extern const char msgOnboardLed[];
extern const char msgOnboardSw1[];
// for A/D 
extern const char msgGetAdc[];
extern const char msgSetAdcEnable[];
extern const char msgGetAdcEnable[];
extern const char msgConfigAdc[];
extern const char msgSetAdcDO[];
extern const char msgGetAdcDI[];
// for PWM
extern const char msgSetPwmState[];
extern const char msgGetPwmState[];
extern const char msgSetPwmFreq[];
extern const char msgGetPwmFreq[];
extern const char msgSetPwmDuty[];
extern const char msgGetPwmDuty[];
extern const char msgConfigPwm[];
extern const char msgSetPwmDO[];
extern const char msgGetPwmDI[];
// for DIO
extern const char msgConfigDIO[];
extern const char msgSetDO[];
extern const char msgGetDI[];
// for SPI
extern const char msgEnableSpi[];
extern const char msgConfigSpi[];
extern const char msgSetSpiDO[];
extern const char msgGetSpiDI[];

// OSC Messages converted from MIDI Message
extern const char midiPrefix[];
extern const char msgSetNote[];
extern const char msgGetNote[];
extern const char msgSetPp[];
extern const char msgGetPp[];
extern const char msgSetCc[];
extern const char msgGetCc[];
extern const char msgSetPc[];
extern const char msgGetPc[];
extern const char msgSetKp[];
extern const char msgGetKp[];
extern const char msgSetCp[];
extern const char msgGetCp[];
extern const char msgGetPb[];
extern const char msgSetPb[];

// System OSC Messages for Network Settings
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
extern const char msgSwitchUsbMode[];
extern const char msgGetUsbMode[];
extern const char msgSoftReset[];
extern const char msgDebug[];

extern BYTE oscPacket[1024];
extern char rcvAddressStrings[128];
extern UINT16 rcvAddressLength;
extern UINT16 rcvTypesStartIndex;
extern INT16 rcvArgumentsLength;
extern char rcvArgsTypeArray[128];
extern UINT16 rcvArgumentsStartIndex[128];
extern UINT16 rcvArgumentsIndexLength[128];

void InitAppConfig(void);
void setOSCPrefix(char* prefix_string);
void setOSCHostName(char* host_name);
BOOL openOSCSendPort(BYTE* ip_address, WORD port_number);
BOOL openOSCReceivePort(WORD localPort);
BOOL isOSCSendPortOpened(void);
BOOL isOSCReceivePortOpened(void);
void closeOSCSendPort(void);
void closeOSCReceivePort(void);
BOOL isOSCGetReady(WORD len);
BOOL isOSCPutReady(void);
void getOSCPacket(void);
void sendOSCMessage(const char* prefix, const char* command, const char* type, ...);
BOOL compareOSCAddress(const char* prefix, const char* address);
BOOL compareTypeTagAtIndex(const UINT16 index, const char typetag);
WORD getArgumentsLength(void);
INT32 getIntArgumentAtIndex(const UINT16 index);
float getFloatArgumentAtIndex(const UINT16 index);
char* getStringArgumentAtIndex(const UINT16 index);

#endif	/* OSC_H */