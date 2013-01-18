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
 * analog.c,v.0.5 2012/08/19
 */

#include "analog.h"

void initAnalogVariables(void)
{
    BYTE i, j;

    for(i = 0; i < USE_ADC_NUM; i++)
    {
        analogEnable[i] = FALSE;
        analogSendFlag[i] = FALSE;
        count[i] = 0;
        currentAnalog[i] = 0;
        prevAnalog[i] = 0;

        for(j = 0; j < FLTR_ADC_CNT; j++)
            analog[i][j] = 0;
    }

    // A/D Manual Scan 
    AD1PCFG = 0x0000FFFF;// 0000 0000 0000 0000 1111 1111 1111 1111
    AD1CON2 = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
    AD1CSSL = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
    AD1CON1 = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
    AD1CHS  = 0x00000000;// 0000 0000 0000 0000 0000 0000 0000 0000
    AD1CON3 = 0x00001F08;// 0000 0000 0000 0000 0000 1111 0000 1000
}

void resetAnalogFlag(BYTE port)
{
	analogSendFlag[port] = FALSE;
}

BOOL getAnalogFlag(BYTE port)
{
	return analogSendFlag[port];
}

BYTE getAnalogByte(BYTE port, BYTE type)
{
  float fader;
  BYTE vol;

  switch(type)
  {
    case MIDI_FADER:
    	fader = (float)(currentAnalog[port] >> 3) * 1.008;
       	if(fader < 2.0)
          fader = 0.0;
       	else if(fader > 127.0)
          fader = 127.0;
       	vol = (BYTE)fader;
    	break;
    case MIDI_VOLUME:
#if 0
      vol = (BYTE)(127 - (currentAnalog[port] >> 2));
      if(vol > 60 && vol < 66)
        vol = 63;
#else
      vol = (BYTE)(255 - (currentAnalog[port] >> 2));
      if(vol > 124 && vol < 130)
        vol = 127;
#endif
    	break;
    case MIDI_ORIGINAL:
      vol = (BYTE)(currentAnalog[port] >> 2);
      break;
    default:
      vol = 0;
  }
	return vol;
}

WORD getAnalogWord(BYTE port, BYTE type)
{
  float fader;
  WORD vol;

  switch(type)
  {
    case LONG_FADER:
      fader = (float)(currentAnalog[port]) * 1.008;
      if(fader < 8.0)
        fader = 0.0;
      else if(fader > 1024.0)
        fader = 1024.0;
      vol = (WORD)fader;
      break;
    case LONG_VOLUME:
      vol = (WORD)(1024 - currentAnalog[port]);
      if(vol > 506 && vol < 518)
        vol = 512;
      break;
    case LONG_ORIGINAL:
      vol = (WORD)currentAnalog[port];
      break;
    default:
      vol = 0;
      break;
  }
  return vol;
}

void analogInHandle(BYTE port, LONG value)
{
	BYTE i;

  analog[port][count[port]] = value;
  count[port]++;

  if(count[port] == FLTR_ADC_CNT)
  {
    currentAnalog[port] = 0;
    for(i = 0; i < FLTR_ADC_CNT; i++)
      currentAnalog[port] += analog[port][i];
    currentAnalog[port] /= (LONG)FLTR_ADC_CNT;
    if(labs(currentAnalog[port] - prevAnalog[port]) > 4)
    {
      prevAnalog[port] = currentAnalog[port];
      analogSendFlag[port] = TRUE;
    }
    count[port] = 0;
  }
}

void sendAdc()
{
    BYTE i;

    for(i = 0; i < USE_ADC_NUM; i++)
    {
        if(getAnalogFlag(i))
        {
            sendOSCMessage(prefix, msgGetAdc, "ii", i, getAnalogWord(i, LONG_ORIGINAL));
            resetAnalogFlag(i);
            delayUs(20);
        }
    }
}