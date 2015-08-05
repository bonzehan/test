/*
 * DotMatrixDisplay.cpp
 *
 *  Created on: Jul 16, 2015
 *      Author: Peter
 */

#include "DotMatrixDisplay.h"

DotMatrixDisplay::DotMatrixDisplay(Pin load, SPI_HandleTypeDef* hspi,
		uint8_t chain) :
		load(load), hspi(hspi), chain(chain) {
	data = (uint8_t*) malloc(8 * chain);
	memset(data, 0, 8 * chain);
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
		sendData(i + 1, i);
	}
}

void DotMatrixDisplay::sendData(uint8_t address, uint8_t index) {
	uint8_t buf[2 * chain];
	for (int i = 0; i < chain; ++i) {
		buf[2 * i] = address;
		buf[2 * i + 1] = data[index + 8 * i];
	}
	load.reset();
	HAL_SPI_Transmit(hspi, buf, 2 * chain, 100);
	load.set();
}

void DotMatrixDisplay::update() {
	sendData();
}

DotMatrixDisplay::~DotMatrixDisplay() {
	free(data);
}

void DotMatrixDisplay::sendCmd(uint8_t address, uint8_t val,
		uint8_t devicePosition) {
	uint8_t buf[2 * chain];
	for (int i = 0; i < chain; ++i) {
		if (i == devicePosition || devicePosition == 0xff) {
			buf[2 * i] = address;
			buf[2 * i + 1] = val;
		} else {
			buf[2 * i] = 0;
			buf[2 * i + 1] = 0;
		}
	}
	load.reset();
	HAL_SPI_Transmit(hspi, buf, 2 * chain, 100);
	load.set();
}

void DotMatrixDisplay::shiftDisplay(bool left) {
	if (left) {
		for (int i = 1; i < 8 * chain; ++i) {
			data[i - 1] = data[i];
		}
	} else {
		for (int i = (8 * chain) - 2; i >= 0; --i) {
			data[i + 1] = data[i];
		}
	}
}
