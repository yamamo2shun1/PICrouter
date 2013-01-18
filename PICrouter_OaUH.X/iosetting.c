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
  * iosetting.c,v.0.1 2013/01/17
 */

#include "iosetting.h"

void configAnPort(BYTE id, BYTE io)
{
	switch(id)
	{
		case 0:
			ADC_AN0_IO(io);
			break;
		case 1:
			ADC_AN1_IO(io);
			break;
		case 2:
			ADC_AN2_IO(io);
			break;
		case 3:
			ADC_AN3_IO(io);
			break;
		case 4:
			ADC_AN4_IO(io);
			break;
		case 5:
			ADC_AN5_IO(io);
			break;
		case 6:
			ADC_AN6_IO(io);
			break;
		case 7:
			ADC_AN7_IO(io);
			break;
		case 8:
			ADC_AN8_IO(io);
			break;
		case 9:
			ADC_AN9_IO(io);
			break;
		case 10:
			ADC_AN10_IO(io);
			break;
		case 11:
			ADC_AN11_IO(io);
			break;
		case 12:
			ADC_AN12_IO(io);
			break;
		case 13:
			ADC_AN13_IO(io);
			break;
	}
}
void outputAnPort(BYTE id, BYTE state)
{
	switch(id)
	{
		case 0:
			ADC_AN0_OUT(state);
			break;
		case 1:
			ADC_AN1_OUT(state);
			break;
		case 2:
			ADC_AN2_OUT(state);
			break;
		case 3:
			ADC_AN3_OUT(state);
			break;
		case 4:
			ADC_AN4_OUT(state);
			break;
		case 5:
			ADC_AN5_OUT(state);
			break;
		case 6:
			ADC_AN6_OUT(state);
			break;
		case 7:
			ADC_AN7_OUT(state);
			break;
		case 8:
			ADC_AN8_OUT(state);
			break;
		case 9:
			ADC_AN9_OUT(state);
			break;
		case 10:
			ADC_AN10_OUT(state);
			break;
		case 11:
			ADC_AN11_OUT(state);
			break;
		case 12:
			ADC_AN12_OUT(state);
			break;
		case 13:
			ADC_AN13_OUT(state);
			break;
	}
}
BYTE inputAnPort(BYTE id)
{
	BYTE state = LOW;
	switch(id)
	{
		case 0:
			state = ADC_AN0_IN();
			break;
		case 1:
			state = ADC_AN1_IN();
			break;
		case 2:
			state = ADC_AN2_IN();
			break;
		case 3:
			state = ADC_AN3_IN();
			break;
		case 4:
			state = ADC_AN4_IN();
			break;
		case 5:
			state = ADC_AN5_IN();
			break;
		case 6:
			state = ADC_AN6_IN();
			break;
		case 7:
			state = ADC_AN7_IN();
			break;
		case 8:
			state = ADC_AN8_IN();
			break;
		case 9:
			state = ADC_AN9_IN();
			break;
		case 10:
			state = ADC_AN10_IN();
			break;
		case 11:
			state = ADC_AN11_IN();
			break;
		case 12:
			state = ADC_AN12_IN();
			break;
		case 13:
			state = ADC_AN13_IN();
			break;
	}
	return state;
}

void configPwmPort(BYTE id, BYTE io)
{
	switch(id)
	{
		case 0:
			PWM_OC1_IO(io);
			break;
		case 1:
			PWM_OC3_IO(io);
			break;
		case 2:
			PWM_OC4_IO(io);
			break;
		case 3:
			PWM_OC5_IO(io);
			break;
	}
}
void outputPwmPort(BYTE id, BYTE state)
{
	switch(id)
	{
		case 0:
			PWM_OC1_OUT(state);
			break;
		case 1:
			PWM_OC3_OUT(state);
			break;
		case 2:
			PWM_OC4_OUT(state);
			break;
		case 3:
			PWM_OC5_OUT(state);
			break;
	}
}
BYTE inputPwmPort(BYTE id)
{
	BYTE state = LOW;

	switch(id)
	{
		case 0:
			state = PWM_OC1_IN();
			break;
		case 1:
			state = PWM_OC3_IN();
			break;
		case 2:
			state = PWM_OC4_IN();
			break;
		case 3:
			state = PWM_OC5_IN();
			break;
	}
	return state;
}

void configDPort(BYTE id, BYTE io)
{
	switch(id)
	{
		case 0:
			D_PORT0_IO(io);
			break;
		case 1:
			D_PORT1_IO(io);
			break;
		case 2:
			D_PORT2_IO(io);
			break;
		case 3:
			D_PORT3_IO(io);
			break;
	}
}
void outputDPort(BYTE id, BYTE state)
{
	switch(id)
	{
		case 0:
			D_PORT0_OUT(state);
			break;
		case 1:
			D_PORT1_OUT(state);
			break;
		case 2:
			D_PORT2_OUT(state);
			break;
		case 3:
			D_PORT3_OUT(state);
			break;
	}
}
BYTE inputDPort(BYTE id)
{
	BYTE state = LOW;
	switch(id)
	{
		case 0:
			state = D_PORT0_IN();
			break;
		case 1:
			state = D_PORT1_IN();
			break;
		case 2:
			state = D_PORT2_IN();
			break;
		case 3:
			state = D_PORT3_IN();
			break;
	}
	return state;
}

void configSpiPort(char* name, BYTE io)
{
	if(!strcmp(name, "sck4"))
    {
    	SPI_SCK4_IO(io);
    }
    else if(!strcmp(name, "sdi4"))
    {
    	SPI_SDI4_IO(io);
    }
    else if(!strcmp(name, "sdo4"))
    {
    	SPI_SDO4_IO(io);
    }
    else if(!strcmp(name, "sck2"))
    {
    	SPI_SCK2_IO(io);
    }
    else if(!strcmp(name, "sdi2"))
    {
    	SPI_SDI2_IO(io);
    }
    else if(!strcmp(name, "sdo2"))
    {
    	SPI_SDO2_IO(io);
    }
}
void outputSpiPort(char* name, BYTE state)
{
	if(!strcmp(name, "sck4"))
    {
    	SPI_SCK4_OUT(state);
    }
    else if(!strcmp(name, "sdi4"))
    {
    	SPI_SDI4_OUT(state);
    }
    else if(!strcmp(name, "sdo4"))
    {
    	SPI_SDO4_OUT(state);
    }
    else if(!strcmp(name, "sck2"))
    {
    	SPI_SCK2_OUT(state);
    }
    else if(!strcmp(name, "sdi2"))
    {
    	SPI_SDI2_OUT(state);
    }
    else if(!strcmp(name, "sdo2"))
    {
    	SPI_SDO2_OUT(state);
    }
}
BYTE inputSpiPort(char* name)
{
	BYTE state = LOW;
	if(!strcmp(name, "sck4"))
    {
    	state = SPI_SCK4_IN();
    }
    else if(!strcmp(name, "sdi4"))
    {
    	state = SPI_SDI4_IN();
    }
    else if(!strcmp(name, "sdo4"))
    {
    	state = SPI_SDO4_IN();
    }
    else if(!strcmp(name, "sck2"))
    {
    	state = SPI_SCK2_IN();
    }
    else if(!strcmp(name, "sdi2"))
    {
    	state = SPI_SDI2_IN();
    }
    else if(!strcmp(name, "sdo2"))
    {
    	state = SPI_SDO2_IN();
    }
    return state;
}