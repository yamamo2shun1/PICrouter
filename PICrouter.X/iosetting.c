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
  * iosetting.c,v.0.7.3 2013/04/02
 */

#include "iosetting.h"

BYTE ioAnPort[14] = {0};
BYTE ioPwmPort[4] = {0};
BYTE ioDPort[4] = {0};
BYTE ioSpiPort[6] = {0};

void setPortIOType(char* port_name, BYTE io)
{
    if(!strcmp(port_name, "b0"))
        setAnPortDioType(0, io);
    else if(!strcmp(port_name, "b1"))
        setAnPortDioType(1, io);
    else if(!strcmp(port_name, "b2"))
        setAnPortDioType(2, io);
    else if(!strcmp(port_name, "b3"))
        setAnPortDioType(3, io);
    else if(!strcmp(port_name, "b4"))
        setAnPortDioType(4, io);
    else if(!strcmp(port_name, "b5"))
        setAnPortDioType(5, io);
    else if(!strcmp(port_name, "b6"))
        setAnPortDioType(6, io);
    else if(!strcmp(port_name, "b7"))
        setAnPortDioType(7, io);
    else if(!strcmp(port_name, "b8"))
        setAnPortDioType(8, io);
    else if(!strcmp(port_name, "b9"))
        setAnPortDioType(9, io);
    else if(!strcmp(port_name, "b10"))
        setAnPortDioType(10, io);
    else if(!strcmp(port_name, "b11"))
        setAnPortDioType(11, io);
    else if(!strcmp(port_name, "b12"))
        setAnPortDioType(12, io);
    else if(!strcmp(port_name, "b13"))
        setAnPortDioType(13, io);
    else if(!strcmp(port_name, "d0"))
        setPwmPortDioType(0, io);
    else if(!strcmp(port_name, "d2"))
        setPwmPortDioType(1, io);
    else if(!strcmp(port_name, "d3"))
        setPwmPortDioType(2, io);
    else if(!strcmp(port_name, "d4"))
        setPwmPortDioType(3, io);
    else if(!strcmp(port_name, "c13"))
        setDigitalPortDioType(0, io);
    else if(!strcmp(port_name, "c14"))
        setDigitalPortDioType(1, io);
    else if(!strcmp(port_name, "f0"))
        setDigitalPortDioType(2, io);
    else if(!strcmp(port_name, "f1"))
        setDigitalPortDioType(3, io);
    else if(!strcmp(port_name, "b14"))
        setSpiPortDioType("sck4", io);
    else if(!strcmp(port_name, "f4"))
        setSpiPortDioType("sdi4", io);
    else if(!strcmp(port_name, "f5"))
        setSpiPortDioType("sdo4", io);
    else if(!strcmp(port_name, "g6"))
        setSpiPortDioType("sck2", io);
    else if(!strcmp(port_name, "g7"))
        setSpiPortDioType("sdi2", io);
    else if(!strcmp(port_name, "g8"))
        setSpiPortDioType("sdo2", io);
}

