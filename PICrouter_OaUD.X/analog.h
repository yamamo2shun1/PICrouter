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
 * analog.h,v.0.5 2012/08/19
 */

#ifndef ANALOG_H
#define	ANALOG_H

#include <plib.h>
#include <GenericTypeDefs.h>
#include <math.h>

#define PITCH
//#define OPT_DRUM

//#define CONFIG_1    ADC_MODULE_OFF | ADC_IDLE_CONTINUE | ADC_FORMAT_INTG16 | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON | ADC_SAMP_ON
//#define CONFIG_2    ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_ON | ADC_SAMPLES_PER_INT_2 | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF
//#define CONFIG_3    ADC_SAMPLE_TIME_1 | ADC_CONV_CLK_SYSTEM | ADC_CONV_CLK_32Tcy
//#define CONFIG_PORT ENABLE_AN0_ANA | ENABLE_AN1_ANA
//#define CONFIG_SKIP SKIP_SCAN_AN2 | SKIP_SCAN_AN3 | SKIP_SCAN_AN4 | SKIP_SCAN_AN5 | SKIP_SCAN_AN6 | SKIP_SCAN_AN7 | SKIP_SCAN_AN8 |\
					SKIP_SCAN_AN9 | SKIP_SCAN_AN10 | SKIP_SCAN_AN11 | SKIP_SCAN_AN12 | SKIP_SCAN_AN13 | SKIP_SCAN_AN14 | SKIP_SCAN_AN15

#define TYPE_MIDI_FADER    0
#define TYPE_MIDI_VOLUME   1
#define TYPE_MIDI_ORIGINAL 2
#define TYPE_LONG_FADER    0
#define TYPE_LONG_VOLUME   1
#define TYPE_LONG_ORIGINAL 2

#ifdef PITCH
	#define USE_ADC_NUM  2
#endif
#ifdef OPT_DRUM
	#define USE_ADC_NUM  10
#endif
#define FLTR_ADC_CNT 8

BOOL analogSendFlag[USE_ADC_NUM];
BYTE count[USE_ADC_NUM];
LONG analog[USE_ADC_NUM][FLTR_ADC_CNT];
LONG currentAnalog[USE_ADC_NUM];
LONG prevAnalog[USE_ADC_NUM];

WORD readAnalogWord(BYTE port);

void resetAnalogFlag(BYTE port);
BOOL getAnalogFlag(BYTE port);
BYTE getAnalogByte(BYTE port, BYTE type);
WORD getAnalogWord(BYTE port, BYTE type);
void analogInHandle(BYTE port, LONG value);

#endif	/* ANALOG_H */

