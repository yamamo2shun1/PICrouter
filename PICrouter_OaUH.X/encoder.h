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
 * encoder.h,v.0.1 2012/10/06
 */

#ifndef ENCODER_H
#define	ENCODER_H

#include <math.h>
#include <GenericTypeDefs.h>

// Timer4 12.5nsec x 8(pre scaler) x TIMER_COUNT = 100nsec x TIMER_COUNT = timer interval
// 100nsec x 100 = 10000nsec = 10usec
#define TIMER_COUNT 50

BYTE reA[2];
BYTE reB[2];

BYTE reD;
int reStep;

BOOL sendEncFlag;
BOOL reFlag[2];
float omega[2];
float alpha[2];
INT8 direction;
DWORD encCount[2];
DWORD dt;

void encoderInit(void);
void encoderCheck(BYTE rea, BYTE reb);

#endif	/* ENCODER_H */

