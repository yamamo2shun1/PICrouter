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
 * encoder.c,v.0.6.6 2013/06/19
 */

#include "encoder.h"
#include "TimeDelay.h"

// for LED_ENC_32
static BOOL initIncEncFlag = FALSE;
static char pin_a[MAX_RE_NUM][4] = {0};
static char pin_b[MAX_RE_NUM][4] = {0};
static char pin_sw[MAX_RE_NUM][4] = {0};

// for LED_ENC_ABS_32
static BOOL initAbsEncFlag = FALSE;
static BYTE numConnectedAbsEnc = 1;
static char pin_cs[4] = {0};
static char pin_clk[4] = {0};
static char pin_do[4] = {0};
static char pin_ss[4] = {0};

static BOOL initLedDrvFlag = FALSE;
static BYTE spi_num = 2;

static BYTE reA[MAX_RE_NUM][2];
static BYTE reB[MAX_RE_NUM][2];

static BYTE reD[MAX_RE_NUM];
static int reStep[MAX_RE_NUM];

static BOOL sendEncFlag[MAX_RE_NUM];
static BOOL reFlag[MAX_RE_NUM][2];
static float omega[MAX_RE_NUM][2];
static float omega_ma[MAX_RE_NUM][2][8];
static INT8 direction[MAX_RE_NUM];

static DWORD dwLedData[MAX_RE_NUM];
static BYTE intensity[MAX_RE_NUM][32];
static BOOL ledOn[MAX_RE_NUM];
static WORD ledCount[MAX_RE_NUM];
static BYTE ledIntensityIndex[MAX_RE_NUM];


// test for EMS22A50
static WORD reCurrentPos[MAX_RE_NUM];
static BYTE reAvgIndex[MAX_RE_NUM];
static BYTE reVelAvgIndex[MAX_RE_NUM];
static int reVelAvgIndex2[MAX_RE_NUM];
static BYTE reMatchCount[MAX_RE_NUM];
static BYTE reState[MAX_RE_NUM][16];
static WORD rePosData[MAX_RE_NUM][AVG_NUM];
static float reAbsAnglePosLast[MAX_RE_NUM];
static float reAbsAnglePos0[MAX_RE_NUM];
static float reAbsAnglePos[MAX_RE_NUM];
static int reDirectionLast[MAX_RE_NUM];
static int reDirection[MAX_RE_NUM];
static DWORD reCounting[MAX_RE_NUM];
static float reCounted[MAX_RE_NUM];
static DWORD reAvgCounted[MAX_RE_NUM][8];
static float reVelocity[MAX_RE_NUM];
static float reAvgVelocity[MAX_RE_NUM][16];

static void receiveEncoderData(BYTE index);
static void matchingEncoderData(BYTE index);
static void calculateEncoderPosition(BYTE index);
static void calculateEncoderVelocity(BYTE index);
static void smoothingEncoderVelocity(BYTE index);

void initEncoderVariables(void)
{
    BYTE i = 0, j = 0;

    for(j = 0; j < MAX_RE_NUM; j++)
    {
        direction[j] = 0;
        reD[j] = 0;
        reStep[j] = 0;

        for(i = 0; i < 2; i++)
        {
            reA[j][i] = 0;
            reB[j][i] = 0;
            reFlag[j][i] = FALSE;
            omega[j][i] = 0.0f;
        }

        sendEncFlag[j] = FALSE;

        reCurrentPos[j] = 0;
        reAvgIndex[j] = 0;
        reVelAvgIndex[j] = 0;
        reVelAvgIndex2[j] = 0;
        reMatchCount[j] = 0;
        reAbsAnglePosLast[j] = 0;
        reAbsAnglePos[j] = 0;
        reDirection[j] = 0;
        reDirectionLast[j] = 0;
        reCounting[j] = 0;
        reCounted[j] = 0;

        reVelocity[j] = 0;

        for(i = 0; i < AVG_NUM; i++)
        {
            rePosData[j][i] = 0;
            reAvgCounted[j][i] = 0;
        }
        for(i = 0; i < 16; i++)
        {
            reState[j][i] = 0;
            reAvgVelocity[j][i] = 0;
        }

        dwLedData[j] = 0;
#if 0
        for(i = 0; i < 100; i++)
            dwLedSequence[j][i] = 0;
#endif
        for(i = 0; i < 32; i++)
            intensity[j][i] = 0;
        ledOn[j] = FALSE;
        ledCount[j] = 0;
        ledIntensityIndex[j] = 0;
    }
}

