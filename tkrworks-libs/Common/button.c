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
 * button.c,v.0.9.0 2014/04/15
 */

#include "button.h"
// for LED_ENC_32
static BOOL initPadFlag = FALSE;
static BYTE numConnectedLatticePad = 1;
static char pin_clk[MAX_PAD_NUM][4] = {0};
static char pin_shld[MAX_PAD_NUM][4] = {0};
static char pin_qh[MAX_PAD_NUM][4] = {0};
static char pin_load[4] = {0};

static WORD btnCurrent[MAX_PAD_NUM][MAX_BTN_ROW] = {0};
static WORD btnLast[MAX_PAD_NUM][MAX_BTN_ROW] = {0};
static WORD btnState[MAX_PAD_NUM][MAX_BTN_ROW] = {0};
static WORD btnDebounceCount[MAX_PAD_NUM][MAX_BTN_ROW][MAX_BTN_COL] = {0};

static BOOL initLatticeLedDrvFlag = FALSE;
static BYTE latticeLedDrvSpiNum = 2;
static WORD latticeLed[MAX_PAD_NUM] = {0};
static BYTE latticeIntensity[MAX_PAD_NUM][MAX_BTN_NUM] = {0};
static BOOL latticeLedOn[MAX_PAD_NUM] = {FALSE};
static BYTE latticeLedIntensityIndex[MAX_PAD_NUM] = {0};

static BOOL initLatticeRgbDrvFlag = FALSE;
static BYTE numConnectedLatticeRgb = 1;
static BYTE latticeRgbDrvSpiNum = 2;
static BYTE latticeRgbLedNum[MAX_PAD_NUM] = {16, 16, 16, 16, 16};
static WORD latticeRgb[MAX_PAD_NUM][MAX_RGB_LAYER] = {0};
static BYTE latticeRgbIntensity[MAX_PAD_NUM][MAX_RGB_LAYER][MAX_BTN_NUM] = {0};
static BOOL latticeRgbOn[MAX_PAD_NUM][MAX_RGB_LAYER] = {FALSE};
static BYTE latticeRgbIntensityIndex[MAX_PAD_NUM][MAX_RGB_LAYER] = {0};

#if 0 // for 64
static const WORD matrixLedData[MAX_BTN_ROW][MAX_BTN_COL] = {{0x01, 0x03, 0x10, 0x12},
                                                             {0x00, 0x02, 0x11, 0x13},
                                                             {0x33, 0x31, 0x22, 0x21},
                                                             {0x32, 0x30, 0x23, 0x20}};
#endif

/*******************************************************************************
  Function:
    void setInitPadFlag(BOOL flag)

  Precondition:


  Summary:


  Description:


  Parameters:
    BOOL flag

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setInitPadFlag(BOOL flag)
{
    initPadFlag = flag;
}

/*******************************************************************************
  Function:
    void setNumConnectedLatticePad(BYTE num)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE num

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setNumConnectedLatticePad(BYTE num)
{
    numConnectedLatticePad = num;
}

/*******************************************************************************
  Function:
    BYTE getNumConnectedLatticePad(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:


  Remarks:
    None
*******************************************************************************/
BYTE getNumConnectedLatticePad(void)
{
    return numConnectedLatticePad;
}

/*******************************************************************************
  Function:
    void setLatticePadPortClkName(BYTE index, char* name)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index
    char* name

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setLatticePadPortClkName(BYTE index, char* name)
{
    memset(pin_clk[index], 0, sizeof(pin_clk[index]));
    strcpy(pin_clk[index], name);
}

/*******************************************************************************
  Function:
    char* getLatticePadPortClkName(BYTE index)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index

  Return Values:


  Remarks:
    None
*******************************************************************************/
char* getLatticePadPortClkName(BYTE index)
{
    return pin_clk[index];
}

/*******************************************************************************
  Function:
    void setLatticePadPortShLdName(BYTE index, char* name)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index
    char* name

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setLatticePadPortShLdName(BYTE index, char* name)
{
    memset(pin_shld[index], 0, sizeof(pin_shld[index]));
    strcpy(pin_shld[index], name);
}

/*******************************************************************************
  Function:
    char* getLatticePadPortShLdName(BYTE index)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index

  Return Values:


  Remarks:
    None
*******************************************************************************/
char* getLatticePadPortShLdName(BYTE index)
{
    return pin_shld[index];
}

