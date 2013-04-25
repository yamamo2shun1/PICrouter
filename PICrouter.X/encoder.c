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
 * encoder.c,v.0.6.1 2013/04/24
 */

#include "encoder.h"
#include "TimeDelay.h"

// for LED_ENC_32
static BOOL initIncEncFlag = FALSE;
static char pin_a[4] = {NULL};
static char pin_b[4] = {NULL};
static char pin_sw[4] = {NULL};

// for LED_ENC_ABS_32
static BOOL initAbsEncFlag = FALSE;
static BYTE numConnectedAbsEnc = 1;
static char pin_cs[4] = {NULL};
static char pin_clk[4] = {NULL};
static char pin_do[4] = {NULL};
static char pin_ss[4] = {NULL};

static BOOL initLedDrvFlag = FALSE;
static BYTE spi_num = 2;

static BYTE reA[2];
static BYTE reB[2];

static BYTE reD;
static int reStep;

static BOOL sendEncFlag[MAX_RE_NUM];
static BOOL reFlag[2];
static float omega[2];
static float omega_ma[2][8];
static float alpha[2];
static INT8 direction;
static DWORD encCount[2];
static DWORD dt;

static DWORD dwLedData[MAX_RE_NUM];
static DWORD dwLedSequence[MAX_RE_NUM][100];
static BYTE intensity[MAX_RE_NUM][32];
static BOOL ledOn[MAX_RE_NUM];
static WORD ledCount[MAX_RE_NUM];
static BYTE ledIntensity[MAX_RE_NUM];
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

    direction = 0;
    reD = 0;
    reStep = 0;

    for(i = 0; i < 2; i++)
    {
        reA[i] = 0;
        reB[i] = 0;
        reFlag[i] = FALSE;
        omega[i] = 0.0f;
        encCount[i] = 0;
    }

    for(j = 0; j < MAX_RE_NUM; j++)
    {
        sendEncFlag[j] = FALSE;

        reCurrentPos[j] = 0;
        reAvgIndex[j] = 0;
        reVelAvgIndex[j] = 0;
        reVelAvgIndex2[j] = 0;
        reMatchCount[j] = 0;
        reAbsAnglePosLast[j] = 0;
        reAbsAnglePos[j] = 0;
        reDirection[j] = 0;
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
        for(i = 0; i < 100; i++)
            dwLedSequence[j][i] = 0;
        for(i = 0; i < 32; i++)
            intensity[j][i] = 0;
        ledOn[j] = FALSE;
        ledCount[j] = 0;
        ledIntensity[j] = 10;
        ledIntensityIndex[j] = 0;
    }

#if 1
    OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_8, TIMER_COUNT);
    ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_6);
#endif
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

void setIncEncoderPortAName(char* name)
{
    memset(pin_a, NULL, sizeof(pin_a));
    strcpy(pin_a, name);
}
char* getIncEncoderPortAName(void)
{
    return pin_a;
}

void setIncEncoderPortBName(char* name)
{
    memset(pin_b, NULL, sizeof(pin_b));
    strcpy(pin_b, name);
}
char* getIncEncoderPortBName(void)
{
    return pin_b;
}

void setIncEncoderPortSwName(char* name)
{
    memset(pin_sw, NULL, sizeof(pin_sw));
    strcpy(pin_sw, name);
}
char* getIncEncoderPortSwName(void)
{
    return pin_sw;
}

void setAbsEncoderPortCsName(char* name)
{
    memset(pin_cs, NULL, sizeof(pin_cs));
    strcpy(pin_cs, name);
}
char* getAbsEncoderPortCsName(void)
{
    return pin_cs;
}

void setAbsEncoderPortClkName(char* name)
{
    memset(pin_clk, NULL, sizeof(pin_clk));
    strcpy(pin_clk, name);
}
char* getAbsEncoderPortClkName(void)
{
    return pin_clk;
}

void setAbsEncoderPortDoName(char* name)
{
    memset(pin_do, NULL, sizeof(pin_do));
    strcpy(pin_do, name);
}
char* getAbsEncoderPortDoName(void)
{
    return pin_do;
}

