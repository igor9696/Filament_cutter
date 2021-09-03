/*
 * stepper.h
 *
 *  Created on: Jul 7, 2021
 *      Author: igur
 */

#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#include "L298_dc.h"

#define STEPEER_MOTOR_MAX_FREQ_HZ 		(MICRO_STEP * 1000)
#define STEPPER_MOTOR_MIN_FREQ_HZ 		1
#define STEPPER_MOTOR_MAX_SPEED 		100
#define MICRO_STEP 						1
#define STEP_PER_REVOLUTION 			200
#define JAGGER_DIAMETER					11		// milimeters
#define FULL_ROTATION_LENGTH			0.033




volatile uint8_t FIRST_MISS_FLAG;
volatile uint8_t EXTRUDE_PROCESS_FLAG;



// stepper mode
typedef enum stepper_mode
{
	IDLE = 0,
	ANGLE = 1,
	CONTINOUS = 2,

}stepper_mode;

// Rotate direction

typedef enum stepper_dir
{
	CW = 0,
	CCW = 1,

}stepper_dir;


typedef struct htim_stepper
{
	TIM_HandleTypeDef *htim;
	uint32_t channel;

}htim_stepper;


// stepper basic definition

typedef struct stepper_motor
{
	htim_stepper timer;
	htim_stepper slave_timer;
	stepper_dir dir;
	stepper_mode mode; // IDLE/ANGLE/CONT

	GPIO_TypeDef* DIR_port;
	uint16_t DIR_pin;


	volatile uint32_t step_counter;
	uint32_t steps_to_count;


}stepper_motor;



void stepper_init(stepper_motor *motor, TIM_HandleTypeDef *htim, uint32_t channel,
		TIM_HandleTypeDef *slave_timer, GPIO_TypeDef* Dir_port, uint16_t Dir_pin);
void stepper_stop(stepper_motor *motor);
void stepper_set_dir(stepper_motor *motor, stepper_dir direction);
void stepper_set_speed(stepper_motor *motor, uint32_t speed);
void stepper_set_angle(stepper_motor *motor, uint32_t angle, uint32_t speed, stepper_dir dir);
void stepper_meters_to_rotations(stepper_motor *motor, uint32_t meters, uint32_t speed, stepper_dir dir);
void stepper_extrude_weight(stepper_motor *motor, uint16_t weight);

#endif /* INC_STEPPER_H_ */