/*******************************************************************************
  Function:
    void setLatticePadPortQhName(BYTE index, char* name)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index
    char* name

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setLatticePadPortQhName(BYTE index, char* name)
{
    memset(pin_qh[index], 0, sizeof(pin_qh[index]));
    strcpy(pin_qh[index], name);
}

/*******************************************************************************
  Function:
    char* getLatticePadPortQhName(BYTE index)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index

  Return Values:


  Remarks:
    None
*******************************************************************************/
char* getLatticePadPortQhName(BYTE index)
{
    return pin_qh[index];
}

/*******************************************************************************
  Function:
    void setLatticePadPortLoadName(char* name)

  Precondition:


  Summary:


  Description:


  Parameters:
    char* name

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setLatticePadPortLoadName(char* name)
{
    memset(pin_load, 0, sizeof(pin_load));
    strcpy(pin_load, name);
}

/*******************************************************************************
  Function:
    char* getLatticePadPortLoadName(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:


  Remarks:
    None
*******************************************************************************/
char* getLatticePadPortLoadName(void)
{
    return pin_load;
}

/*******************************************************************************
  Function:
    void setInitLatticeLedDrvFlag(BOOL flag)

  Precondition:


  Summary:


  Description:


  Parameters:
    BOOL flag

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setInitLatticeLedDrvFlag(BOOL flag)
{
    initLatticeLedDrvFlag = flag;
}

/*******************************************************************************
  Function:
    BOOL getInitLatticeLedDrvFlag(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:


  Remarks:
    None
*******************************************************************************/
BOOL getInitLatticeLedDrvFlag(void)
{
    return initLatticeLedDrvFlag;
}

/*******************************************************************************
  Function:
    void setLatticeLedDriverSpiNumber(BYTE num)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE num

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setLatticeLedDriverSpiNumber(BYTE num)
{
    latticeLedDrvSpiNum = num;
}

/*******************************************************************************
  Function:
    BYTE getLatticeLedDriverSpiNumber(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:


  Remarks:
    None
*******************************************************************************/
BYTE getLatticeLedDriverSpiNumber(void)
{
    return latticeLedDrvSpiNum;
}

/*******************************************************************************
  Function:
    void setLatticeLed(BYTE index, WORD data)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index
    WORD data

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setLatticeLed(BYTE index, WORD data)
{
    latticeLed[index] = data;
}

/*******************************************************************************
  Function:
    WORD getLatticeLed(BYTE index)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index

  Return Values:


  Remarks:
    None
*******************************************************************************/
WORD getLatticeLed(BYTE index)
{
    return latticeLed[index];
}

/*******************************************************************************
  Function:
    void setLatticeIntensity(BYTE index0, BYTE index1, BYTE value)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index0
    BYTE index1
    BYTE value

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setLatticeIntensity(BYTE index0, BYTE index1, BYTE value)
{
    latticeIntensity[index0][index1] = value;
}

/*******************************************************************************
  Function:
    BYTE getLatticeIntensity(BYTE index0, BYTE index1)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index0
    BYTE index1

  Return Values:


  Remarks:
    None
*******************************************************************************/
BYTE getLatticeIntensity(BYTE index0, BYTE index1)
{
    return latticeIntensity[index0][index1];
}

/*******************************************************************************
  Function:
    void setLatticeLedOn(BYTE index, BOOL flag)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index
    BOOL flag

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setLatticeLedOn(BYTE index, BOOL flag)
{
    latticeLedOn[index] = flag;
}

/*******************************************************************************
  Function:
    BOOL getLatticeLedOn(BYTE index)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index

  Return Values:


  Remarks:
    None
*******************************************************************************/
BOOL getLatticeLedOn(BYTE index)
{
    return latticeLedOn[index];
}

/*******************************************************************************
  Function:
    void setInitLatticeRgbDrvFlag(BOOL flag)

  Precondition:


  Summary:


  Description:


  Parameters:
    BOOL flag

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setInitLatticeRgbDrvFlag(BOOL flag)
{
    initLatticeRgbDrvFlag = flag;
}

/*******************************************************************************
  Function:
    BOOL getInitLatticeRgbDrvFlag(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:


  Remarks:
    None
*******************************************************************************/
BOOL getInitLatticeRgbDrvFlag(void)
{
    return initLatticeRgbDrvFlag;
}

