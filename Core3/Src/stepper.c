/*
 * stepper.c
 *
 *  Created on: Jul 7, 2021
 *      Author: igur
 */
#include "main.h"
#include "stepper.h"


void stepper_init(stepper_motor *motor, TIM_HandleTypeDef *htim, uint32_t channel,
		TIM_HandleTypeDef *slave_timer, GPIO_TypeDef* Dir_port, uint16_t Dir_pin)
{
	motor->timer.htim = htim;
	motor->timer.channel = channel;
	motor->slave_timer.htim = slave_timer;
	motor->DIR_port = Dir_port;
	motor->DIR_pin = Dir_pin;
}

// Basic stepper motor control functions

void stepper_stop(stepper_motor *motor)
{
	motor->mode = IDLE;

	__HAL_TIM_SET_COMPARE(motor->timer.htim, motor->timer.channel, 0);
	HAL_TIM_PWM_Stop(motor->timer.htim, motor->timer.channel);
	HAL_TIM_Base_Stop_IT(motor->slave_timer.htim);
}

void stepper_set_dir(stepper_motor *motor, stepper_dir direction)
{
	motor->dir = direction;
	if(motor->dir == CW)
	{
		HAL_GPIO_WritePin(motor->DIR_port, motor->DIR_pin , GPIO_PIN_RESET);
	}

	else
	{
		HAL_GPIO_WritePin(motor->DIR_port, motor->DIR_pin , GPIO_PIN_SET);
	}
}

void stepper_set_speed(stepper_motor *motor, uint32_t speed)
{
	uint32_t freq, counter;

	if(speed > 100)
	{
		speed = 100;
	}

	else if(speed == 0)
	{
		stepper_stop(motor);
		return;
	}

	freq = (speed * (STEPEER_MOTOR_MAX_FREQ_HZ - STEPPER_MOTOR_MIN_FREQ_HZ)) / STEPPER_MOTOR_MAX_SPEED;

	counter = HAL_RCC_GetPCLK1Freq() / (motor->timer.htim->Init.Prescaler * freq);

	__HAL_TIM_SET_COUNTER(motor->timer.htim, 0);
	__HAL_TIM_SET_AUTORELOAD(motor->timer.htim, counter - 1);
	__HAL_TIM_SET_COMPARE(motor->timer.htim, motor->timer.channel, (counter/2)-1);

}


void stepper_set_angle(stepper_motor *motor, uint32_t angle, uint32_t speed, stepper_dir dir)
{
	motor->mode = ANGLE;
	stepper_set_dir(motor, dir);
	stepper_set_speed(motor, speed);


	// set angle
	motor->step_counter = 0;
	motor->steps_to_count = angle * (STEP_PER_REVOLUTION * MICRO_STEP) / 360;


	if(0 == motor->steps_to_count)
	{
		stepper_stop(motor);
	}
	__HAL_TIM_SET_COUNTER(motor->slave_timer.htim, 0);
	__HAL_TIM_SET_AUTORELOAD(motor->slave_timer.htim, motor->steps_to_count - 1);

	HAL_TIM_Base_Start_IT(motor->slave_timer.htim);
	FIRST_MISS_FLAG = 1;
	HAL_TIM_PWM_Start(motor->timer.htim, motor->timer.channel);

}
