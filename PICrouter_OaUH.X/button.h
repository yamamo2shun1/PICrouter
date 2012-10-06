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
 * button.h,v.0.1 2012/07/08
 */

#ifndef BUTTON_H
#define	BUTTON_H

#include <GenericTypeDefs.h>


#define MAX_BTN_ROW 8//4//1 // 2
#define MAX_BTN_COL 8//4//16 // 2

WORD btnCurrent[MAX_BTN_ROW];
WORD btnLast[MAX_BTN_ROW];
WORD btnState[MAX_BTN_ROW];
WORD btnDebounceCount[MAX_BTN_ROW][MAX_BTN_COL];

void buttonInit(void);
BOOL buttonCheck(BYTE row, BYTE index);

#endif	/* BUTTON_H */