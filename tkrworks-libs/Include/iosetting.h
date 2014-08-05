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
 * iosetting.h,v.1.0.0 2014/06/17
 */

#ifndef IOSETTING_H
#define IOSETTING_H

#include <plib.h>
#include <GenericTypeDefs.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "HardwareProfile.h"

// Digital Out Port State
#define HIGH 1
#define LOW  0

// Port Input and Output
#define IO_OUT 0
#define IO_IN  1

// OnBoard Switch
#define initSW()      TRISDbits.TRISD10 = IO_IN;
#define getSW1State() PORTDbits.RD10

// OnBoard LED Settings
#define initLEDs()    TRISDbits.TRISD8 = 0;\
                      TRISDbits.TRISD9 = 0;\
      	               LATDbits.LATD8 = 0;\
                      LATDbits.LATD9 = 0;
#define LED_1         LATDbits.LATD8
#define LED_2         LATDbits.LATD9
#define turnOnLED1()  LED_1 = 1;
#define turnOnLED2()  LED_2 = 1;
#define turnOffLED1() LED_1 = 0;
#define turnOffLED2() LED_2 = 0;
#define toggleLED1()  LED_1 = !LED_1;
#define toggleLED2()  LED_2 = !LED_2;

// SPI
#define SPI_2 2
#define SPI_4 4
#define putcSPI4(data_out)  do{while(!SPI4STATbits.SPITBE); SPI4BUF=(data_out); }while(0)
#define DataRdySPI4() (SPI4STATbits.SPIRBF)
#define	ReadSPI4()	(SPI4BUF)

// I2C
#define I2C_3 3
#define I2C_4 4
#define I2C_5 5

#define AN_NUM  14
#define PWM_NUM 4
#define D_NUM   4
#define SPI_NUM 6

#define TIMER4_COUNT 500//500
#define TIMER5_COUNT 2000

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
void sendSpiThreeWord(BYTE spi_id, WORD msb0, WORD lsb0, WORD msb1, DWORD usec);
void sendSpiFourWord(BYTE spi_id, WORD msb0, WORD lsb0, WORD msb1, WORD lsb1, DWORD usec);

WORD receiveSpiOneWord(BYTE spi_id, DWORD usec);

unsigned int getcSPI4(void);

void idleI2C(BYTE i2c_id);
BOOL startI2C(BYTE i2c_id);
void setAddressToI2C(BYTE i2c_id, BYTE data, char chflag);
void setDataToI2C(BYTE i2c_id, BYTE data);
BOOL checkAckI2C(BYTE i2c_id);
void restartI2C(BYTE i2c_id);
void stopI2C(BYTE i2c_id);
BYTE getDataFromI2C(BYTE i2c_id);

#if defined(USE_RN131)
void sendCommandToRN134(unsigned char* cmd);
void sendMessageToRN134(unsigned char* msg, INT32 len);
#endif

#if defined(USE_DAC_PLUS_ADC)
void sendDataToAD5328(BYTE id, WORD msb);
void sendDataToAD7490(WORD msb);
WORD receiveDataFromAD7490(WORD msb);
#endif

#if defined(USE_ADG1414)
void sendDataToADG1414(WORD msb);
#endif

#endif	/* IOSETTING_H */