void setNumConnectedAbsEnc(BYTE num)
{
    numConnectedAbsEnc = num;
}
BYTE getNumConnectedAbsEnc(void)
{
    return numConnectedAbsEnc;
}

void setInitIncEncFlag(BOOL flag)
{
    initIncEncFlag = flag;
}
void setInitAbsEncFlag(BOOL flag)
{
    initAbsEncFlag = flag;
}
void setInitLedDrvFlag(BOOL flag)
{
    initLedDrvFlag = flag;
}

void setIncEncoderPortAName(BYTE index, char* name)
{
    memset(pin_a[index], 0, sizeof(pin_a[index]));
    strcpy(pin_a[index], name);
}
char* getIncEncoderPortAName(BYTE index)
{
    return pin_a[index];
}

void setIncEncoderPortBName(BYTE index, char* name)
{
    memset(pin_b[index], 0, sizeof(pin_b[index]));
    strcpy(pin_b[index], name);
}
char* getIncEncoderPortBName(BYTE index)
{
    return pin_b[index];
}

void setIncEncoderPortSwName(BYTE index, char* name)
{
    memset(pin_sw[index], 0, sizeof(pin_sw[index]));
    strcpy(pin_sw[index], name);
}
char* getIncEncoderPortSwName(BYTE index)
{
    return pin_sw[index];
}

void setAbsEncoderPortCsName(char* name)
{
    memset(pin_cs, 0, sizeof(pin_cs));
    strcpy(pin_cs, name);
}
char* getAbsEncoderPortCsName(void)
{
    return pin_cs;
}

void setAbsEncoderPortClkName(char* name)
{
    memset(pin_clk, 0, sizeof(pin_clk));
    strcpy(pin_clk, name);
}
char* getAbsEncoderPortClkName(void)
{
    return pin_clk;
}

void setAbsEncoderPortDoName(char* name)
{
    memset(pin_do, 0, sizeof(pin_do));
    strcpy(pin_do, name);
}
char* getAbsEncoderPortDoName(void)
{
    return pin_do;
}

void setLedDriverPortSsName(char* name)
{
    memset(pin_ss, 0, sizeof(pin_ss));
    strcpy(pin_ss, name);
}
char* getLedDriverPortSsName(void)
{
    return pin_ss;
}

void setLedDriverSpiNumber(BYTE num)
{
    spi_num = num;
}
BYTE getLedDriverSpiNumber(void)
{
    return spi_num;
}

BOOL getInitIncEncFlag(void)
{
    return initIncEncFlag;
}
BOOL getInitAbsEncFlag(void)
{
    return initAbsEncFlag;
}
BOOL getInitLedDrvFlag(void)
{
    return initLedDrvFlag;
}

void setDwLedData(BYTE index, DWORD data)
{
    dwLedData[index] = data;
}
DWORD getDwLedData(BYTE index)
{
    return dwLedData[index];
}

#if 0
void setDwLedSequence(BYTE index, BYTE step, DWORD data)
{
    dwLedSequence[index][step] = data;
}
DWORD getDwLedSequence(BYTE index, BYTE step)
{
    return dwLedSequence[index][step];
}
#endif

void setLedOn(BYTE index, BOOL flag)
{
    ledOn[index] = flag;
}
BOOL getLedOn(BYTE index)
{
    return ledOn[index];
}

void setIntensity(BYTE index0, BYTE index1, BYTE value)
{
    intensity[index0][index1] = value;
}
BYTE getIntensity(BYTE index0, BYTE index1)
{
    return intensity[index0][index1];
}

