/*
 * LedMatrix.c
 *
 *  Created on: 6 sep. 2020
 *      Author: Lucas A. Kammann
 */

#include "LedMatrix.h"

display_t currentDisplay;
uint8_t currentSize;
pin_t outputPin;
bool refreshingFlag;

void initMatrixController(pin_t output)
{
	outputPin = output;
}

void setDisplay(display_t display, uint8_t size)
{
	currentDisplay = display;
	currentSize = size;
}

void refreshDisplay(void)
{
	if (currentDisplay)
	{
		refreshingFlag = true;
	}
}
