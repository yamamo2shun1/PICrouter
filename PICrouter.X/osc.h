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
 * osc.h,v.1.2.1 2013/07/19
 */

#ifndef OSC_H
#define	OSC_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "TCPIP Stack/TCPIP.h"
#include "HardwareProfile.h"

#define DEFAULT_HOST_NAME "picrouter"

#define MAX_BUF_SIZE    64
#define MAX_PACKET_SIZE 320// 1024
#define MAX_ADDRESS_LEN 64
#define MAX_ARGS_LEN 40

// Network
APP_CONFIG AppConfig;

// for LED_PAD_16 or LED_PAD_64
extern const char msgLatticePadConnectedNum[];
extern const char msgSetLatticePadConnectedNum[];
extern const char msgGetLatticePadConnectedNum[];
extern const char msgLatticePadPinSelect[];
extern const char msgLatticePad[];
extern const char msgLatticeLedDrvPinSelect[];
extern const char msgSetLatticeLed[];
extern const char msgSetLatticeLedColumn[];
extern const char msgSetLatticeLedRow[];
extern const char msgSetLatticeLedAll[];
extern const char msgLatticeLedClear[];
extern const char msgLatticeLedIntensity[];
extern const char msgGetLatticeLedIntensity[];
extern const char msgLatticeLedIntensityAll[];
extern const char msgGetLatticeLedIntensityAll[];

// for RGB_PAD_16
extern const char msgLatticeRgbDrvPinSelect[];
extern const char msgSetLatticeRgb[];
extern const char msgSetLatticeRgbColumn[];
extern const char msgSetLatticeRgbRow[];
extern const char msgSetLatticeRgbAll[];
extern const char msgLatticeRgbClear[];
extern const char msgLatticeRgbIntensity[];
extern const char msgGetLatticeRgbIntensity[];
extern const char msgLatticeRgbIntensityAll[];
extern const char msgGetLatticeRgbIntensityAll[];

// for LED_ENC_32 or LED_ENC_ABS_32
extern const char msgRotaryLedDrvPinSelect[];
//extern const char msgRotaryLedStep[];
extern const char msgSetRotaryLedStep[];
//extern const char msgRotaryLedBits[];
extern const char msgSetRotaryLedBits[];
extern const char msgRotaryLedIntensity[];
extern const char msgSetRotaryLedIntensity[];
extern const char msgGetRotaryLedIntensity[];
extern const char msgSetRotaryLedIntensityAll[];
extern const char msgRotaryIncEncPinSelect[];
extern const char msgRotaryIncEnc[];
extern const char msgRotaryIncEncSwitch[];
extern const char msgRotaryAbsEncPinSelect[];
extern const char msgRotaryAbsEncConnectedNum[];
extern const char msgSetRotaryAbsEncConnectedNum[];
extern const char msgGetRotaryAbsEncConnectedNum[];
extern const char msgRotaryAbsEnc[];

// Standard OSC Messages
// for Onboard
extern const char msgOnboardLed[];
extern const char msgOnboardSw1[];

extern const char msgPortIO[];
extern const char msgSetPortIO[];
extern const char msgGetPortIO[];
extern const char msgSetPortOut[];
extern const char msgPortIn[];
extern const char msgGetPortIn[];

// for A/D 
extern const char msgAdc[];
extern const char msgAdcEnable[];
extern const char msgSetAdcEnable[];
extern const char msgGetAdcEnable[];
extern const char msgAdcType[];
extern const char msgSetAdcType[];
extern const char msgGetAdcType[];
extern const char msgAdcCurve[];
extern const char msgSetAdcCurve[];
extern const char msgGetAdcCurve[];
extern const char msgAdcDio[];
extern const char msgSetAdcDio[];
extern const char msgGetAdcDio[];
extern const char msgSetAdcDo[];
extern const char msgAdcDi[];
extern const char msgGetAdcDi[];
// for PWM
extern const char msgPwmEnable[];
extern const char msgSetPwmEnable[];
extern const char msgGetPwmEnable[];
extern const char msgPwmFreq[];
extern const char msgSetPwmFreq[];
extern const char msgGetPwmFreq[];
extern const char msgPwmDuty[];
extern const char msgSetPwmDuty[];
extern const char msgGetPwmDuty[];
extern const char msgPwmDio[];
extern const char msgSetPwmDio[];
extern const char msgGetPwmDio[];
extern const char msgSetPwmDo[];
extern const char msgPwmDi[];
extern const char msgGetPwmDi[];
// for DIO
extern const char msgDigitalDio[];
extern const char msgSetDigitalDio[];
extern const char msgGetDigitalDio[];
extern const char msgSetDigitalDo[];
extern const char msgDigitalDi[];
extern const char msgGetDigitalDi[];
// for SPI
extern const char msgSetSpiConfig[];
extern const char msgDisableSpi[];
extern const char msgSpiData[];
extern const char msgSetSpiData[];
extern const char msgGetSpiData[];
extern const char msgSpiDio[];
extern const char msgSetSpiDio[];
extern const char msgGetSpiDio[];
extern const char msgSetSpiDo[];
extern const char msgSpiDi[];
extern const char msgGetSpiDi[];
// for I2C
extern const char msgEnableI2c[];
extern const char msgSetI2cConfig[];
extern const char msgSetI2cFreq[];
extern const char msgSetI2cSlaveAddress[];
extern const char msgI2cData[];
extern const char msgSetI2cData[];
extern const char msgGetI2cData[];