BYTE getPortIOType(char* port_name)
{
    if(!strcmp(port_name, "b0"))
        getAnPortDioType(0);
    else if(!strcmp(port_name, "b1"))
        getAnPortDioType(1);
    else if(!strcmp(port_name, "b2"))
        getAnPortDioType(2);
    else if(!strcmp(port_name, "b3"))
        getAnPortDioType(3);
    else if(!strcmp(port_name, "b4"))
        getAnPortDioType(4);
    else if(!strcmp(port_name, "b5"))
        getAnPortDioType(5);
    else if(!strcmp(port_name, "b6"))
        getAnPortDioType(6);
    else if(!strcmp(port_name, "b7"))
        getAnPortDioType(7);
    else if(!strcmp(port_name, "b8"))
        getAnPortDioType(8);
    else if(!strcmp(port_name, "b9"))
        getAnPortDioType(9);
    else if(!strcmp(port_name, "b10"))
        getAnPortDioType(10);
    else if(!strcmp(port_name, "b11"))
        getAnPortDioType(11);
    else if(!strcmp(port_name, "b12"))
        getAnPortDioType(12);
    else if(!strcmp(port_name, "b13"))
        getAnPortDioType(13);
    else if(!strcmp(port_name, "d0"))
        getPwmPortDioType(0);
    else if(!strcmp(port_name, "d2"))
        getPwmPortDioType(1);
    else if(!strcmp(port_name, "d3"))
        getPwmPortDioType(2);
    else if(!strcmp(port_name, "d4"))
        getPwmPortDioType(3);
    else if(!strcmp(port_name, "c13"))
        getDigitalPortDioType(0);
    else if(!strcmp(port_name, "c14"))
        getDigitalPortDioType(1);
    else if(!strcmp(port_name, "f0"))
        getDigitalPortDioType(2);
    else if(!strcmp(port_name, "f1"))
        getDigitalPortDioType(3);
    else if(!strcmp(port_name, "b14"))
        getSpiPortDioType("sck4");
    else if(!strcmp(port_name, "f4"))
        getSpiPortDioType("sdi4");
    else if(!strcmp(port_name, "f5"))
        getSpiPortDioType("sdo4");
    else if(!strcmp(port_name, "g6"))
        getSpiPortDioType("sck2");
    else if(!strcmp(port_name, "g7"))
        getSpiPortDioType("sdi2");
    else if(!strcmp(port_name, "g8"))
        getSpiPortDioType("sdo2");
}

void outputPort(char* port_name, BYTE state)
{
    if(!strcmp(port_name, "b0"))
        outputAnPort(0, state);
    else if(!strcmp(port_name, "b1"))
        outputAnPort(1, state);
    else if(!strcmp(port_name, "b2"))
        outputAnPort(2, state);
    else if(!strcmp(port_name, "b3"))
        outputAnPort(3, state);
    else if(!strcmp(port_name, "b4"))
        outputAnPort(4, state);
    else if(!strcmp(port_name, "b5"))
        outputAnPort(5, state);
    else if(!strcmp(port_name, "b6"))
        outputAnPort(6, state);
    else if(!strcmp(port_name, "b7"))
        outputAnPort(7, state);
    else if(!strcmp(port_name, "b8"))
        outputAnPort(8, state);
    else if(!strcmp(port_name, "b9"))
        outputAnPort(9, state);
    else if(!strcmp(port_name, "b10"))
        outputAnPort(10, state);
    else if(!strcmp(port_name, "b11"))
        outputAnPort(11, state);
    else if(!strcmp(port_name, "b12"))
        outputAnPort(12, state);
    else if(!strcmp(port_name, "b13"))
        outputAnPort(13, state);
    else if(!strcmp(port_name, "d0"))
        outputPwmPort(0, state);
    else if(!strcmp(port_name, "d2"))
        outputPwmPort(1, state);
    else if(!strcmp(port_name, "d3"))
        outputPwmPort(2, state);
    else if(!strcmp(port_name, "d4"))
        outputPwmPort(3, state);
    else if(!strcmp(port_name, "c13"))
        outputDigitalPort(0, state);
    else if(!strcmp(port_name, "c14"))
        outputDigitalPort(1, state);
    else if(!strcmp(port_name, "f0"))
        outputDigitalPort(2, state);
    else if(!strcmp(port_name, "f1"))
        outputDigitalPort(3, state);
    else if(!strcmp(port_name, "b14"))
        outputSpiPort("sck4", state);
    else if(!strcmp(port_name, "f4"))
        outputSpiPort("sdi4", state);
    else if(!strcmp(port_name, "f5"))
        outputSpiPort("sdo4", state);
    else if(!strcmp(port_name, "g6"))
        outputSpiPort("sck2", state);
    else if(!strcmp(port_name, "g7"))
        outputSpiPort("sdi2", state);
    else if(!strcmp(port_name, "g8"))
        outputSpiPort("sdo2", state);
}

