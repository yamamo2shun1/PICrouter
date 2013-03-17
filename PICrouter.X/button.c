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
 * button.c,v.0.5 2013/01/23
 */

#include "button.h"

WORD ledState = 0;
WORD matrixLed[4] = {0};
const WORD matrixLedData[4][4] = {{0x01, 0x03, 0x10, 0x12},
                                  {0x00, 0x02, 0x11, 0x13},
                                  {0x33, 0x31, 0x22, 0x21},
                                  {0x32, 0x30, 0x23, 0x20}};

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

// for LED_PAD_16
#if 0
void sendPad(void)
{
    BYTE i, j, k, l;

    SR_SL(0);
    delayUs(1);
    SR_SL(1);

    for(i = 0; i < MAX_BTN_ROW; i++)
    {
        btnLast[i] = btnCurrent[i];
    }

    for(i = 0; i < MAX_BTN_ROW; i++)
    {
        switch(i)
        {
            case 0:
                l = 1;
                break;
            case 1:
                l = 0;
                break;
            case 2:
                l = 3;
                break;
            case 3:
                l = 2;
                break;
        }
        for(j = 0; j < MAX_BTN_COL; j++)
        {
            k = 3 - j;

            if(SR_QH())
            {
                btnCurrent[l] &= ~(1 << k);
            }
            else
            {
                btnCurrent[l] |= (1 << k);
            }
            SR_CLK(1);
            SR_CLK(0);

            if(buttonCheck(l, k))
            {
                sendOSCMessage(prefix, msgLatticePad, "iii", l, k, (btnCurrent[l] & (1 << k)) ? 1 : 0);
            }
        }
    }
}
#endif