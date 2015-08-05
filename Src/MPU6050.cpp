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

void MPU6050::writeReg(MPU_REG reg, uint8_t val) {
	uint8_t buf[2] = { reg, val };
	HAL_I2C_Master_Transmit(i2c, address, buf, 2, 100);
}

uint8_t MPU6050::readReg(MPU_REG reg) {
	uint8_t res = 0;
	HAL_I2C_Master_Transmit(i2c, address, (uint8_t*) &reg, 1, 100);
	HAL_I2C_Master_Receive(i2c, address, &res, 1, 100);
	return res;
}

uint16_t MPU6050::readReg16(MPU_REG regHigh) {
	uint8_t high = readReg(regHigh);
	uint8_t low = readReg(MPU_REG(regHigh + 1));
	uint16_t res = (high << 8) | low;
	return res;
}

void MPU6050::setSampleRateDivider(uint8_t div) {
	writeReg(SMPLRT_DIV, div - 1);
}

void MPU6050::setDLPF(uint8_t lpf) {
	if (lpf >= 7)
		lpf = 6;
	writeReg(CONFIG, lpf);
}

void MPU6050::setGyroScale(GYRO_SCALE scale) {
	writeReg(GYRO_CONFIG, scale << 3);
}

void MPU6050::setAccelScale(ACCEL_SCALE scale) {
	writeReg(ACCEL_CONFIG, scale << 3);
}

void MPU6050::enableFIFO(MPU_FIFO fifo) {
	writeReg(FIFO_EN, fifo);
}

void MPU6050::enableFIFO(bool en) {
	uint8_t val;
	if (en)
		val = 1;
	else
		val = 0;
	writeReg(USER_CTRL, val << 6);
}

void MPU6050::enableINT(MPU_INT interrupt) {
	writeReg(INT_ENABLE, interrupt);
}
