/*
 * SevenSegmentDisplay.h
 *
 *  Created on: Jul 16, 2015
 *      Author: Peter
 */

#ifndef SEVENSEGMENTDISPLAY_H_
#define SEVENSEGMENTDISPLAY_H_

#include "stm32f1xx_hal.h"
#include "Pin.h"
#include <string.h>

class SevenSegmentDisplay {
	Pin load;
	SPI_HandleTypeDef *hspi;
	char data[8];

	static uint8_t toSegment[16];
public:
	SevenSegmentDisplay(Pin load, SPI_HandleTypeDef *hspi);

	void dispNum(int num, int base);
	void setIntensity(uint8_t intensity);


private:
	void sendData();
	void sendCmd(uint8_t address, uint8_t val);
};

#endif /* SEVENSEGMENTDISPLAY_H_ */