/*******************************************************************************
  Function:
    void setNumConnectedLatticeRgb(BYTE num)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE num

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setNumConnectedLatticeRgb(BYTE num)
{
    numConnectedLatticeRgb = num;
}

/*******************************************************************************
  Function:
    BYTE getNumConnectedLatticeRgb(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:


  Remarks:
    None
*******************************************************************************/
BYTE getNumConnectedLatticeRgb(void)
{
    return numConnectedLatticeRgb;
}

/*******************************************************************************
  Function:
    void setLatticeRgbDriverSpiNumber(BYTE num)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE num

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setLatticeRgbDriverSpiNumber(BYTE num)
{
    latticeRgbDrvSpiNum = num;
}

/*******************************************************************************
  Function:
    BYTE getLatticeRgbDriverSpiNumber(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:


  Remarks:
    None
*******************************************************************************/
BYTE getLatticeRgbDriverSpiNumber(void)
{
    return latticeRgbDrvSpiNum;
}

/*******************************************************************************
  Function:
    void setLatticeRgbLedNumber(BYTE index, BYTE num)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index
    BYTE num

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setLatticeRgbLedNumber(BYTE index, BYTE num)
{
    latticeRgbLedNum[index] = num;
}

/*******************************************************************************
  Function:
    BYTE getLatticeRgbLedNumber(BYTE index)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index

  Return Values:


  Remarks:
    None
*******************************************************************************/
BYTE getLatticeRgbLedNumber(BYTE index)
{
    return latticeRgbLedNum[index];
}

/*******************************************************************************
  Function:
    void setLatticeRgb(BYTE index, BYTE layer WORD data)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index
    BYTE layer
    WORD data

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setLatticeRgb(BYTE index, BYTE layer, WORD data)
{
    latticeRgb[index][layer] = data;
}

/*******************************************************************************
  Function:
    WORD getLatticeRgb(BYTE index, BYTE layer)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index
    BYTE layer

  Return Values:


  Remarks:
    None
*******************************************************************************/
WORD getLatticeRgb(BYTE index, BYTE layer)
{
    return latticeRgb[index][layer];
}

/*******************************************************************************
  Function:
    void setLatticeRgbIntensity(BYTE index0, BYTE layer, BYTE index1, BYTE value)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index0
    BYTE layer
    BYTE index1
    BYTE value

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setLatticeRgbIntensity(BYTE index0, BYTE layer, BYTE index1, BYTE value)
{
    latticeRgbIntensity[index0][layer][index1] = value;
}

/*******************************************************************************
  Function:
    BYTE getLatticeRgbIntensity(BYTE index0, BYTE layer, BYTE index1)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index0
    BYTE layer
    BYTE index1

  Return Values:


  Remarks:
    None
*******************************************************************************/
BYTE getLatticeRgbIntensity(BYTE index0, BYTE layer, BYTE index1)
{
    return latticeRgbIntensity[index0][layer][index1];
}

/*******************************************************************************
  Function:
    void setLatticeRgbOn(BYTE index, BYTE layer BOOL flag)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index
    BYTE layer
    BOOL flag

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void setLatticeRgbOn(BYTE index, BYTE layer, BOOL flag)
{
    latticeRgbOn[index][layer] = flag;
}

/*******************************************************************************
  Function:
    BOOL getLatticeRgbOn(BYTE index, BYTE layer)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index

  Return Values:


  Remarks:
    None
*******************************************************************************/
BOOL getLatticeRgbOn(BYTE index, BYTE layer)
{
    return latticeRgbOn[index][layer];
}

