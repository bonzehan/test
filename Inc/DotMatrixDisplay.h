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
#include <cstdlib>

class DotMatrixDisplay {
	Pin load;
	SPI_HandleTypeDef* hspi;
	uint8_t chain;
	uint8_t *data;
	bool externalBuffer;
public:
	DotMatrixDisplay(Pin load, SPI_HandleTypeDef *hspi, uint8_t chain = 1,
			uint8_t* buffer = nullptr);
	~DotMatrixDisplay();

	void setIntensity(uint8_t intensity);

	void writePixel(uint8_t x, uint8_t y, uint8_t val);
	void setPixel(uint8_t x, uint8_t y);
	void resetPixel(uint8_t x, uint8_t y);

	void writeLine(uint8_t x, uint8_t val);

	void shiftDisplay(bool left);

	void update();

private:
	void sendData();
	void sendData(uint8_t address, uint8_t index);
	void sendCmd(uint8_t address, uint8_t val, uint8_t devicePosition = 0xff);

};

#endif /* DOTMATRIXDISPLAY_H_ */
