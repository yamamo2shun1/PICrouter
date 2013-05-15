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
 * encoder.h,v.0.6.4 2013/05/15
 */

#ifndef ENCODER_H
#define	ENCODER_H

#include <math.h>
#include <GenericTypeDefs.h>
#include "osc.h"
#include "iosetting.h"

// Timer4 12.5nsec x 8(pre scaler) x TIMER_COUNT = 100nsec x TIMER_COUNT = timer interval
// 100nsec x 1000 = 100000nsec = 100usec
#define MA_COUNT    8
#define DELTA       69.230769231

#define MAX_RE_NUM 4
#define AVG_NUM 8

void initEncoderVariables(void);

void setNumConnectedAbsEnc(BYTE num);
BYTE getNumConnectedAbsEnc(void);

void setInitIncEncFlag(BOOL flag);
void setInitAbsEncFlag(BOOL flag);
void setInitLedDrvFlag(BOOL flag);

void setIncEncoderPortAName(BYTE index, char* name);
char* getIncEncoderPortAName(BYTE index);
void setIncEncoderPortBName(BYTE index, char* name);
char* getIncEncoderPortBName(BYTE index);
void setIncEncoderPortSwName(BYTE index, char* name);
char* getIncEncoderPortSwName(BYTE index);

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
#if 0
void setDwLedSequence(BYTE index, BYTE step, DWORD data);
DWORD getDwLedSequence(BYTE index, BYTE step);
#endif
void setLedOn(BYTE index, BOOL flag);
BOOL getLedOn(BYTE index);
void setIntensity(BYTE index0, BYTE index1, BYTE value);
BYTE getIntensity(BYTE index0, BYTE index1);

void encoderCheck(BYTE index);
void incEncoderHandle(BYTE index);
void absEncoderHandle(void);
void annularLedHandle(void);
void sendEncInc32(BYTE index);
void sendEncAbs32(BYTE index);

#endif	/* ENCODER_H */