/*******************************************************************************
  Function:
    void buttonInit(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void buttonInit(void)
{
  int i, j, k;
  for(k = 0; k < MAX_PAD_NUM; k++)
  {
      for(i = 0; i < MAX_BTN_ROW; i++)
      {
        btnCurrent[k][i] = 0x00;
        btnLast[k][i] = 0x00;
        btnState[k][i] = 0x00;

        for(j = 0; j < MAX_BTN_COL; j++)
          btnDebounceCount[k][i][j] = 0;
      }
  }
}

/*******************************************************************************
  Function:
    BOOL buttonCheck(BYTE index0, BYTE row, BYTE index)

  Precondition:


  Summary:


  Description:


  Parameters:
    BYTE index0
    BYTE row
    BYTE index

  Return Values:
    TRUE - pressed a button
    FALSE - released a button

  Remarks:
    None
*******************************************************************************/
BOOL buttonCheck(BYTE index0, BYTE row, BYTE index)
{
  BOOL flag = FALSE;

  if(((btnCurrent[index0][row] ^ btnLast[index0][row]) & (1 << index)) && ((btnCurrent[index0][row] ^ btnState[index0][row]) & (1 << index)))
    btnDebounceCount[index0][row][index] = 0;
  else if (((btnCurrent[index0][row] ^ btnLast[index0][row]) & (1 << index)) == 0 && ((btnCurrent[index0][row] ^ btnState[index0][row]) & (1 << index)))
  {
    if(btnDebounceCount[index0][row][index] < 4 && ++btnDebounceCount[index0][row][index] == 4)
    {
      if(btnCurrent[index0][row] & (1 << index))
        btnState[index0][row] |= (1 << index);
      else
        btnState[index0][row] &= ~(1 << index);
      flag = TRUE;
    }
  }
  return flag;
}

