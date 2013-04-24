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
 * encoder.h,v.0.6.1 2013/04/24
 */

#ifndef ENCODER_H
#define	ENCODER_H

#include <math.h>
#include <GenericTypeDefs.h>
#include "osc.h"

// Timer4 12.5nsec x 8(pre scaler) x TIMER_COUNT = 100nsec x TIMER_COUNT = timer interval
// 100nsec x 1000 = 100000nsec = 100usec
#define TIMER_COUNT 1000
#define MA_COUNT    8
#define DELTA       69.230769231

#define MAX_RE_NUM 4
#define AVG_NUM 8

// LED_ENC
#if 0 // for LED_ENC_32
    #define LD32_LOAD(state) LATCbits.LATC13 = state;
    #define RE_SW()          PORTCbits.RC14
    #define RE_A()           PORTFbits.RF0
    #define RE_B()           PORTFbits.RF1
    #define RE_CLK(state)    LATFbits.LATF0 = state;
#endif

void initEncoderVariables(void);

void setNumConnectedAbsEnc(BYTE num);
BYTE getNumConnectedAbsEnc(void);

void setInitIncEncFlag(BOOL flag);
void setInitAbsEncFlag(BOOL flag);
void setInitLedDrvFlag(BOOL flag);

void setIncEncoderPortAName(char* name);
char* getIncEncoderPortAName(void);
void setIncEncoderPortBName(char* name);
char* getIncEncoderPortBName(void);
void setIncEncoderPortSwName(char* name);
char* getIncEncoderPortSwName(void);
void setAbsEncoderPortCsName(char* name);
char* getAbsEncoderPortCsName(void);
void setAbsEncoderPortClkName(char* name);
char* getAbsEncoderPortClkName(void);
void setAbsEncoderPortDoName(char* name);
char* getAbsEncoderPortDoName(void);
void setLedDriverPortSsName(char* name);
char* getLedDriverPortSsName(void);
void setLedDriverSpiNumber(BYTE num);
BYTE getLedDriverSpiNumber(void);

BOOL getInitIncEncFlag(void);
BOOL getInitAbsEncFlag(void);
BOOL getInitLedDrvFlag(void);

void setDwLedData(BYTE index, DWORD data);
DWORD getDwLedData(BYTE index);
void setDwLedSequence(BYTE index, BYTE step, DWORD data);
DWORD getDwLedSequence(BYTE index, BYTE step);
void setLedOn(BYTE index, BOOL flag);
BOOL getLedOn(BYTE index);
void setIntensity(BYTE index0, BYTE index1, BYTE value);
BYTE getIntensity(BYTE index0, BYTE index1);

void encoderCheck(BYTE rea, BYTE reb);
void incEncoderHandle(void);
void absEncoderHandle(void);
void annularLedHandle(void);
void sendEncInc32(void);
void sendEncAbs32(BYTE index);

#endif	/* ENCODER_H */

