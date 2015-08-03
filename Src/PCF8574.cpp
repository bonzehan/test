/*
 * PCF8574.cpp
 *
 *  Created on: Jul 20, 2015
 *      Author: Peter
 */

#include "PCF8574.h"

PCF8574::PCF8574(I2C_HandleTypeDef* i2c, uint8_t address) :i2c(i2c), address(((address&0x07)<<1)|0x40) {
}

void PCF8574::shiftOut(uint8_t val) {
	shiftOut(&val,1);
}

void PCF8574::shiftOut(uint8_t* dat, uint8_t len) {
	HAL_I2C_Master_Transmit(i2c,address,dat,len,100);
}

void PCF8574::shiftIn(uint8_t* val) {
	shiftIn(val,1);
}

void PCF8574::shiftIn(uint8_t* dat, uint8_t len) {
	HAL_I2C_Master_Receive(i2c,address,dat,len,100);
}
