/*
 * delay.c
 *
 *  Created on: 07-Jun-2019
 *      Author: poe
 */


/* using TIM 1 to create a delay in microseconds
 * TIM2 Running at 72 MHz
 * therefore prescalar is set to 72-1 to reduce it to 1 MHz
 * ARR is set to MAX i.e. 0xffff
 * rest is unchanged
 */

#include <delay.h>
#include <main.h>
//#include "stm32f1xx.h"



void delay_init ()
{
	HAL_TIM_Base_Start(_TIMER);
}

void delay_us (uint16_t delay)
{
	__HAL_TIM_SET_COUNTER(_TIMER, 0);  // reset the counter
	while ((__HAL_TIM_GET_COUNTER(_TIMER))<delay);  // wait for the delay to complete
}

void delay_ms(uint16_t delay)
{
	HAL_Delay (delay);
}
