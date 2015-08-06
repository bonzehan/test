/*
 * BMP180.cpp
 *
 *  Created on: Jul 16, 2015
 *      Author: Peter
 */

#include "BMP180.h"

uint8_t BMP180::i2cAddress = 0xEE;

BMP180::BMP180(I2C_HandleTypeDef* i2c) :
		i2c(i2c) {
	uint8_t t = 0xAA;
	HAL_I2C_Master_Transmit(i2c, i2cAddress, &t, 1, 10);
	HAL_I2C_Master_Receive(i2c, i2cAddress, calibrationData, 22, 100);
	pressure = 0.0;
	temperature = 0.0;
	altitude = 0.0;
	oversampling = 3;
}

float BMP180::readPressure() {
	makeMeasurement();
	return pressure;
}

float BMP180::readTemperature() {
	makeMeasurement();
	return temperature;
}

void BMP180::makeMeasurement() {
	uint8_t buf[3];
	int32_t b6, b3, x3;
	uint8_t t = 0xf6;
	int32_t x1, x2, b5, temp;
	uint32_t b4, b7;
	int32_t p;
	int32_t uTemp;
	int32_t uPressure;
	// reading uncompensated temperature
	buf[0] = 0xf4;
	buf[1] = 0x2e;
	HAL_I2C_Master_Transmit(i2c, i2cAddress, buf, 2, 10);	// set register 0xf4
	HAL_Delay(5);
	HAL_I2C_Master_Transmit(i2c, i2cAddress, &t, 1, 10);// read registers 0xf6 0xf7
	HAL_I2C_Master_Receive(i2c, i2cAddress, buf, 2, 10);
	uTemp = (buf[0] << 8) + buf[1];
	// reading uncompensated pressure
	buf[0] = 0xf4;
	buf[1] = 0x34 + (oversampling << 6);
	HAL_I2C_Master_Transmit(i2c, i2cAddress, buf, 2, 10);	// set register 0xf4
	HAL_Delay(26);
	HAL_I2C_Master_Transmit(i2c, i2cAddress, &t, 1, 10);// read registers 0xf6 0xf7 0xf8
	HAL_I2C_Master_Receive(i2c, i2cAddress, buf, 3, 10);
	uPressure = ((buf[0] << 16) + (buf[1] << 8) + buf[0]) >> (8 - oversampling);
	// calculate true temperature
	x1 = (uTemp - AC6) * AC5 / (1 << 15);
	x2 = MC * (1 << 11) / (x1 + MD);
	b5 = x1 + x2;
	temp = (b5 + 8) / (1 << 4);
	// calculate true pressure
	b6 = b5 - 4000;
	x1 = (B2 * (b6 * b6 / (1 << 12))) / (1 << 11);
	x2 = AC2 * b6 / (1 << 11);
	x3 = x1 + x2;
	b3 = (((AC1 * 4 + x3) << oversampling) + 2) / 4;
	x1 = AC3 * b6 / (1 << 13);
	x2 = (B1 * (b6 * b6 / (1 << 12))) / (1 << 16);
	x3 = (x1 + x2 + 2) / 4;
	b4 = AC4 * (uint32_t(x3 + 32768)) / (1 << 15);
	b7 = (uint32_t(uPressure) - b3) * (50000 >> oversampling);
	if (b7 < 0x80000000) {
		p = (b7 * 2) / b4;
	} else {
		p = (b7 / b4) * 2;
	}
	x1 = (p / (1 << 8)) * (p / (1 << 8));
	x1 = (x1 * 3038) / (1 << 16);
	x2 = (-7357 * p) / (1 << 16);
	p = p + (x1 + x2 + 3791) / (1 << 4);
	// store the results
	temperature = temp * 0.1;
	pressure = p * 0.001;
	calcAltitude();
}

float BMP180::readAltitude() {
	makeMeasurement();
	return altitude;
}

void BMP180::calcAltitude() {
	altitude = 44330 * (1 - pow(pressure / 101.325, 1 / 5.255));
}
