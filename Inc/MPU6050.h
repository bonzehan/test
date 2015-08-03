/*
 * MPU6050.h
 *
 *  Created on: Jul 26, 2015
 *      Author: peter
 */

#ifndef MPU6050_H_
#define MPU6050_H_

#include "stm32f1xx_hal.h"

class MPU6050 {
	I2C_HandleTypeDef *i2c;
	float measurement[7];
	uint8_t address;

public:
	MPU6050(I2C_HandleTypeDef *i2c, uint8_t address = 0);

	void getTemp(float* data);
	void getAccel(float* data);
	void getGyro(float* data);

	void getMeasurement(float* data);

private:

	void writeReg(uint8_t reg,uint8_t val);
	uint8_t readReg(uint8_t reg);



};

#endif /* MPU6050_H_ */