void setLedDriverPortSsName(char* name)
{
    memset(pin_ss, NULL, sizeof(pin_ss));
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

void setDwLedSequence(BYTE index, BYTE step, DWORD data)
{
    dwLedSequence[index][step] = data;
}
DWORD getDwLedSequence(BYTE index, BYTE step)
{
    return dwLedSequence[index][step];
}

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

void encoderCheck(BYTE rea, BYTE reb)
{
    static BYTE cnt = 0;
    BYTE i;
    float sum = 0.0;

    reA[1] = rea;
    reB[1] = reb;
    
    if((reA[1] != reA[0]) || (reB[1] != reB[0]))
    {
        reD = (((reA[0] << 1) + reB[0]) << 1) + ((reA[1] << 1) + reB[1]);
        direction = ((reD & 0x02) >> 1) ? 1 : -1;
        if(reA[1] != reA[0])
        {
            //omega[0] = (float)(direction * (2.0 * M_PI / 256.0) / (encCount[0] * TIMER_COUNT) * 10000000.0);
            //omega_ma[0][cnt] = (float)(direction * (2.0 * M_PI / 256.0) / (encCount[0] * TIMER_COUNT) * 10000000.0);
            omega_ma[0][cnt] = (float)(direction * (1.0) / (encCount[0] * TIMER_COUNT) * 10000000.0);
            sum = 0.0;
            for(i = 0; i < MA_COUNT; i++)
                sum += omega_ma[0][i];
            omega[0] = sum / (float)MA_COUNT;
            alpha[0] = (omega[0] - omega[1]) / (float)(dt * TIMER_COUNT) * 10000000.0;
            reFlag[0] = TRUE;
            reFlag[1] = FALSE;
            encCount[0] = 1;
        }
        else if(reB[1] != reB[0])
        {
            //omega[1] = (float)(direction * (2.0 * M_PI / 256.0) / (encCount[1] * TIMER_COUNT) * 10000000.0);
            //omega_ma[1][cnt] = (float)(direction * (2.0 * M_PI / 256.0) / (encCount[1] * TIMER_COUNT) * 10000000.0);
            omega_ma[1][cnt] = (float)(direction * (1.0) / (encCount[1] * TIMER_COUNT) * 10000000.0);
            sum = 0.0;
            for(i = 0; i < MA_COUNT; i++)
                sum += omega_ma[1][i];
            omega[1] = sum / (float)MA_COUNT;
            alpha[1] = (omega[1] - omega[0]) / (float)(dt * TIMER_COUNT) * 10000000.0;
            reFlag[0] = FALSE;
            reFlag[1] = TRUE;
            encCount[1] = 1;
        }
        dt = 1;
        reA[0] = reA[1];
        reB[0] = reB[1];

        if(direction == 1)
        {
            reStep++;
            if(reStep > 255)
                reStep = 0;
        }
        else
        {
            reStep--;
            if(reStep < 0)
                reStep = 255;
        }
        sendEncFlag[0] = TRUE;
    }
#if 0
    else
    {
        if(encCount[0] < 100000)
            encCount[0]++;
        if(encCount[1] < 100000)
            encCount[1]++;
        if(dt < 100000)
            dt++;
    }
#endif
}

void incEncoderHandle(void)
{
#if 0 // for LED_ENC_32
    encoderCheck(RE_A(), RE_B());
    if(ledOn)
    {
        WORD msb, lsb;
        msb = (WORD)((dwLedSequence[ledIntensityIndex] >> 16) & 0x0000FFFF);
        lsb = (WORD)(dwLedSequence[ledIntensityIndex] & 0x0000FFFF);
        sendSpiTwoWord(msb, lsb, 8);
        ledIntensityIndex++;
        if(ledIntensityIndex == 100)
            ledIntensityIndex = 0;
    }

    if(encCount[0] < 100000)
        encCount[0]++;
    if(encCount[1] < 100000)
        encCount[1]++;
    if(dt < 100000)
        dt++;
#endif
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
                if(rePosData[index][i] <= 10)
                {
                    raap_min += rePosData[index][i];
                    startBoundaryCount++;
                }
                else if(rePosData[index][i] >= 1013)
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

                if(fabs(reAbsAnglePos[index] - reAbsAnglePos0[index]) > 700.0)
                {
                    if(reAbsAnglePos[index] - reAbsAnglePos0[index] < 0.0)
                        diff = (reAbsAnglePos[index] + 1023.0) - reAbsAnglePos0[index];
                    else if(reAbsAnglePos[index] - reAbsAnglePos0[index] > 0.0)
                        diff = reAbsAnglePos[index] - (reAbsAnglePos0[index] + 1023.0);
                }
                else
                    diff = reAbsAnglePos[index] - reAbsAnglePos0[index];

                 reAvgVelocity[index][reVelAvgIndex2[index]] = diff / (reCounted[index] * ((float)TIMER_COUNT / 10.0)) * 1000000.0;
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
        reAvgIndex[index] = 0;
    }
}

