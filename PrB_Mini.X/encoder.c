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
 * encoder.c,v.0.1 2012/10/06
 */

#include "encoder.h"

void encoderInit(void)
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