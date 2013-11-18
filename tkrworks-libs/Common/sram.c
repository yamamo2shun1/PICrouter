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
 * sram.c,v.0.1.0 2013/11/05
 */

#include "sram.h"

void initSPI2ForSRAM(void)
{
    outputPort("c13", HIGH);
    SpiChnOpen(SPI_CHANNEL2, SPICON_MSTEN | SPICON_SMP | SPICON_CKE | SPICON_ON, 5);
}

BYTE readByteFromSRAM(WORD address)
{
    BYTE value = 0;
    BYTE dummy = 0;

    outputPort("c13", LOW);

    putcSPI2(0x03);
    dummy = getcSPI2();

    putcSPI2((BYTE)(address >> 8));
    dummy = getcSPI2();

    putcSPI2((BYTE)(address & 0x00FF));
    dummy = getcSPI2();

    putcSPI2(0x00);
    value = (BYTE)getcSPI2();

    outputPort("c13", HIGH);

    return value;
}

void writeByteToSRAM(WORD address, BYTE value)
{
    BYTE dummy = 0;

    outputPort("c13", LOW);

    putcSPI2(0x02);
    dummy = getcSPI2();

    putcSPI2(address >> 8);
    dummy = getcSPI2();

    putcSPI2(address & 0x00FF);
    dummy = getcSPI2();

    putcSPI2(value);
    dummy = getcSPI2();

    outputPort("c13", HIGH);
}

void readBytesFromSRAM(WORD address, BYTE* data, WORD size)
{
    volatile BYTE ii = 0;
    WORD i = 0;
    BYTE dummy = 0;
 
    outputPort("c13", LOW);

    putcSPI2(0x01);
    dummy = getcSPI2();

    putcSPI2(0x40);
    dummy = getcSPI2();

    outputPort("c13", HIGH);

    Nop();

    outputPort("c13", LOW);

    putcSPI2(0x03);
    dummy = getcSPI2();

    putcSPI2(address >> 8);
    dummy = getcSPI2();

    putcSPI2(address & 0x00FF);
    dummy = getcSPI2();

    for(i = 0; i < size; i++)
    {
        putcSPI2(0x00);
        data[i] = getcSPI2();
    }

    outputPort("c13", HIGH);
}

void writeBytesToSRAM(WORD address, BYTE* data, WORD size)
{
    WORD i = 0;
    BYTE dummy = 0;

    outputPort("c13", LOW);

    putcSPI2(0x01);
    dummy = getcSPI2();

    putcSPI2(0x40);
    dummy = getcSPI2();

    outputPort("c13", HIGH);

    Nop();

    outputPort("c13", LOW);

    putcSPI2(0x02);
    dummy = getcSPI2();

    putcSPI2(address >> 8);
    dummy = getcSPI2();

    putcSPI2(address & 0x00FF);
    dummy = getcSPI2();

    for(i = 0; i < size; i++)
    {
        putcSPI2(data[i]);
        dummy = getcSPI2();
    }

    outputPort("c13", HIGH);
}

void clearSRAM(WORD address, WORD size)
{
    WORD i = 0;
    BYTE dummy = 0;

    outputPort("c13", LOW);

    putcSPI2(0x01);
    dummy = getcSPI2();

    putcSPI2(0x40);
    dummy = getcSPI2();

    outputPort("c13", HIGH);

    Nop();

    outputPort("c13", LOW);

    putcSPI2(0x02);
    dummy = getcSPI2();

    putcSPI2(address >> 8);
    dummy = getcSPI2();

    putcSPI2(address & 0x00FF);
    dummy = getcSPI2();

    for(i = 0; i < size; i++)
    {
        putcSPI2(0x00);
        dummy = getcSPI2();
    }

    outputPort("c13", HIGH);
}