#if 1
void __ISR(_TIMER_4_VECTOR, ipl6) encoderHandle(void)
{
    if(initLedDrvFlag)
        annularLedHandle();

    mT4ClearIntFlag();
}
#endif

void annularLedHandle(void)
{
    int i;
    WORD msb, lsb;
    
    outputPort(pin_ss, HIGH);

    for(i = numConnectedAbsEnc - 1; i >= 0; i--)
    {
        if(ledOn[i])
        {
            msb = (WORD)((dwLedSequence[i][ledIntensityIndex[i]] >> 16) & 0x0000FFFF);
            lsb = (WORD)(dwLedSequence[i][ledIntensityIndex[i]] & 0x0000FFFF);
        }
        else
        {
            msb = 0;
            lsb = 0;
        }
        sendSpiTwoWord(spi_num, msb, lsb, 1);

        ledIntensityIndex[i]++;
        if(ledIntensityIndex[i] >= 100)
            ledIntensityIndex[i] = 0;
    }

    outputPort(pin_ss, LOW);
}

void sendEncInc32(void)
{
#if 0 // for LED_ENC_32
    currentSwitch = RE_SW();
    if(prevSwitch != currentSwitch)
    {
        switchFlag = TRUE;
        prevSwitch = currentSwitch;
    }

    if(initSendFlag && sendEncFlag)
    {
        if(reFlag[0])
        {
            sendOSCMessage(TxSocket, prefix, msgRotaryEnc, "iiffi", direction, reStep, omega[0], alpha[0], (1 - currentSwitch));
        }
        else if(reFlag[1])
        {
            sendOSCMessage(TxSocket, prefix, msgRotaryEnc, "iiffi", direction, reStep, omega[1], alpha[1], (1 - currentSwitch));
        }
        sendEncFlag = FALSE;
    }

    if(initSendFlag && switchFlag)
    {
        sendOSCMessage(TxSocket, prefix, msgRotaryEncSwitch, "i", 1 - currentSwitch);
        switchFlag = FALSE;
    }
#endif
}

void sendEncAbs32(BYTE index)
{
    if(getInitSendFlag() && sendEncFlag[index])
    {
        //debug sendOSCMessage(TxSocket, prefix, msgRotaryEnc, "fiiiiiiiii", reAbsAnglePos, reMatchCount, rePosData[0], rePosData[1], rePosData[2], rePosData[3], rePosData[4], rePosData[5], rePosData[6], rePosData[7]);
        if(reAbsAnglePos[index] > reAbsAnglePosLast[index])
        {
            if(reAbsAnglePos[index] - reAbsAnglePosLast[index] > 1000)
                reDirection[index] = -1;
            else
                reDirection[index] = 1;
        }
        else if(reAbsAnglePos[index] < reAbsAnglePosLast[index])
        {
            if(reAbsAnglePos[index] - reAbsAnglePosLast[index] < -1000)
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
    }
}