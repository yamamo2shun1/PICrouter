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
 * myDelay.h,v.0.1 2012/07/08
 */

#ifndef _MY_DELAY_HEADER_FILE
#define _MY_DELAY_HEADER_FILE

#ifndef __GENERIC_TYPE_DEFS_H_
#include "GenericTypeDefs.h"
#endif

#define Fosc 80

void delayUs(unsigned int usec);
void delayMs(unsigned int msec);

#endif