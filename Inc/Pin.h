/*
 * Pin.h
 *
 *  Created on: Jul 16, 2015
 *      Author: Peter
 */

#ifndef PIN_H_
#define PIN_H_

#include "stm32f1xx_hal.h"

class Pin {
	GPIO_TypeDef* port;
	uint16_t pin;
public:
	Pin(GPIO_TypeDef* port=GPIOA,uint16_t pin=GPIO_PIN_0);
	void set();
	void reset();
};

#endif /* PIN_H_ */
