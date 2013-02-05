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
 * encoder.h,v.0.5 2013/01/26
 */

#ifndef ENCODER_H
#define	ENCODER_H

#include <math.h>
#include <GenericTypeDefs.h>
#include "osc.h"

// Timer4 12.5nsec x 8(pre scaler) x TIMER_COUNT = 100nsec x TIMER_COUNT = timer interval
// 100nsec x 100 = 10000nsec = 10usec
#define TIMER_COUNT 2000//150
#define MA_COUNT    8
#define DELTA       69.230769231

BYTE reA[2];
BYTE reB[2];

BYTE reD;
int reStep;

volatile BOOL sendEncFlag;
BOOL reFlag[2];
float omega[2];
float omega_ma[2][8];
float alpha[2];
INT8 direction;
DWORD encCount[2];
DWORD dt;

DWORD dwLedData;
volatile DWORD dwLedSequence[100];
BYTE intensity[32];
BOOL ledOn;
WORD ledCount;
BYTE ledIntensity;
volatile BYTE ledIntensityIndex;

// test for EMS22A50
#define AVG_NUM 8
BYTE reDataIndex;
BYTE reAvgIndex;
BYTE reVelAvgIndex;
int reVelAvgIndex2;
volatile BYTE reMatchCount;
BYTE reState[16];
WORD rePosData[AVG_NUM];
float reAbsAnglePosLast;
float reAbsAnglePos;
int reDirection;
DWORD reCounting;
float reCounted;
DWORD reAvgCounted[8];
float reVelocity; 
float reAvgVelocity[16];

void initEncoderVariables(void);
void encoderCheck(BYTE rea, BYTE reb);
//void encoderHandle(void);
void annularLedHandle(void);
void sendEnc(void);

#endif	/* ENCODER_H */

