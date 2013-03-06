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
 * iosetting.h,v.0.3 2013/03/06
 */

#ifndef IOSETTING_H
#define	IOSETTING_H

#include <plib.h>
#include <GenericTypeDefs.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "HardwareProfile.h"

extern BYTE ioAnPort[14];
extern BYTE ioPwmPort[4];
extern BYTE ioDPort[4];
extern BYTE ioSpiPort[6];

void setupIOPorts(void);

void configAnPort(BYTE id, BYTE io);
void outputAnPort(BYTE id, BYTE state);
BYTE inputAnPort(BYTE id);

void configPwmPort(BYTE id, BYTE io);
void outputPwmPort(BYTE id, BYTE state);
BYTE inputPwmPort(BYTE id);

void configDPort(BYTE id, BYTE io);
void outputDPort(BYTE id, BYTE state);
BYTE inputDPort(BYTE id);

void configSpiPort(char* name, BYTE io);
void outputSpiPort(char* name, BYTE state);
BYTE inputSpiPort(char* name);

#endif	/* IOSETTING_H */