BYTE inputPort(char* port_name)
{
    if(!strcmp(port_name, "b0"))
        return inputAnPort(0);
    else if(!strcmp(port_name, "b1"))
        return inputAnPort(1);
    else if(!strcmp(port_name, "b2"))
        return inputAnPort(2);
    else if(!strcmp(port_name, "b3"))
        return inputAnPort(3);
    else if(!strcmp(port_name, "b4"))
        return inputAnPort(4);
    else if(!strcmp(port_name, "b5"))
        return inputAnPort(5);
    else if(!strcmp(port_name, "b6"))
        return inputAnPort(6);
    else if(!strcmp(port_name, "b7"))
        return inputAnPort(7);
    else if(!strcmp(port_name, "b8"))
        return inputAnPort(8);
    else if(!strcmp(port_name, "b9"))
        return inputAnPort(9);
    else if(!strcmp(port_name, "b10"))
        return inputAnPort(10);
    else if(!strcmp(port_name, "b11"))
        return inputAnPort(11);
    else if(!strcmp(port_name, "b12"))
        return inputAnPort(12);
    else if(!strcmp(port_name, "b13"))
        return inputAnPort(13);
    else if(!strcmp(port_name, "d0"))
        return inputPwmPort(0);
    else if(!strcmp(port_name, "d2"))
        return inputPwmPort(1);
    else if(!strcmp(port_name, "d3"))
        return inputPwmPort(2);
    else if(!strcmp(port_name, "d4"))
        return inputPwmPort(3);
    else if(!strcmp(port_name, "c13"))
        return inputDigitalPort(0);
    else if(!strcmp(port_name, "c14"))
        return inputDigitalPort(1);
    else if(!strcmp(port_name, "f0"))
        return inputDigitalPort(2);
    else if(!strcmp(port_name, "f1"))
        return inputDigitalPort(3);
    else if(!strcmp(port_name, "b14"))
        return inputSpiPort("sck4");
    else if(!strcmp(port_name, "f4"))
        return inputSpiPort("sdi4");
    else if(!strcmp(port_name, "f5"))
        return inputSpiPort("sdo4");
    else if(!strcmp(port_name, "g6"))
        return inputSpiPort("sck2");
    else if(!strcmp(port_name, "g7"))
        return inputSpiPort("sdi2");
    else if(!strcmp(port_name, "g8"))
        return inputSpiPort("sdo2");
    else
        return 0;
}

BOOL comparePortNameAtIndex(char* port_name)
{
    if(!strcmp(port_name, "b0"))
        return TRUE;
    else if(!strcmp(port_name, "b1"))
        return TRUE;
    else if(!strcmp(port_name, "b2"))
        return TRUE;
    else if(!strcmp(port_name, "b3"))
        return TRUE;
    else if(!strcmp(port_name, "b4"))
        return TRUE;
    else if(!strcmp(port_name, "b5"))
        return TRUE;
    else if(!strcmp(port_name, "b6"))
        return TRUE;
    else if(!strcmp(port_name, "b7"))
        return TRUE;
    else if(!strcmp(port_name, "b8"))
        return TRUE;
    else if(!strcmp(port_name, "b9"))
        return TRUE;
    else if(!strcmp(port_name, "b10"))
        return TRUE;
    else if(!strcmp(port_name, "b11"))
        return TRUE;
    else if(!strcmp(port_name, "b12"))
        return TRUE;
    else if(!strcmp(port_name, "b13"))
        return TRUE;
    else if(!strcmp(port_name, "d0"))
        return TRUE;
    else if(!strcmp(port_name, "d2"))
        return TRUE;
    else if(!strcmp(port_name, "d3"))
        return TRUE;
    else if(!strcmp(port_name, "d4"))
        return TRUE;
    else if(!strcmp(port_name, "c13"))
        return TRUE;
    else if(!strcmp(port_name, "c14"))
        return TRUE;
    else if(!strcmp(port_name, "f0"))
        return TRUE;
    else if(!strcmp(port_name, "f1"))
        return TRUE;
    else if(!strcmp(port_name, "b14"))
        return TRUE;
    else if(!strcmp(port_name, "f4"))
        return TRUE;
    else if(!strcmp(port_name, "f5"))
        return TRUE;
    else if(!strcmp(port_name, "g6"))
        return TRUE;
    else if(!strcmp(port_name, "g7"))
        return TRUE;
    else if(!strcmp(port_name, "g8"))
        return TRUE;
    else
        return FALSE;
}

