/*
 * L298_dc.h
 *
 *  Created on: Aug 10, 2021
 *      Author: DELL
 */

#ifndef INC_L298_DC_H_
#define INC_L298_DC_H_

#include "main.h"


#define FULL_ROTATION_PULSE (2*2249)
volatile uint8_t CUTTING_PROCESS_FLAG;




typedef enum DC_direction
{
	RIGHT,
	LEFT,
}DC_direction;


typedef struct htim_dc
{
	TIM_HandleTypeDef* 		htim;
	uint32_t 			 channel;

}htim_dc;





typedef struct dc_motor
{
	htim_dc 			pwm_timer;
	GPIO_TypeDef*		Port_IN1;
	GPIO_TypeDef*		Port_IN2;
	uint16_t 			Pin_IN1;
	uint16_t 			Pin_IN2;
	DC_direction		DIR;

	LPTIM_HandleTypeDef* 	encoder_timer;

}dc_motor;



void DC_motor_Init(dc_motor* motor, TIM_HandleTypeDef* Timer, uint32_t PWM_Channel,
		GPIO_TypeDef* Port_IN1, uint16_t IN1_Pin, GPIO_TypeDef* Port_IN2, uint16_t IN2_Pin, LPTIM_HandleTypeDef* Encoder_timer);
void DC_rotate(dc_motor* motor, DC_direction DIR, uint8_t speed);
uint16_t DC_get_encoder_counter(dc_motor* motor);
void DC_stop(dc_motor* motor);
void DC_set_angle(dc_motor *motor, uint16_t angle, uint8_t speed, DC_direction DIR);

#endif /* INC_L298_DC_H_ */
