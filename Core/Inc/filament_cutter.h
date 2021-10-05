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

#define Filament_diameter_175		1.75
#define Filament_diameter_285		2.85
#define Filament_density_PLA		1.24
#define Filament_density_ABS		1.04
#define Filament_density_PETG		1.23

#define Sample_weight_5g			5
#define Sample_weight_25g			25
#define Sample_weight_50g			50
#define Sample_weight_100g			100


typedef enum filament_cutter_mode
{
	STANDBY,
	EXTRUDE,
	CUTTING,
}filament_cutter_mode;


typedef struct FC_parameters
{
	float 					filament_density;
	float		 			filament_diameter;
	uint16_t 				target_weight;
	uint16_t				temp_weight;

	uint8_t 				sample_quantities;
	uint16_t				current_length_cm;

	volatile uint8_t 		current_qty;
	uint8_t 				target_qty;
	uint8_t					temp_qty;
	uint8_t					temp_qty_increment;

	uint8_t					ACTIVE_START_FLAG;

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
