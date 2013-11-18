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
 * sram.h,v.0.1.0 2013/11/05
 */

#ifndef SRAM_H
#define	SRAM_H

#include "iosetting.h"

void initSPI2ForSRAM(void);

// Byte Read/Write
BYTE readByteFromSRAM(WORD address);
void writeByteToSRAM(WORD address, BYTE value);

// Bytes Read/Write
void readBytesFromSRAM(WORD address, BYTE* data, WORD size);
void writeBytesToSRAM(WORD address, BYTE* data, WORD size);

// Clear
void clearSRAM(WORD address, WORD size);

#endif	/* SRAM_H */

