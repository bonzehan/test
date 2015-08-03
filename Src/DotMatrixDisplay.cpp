/*
 * DotMatrixDisplay.cpp
 *
 *  Created on: Jul 16, 2015
 *      Author: Peter
 */

#include "DotMatrixDisplay.h"

DotMatrixDisplay::DotMatrixDisplay(Pin load, SPI_HandleTypeDef* hspi) :
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

void DotMatrixDisplay::setIntensity(uint8_t intensity) {
	intensity &= 0x0f;
	sendCmd(0x0a, intensity);
}

void DotMatrixDisplay::writePixel(uint8_t x, uint8_t y, uint8_t val) {
	if (val) {
		setPixel(x, y);
	} else {
		resetPixel(x, y);
	}
}

void DotMatrixDisplay::setPixel(uint8_t x, uint8_t y) {
	data[y] |= 1 << x;
}

void DotMatrixDisplay::resetPixel(uint8_t x, uint8_t y) {
	data[y] &= ~(1 << x);
}

void DotMatrixDisplay::writeLine(uint8_t x, uint8_t val) {
	data[x] = val;
}

void DotMatrixDisplay::sendData() {
	for (int i = 0; i < 8; ++i) {
		sendCmd(i + 1, data[i]);
	}
}

void DotMatrixDisplay::update() {
	sendData();
}

void DotMatrixDisplay::sendCmd(uint8_t address, uint8_t val) {
	uint8_t buf[2];
	buf[0] = address;
	buf[1] = val;
	load.reset();
	HAL_SPI_Transmit(hspi, buf, 2, 100);
	load.set();
}
