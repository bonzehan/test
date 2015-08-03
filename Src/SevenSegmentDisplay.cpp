/*
 * SevenSegmentDisplay.cpp
 *
 *  Created on: Jul 16, 2015
 *      Author: Peter
 */

#include "SevenSegmentDisplay.h"

uint8_t SevenSegmentDisplay::toSegment[16] = { 0x7e, 0x30, 0x6d, 0x79, 0x33,
		0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x77, 0x1f, 0x4e, 0x3d, 0x4f, 0x47 };

SevenSegmentDisplay::SevenSegmentDisplay(Pin load, SPI_HandleTypeDef *hspi) :
		load(load), hspi(hspi) {
	memset(data, 0, 8);
	load.set();
	sendCmd(0x0c, 0x01);	// no shutdown
	sendCmd(0x0f, 0x00);	// no test mode
	sendCmd(0x09, 0x00);	// no decoding
	sendCmd(0x0b, 0x07);	// all digits
	setIntensity(0x03);
	sendData();
}

void SevenSegmentDisplay::dispNum(int num, int base) {
	memset(data, 0, 8);
	int i = 0;
	while (num) {
		uint8_t digit = num % base;
		num /= base;
		data[i++] = toSegment[digit];
	}
	sendData();
}

void SevenSegmentDisplay::setIntensity(uint8_t intensity) {
	intensity &= 0x0f;
	sendCmd(0x0a, intensity);
}

void SevenSegmentDisplay::sendData() {
	for (int i = 0; i < 8; ++i) {
		sendCmd(i + 1, data[i]);
	}
}

void SevenSegmentDisplay::sendCmd(uint8_t address, uint8_t val) {
	uint8_t buf[2];
	buf[0] = address;
	buf[1] = val;
	load.reset();
	HAL_SPI_Transmit(hspi, buf, 2, 100);
	load.set();
}
