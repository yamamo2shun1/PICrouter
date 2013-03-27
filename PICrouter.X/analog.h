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
 * analog.h,v.0.7.1 2013/03/27
 */

#ifndef ANALOG_H
#define	ANALOG_H

#include <plib.h>
#include <math.h>
#include <GenericTypeDefs.h>
#include "osc.h"
#include "iosetting.h"

typedef enum {
	MIDI_FADER     =  0,
	MIDI_VOLUME,
	MIDI_ORIGINAL,
	LONG_FADER,
	LONG_VOLUME,
	LONG_ORIGINAL
} TYPE_AN_VAL;

#define FLTR_ADC_CNT 8

BOOL analogEnable[AN_NUM];
BOOL analogSendFlag[AN_NUM];
BYTE count[AN_NUM];
LONG analog[AN_NUM][FLTR_ADC_CNT];
LONG currentAnalog[AN_NUM];
LONG prevAnalog[AN_NUM];

// for Infinium or CF-X2
int currentValue[AN_NUM];// = {0};
int prevValue[AN_NUM];// = {0};
int boundaryValue[AN_NUM];// = {0};
int currentDirectionValue[AN_NUM];// = {0};
int currentPosition;// = 0;
int currentPosition1;// = 0;
int prevPosition;// = 0;
int boundaryPosition[AN_NUM];// = {0};
int centerPosition;// = 0;
int currentSection;// = 0;
int prevSection;// = 0;
int currentDirection;// = 0;
int currentDirection1;// = 0;
int prevDirection;// = 0;

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

