#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "gpio.h"
#include "DotMatrixDisplay.h"
#include "BMP180.h"
#include "PCF8574.h"
#include "PCF8574LCD.h"
#include "MPU6050.h"

PCF8574 lcdPcf(&hi2c1, 0x07);
PCF8574_LCD lcd(&lcdPcf);
uint8_t matrixBuf[16];
DotMatrixDisplay display(Pin(GPIOA, GPIO_PIN_6), &hspi1, 2, matrixBuf);
BMP180 bmp(&hi2c1);
PCF8574 relays(&hi2c1);
MPU6050 mpu(&hi2c1);

int main(void) {

	int count = 0;

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.writeString("Hello World!");

	while (1) {

		display.shiftDisplay(true);
		display.writeLine(15, 1 << (count % 8));
		display.update();

		relays.shiftOut(~count);

		lcd.setCursor(0, 1);
		lcd.writeNumber(count, 10);

		count++;
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		//HAL_Delay(10);
	}
}
