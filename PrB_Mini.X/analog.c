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
    case TYPE_MIDI_FADER:
    	fader = (float)(currentAnalog[port] >> 3) * 1.008;
       	if(fader < 2.0)
          fader = 0.0;
       	else if(fader > 127.0)
          fader = 127.0;
       	vol = (BYTE)fader;
    	break;
    case TYPE_MIDI_VOLUME:
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
    case TYPE_MIDI_ORIGINAL:
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
    case TYPE_LONG_FADER:
      fader = (float)(currentAnalog[port]) * 1.008;
      if(fader < 8.0)
        fader = 0.0;
      else if(fader > 1024.0)
        fader = 1024.0;
      vol = (WORD)fader;
      break;
    case TYPE_LONG_VOLUME:
      vol = (WORD)(1024 - currentAnalog[port]);
      if(vol > 506 && vol < 518)
        vol = 512;
      break;
    case TYPE_LONG_ORIGINAL:
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
    if(labs(currentAnalog[port] - prevAnalog[port]) > MIN_DIFF)
    {
      prevAnalog[port] = currentAnalog[port];
      analogSendFlag[port] = TRUE;
    }
    count[port] = 0;
  }
}