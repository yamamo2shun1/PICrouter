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
 * button.c,v.0.1 2012/07/08
 */

#include "button.h"

void buttonInit(void)
{
  int i, j;
  for(i = 0; i < MAX_BTN_ROW; i++)
  {
    btnCurrent[i] = 0x00;
    btnLast[i] = 0x00;
    btnState[i] = 0x00;

    for(j = 0; j < MAX_BTN_COL; j++)
      btnDebounceCount[i][j] = 0;
  }
}

BOOL buttonCheck(BYTE row, BYTE index)
{
  BOOL flag = FALSE;

  if(((btnCurrent[row] ^ btnLast[row]) & (1 << index)) && ((btnCurrent[row] ^ btnState[row]) & (1 << index)))
    btnDebounceCount[row][index] = 0;
  else if (((btnCurrent[row] ^ btnLast[row]) & (1 << index)) == 0 && ((btnCurrent[row] ^ btnState[row]) & (1 << index)))
  {
    if(btnDebounceCount[row][index] < 4 && ++btnDebounceCount[row][index] == 4)
    {
      if(btnCurrent[row] & (1 << index))
        btnState[row] |= (1 << index);
      else
        btnState[row] &= ~(1 << index);
      flag = TRUE;
    }
  }
  return flag;
}