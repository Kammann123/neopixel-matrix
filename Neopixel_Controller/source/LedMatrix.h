/*
 * LedMatrix.h
 *
 *  Created on: 6 sep. 2020
 *      Author: Lucas A. Kammann
 */

#ifndef LEDMATRIX_H_
#define LEDMATRIX_H_

#include <stdint.h>

#include "gpio.h"

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} pixel_t;

typedef pixel_t* display_t;

/**
 * initMatrixController
 * @param output Pin assigned to send the matrix data
 */
void initMatrixController(pin_t output);

/**
 * setDisplay
 * Sets the current display to be sent to the neopixel matrix
 * @param display 	Pointer to the display matrix
 * @param size		Size of the matrix = width * height
 */
void setDisplay(display_t display, uint8_t size);

/**
 * refreshDisplay
 * Updates the Neopixel Matrix, sending the current display.
 */
void refreshDisplay(void);

#endif /* LEDMATRIX_H_ */
