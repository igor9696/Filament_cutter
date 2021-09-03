/*
 * L298_dc.c
 *
 *  Created on: Aug 10, 2021
 *      Author: DELL
 */

#include "L298_dc.h"

void DC_motor_encoder_Init(dc_motor *motor) {
	//__HAL_TIM_SET_AUTORELOAD(motor->encoder_timer, FULL_ROTATION_PULSE);
	//HAL_TIM_Encoder_Start(motor->encoder_timer, TIM_CHANNEL_ALL);
	HAL_LPTIM_Encoder_Start_IT(motor->encoder_timer, FULL_ROTATION_PULSE);
	/* Enable Autoreload match interrupt */
	__HAL_LPTIM_ENABLE_IT(motor->encoder_timer, LPTIM_IT_ARRM);
}

void DC_motor_Init(dc_motor *motor, TIM_HandleTypeDef *Timer,
		uint32_t PWM_Channel, GPIO_TypeDef *Port_IN1, uint16_t IN1_Pin,
		GPIO_TypeDef *Port_IN2, uint16_t IN2_Pin,
		LPTIM_HandleTypeDef *Encoder_timer) {
	motor->pwm_timer.htim = Timer;
	motor->pwm_timer.channel = PWM_Channel;

	motor->Port_IN1 = Port_IN1;
	motor->Port_IN2 = Port_IN2;
	motor->Pin_IN1 = IN1_Pin;
	motor->Pin_IN2 = IN2_Pin;
	motor->encoder_timer = Encoder_timer;

	DC_motor_encoder_Init(motor);
}

void DC_set_speed(dc_motor *motor, uint8_t speed) {
	if (speed > 100) speed = 100;

	__HAL_TIM_SET_COUNTER(motor->pwm_timer.htim, 0);
	__HAL_TIM_SET_COMPARE(motor->pwm_timer.htim, motor->pwm_timer.channel, speed);
}

void DC_set_direction(dc_motor *motor, DC_direction DIR) {
	motor->DIR = DIR;

	if (motor->DIR == LEFT) {
		HAL_GPIO_WritePin(motor->Port_IN1, motor->Pin_IN1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(motor->Port_IN2, motor->Pin_IN2, GPIO_PIN_SET);
	}

	else {
		HAL_GPIO_WritePin(motor->Port_IN1, motor->Pin_IN1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(motor->Port_IN2, motor->Pin_IN2, GPIO_PIN_RESET);
	}
}

void DC_rotate(dc_motor *motor, DC_direction DIR, uint8_t speed) {
	DC_set_direction(motor, DIR);
	DC_set_speed(motor, speed);

	HAL_TIM_PWM_Start(motor->pwm_timer.htim, motor->pwm_timer.channel);
}

void DC_stop(dc_motor *motor) {
	HAL_GPIO_WritePin(motor->Port_IN1, motor->Pin_IN1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(motor->Port_IN2, motor->Pin_IN2, GPIO_PIN_SET);

	__HAL_TIM_SET_COMPARE(motor->pwm_timer.htim, motor->pwm_timer.channel, 100);
}

uint16_t DC_get_encoder_counter(dc_motor *motor)
{
	return __HAL_TIM_GET_COUNTER(motor->encoder_timer);
}

void DC_set_angle(dc_motor *motor, uint16_t angle, uint8_t speed, DC_direction DIR)
{
	if(CUTTING_PROCESS_FLAG == 0)
	{
		CUTTING_PROCESS_FLAG = 1;
		uint16_t pulses_to_count;
		pulses_to_count = (angle * FULL_ROTATION_PULSE) / 360;
		__HAL_LPTIM_AUTORELOAD_SET(motor->encoder_timer, pulses_to_count);
		DC_rotate(motor, DIR, speed);
	}

}
