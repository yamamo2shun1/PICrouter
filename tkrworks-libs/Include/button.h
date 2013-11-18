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
 * button.h,v.0.8.1 2013/10/28
 */

#ifndef BUTTON_H
#define	BUTTON_H

#include <plib.h>
#include <GenericTypeDefs.h>
#include "TimeDelay.h"
#include "HardwareProfile.h"
#include "osc.h"

#define MAX_PAD_NUM 5

#define MAX_RGB_LAYER 3
#define MAX_BTN_ROW 4//1 // 2
#define MAX_BTN_COL 4//16 // 2
#define MAX_BTN_NUM 16

void setInitPadFlag(BOOL flag);
void setNumConnectedLatticePad(BYTE num);
BYTE getNumConnectedLatticePad(void);
void setLatticePadPortClkName(BYTE index, char* name);
char* getLatticePadPortClkName(BYTE index);
void setLatticePadPortShLdName(BYTE index, char* name);
char* getLatticePadPortShLdName(BYTE index);
void setLatticePadPortQhName(BYTE index, char* name);
char* getLatticePadPortQhName(BYTE index);
void setLatticePadPortLoadName(char* name);
char* getLatticePadPortLoadName(void);

void setInitLatticeLedDrvFlag(BOOL flag);
BOOL getInitLatticeLedDrvFlag(void);
void setLatticeLedDriverSpiNumber(BYTE num);
BYTE getLatticeLedDriverSpiNumber(void);
void setLatticeLed(BYTE index, WORD data);
WORD getLatticeLed(BYTE index);
void setLatticeIntensity(BYTE index0, BYTE index1, BYTE value);
BYTE getLatticeIntensity(BYTE index0, BYTE index1);
void setLatticeLedOn(BYTE index, BOOL flag);
BOOL getLatticeLedOn(BYTE index);

void setInitLatticeRgbDrvFlag(BOOL flag);
BOOL getInitLatticeRgbDrvFlag(void);
void setNumConnectedLatticeRgbPad(BYTE num);
BYTE getNumConnectedLatticeRgbPad(void);
void setLatticeRgbDriverSpiNumber(BYTE num);
BYTE getLatticeRgbDriverSpiNumber(void);
void setLatticeRgbLedNumber(BYTE index, BYTE num);
BYTE getLatticeRgbLedNumber(BYTE index);
void setLatticeRgb(BYTE index, BYTE layer, WORD data);
WORD getLatticeRgb(BYTE index, BYTE layer);
void setLatticeRgbIntensity(BYTE index0, BYTE layer, BYTE index1, BYTE value);
BYTE getLatticeRgbIntensity(BYTE index0, BYTE layer, BYTE index1);
void setLatticeRgbOn(BYTE index, BYTE layer, BOOL flag);
BOOL getLatticeRgbOn(BYTE index, BYTE layer);

void buttonInit(void);
BOOL buttonCheck(BYTE index0, BYTE row, BYTE index);
void sendPad16(void);
void latticeLedHandle(void);
void latticeRgbHandle(void);

#endif	/* BUTTON_H */