void encoderCheck(BYTE index)
{
    static BYTE cnt = 0;
    int i;
    float sum = 0.0;

    reA[index][1] = inputPort(pin_a[index]);
    reB[index][1] = inputPort(pin_b[index]);
    
    if((reA[index][1] != reA[index][0]) || (reB[index][1] != reB[index][0]))
    {
        reD[index] = (((reA[index][0] << 1) + reB[index][0]) << 1) + ((reA[index][1] << 1) + reB[index][1]);
        direction[index] = ((reD[index] & 0x02) >> 1) ? 1 : -1;
        if(reA[index][1] != reA[index][0])
        {
            //omega_ma[index][0][cnt] = (float)(direction[index] * (1.0) / (encCount[index][0] * TIMER_COUNT) * 10000000.0);
            //sum = 0.0;
            //for(i = 0; i < MA_COUNT; i++)
            //    sum += omega_ma[index][0][i];
            //omega[index][0] = sum / (float)MA_COUNT;
            reFlag[index][0] = TRUE;
            reFlag[index][1] = FALSE;
            //encCount[index][0] = 1;
        }
        else if(reB[index][1] != reB[index][0])
        {
            //omega_ma[index][1][cnt] = (float)(direction[index] * (1.0) / (encCount[index][1] * TIMER_COUNT) * 10000000.0);
            //sum = 0.0;
            //for(i = 0; i < MA_COUNT; i++)
            //    sum += omega_ma[index][1][i];
            //omega[index][1] = sum / (float)MA_COUNT;
            reFlag[index][0] = FALSE;
            reFlag[index][1] = TRUE;
            //encCount[index][1] = 1;
        }
        //dt[index] = 1;
        reA[index][0] = reA[index][1];
        reB[index][0] = reB[index][1];

        if(direction[index] == 1)
        {
            reStep[index]++;
            if(reStep[index] > 255)
                reStep[index] = 0;
        }
        else
        {
            reStep[index]--;
            if(reStep[index] < 0)
                reStep[index] = 255;
        }
        sendEncFlag[index] = TRUE;
    }
}

void incEncoderHandle(BYTE index)
{
    if(!getInitSendFlag() || !initIncEncFlag)
        return;

    encoderCheck(index);
}

void sendEncInc32(BYTE index)
{
    BYTE currentSwitch[MAX_RE_NUM];
    static BYTE prevSwitch[MAX_RE_NUM];
    static BOOL switchFlag[MAX_RE_NUM];

    currentSwitch[index] = inputPort(pin_sw[index]);
    if(prevSwitch[index] != currentSwitch[index])
    {
        switchFlag[index] = TRUE;
        prevSwitch[index] = currentSwitch[index];
    }

    if(getInitSendFlag() && sendEncFlag[index])
    {
        if(reFlag[index][0])
            sendOSCMessage(getOSCPrefix(), msgRotaryIncEnc, "iiii", index, direction[index], reStep[index], (1 - currentSwitch[index]));
        else if(reFlag[index][1])
            sendOSCMessage(getOSCPrefix(), msgRotaryIncEnc, "iiii", index, direction[index], reStep[index], (1 - currentSwitch[index]));

        sendEncFlag[index] = FALSE;
    }

    if(getInitSendFlag() && switchFlag[index])
    {
        sendOSCMessage(getOSCPrefix(), msgRotaryIncEncSwitch, "i", 1 - currentSwitch[index]);
        switchFlag[index] = FALSE;
    }
}

void absEncoderHandle(void)
{
    int i = 0;
    static BYTE state_index = 0;
    static BYTE j = 0;

    if(!getInitSendFlag() || !initAbsEncFlag)
        return;

    switch(state_index)
    {
        case 0:
            receiveEncoderData(j);
            state_index = 1;
            break;
        case 1:
            matchingEncoderData(j);
            state_index = 2;
            break;
        case 2:
            calculateEncoderPosition(j);
            state_index = 3;
            break;
        case 3:
            calculateEncoderVelocity(j);
            state_index = 4;
        case 4:
            smoothingEncoderVelocity(j);
            j++;
            if(j >= numConnectedAbsEnc)
                j = 0;
            state_index = 0;
            break;
    }

    for(i = 0; i < numConnectedAbsEnc; i++)
    {
        if(reCounting[i] < 100000)
            reCounting[i]++;
    }
}

static void receiveEncoderData(BYTE index)
{
    int i;

    if(index == 0)
    {
        outputPort(pin_cs, HIGH);
        Nop();
        outputPort(pin_cs, LOW);
        Nop();
    }

    if(index > 0)
    {
        outputPort(pin_clk, LOW);
        Nop();
        outputPort(pin_clk, HIGH);
        Nop();
    }

    reCurrentPos[index] = 0;
    for(i = 0; i < 16; i++)
    {
        outputPort(pin_clk, LOW);
        Nop();
        outputPort(pin_clk, HIGH);
        Nop();
        reState[index][i] = inputPort(pin_do);

        if(i < 10)
            reCurrentPos[index] |= (reState[index][i] << (9 - i));
    }
}

