/* 
 * File:   button.h
 * Author: shun
 *
 * Created on 2012/06/29, 23:31
 */

#ifndef BUTTON_H
#define	BUTTON_H

#include <GenericTypeDefs.h>

#define MAX_BTN_ROW 2
#define MAX_BTN_COL 2

WORD btnCurrent[MAX_BTN_ROW];
WORD btnLast[MAX_BTN_ROW];
WORD btnState[MAX_BTN_ROW];
WORD btnDebounceCount[MAX_BTN_ROW][MAX_BTN_COL];

void buttonInit(void);
BOOL buttonCheck(int row, int index);

#endif	/* BUTTON_H */

