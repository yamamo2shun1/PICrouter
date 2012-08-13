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
 * myDelay.c,v.0.1 2012/07/08
 */

#include "Compiler.h"
#include "myDelay.h"

void delayUs(unsigned int usec)
{
    unsigned int i, max;
    max = usec * 80 / Fosc;
    for(i = 0; i < max; i++) {}
}

void delayMs(unsigned int msec)
{
    unsigned int i;
    for(i = 0; i < msec; i++)
        delayUs(1000);
}
