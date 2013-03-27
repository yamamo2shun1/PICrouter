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
 * iosetting.h,v.0.7.2 2013/03/27
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

#define SPI_2 2
#define SPI_4 4
#define putcSPI4(data_out)  do{while(!SPI4STATbits.SPITBE); SPI4BUF=(data_out); }while(0)
#define DataRdySPI4() (SPI4STATbits.SPIRBF)
#define	ReadSPI4()	(SPI4BUF)

#define AN_NUM  14
#define PWM_NUM 4
#define D_NUM   4
#define SPI_NUM 6

extern BYTE ioAnPort[14];
extern BYTE ioPwmPort[4];
extern BYTE ioDPort[4];
extern BYTE ioSpiPort[6];

void setupIOPorts(void);

void setPortIOType(char* port_name, BYTE io);
BYTE getPortIOType(char* port_name);
void outputPort(char* port_name, BYTE state);
BYTE inputPort(char* port_name);
BOOL comparePortNameAtIndex(char* port_name);

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

void sendSpiOneWord(BYTE spi_id, WORD msb, DWORD usec);
void sendSpiTwoWord(BYTE spi_id, WORD msb, WORD lsb, DWORD usec);
void sendSpiFourWord(BYTE spi_id, WORD msb0, WORD lsb0, WORD msb1, WORD lsb1, DWORD usec);

WORD receiveSpiOneWord(BYTE spi_id, DWORD usec);

unsigned int getcSPI4(void);

#endif	/* IOSETTING_H */

