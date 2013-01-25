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
 * encoder.c,v.0.5 2013/01/26
 */

#include "encoder.h"

void initEncoderVariables(void)
{
    BYTE i = 0;

    sendEncFlag = FALSE;
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

    reDataIndex = 0;
    reAvgIndex = 0;
    reVelAvgIndex = 0;
    reVelAvgIndex2 = 0;
    reMatchCount = 0;
    reAbsAnglePosLast = 0;
    reAbsAnglePos = 0;
    reDirection = 0;
    reCounting = 0;
    reCounted = 0;

    reVelocity = 0; 

    for(i = 0; i < AVG_NUM; i++)
    {
        rePosData[i] = 0;
        reAvgCounted[i] = 0;
    }
    for(i = 0; i < 16; i++)
    {
        reState[i] = 0;
        reAvgVelocity[i] = 0;
    }

    dwLedData = 0;
    for(i = 0; i < 100; i++)
        dwLedSequence[i] = 0;
    for(i = 0; i < 32; i++)
        intensity[i] = 0;
    ledOn = FALSE;
    ledCount = 0;
    ledIntensity = 10;
    ledIntensityIndex = 0;

    OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_8, TIMER_COUNT);
    ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_1);
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
        sendEncFlag = TRUE;
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

void __ISR(_TIMER_4_VECTOR, ipl1) encoderHandle(void)
//void encoderHandle(void)
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
#else // for LED_ENC_ABS_32
    int i;
    WORD currentPos;

    if(!initSendFlag)
    {
        mT4ClearIntFlag();
        return;
    }

    annularLedHandle();

    RE_CS(1);
    delayUs(1);
    RE_CS(0);
    delayUs(1);

    for(reDataIndex = 0; reDataIndex < 16; reDataIndex++)
    {
        RE_CLK(0);
        delayUs(1);
        RE_CLK(1);
        delayUs(1);
        reState[reDataIndex] = RE_DO();
    }

    currentPos = 0;
    for(i = 0; i < 10; i++)
        currentPos |= (reState[9 - i] << i);

    reAvgIndex++;
    if(reMatchCount == 0)
    {
        for(i = 0; i < AVG_NUM; i++)
            rePosData[i] = 0;
        rePosData[0] = currentPos;
        reMatchCount = 1;
    }
    else
    {
        BOOL matchFlag = FALSE;
        if(reMatchCount > 8)
            reMatchCount = 8;
        for(i = 0; i < reMatchCount; i++)
        {
            if(currentPos != rePosData[i])
                matchFlag = TRUE;
            else
            {
                matchFlag = FALSE;
                break;
            }
        }
        if(matchFlag)
        {
            if(reMatchCount > 7)
                reMatchCount = 7;
            rePosData[reMatchCount] = currentPos;
            reMatchCount++;
            if(reMatchCount > 8)
                reMatchCount = 8;
        }
    }

    if(reAvgIndex == AVG_NUM)
    {
        if(reMatchCount > 3)
        {
            BYTE startBoundaryCount = 0;
            BYTE endBoundaryCount = 0;
            if(reMatchCount > 8)
                reMatchCount = 8;

            float test = reAbsAnglePos;
            reAbsAnglePos = 0.0;
            for(i = 0; i < reMatchCount; i++)
            {
                if(rePosData[i] <= 10)
                    startBoundaryCount++;
                if(rePosData[i] >= 1013)
                    endBoundaryCount++;
            }

            if(startBoundaryCount == 0 || endBoundaryCount == 0)
            {
                for(i = 0; i < reMatchCount; i++)
                {
                    reAbsAnglePos += (float)rePosData[i];
                }
                reAbsAnglePos /= (float)reMatchCount;
            }
            else
            {
                if(startBoundaryCount > endBoundaryCount)
                {
                    for(i = 0; i < reMatchCount; i++)
                    {
                        if(rePosData[i] <= 10)
                            reAbsAnglePos += (float)rePosData[i];
                    }
                    reAbsAnglePos /= (float)startBoundaryCount;
                }
                else
                {
                    for(i = 0; i < reMatchCount; i++)
                    {
                        if(rePosData[i] >= 1013)
                            reAbsAnglePos += (float)rePosData[i];
                    }
                    reAbsAnglePos /= (float)endBoundaryCount;
                }
            }
            sendEncFlag = TRUE;

            if(reCounting != 0)
            {
                reAvgCounted[reVelAvgIndex] = reCounting;
                reCounted = 0.0;
                for(i = 0; i < 8; i++)
                {
                    if(i <= reVelAvgIndex)
                        reCounted += (7 - (reVelAvgIndex - i)) * reAvgCounted[i];
                    else
                        reCounted += (7 - (reVelAvgIndex - i) - 8) * reAvgCounted[i];
                }
                reCounted /= ((8.0 * 9.0) / 2.0);
                reVelAvgIndex++;
                if(reVelAvgIndex >= 8)
                    reVelAvgIndex = 0;

                float diff = 0;
                if(fabs(reAbsAnglePos - test) > 1000.0)
                {
                    if(reAbsAnglePos - test < 1000.0)
                        diff = (reAbsAnglePos + 1023.0) - test;
                    else if(reAbsAnglePos - test > 1000.0)
                        diff = reAbsAnglePos - (test + 1023.0);
                }
                else
                    diff = reAbsAnglePos - test;

                reAvgVelocity[reVelAvgIndex2] = diff / (reCounted * ((float)TIMER_COUNT / 10.0)) * 1000000.0;
                reVelocity = 0.0;
                for(i = 0; i < 16; i++)
                {
                    if(i <= reVelAvgIndex2)
                        reVelocity += (15 - (reVelAvgIndex2 - i)) * reAvgVelocity[i];
                    else
                        reVelocity += (15 - (reVelAvgIndex2 - i) - 16) * reAvgVelocity[i];
                }
                reVelocity /= ((17.0 * 16.0) / 2.0);
                reVelAvgIndex2++;
                if(reVelAvgIndex2 >= 16)
                    reVelAvgIndex2 = 0;
            }

            reCounting = 0;
        }
        else
        {
            if(reCounting < 100000)
                reCounting++;
        }
        reAvgIndex = 0;
    }
