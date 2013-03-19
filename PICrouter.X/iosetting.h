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
 * iosetting.h,v.0.6 2013/03/19
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

#define AN_NUM  14
#define PWM_NUM 4
#define D_NUM   4
#define SPI_NUM 6

extern BYTE ioAnPort[14];
extern BYTE ioPwmPort[4];
extern BYTE ioDPort[4];
extern BYTE ioSpiPort[6];

void setupIOPorts(void);

void setAnPortDioType(BYTE id, BYTE io);
BYTE getAnPortDioType(BYTE id);
void outputAnPort(BYTE id, BYTE state);
BYTE inputAnPort(BYTE id);

void setPwmPortDioType(BYTE id, BYTE io);
BYTE getPwmPortDioType(BYTE id);
void outputPwmPort(BYTE id, BYTE state);
BYTE inputPwmPort(BYTE id);

void setDigitalPortDioType(BYTE id, BYTE io);
BYTE getDigitalPortDioType(BYTE id);
void outputDigitalPort(BYTE id, BYTE state);
BYTE inputDigitalPort(BYTE id);

void setSpiPortDioType(char* name, BYTE io);
BYTE getSpiPortDioType(char* name);
void outputSpiPort(char* name, BYTE state);
BYTE inputSpiPort(char* name);

#endif	/* IOSETTING_H */

