/*
 * filament_cutter.h
 *
 *  Created on: 24 sie 2021
 *      Author: DELL
 */

#ifndef INC_FILAMENT_CUTTER_H_
#define INC_FILAMENT_CUTTER_H_

#include "main.h"
#include "stepper.h"
#include "L298_dc.h"


typedef enum filament_cutter_mode
{
	STANDBY,
	EXTRUDE,
	CUTTING,
}filament_cutter_mode;


typedef struct FC_parameters
{
	float 			filament_density;
	float		 	filament_diameter;
	uint16_t 		target_weight;
	uint8_t 		sample_quantities;


}FC_parameters;


typedef struct filament_cutter
{
	filament_cutter_mode 		mode;
	FC_parameters				parameters;
	dc_motor                    *dc_motor;
	stepper_motor 				*motor;

}filament_cutter;

void Filament_Cutter_Init(stepper_motor *motor, dc_motor* dc_motor);
void motors_update(stepper_motor *motor, dc_motor* dc_motor);

#endif /* INC_FILAMENT_CUTTER_H_ */