static void matchingEncoderData(BYTE index)
{
    int i;
    BOOL matchFlag = FALSE;

    reAvgIndex[index]++;
    if(reMatchCount[index] == 0)
    {
        for(i = 0; i < AVG_NUM; i++)
            rePosData[index][i] = 0;
        rePosData[index][0] = reCurrentPos[index];
        reMatchCount[index] = 1;
    }
    else
    {
        if(reMatchCount[index] > 8)
            reMatchCount[index] = 8;
        for(i = 0; i < reMatchCount[index]; i++)
        {
            if(reCurrentPos[index] != rePosData[index][i])
                matchFlag = TRUE;
            else
            {
                matchFlag = FALSE;
                break;
            }
        }
        if(matchFlag)
        {
            if(reMatchCount[index] > 7)
                reMatchCount[index] = 7;
            rePosData[index][reMatchCount[index]] = reCurrentPos[index];
            reMatchCount[index]++;
        }
    }
}

static void calculateEncoderPosition(BYTE index)
{
    int i;
    BYTE startBoundaryCount = 0;
    BYTE endBoundaryCount = 0;
    float raap_min = 0.0;
    float raap_max = 0.0;
    float raap_mid = 0.0;

    if(reAvgIndex[index] == AVG_NUM)
    {
        if(reMatchCount[index] > 3)
        {
            reAbsAnglePos0[index] = reAbsAnglePos[index];
                
            if(reMatchCount[index] > 8)
                reMatchCount[index] = 8;

            for(i = 0; i < reMatchCount[index]; i++)
            {
                //if(rePosData[index][i] <= 10)
                if(rePosData[index][i] <= 200)
                {
                    raap_min += rePosData[index][i];
                    startBoundaryCount++;
                }
                //else if(rePosData[index][i] >= 1013)
                else if(rePosData[index][i] >= 800)
                {
                    raap_max += rePosData[index][i];
                    endBoundaryCount++;
                }
                raap_mid += rePosData[index][i];
            }

            if(startBoundaryCount == 0 || endBoundaryCount == 0)
                reAbsAnglePos[index] = raap_mid / (float)reMatchCount[index];
            else
            {
                if(startBoundaryCount > endBoundaryCount)
                    reAbsAnglePos[index] = raap_min / (float)startBoundaryCount;
                else
                    reAbsAnglePos[index] = raap_max / (float)endBoundaryCount;
            }
        }
    }
}

static void calculateEncoderVelocity(BYTE index)
{
    int i;
    float diff = 0.0;
    
    if(reAvgIndex[index] == AVG_NUM)
    {
        if(reMatchCount[index] > 3)
        {
            if(reCounting[index] != 0)
            {
                reAvgCounted[index][reVelAvgIndex[index]] = reCounting[index];
                reCounted[index] = 0.0;
                for(i = 0; i < 8; i++)
                {
                    if(i <= reVelAvgIndex[index])
                        reCounted[index] += (7 - (reVelAvgIndex[index] - i)) * reAvgCounted[index][i];
                    else
                        reCounted[index] += (7 - (reVelAvgIndex[index] - i) - 8) * reAvgCounted[index][i];
                }
                reCounted[index] /= 36.0;// ((8.0 * 9.0) / 2.0);
                reVelAvgIndex[index]++;
                if(reVelAvgIndex[index] >= 8)
                    reVelAvgIndex[index] = 0;

                //if(fabs(reAbsAnglePos[index] - reAbsAnglePos0[index]) > 700.0)
                if((reAbsAnglePos[index] > 600 && reAbsAnglePos0[index] < 400) ||
                   (reAbsAnglePos[index] < 400 && reAbsAnglePos0[index] > 600))
                {
                    if(reAbsAnglePos[index] - reAbsAnglePos0[index] < 0.0)
                        diff = (reAbsAnglePos[index] + 1023.0) - reAbsAnglePos0[index];
                    else if(reAbsAnglePos[index] - reAbsAnglePos0[index] > 0.0)
                        diff = reAbsAnglePos[index] - (reAbsAnglePos0[index] + 1023.0);
                }
                else
                    diff = reAbsAnglePos[index] - reAbsAnglePos0[index];

                 reAvgVelocity[index][reVelAvgIndex2[index]] = diff / (reCounted[index] * ((float)TIMER4_COUNT / 10.0)) * 1000000.0;
            }
        }
    }
}

