/*
 * MPU6050.cpp
 *
 *  Created on: Jul 26, 2015
 *      Author: peter
 */

#include "MPU6050.h"

MPU6050::MPU6050(I2C_HandleTypeDef* i2c, uint8_t address) :
		i2c(i2c), address(((address & 0x01) << 1) | 0xD0) {
}

void MPU6050::getTemp(float* data) {
}

void MPU6050::getAccel(float* data) {
}

void MPU6050::getGyro(float* data) {
}

void MPU6050::getMeasurement(float* data) {
}

void MPU6050::writeReg(uint8_t reg, uint8_t val) {
	uint8_t buf[2] = { reg, val };
	HAL_I2C_Master_Transmit(i2c, address, buf, 2, 100);
}

uint8_t MPU6050::readReg(uint8_t reg) {
	uint8_t res = 0;
	HAL_I2C_Master_Transmit(i2c, address, &reg, 1, 100);
	HAL_I2C_Master_Receive(i2c, address, &res, 1, 100);
	return res;
}
