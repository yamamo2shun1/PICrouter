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
 * analog.h,v.0.7.5 2013/07/10
 */

#ifndef ANALOG_H
#define	ANALOG_H

#include <plib.h>
#include <math.h>
#include <GenericTypeDefs.h>
#include "osc.h"
#include "iosetting.h"

typedef enum {
	BYTE_FADER =  0,
	BYTE_VOLUME,
	BYTE_ORIGINAL,
	WORD_FADER,
	WORD_VOLUME,
	WORD_ORIGINAL
} TYPE_AN_VAL;

#define FLTR_ADC_CNT 16

void setAnalogEnable(BYTE port, BOOL flag);
BOOL getAnalogEnable(BYTE port);
void setAnalogType(BYTE port, BYTE type);
BYTE getAnalogType(BYTE port);
void setAnalogCurve(BYTE port, float curve);
float getAnalogCurve(BYTE port);

void initAnalogVariables(void);
void resetAnalogFlag(BYTE port);
BOOL getAnalogFlag(BYTE port);
BYTE getAnalogByte(BYTE port, BYTE type);
WORD getAnalogWord(BYTE port, BYTE type);
void analogInHandle(BYTE port, LONG value);
void sendAdc();
void sendInfinium();
void sendCFX2();

#endif	/* ANALOG_H */

