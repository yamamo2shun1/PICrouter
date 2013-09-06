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
 * lcd.h,v.0.1.2 2013/09/06
 */

#ifndef __LCD_H
#define __LCD_H

#include <string.h>

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "iosetting.h"

#define CHARS_NUM 20
#define LINE_NUM 4

extern BYTE lcdText[CHARS_NUM * LINE_NUM + 1];

void setLcdInitFlag(BOOL flag);
BOOL getLcdInitFlag(void);
void setLcdBitMode(BYTE num);
BYTE getLcdBitMode(void);
void setLcdPortRsName(char* name);
char* getLcdPortRsName(void);
void setLcdPortRwName(char* name);
char* getLcdPortRwName(void);
void setLcdPortEName(char* name);
char* getLcdPortEName(void);
void setLcdPortDb0Name(char* name);
char* getLcdPortDb0Name(void);
void setLcdPortDb1Name(char* name);
char* getLcdPortDb1Name(void);
void setLcdPortDb2Name(char* name);
char* getLcdPortDb2Name(void);
void setLcdPortDb3Name(char* name);
char* getLcdPortDb3Name(void);
void setLcdPortDb4Name(char* name);
char* getLcdPortDb4Name(void);
void setLcdPortDb5Name(char* name);
char* getLcdPortDb5Name(void);
void setLcdPortDb6Name(char* name);
char* getLcdPortDb6Name(void);
void setLcdPortDb7Name(char* name);
char* getLcdPortDb7Name(void);

void initLcd(BYTE bit_mode_num, char* rs_name, char* rw_name, char* e_name, char* db0_name, char* db1_name, char* db2_name, char* db3_name, char* db4_name, char* db5_name, char* db6_name, char* db7_name);
void updateLcd(void);
void eraseLcd(void);
void writeLineLcd(WORD number, char *line);

#endif

