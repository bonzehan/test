/*
 * BMP180.h
 *
 *  Created on: Jul 16, 2015
 *      Author: Peter
 */

#ifndef BMP180_H_
#define BMP180_H_

#include "stm32f1xx_hal.h"
#include <cmath>

#define AC1	int16_t((calibrationData[0]<<8)+calibrationData[1])
#define AC2	int16_t((calibrationData[2]<<8)+calibrationData[3])
#define AC3	int16_t((calibrationData[4]<<8)+calibrationData[5])
#define AC4	uint16_t((calibrationData[6]<<8)+calibrationData[7])
#define AC5	uint16_t((calibrationData[8]<<8)+calibrationData[9])
#define AC6	uint16_t((calibrationData[10]<<8)+calibrationData[11])
#define B1	int16_t((calibrationData[12]<<8)+calibrationData[13])
#define B2	int16_t((calibrationData[14]<<8)+calibrationData[15])
#define MB	int16_t((calibrationData[16]<<8)+calibrationData[17])
#define MC	int16_t((calibrationData[18]<<8)+calibrationData[19])
#define MD	int16_t((calibrationData[20]<<8)+calibrationData[21])

class BMP180 {
	I2C_HandleTypeDef *i2c;
	uint8_t calibrationData[22];
	float temperature;
	float pressure;
	float altitude;
	uint8_t oversampling;
	static uint8_t i2cAddress;
public:
	BMP180(I2C_HandleTypeDef *i2c);

	float readPressure();
	float readTemperature();
	float readAltitude();

private:
	void makeMeasurement();
	void calcAltitude();
};

#endif /* BMP180_H_ */
