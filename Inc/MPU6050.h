/*
 * MPU6050.h
 *
 *  Created on: Jul 26, 2015
 *      Author: peter
 */

#ifndef MPU6050_H_
#define MPU6050_H_

#include "stm32f1xx_hal.h"

enum MPU_REG {
	SELF_TEST_X = 13,
	SELF_TEST_Y = 14,
	SELF_TEST_Z = 15,
	SELF_TEST_A = 16,
	SMPLRT_DIV = 25,
	CONFIG = 26,
	GYRO_CONFIG = 27,
	ACCEL_CONFIG = 28,
	FIFO_EN = 35,
	I2C_MST_CTRL = 36,
	I2C_SLV0_ADDR = 37,
	INT_PIN_CFG = 55,
	INT_ENABLE = 56,
	INT_STATUS = 58,
	ACCEL_XOUT_H = 59,
	ACCEL_YOUT_H = 61,
	ACCEL_ZOUT_H = 63,
	TEMP_OUT_H = 65,
	GYRO_XOUT_H = 67,
	GYRO_YOUT_H = 69,
	GYRO_ZOUT_H = 71,
	SIGNAL_PATH_RESET = 104,
	USER_CTRL = 106,
	PWR_MGMT_1 = 107,
	PWR_MGMT_2 = 108,
	FIFO_COUNTH = 114,
	FIFO_R_W = 116,
	WHO_AM_I = 117
};

enum GYRO_SCALE {
	GYRO_250DPS, GYRO_500DPS, GYRO_1000DPS, GYRO_2000DPS
};

enum ACCEL_SCALE {
	ACCEL_2G, ACCEL_4G, ACCEL_8G, ACCEL_16G
};

enum MPU_FIFO {
	FIFO_ACCEL = 1 << 3,
	FIFO_GYRO_X = 1 << 6,
	FIFO_GYRO_Y = 1 << 5,
	FIFO_GYRO_Z = 1 << 4,
	FIFO_TEMP = 1 << 7
};

enum MPU_INT {
	FIFO_OFLOW_EN = 1 << 4, I2C_MST_INT_EN = 1 << 3, DATA_RDY_EN = 1
};

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

	void writeReg(MPU_REG reg, uint8_t val);
	uint8_t readReg(MPU_REG reg);

	uint16_t readReg16(MPU_REG regHigh);

	void setSampleRateDivider(uint8_t div);

	void setDLPF(uint8_t lpf);

	void setGyroScale(GYRO_SCALE scale);

	void setAccelScale(ACCEL_SCALE scale);

	void enableFIFO(MPU_FIFO fifo);
	void enableFIFO(bool en);

	void enableINT(MPU_INT interrupt);

};

#endif /* MPU6050_H_ */
