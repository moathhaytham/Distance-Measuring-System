#include "lcd.h"
#include "ultrasonic.h"
#include <avr/io.h>
#include <util/delay.h>

#include "gpio.h"
//#include ""

//Icu_ConfigType Icu_Config;

uint32 distance;
int main(void)
{
	SREG |= (1<<7);

	Ultrasonic_init();

	LCD_init();

	LCD_displayString("Distance =    cm");

	while(1)
	{
		distance = Ultrasonic_readDistance();

		LCD_moveCursor(0,11);
		LCD_intgerToString(distance);

		_delay_ms(200);

	}
}
