/******************************************************************************
 *
 * Module: Ultrasonic
 *
 * File Name: Ultrasonic.c
 *
 * Description: Source file for the Ultrasonic driver
 *
 * Author: Moaz Haytham
 *
 *******************************************************************************/


#include "ultrasonic.h"
#include "common_macros.h"
#include "icu.h"
#include "gpio.h"
#include "lcd.h"
#include <util/delay.h>
#include <avr/io.h>

Icu_ConfigType Icu_Config = {F_CPU_8,RISING};

uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;

void Ultrasonic_init(void)
{

	/*Trigger Pin is output pin	 */
	GPIO_setupPinDirection(ULTRASONIC_TRIGGER_PORT_ID, ULTRASONIC_TRIGGER_PIN_ID, PIN_OUTPUT);

	/*	Configuring the ICU */
	Icu_init(&Icu_Config);

	/*	Setup the ICU Callback function	 */
	Icu_setCallBack(Ultrasonic_edgeProcessing);

}


void Ultrasonic_Trigger(void)
{
	/*	Logic High applied on TRIGGER PIN */
	GPIO_writePin(ULTRASONIC_TRIGGER_PORT_ID, ULTRASONIC_TRIGGER_PIN_ID, LOGIC_HIGH);

	/*	Maintain High logic on TRIGGER PIN for 10us */
	_delay_us(10);

	/*	Logic Low applied on TRIGGER PIN */
	GPIO_writePin(ULTRASONIC_TRIGGER_PORT_ID, ULTRASONIC_TRIGGER_PIN_ID, LOGIC_LOW);

}


uint16 Ultrasonic_readDistance(void)
{

	/*Send the trigger pulse by using Ultrasonic_Trigger function.	 */
	Ultrasonic_Trigger();

	while( g_edgeCount != 2);

	g_edgeCount=0;

	return (g_timeHigh)/58.8;

}


void Ultrasonic_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}

	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);

	}
}