void setAnPortDioType(BYTE id, BYTE io)
{
    ioAnPort[id] = io;
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

BYTE getAnPortDioType(BYTE id)
{
    return ioAnPort[id];
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

void setPwmPortDioType(BYTE id, BYTE io)
{
    ioPwmPort[id] = io;
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

BYTE getPwmPortDioType(BYTE id)
{
    return ioPwmPort[id];
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

void setDigitalPortDioType(BYTE id, BYTE io)
{
    ioDPort[id] = io;
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

BYTE getDigitalPortDioType(BYTE id)
{
    return ioDPort[id];
}

void outputDigitalPort(BYTE id, BYTE state)
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

BYTE inputDigitalPort(BYTE id)
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

void setSpiPortDioType(char* name, BYTE io)
{
    if(!strcmp(name, "sck4"))
    {
        ioSpiPort[0] = io;
    	SPI_SCK4_IO(io);
    }
    else if(!strcmp(name, "sdi4"))
    {
        ioSpiPort[1] = io;
    	SPI_SDI4_IO(io);
    }
    else if(!strcmp(name, "sdo4"))
    {
        ioSpiPort[2] = io;
    	SPI_SDO4_IO(io);
    }
    else if(!strcmp(name, "sck2"))
    {
        ioSpiPort[3] = io;
    	SPI_SCK2_IO(io);
    }
    else if(!strcmp(name, "sdi2"))
    {
        ioSpiPort[4] = io;
    	SPI_SDI2_IO(io);
    }
    else if(!strcmp(name, "sdo2"))
    {
        ioSpiPort[5] = io;
    	SPI_SDO2_IO(io);
    }
}

BYTE getSpiPortDioType(char* name)
{
    if(!strcmp(name, "sck4"))
        return ioSpiPort[0];
    else if(!strcmp(name, "sdi4"))
        return ioSpiPort[1];
    else if(!strcmp(name, "sdo4"))
        return ioSpiPort[2];
    else if(!strcmp(name, "sck2"))
        return ioSpiPort[3];
    else if(!strcmp(name, "sdi2"))
        return ioSpiPort[4];
    else if(!strcmp(name, "sdo2"))
        return ioSpiPort[5];
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

void sendSpiOneWord(BYTE spi_id, WORD msb, DWORD usec)
{
    // Set LOAD_PIN to low
    //
    switch(spi_id)
    {
        case SPI_2:
            putcSPI2(msb);
            break;
        case SPI_4:
            putcSPI4(msb);
            break;
    }
    Delay10us(usec);
    // Set LOAD_PIN to high
    //
}

void sendSpiTwoWord(BYTE spi_id, WORD msb, WORD lsb, DWORD usec)
{
    // Set LOAD_PIN to low
    //
    switch(spi_id)
    {
        case SPI_2:
            putcSPI2(lsb);
            putcSPI2(msb);
            break;
        case SPI_4:
            putcSPI4(lsb);
            putcSPI4(msb);
            break;
    }
    Delay10us(usec);
    // Set LOAD_PIN to low
    //
}

void sendSpiFourWord(BYTE spi_id, WORD msb0, WORD lsb0, WORD msb1, WORD lsb1, DWORD usec)
{
    // Set LOAD_PIN to low
    //
    switch(spi_id)
    {
        case SPI_2:
            putcSPI2(lsb1);
            putcSPI2(msb1);
            putcSPI2(lsb0);
            putcSPI2(msb0);
            break;
        case SPI_4:
            putcSPI4(lsb1);
            putcSPI4(msb1);
            putcSPI4(lsb0);
            putcSPI4(msb0);
            break;
    }
    Delay10us(usec);
    // Set LOAD_PIN to low
    //
}

WORD receiveSpiOneWord(BYTE spi_id, DWORD usec)
{
    WORD data = 0;

    switch(spi_id)
    {
        case SPI_2:
            data = getcSPI2();
            break;
        case SPI_4:
            data = getcSPI4();
            break;
    }

    return data;
}

unsigned int getcSPI4(void)
{
    while(!DataRdySPI4());
    return ReadSPI4();
}
