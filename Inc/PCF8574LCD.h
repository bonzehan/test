/*
 * PCF8574LCD.h
 *
 *  Created on: Jul 20, 2015
 *      Author: Peter
 */

#ifndef PCF8574LCD_H_
#define PCF8574LCD_H_

#include "stm32f1xx_hal.h"
#include "PCF8574.h"

enum StateBit {
	STATE_RS,
	STATE_RW,
	STATE_E,
	STATE_LED,
	STATE_D4,
	STATE_D5,
	STATE_D6,
	STATE_D7
};

enum LcdPin {
	LCD_RS, LCD_RW, LCD_E, LCD_LED, LCD_D4, LCD_D5, LCD_D6, LCD_D7
};

enum LcdInstruction {
	CLEAR = 0x01,
	RETURN_HOME = 0x02,
	ENTRY_MODE_SET = 0x04,
	DISPLAY_CTRL = 0x08,
	CURSOR_DISPLAY_SHIFT = 0x10,
	FUNCTION_SET = 0x20,
	SET_CGRAM_ADDRESS = 0x40,
	SET_DDRAM_ADDRESS = 0X80
};

class PCF8574_LCD {
	PCF8574 *pcf;
	uint8_t sizeX, sizeY;
	uint8_t state;
	bool led;
	bool initialized;
public:
	PCF8574_LCD(PCF8574 *pcf);

	void clear();
	void returnHome();
	void setEntryMode(bool inc_dec,bool shift);
	void setDisplayControl(bool disp,bool cursor, bool blinking);
	void setFunction(bool dataLength, bool numberOfLines, bool font);
	void setCGRAMAddress(uint8_t address);
	void setDDRAMAddress(uint8_t address);

	void setCursor(int x, int y);

	void writeNumber(int num, uint8_t base);
	void writeString(char *str);

private:
	bool checkBusy();
	void waitForBusy();

	void writeIR(uint8_t val);
	uint8_t readIR();
	uint8_t readDR();
	void writeDR(uint8_t val);

	void setStateBit(StateBit bit, bool val);
	void writeOutState();

	void setDataBus(uint8_t val);

	void clock();

};

#endif /* PCF8574LCD_H_ */
