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
 * button.c,v.0.6 2013/05/04
 */

#include "button.h"

// for LED_ENC_32
static BOOL initPadFlag = FALSE;
static BYTE numConnectedLatticePad = 1;
static char pin_clk[MAX_PAD_NUM][4] = {0};
static char pin_shld[MAX_PAD_NUM][4] = {0};
static char pin_qh[MAX_PAD_NUM][4] = {0};
static char pin_load[4] = {0};

static WORD btnCurrent[MAX_BTN_ROW] = {0};
static WORD btnLast[MAX_BTN_ROW] = {0};
static WORD btnState[MAX_BTN_ROW] = {0};
static WORD btnDebounceCount[MAX_BTN_ROW][MAX_BTN_COL] = {0};

static BOOL initLatticeLedDrvFlag = FALSE;
static BYTE latticeLedDrvSpiNum = 2;

static WORD latticeLed[MAX_PAD_NUM] = {0};
static BYTE latticeIntensity[MAX_PAD_NUM][MAX_BTN_NUM] = {0};
static BOOL latticeLedOn[MAX_PAD_NUM] = {FALSE};
static BYTE latticeLedIntensityIndex[MAX_PAD_NUM] = {0};

#if 0 // for 64
static const WORD matrixLedData[MAX_BTN_ROW][MAX_BTN_COL] = {{0x01, 0x03, 0x10, 0x12},
                                                             {0x00, 0x02, 0x11, 0x13},
                                                             {0x33, 0x31, 0x22, 0x21},
                                                             {0x32, 0x30, 0x23, 0x20}};
#endif

void setInitPadFlag(BOOL flag)
{
    initPadFlag = flag;
}

void setNumConnectedLatticePad(BYTE num)
{
    numConnectedLatticePad = num;
}
BYTE getNumConnectedLatticePad(void)
{
    return numConnectedLatticePad;
}

void setLatticePadPortClkName(BYTE index, char* name)
{
    memset(pin_clk[index], 0, sizeof(pin_clk[index]));
    strcpy(pin_clk[index], name);
}
char* getLatticePadPortClkName(BYTE index)
{
    return pin_clk[index];
}

void setLatticePadPortShLdName(BYTE index, char* name)
{
    memset(pin_shld[index], 0, sizeof(pin_shld[index]));
    strcpy(pin_shld[index], name);
}
char* getLatticePadPortShLdName(BYTE index)
{
    return pin_shld[index];
}

void setLatticePadPortQhName(BYTE index, char* name)
{
    memset(pin_qh[index], 0, sizeof(pin_qh[index]));
    strcpy(pin_qh[index], name);
}
char* getLatticePadPortQhName(BYTE index)
{
    return pin_qh[index];
}

void setLatticePadPortLoadName(char* name)
{
    memset(pin_load, 0, sizeof(pin_load));
    strcpy(pin_load, name);
}
char* getLatticePadPortLoadName(void)
{
    return pin_load;
}

void setInitLatticeLedDrvFlag(BOOL flag)
{
    initLatticeLedDrvFlag = flag;
}
BOOL getInitLatticeLedDrvFlag(void)
{
    return initLatticeLedDrvFlag;
}

void setLatticeLedDriverSpiNumber(BYTE num)
{
    latticeLedDrvSpiNum = num;
}
BYTE getLatticeLedDriverSpiNumber(void)
{
    return latticeLedDrvSpiNum;
}

void setLatticeLed(BYTE index, WORD data)
{
    latticeLed[index] = data;
}
WORD getLatticeLed(BYTE index)
{
    return latticeLed[index];
}

void setLatticeIntensity(BYTE index0, BYTE index1, BYTE value)
{
    latticeIntensity[index0][index1] = value;
}
BYTE getLatticeIntensity(BYTE index0, BYTE index1)
{
    return latticeIntensity[index0][index1];
}

void setLatticeLedOn(BYTE index, BOOL flag)
{
    latticeLedOn[index] = flag;
}
BOOL getLatticeLedOn(BYTE index)
{
    return latticeLedOn[index];
}

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
void sendPad16(void)
{
    int i, j, k, l;

    if(getInitSendFlag())
    {
        outputPort(pin_shld, LOW);
        //SR_SL(0);
        Delay10us(1);
        outputPort(pin_shld, HIGH);
        //SR_SL(1);

        for(i = 0; i < MAX_BTN_ROW; i++)
            btnLast[i] = btnCurrent[i];

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

                //if(SR_QH())
                if(inputPort(pin_qh))
                {
                    btnCurrent[l] &= ~(1 << k);
                }
                else
                {
                    btnCurrent[l] |= (1 << k);
                }
                //SR_CLK(1);
                outputPort(pin_clk, HIGH);
                //SR_CLK(0);
                outputPort(pin_clk, LOW);

                if(buttonCheck(l, k))
                {
                    sendOSCMessage(getOSCPrefix(), msgLatticePad, "iii", l, k, (btnCurrent[l] & (1 << k)) ? 1 : 0);
                }
            }
        }
    }
}

void latticeLedHandle(void)
{
    int i, j;
    WORD data = 0;
    static WORD data0 = 0xFFFF;

    outputPort(pin_load, HIGH);

    for(i = numConnectedLatticePad - 1; i >= 0; i--)
    {
        if(latticeLedOn[i])
        {
            data = latticeLed[i];
            for(j = 0; j < 16; j++)
            {
                if(latticeLedIntensityIndex[i] > latticeIntensity[i][j])
                    data &= ~(1 << j);
            }
            //data = latticeLedSequence[i][latticeLedIntensityIndex[i]];
        }
        else
            data = 0;

        if(data != data0)
        {
            sendSpiOneWord(latticeLedDrvSpiNum, data, 1);
            data0 = data;
        }

        latticeLedIntensityIndex[i]++;
        if(latticeLedIntensityIndex[i] >= 100)
            latticeLedIntensityIndex[i] = 0;
    }

    outputPort(pin_load, LOW);
}