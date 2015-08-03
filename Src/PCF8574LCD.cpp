/*
 * PCF8574LCD.cpp
 *
 *  Created on: Jul 20, 2015
 *      Author: Peter
 */

#include "PCF8574LCD.h"

PCF8574_LCD::PCF8574_LCD(PCF8574 *pcf) :
		pcf(pcf) {
	sizeX = 16;
	sizeY = 2;
	state = 0;
	led = true;
	initialized = false;
	state |= 1 << STATE_LED;

	HAL_Delay(50);

	setStateBit(STATE_E, false);
	setStateBit(STATE_RS, false);
	setStateBit(STATE_RW, false);
	writeOutState();

	setDataBus(0x03);
	writeOutState();

	clock();

	HAL_Delay(5);

	clock();

	HAL_Delay(1);

	clock();

	HAL_Delay(1);

	setDataBus(0x02);
	writeOutState();

	clock();

	HAL_Delay(1);

	setFunction(false, true, false);	// 4bits, two lines, 5x8 font
	HAL_Delay(1);
	setDisplayControl(false, false, false);
	HAL_Delay(1);
	clear();
	HAL_Delay(2);
	setEntryMode(true, false);
	HAL_Delay(1);

	initialized = true;

	setDisplayControl(true, false, false);

}

void PCF8574_LCD::setCursor(int x, int y) {
	uint8_t address = 0;
	address += y * 0x40;
	address += x;
	setDDRAMAddress(address);
}

void PCF8574_LCD::writeString(char* str) {
	while (*str) {
		writeDR(*str);
		str++;
	}
}

bool PCF8574_LCD::checkBusy() {

	uint8_t ir = readIR();
	if (ir & 0x80) {
		return true;
	}
	return false;
}

void PCF8574_LCD::writeIR(uint8_t val) {
	setStateBit(STATE_E, false);
	setStateBit(STATE_RS, false);
	setStateBit(STATE_RW, false);
	writeOutState();

	setDataBus(val >> 4);
	writeOutState();

	clock();

	setDataBus(val);
	writeOutState();

	clock();

	waitForBusy();
}

uint8_t PCF8574_LCD::readDR() {
	uint8_t res = 0;
	uint8_t temp = 0;

	setStateBit(STATE_E, false);
	setStateBit(STATE_RS, true);
	setStateBit(STATE_RW, true);
	writeOutState();

	setStateBit(STATE_E, true);
	writeOutState();

	pcf->shiftIn(&temp);
	for (int i = 0; i < 4; ++i) {
		if (temp & (1 << StateBit(STATE_D4 + i))) {
			res |= 1 << (4 + i);
		}
	}

	setStateBit(STATE_E, false);
	writeOutState();

	setStateBit(STATE_E, true);
	writeOutState();

	pcf->shiftIn(&temp);
	for (int i = 0; i < 4; ++i) {
		if (temp & (1 << StateBit(STATE_D4 + i))) {
			res |= 1 << (i);
		}
	}

	setStateBit(STATE_E, false);
	writeOutState();

	//waitForBusy();

	return res;
}

void PCF8574_LCD::waitForBusy() {
	if (initialized) {
		uint32_t timeout = 100;
		uint32_t startTick = HAL_GetTick();
		while (checkBusy() && (HAL_GetTick() - startTick < timeout))
			;
	}
}

uint8_t PCF8574_LCD::readIR() {
	uint8_t res = 0;
	uint8_t temp = 0;

	setStateBit(STATE_E, false);
	setStateBit(STATE_RS, false);
	setStateBit(STATE_RW, true);
	writeOutState();

	setStateBit(STATE_E, true);
	writeOutState();

	pcf->shiftIn(&temp);
	for (int i = 0; i < 4; ++i) {
		if (temp & (1 << StateBit(STATE_D4 + i))) {
			res |= 1 << (4 + i);
		}
	}

	setStateBit(STATE_E, false);
	writeOutState();

	setStateBit(STATE_E, true);
	writeOutState();

	pcf->shiftIn(&temp);
	for (int i = 0; i < 4; ++i) {
		if (temp & (1 << StateBit(STATE_D4 + i))) {
			res |= 1 << (i);
		}
	}

	setStateBit(STATE_E, false);
	writeOutState();

	//waitForBusy();

	return res;
}

void PCF8574_LCD::writeDR(uint8_t val) {
	setStateBit(STATE_E, false);
	setStateBit(STATE_RS, true);
	setStateBit(STATE_RW, false);
	writeOutState();

	setDataBus(val >> 4);
	writeOutState();

	clock();

	setDataBus(val);
	writeOutState();

	clock();

	waitForBusy();
}

void PCF8574_LCD::setStateBit(StateBit bit, bool val) {
	if (val) {
		state |= 1 << bit;
	} else {
		state &= ~(1 << bit);
	}
}

void PCF8574_LCD::writeOutState() {
	pcf->shiftOut(state);
}

void PCF8574_LCD::setEntryMode(bool inc_dec, bool shift) {
	writeIR(ENTRY_MODE_SET | (inc_dec ? 0x02 : 0) | (shift ? 0x01 : 0));
}

void PCF8574_LCD::setDisplayControl(bool disp, bool cursor, bool blinking) {
	writeIR(
			DISPLAY_CTRL | (disp ? 0x04 : 0) | (cursor ? 0x02 : 0)
					| (blinking ? 0x01 : 0));
}

void PCF8574_LCD::setCGRAMAddress(uint8_t address) {
	writeIR(SET_CGRAM_ADDRESS | (address & 0x3F));
}

void PCF8574_LCD::setDDRAMAddress(uint8_t address) {
	writeIR(SET_DDRAM_ADDRESS | (address & 0x7F));
}

void PCF8574_LCD::setFunction(bool dataLength, bool numberOfLines, bool font) {
	writeIR(
			FUNCTION_SET | (dataLength ? 0x10 : 0) | (numberOfLines ? 0x08 : 0)
					| (font ? 0x04 : 0));
}

void PCF8574_LCD::setDataBus(uint8_t val) {
	for (int i = 0; i < 4; ++i) {
		setStateBit(StateBit(STATE_D4 + i), val & (1 << i) ? true : false);
	}
}

void PCF8574_LCD::writeNumber(int num, uint8_t base) {

	char buf[16];
	buf[15]=0;
	int pos = 15;
	while (num) {
		char digit = num % base;
		if (digit < 10) {
			digit += '0';
		} else {
			digit -= 10;
			digit += 'A';
		}
		buf[--pos] = digit;
		num /= base;
	}
	writeString(&buf[pos]);
}

void PCF8574_LCD::clear() {
	writeIR(CLEAR);
}

void PCF8574_LCD::returnHome() {
	writeIR(RETURN_HOME);
}

void PCF8574_LCD::clock() {
	//uint8_t buf[3];

	setStateBit(STATE_E, false);
	//buf[0]=state;
	writeOutState();

	setStateBit(STATE_E, true);
	//buf[1]=state;
	writeOutState();

	setStateBit(STATE_E, false);
	//buf[2]=state;
	writeOutState();

	//pcf->shiftOut(buf,3);

}
