/*
 * DotMatrixDisplay.h
 *
 *  Created on: Jul 16, 2015
 *      Author: Peter
 */

#ifndef DOTMATRIXDISPLAY_H_
#define DOTMATRIXDISPLAY_H_

#include "stm32f1xx_hal.h"
#include "Pin.h"
#include <string.h>

class DotMatrixDisplay {
	Pin load;
	uint8_t data[8];
	SPI_HandleTypeDef* hspi;
public:
	DotMatrixDisplay(Pin load, SPI_HandleTypeDef *hspi);

	void setIntensity(uint8_t intensity);

	void writePixel(uint8_t x, uint8_t y, uint8_t val);
	void setPixel(uint8_t x, uint8_t y);
	void resetPixel(uint8_t x, uint8_t y);

	void writeLine(uint8_t x, uint8_t val);

	void update();

private:
	void sendData();
	void sendCmd(uint8_t address, uint8_t val);
};

#endif /* DOTMATRIXDISPLAY_H_ */
