/*
 * Pin.cpp
 *
 *  Created on: Jul 16, 2015
 *      Author: Peter
 */

#include "Pin.h"

Pin::Pin(GPIO_TypeDef* port, uint16_t pin) :
		port(port), pin(pin) {
}

void Pin::set() {
	HAL_GPIO_WritePin(port,pin,GPIO_PIN_SET);
}

void Pin::reset() {
	HAL_GPIO_WritePin(port,pin,GPIO_PIN_RESET);
}