extern const char msgSetLcdConfig[];
extern const char msgSetLcdText[];
extern const char msgClearLcdText[];

// OSC Messages converted from MIDI Messages
extern const char midiPrefix[];
extern const char msgSetNote[];
extern const char msgNote[];
extern const char msgSetPp[];
extern const char msgPp[];
extern const char msgSetCc[];
extern const char msgCc[];
extern const char msgSetPc[];
extern const char msgPc[];
extern const char msgSetKp[];
extern const char msgKp[];
extern const char msgSetCp[];
extern const char msgCp[];
extern const char msgSetPb[];
extern const char msgPb[];

// OSC Messages converted from CDC Messages
extern const char cdcPrefix[];
extern const char msgSetData[];
extern const char msgData[];

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
extern const char msgUsbMode[];
extern const char msgGetUsbMode[];
extern const char msgSoftReset[];
extern const char msgConfiguration[];
extern const char msgDebug[];
extern const char msgError[];
extern const char msgVersion[];
extern const char msgGetVersion[];

// for touchOSC
extern const char toscPrefix[];

extern const char msgOnboardLed1[];
extern const char msgOnboardLed2[];

extern const char msgSetPwmEnable1[];
extern const char msgSetPwmEnable2[];
extern const char msgSetPwmEnable3[];
extern const char msgSetPwmEnable4[];
extern const char msgSetPwmDuty1[];
extern const char msgSetPwmDuty2[];
extern const char msgSetPwmDuty3[];
extern const char msgSetPwmDuty4[];


void InitAppConfig(void);

void setInitReceiveFlag(BOOL flag);
BOOL getInitReceiveFlag(void);
void setInitSendFlag(BOOL flag);
BOOL getInitSendFlag(void);
void setChCompletedFlag(BOOL flag);
BOOL getChCompletedFlag(void);

void setRemoteIpAtIndex(BYTE index, BYTE number);
BYTE getRemoteIpAtIndex(BYTE index);
BYTE* getRemoteIp(void);
void setRemotePort(WORD number);
WORD getRemotePort(void);
void setLocalPort(WORD number);
WORD getLocalPort(void);

void setOSCPrefix(char* prefix_string);
char* getOSCPrefix(void);
void clearOSCPrefix(void);
void setOSCHostName(char* host_name);
char* getOSCHostName(void);
void clearOSCHostName(void);
BOOL openOSCSendPort(BYTE* ip_address, WORD port_number);
BOOL openOSCReceivePort(WORD localPort);
BOOL isOSCSendPortOpened(void);
BOOL isOSCReceivePortOpened(void);
void closeOSCSendPort(void);
void closeOSCReceivePort(void);

BOOL isOSCGetReady(void);
BOOL isOSCPutReady(void);

void getOSCPacket(void);
BOOL processOSCPacket(void);
void sendOSCMessage(const char* prefix, const char* command, const char* type, ...);
void clearOSCBundle(void);
void appendOSCMessageToBundle(const char* prefix, const char* command, const char* type, ...);
void sendOSCBundle(void);
BOOL compareOSCPrefix(const char* prefix);
BOOL compareOSCAddress(const char* address);
BOOL compareTypeTagAtIndex(const UINT16 index, const char typetag);
WORD getArgumentsLength(void);
INT32 getIntArgumentAtIndex(const UINT16 index);
float getFloatArgumentAtIndex(const UINT16 index);
char* getStringArgumentAtIndex(const UINT16 index);

#endif	/* OSC_H */