static void smoothingEncoderVelocity(BYTE index)
{
    int i;

    if(reAvgIndex[index] == AVG_NUM)
    {
        if(reMatchCount[index] > 3)
        {
            if(reCounting[index] != 0)
            {
                reVelocity[index] = 0.0;
                for(i = 0; i < 16; i++)
                {
                    if(i <= reVelAvgIndex2[index])
                        reVelocity[index] += (15 - (reVelAvgIndex2[index] - i)) * reAvgVelocity[index][i];
                    else
                        reVelocity[index] += (15 - (reVelAvgIndex2[index] - i) - 16) * reAvgVelocity[index][i];
                }
                reVelocity[index] /= 136.0;// ((17.0 * 16.0) / 2.0);
                reVelAvgIndex2[index]++;
                if(reVelAvgIndex2[index] >= 16)
                    reVelAvgIndex2[index] = 0;
            }
            reCounting[index] = 0;
            sendEncFlag[index] = TRUE;
        }
        else
        {
            if(reDirectionLast[index] != 0)
            {
                reDirection[index] = 0;
                sendEncFlag[index] = TRUE;
            }
        }
        reAvgIndex[index] = 0;
    }
}

void annularLedHandle(void)
{
    int i, j;
    DWORD data;
    WORD msb, lsb;
    static WORD msb0, lsb0;
    
    outputPort(pin_ss, HIGH);

    for(i = numConnectedAbsEnc - 1; i >= 0; i--)
    {
        if(ledOn[i])
        {
#if 0
            msb = (WORD)((dwLedSequence[i][ledIntensityIndex[i]] >> 16) & 0x0000FFFF);
            lsb = (WORD)(dwLedSequence[i][ledIntensityIndex[i]] & 0x0000FFFF);
#else
            data = dwLedData[i];
            for(j = 0; j < 32; j++)
            {
                if(ledIntensityIndex[i] > intensity[i][j])
                    data &= ~(1 << j);
            }
            msb = (WORD)((data >> 16) & 0x0000FFFF);
            lsb = (WORD)(data & 0x0000FFFF);
#endif
        }
        else
        {
            msb = 0;
            lsb = 0;
        }
        if(msb != msb0 || lsb != lsb0)
        {
            sendSpiTwoWord(spi_num, msb, lsb, 1);
            msb0 = msb;
            lsb0 = lsb;
        }

        ledIntensityIndex[i]++;
        if(ledIntensityIndex[i] >= 100)
            ledIntensityIndex[i] = 0;
    }

    outputPort(pin_ss, LOW);
}

void sendEncAbs32(BYTE index)
{
    if(getInitSendFlag() && sendEncFlag[index])
    {
        //debug sendOSCMessage(TxSocket, prefix, msgRotaryEnc, "fiiiiiiiii", reAbsAnglePos, reMatchCount, rePosData[0], rePosData[1], rePosData[2], rePosData[3], rePosData[4], rePosData[5], rePosData[6], rePosData[7]);
        if(reAbsAnglePos[index] > reAbsAnglePosLast[index])
        {
            //if(reAbsAnglePos[index] - reAbsAnglePosLast[index] > 1000)
            if(reAbsAnglePos[index] - reAbsAnglePosLast[index] > 700)
                reDirection[index] = -1;
            else
                reDirection[index] = 1;
        }
        else if(reAbsAnglePos[index] < reAbsAnglePosLast[index])
        {
            //if(reAbsAnglePos[index] - reAbsAnglePosLast[index] < -1000)
            if(reAbsAnglePos[index] - reAbsAnglePosLast[index] < -700)
                reDirection[index] = 1;
            else
                reDirection[index] = -1;
        }
        else
            reDirection[index] = 0;

        sendOSCMessage(getOSCPrefix(), msgRotaryAbsEnc, "iiif", index, (int)reAbsAnglePos[index], reDirection[index], reVelocity[index]);
        sendEncFlag[index] = FALSE;
        reMatchCount[index] = 0;
        reAbsAnglePosLast[index] = reAbsAnglePos[index];
        reDirectionLast[index] = reDirection[index];
    }
}