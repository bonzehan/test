/*
 * PCF8574.h
 *
 *  Created on: Jul 20, 2015
 *      Author: Peter
 */

#ifndef PCF8574_H_
#define PCF8574_H_

#include "stm32f1xx_hal.h"

class PCF8574 {
	I2C_HandleTypeDef *i2c;
	uint8_t address;
public:
	PCF8574(I2C_HandleTypeDef *i2c, uint8_t address = 0);

	void shiftOut(uint8_t val);
	void shiftOut(uint8_t *dat,uint8_t len);
	void shiftIn(uint8_t *val);
	void shiftIn(uint8_t *dat, uint8_t len);

private:
};

#endif /* PCF8574_H_ */
