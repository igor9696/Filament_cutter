/*
 * filament_cutter.c
 *
 *  Created on: 24 sie 2021
 *      Author: DELL
 */

#include "filament_cutter.h"

filament_cutter FC_struct;
FC_parameters FC_params;

void Filament_Cutter_Init(stepper_motor *motor, dc_motor* dc_motor)
{
	FC_struct.motor = motor;
	FC_struct.dc_motor = dc_motor;
	FC_struct.mode = STANDBY;
	FC_params.filament_density = Filament_density_PLA;
	FC_params.filament_diameter = Filament_diameter_175;
	FC_params.sample_quantities = 1;
	FC_params.target_weight = Sample_weight_5g;
	FC_params.current_length_cm = 0;
	FC_struct.parameters = FC_params;
}



void motors_update(stepper_motor *motor, dc_motor* dc_motor)
{
	switch(FC_struct.mode)
	{
	case STANDBY:
		// do nothing
		break;

	case EXTRUDE:
		stepper_extrude_weight(motor, FC_struct.parameters.target_weight);
		break;

	case CUTTING:
		DC_set_angle(dc_motor, 360, 50, RIGHT);
		break;
	}
}