/*******************************************************************************
  Function:
    void sendPad4(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
// for LED_PAD_4
#if defined(USE_PITCH)
void sendPad4(void)
{
    int i, j, k, l, m;

    if(getInitSendFlag())
    {
        for(m = 0; m < 1; m++)
        {
            for(i = 0; i < 2; i++)
                btnLast[m][i] = btnCurrent[m][i];

            btnCurrent[m][0] = inputPort("f5") | (inputPort("g7") << 1);
            btnCurrent[m][1] = inputPort("f0") | (inputPort("b14") << 1);

            for(i = 0; i < 2; i++)
            {
                for(j = 0; j < 2; j++)
                {
                    if(buttonCheck(m, i, j))
                    {
                        sendOSCMessage(getOSCPrefix(), "/pad", "iiii", m, i, j, (btnCurrent[m][i] & (1 << j)) ? 1 : 0);
                    }
                }
            }
        }
    }
}
#endif

/*******************************************************************************
  Function:
    void sendPad16(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
// for LED_PAD_16
void sendPad16(void)
{
    int i, j, k, l, m;

    if(getInitSendFlag())
    {
        outputPort(pin_shld[0], LOW);
        //SR_SL(0);
        Delay10us(1);
        outputPort(pin_shld[0], HIGH);
        //SR_SL(1);

        for(m = 0; m < MAX_PAD_NUM; m++)
        {
            for(i = 0; i < MAX_BTN_ROW; i++)
                btnLast[m][i] = btnCurrent[m][i];

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
                    if(inputPort(pin_qh[0]))
                    {
                        btnCurrent[m][l] &= ~(1 << k);
                    }
                    else
                    {
                        btnCurrent[m][l] |= (1 << k);
                    }
                    //SR_CLK(1);
                    outputPort(pin_clk[0], HIGH);
                    //SR_CLK(0);
                    outputPort(pin_clk[0], LOW);

                    if(buttonCheck(m, l, k))
                    {
                        sendOSCMessage(getOSCPrefix(), msgLatticePad, "iiii", m, l, k, (btnCurrent[m][l] & (1 << k)) ? 1 : 0);
                    }
                }
            }
        }
    }
}

/*******************************************************************************
  Function:
    void latticeLedHandle(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
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

/*******************************************************************************
  Function:
    void latticeRgbHandle(void)

  Precondition:


  Summary:


  Description:


  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
*******************************************************************************/
void latticeRgbHandle(void)
{
#if 0
    int i, j, k, l;
    BOOL sendSpiFlag = FALSE;
#else
    static int i = -100;
    int j, k, l;
    static BOOL sendSpiFlag = FALSE;
#endif

    static WORD data[MAX_PAD_NUM][MAX_RGB_LAYER] = {0};
    static WORD data0[MAX_PAD_NUM][MAX_RGB_LAYER] = {0xFFFF};

#if 0
    for(i = numConnectedLatticeRgb - 1; i >= 0; i--)
#endif
    {
        switch(latticeRgbLedNum[i])
        {
            case 8:
                for(k = MAX_RGB_LAYER - 1; k >= 0; k--)
                {
                    if(latticeRgbOn[i][k])
                    {
                        switch(k)
                        {
                            case 0:
                                data[i][0] &= 0xFF00;
                                data[i][0] |= latticeRgb[i][0] & 0x00FF;
                                break;
                            case 1:
                                data[i][0] &= 0x00FF;
                                data[i][0] |= ((latticeRgb[i][1] & 0x00FF) << 8);
                                break;
                            case 2:
                                data[i][1] &= 0x0000;
                                data[i][1] |= latticeRgb[i][2] & 0x00FF;
                                break;
                        }

                        for(j = 7; j >= 0; j--)
                        {
                            if(latticeRgbIntensityIndex[i][k] > latticeRgbIntensity[i][k][j])
                            {
                                switch(k)
                                {
                                    case 0:
                                        data[i][0] &= ~(1 << j);
                                        break;
                                    case 1:
                                        data[i][0] &= ~(1 << (j + 8));
                                        break;
                                    case 2:
                                        data[i][1] &= ~(1 << j);
                                        break;
                                }
                            }
                        }
                    }

                    if(data[i][k] != data0[i][k])
                    {
                        sendSpiFlag = TRUE;
                        data0[i][k] = data[i][k];
                    }

                    latticeRgbIntensityIndex[i][k]++;
                    if(latticeRgbIntensityIndex[i][k] >= 100)
                        latticeRgbIntensityIndex[i][k] = 0;
                }
                break;
            case 16:
                for(k = MAX_RGB_LAYER - 1; k >= 0; k--)
                {
                    if(latticeRgbOn[i][k])
                    {
                        data[i][k] = latticeRgb[i][k];
                        for(j = 15; j >= 0; j--)
                        {
                            if(latticeRgbIntensityIndex[i][k] > latticeRgbIntensity[i][k][j])
                                data[i][k] &= ~(1 << j);
                        }
                    }

                    if(data[i][k] != data0[i][k])
                    {
                        sendSpiFlag = TRUE;
                        data0[i][k] = data[i][k];
                    }

                    latticeRgbIntensityIndex[i][k]++;
                    if(latticeRgbIntensityIndex[i][k] >= 100)
                        latticeRgbIntensityIndex[i][k] = 0;
                }
                break;
            default:
                break;
        }
    }

#if 0
    if(sendSpiFlag)
    {
        outputPort(pin_load, HIGH);

        for(l = numConnectedLatticeRgb - 1; l >= 0; l--)
        {
            switch(latticeRgbLedNum[l])
            {
                case 8:
                    sendSpiTwoWord(latticeRgbDrvSpiNum, data[l][0], data[l][1], 1);
                    break;
                case 16:
                    sendSpiThreeWord(latticeRgbDrvSpiNum, data[l][0], data[l][1], data[l][2], 1);
                    break;
                default:
                    break;
            }
        }
        outputPort(pin_load, LOW);
    }
#endif

#if 1
    if(i <= 0)
#endif
    {
#if 1
        if(sendSpiFlag)
        {
            outputPort(pin_load, HIGH);

            for(l = numConnectedLatticeRgb - 1; l >= 0; l--)
            {
                switch(latticeRgbLedNum[l])
                {
                    case 8:
                        sendSpiTwoWord(latticeRgbDrvSpiNum, data[l][0], data[l][1], 1);
                        break;
                    case 16:
                        sendSpiThreeWord(latticeRgbDrvSpiNum, data[l][0], data[l][1], data[l][2], 1);
                        break;
                    default:
                        break;
                }
            }
            outputPort(pin_load, LOW);
        }
#endif

#if 1
        i = numConnectedLatticeRgb - 1;
        sendSpiFlag = FALSE;
        for(j = MAX_PAD_NUM - 1; j >= 0; j--)
            for(k = MAX_RGB_LAYER - 1; k >= 0; k--)
                data[j][k] = 0;
#endif
    }
#if 1
    else
        i--;
#endif
}