#endif
    mT4ClearIntFlag();
}

void annularLedHandle(void)
{
    if(ledOn)
    {
        WORD msb, lsb;
        msb = (WORD)((dwLedSequence[ledIntensityIndex] >> 16) & 0x0000FFFF);
        lsb = (WORD)(dwLedSequence[ledIntensityIndex] & 0x0000FFFF);
        sendSpiTwoWord(msb, lsb, 8);
        ledIntensityIndex++;
        if(ledIntensityIndex >= 100)
            ledIntensityIndex = 0;
    }
}

void sendEnc(void)
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
#else// for LED_ENC_ABS_32
    if(initSendFlag && sendEncFlag)
    {
        //debug sendOSCMessage(TxSocket, prefix, msgRotaryEnc, "fiiiiiiiii", reAbsAnglePos, reMatchCount, rePosData[0], rePosData[1], rePosData[2], rePosData[3], rePosData[4], rePosData[5], rePosData[6], rePosData[7]);
        if(reAbsAnglePos > reAbsAnglePosLast)
        {
            if(reAbsAnglePos - reAbsAnglePosLast > 1000)
                reDirection = -1;
            else
                reDirection = 1;
        }
        else if(reAbsAnglePos < reAbsAnglePosLast)
        {
            if(reAbsAnglePos - reAbsAnglePosLast < -1000)
                reDirection = 1;
            else
                reDirection = -1;
        }
        else
            reDirection = 0;

        sendOSCMessage(prefix, msgRotaryEnc, "fiff", reAbsAnglePos, reDirection, reVelocity, reCounted);
        sendEncFlag = FALSE;
        reMatchCount = 0;
        reAbsAnglePosLast = reAbsAnglePos;
    }
#endif
}
