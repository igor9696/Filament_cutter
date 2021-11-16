/*
 * filament_cutter.c
 *
 *  Created on: 24 sie 2021
 *      Author: DELL
 */

#include "filament_cutter.h"
#include "stdio.h"
#include "L298_dc.h"
#include "stepper.h"
#include "menu_LCD.h"

volatile filament_cutter FC_struct;
FC_parameters FC_params;
extern filament_cutter_mode prev_mode;
extern stepper_motor extruder;
extern dc_motor DC_motor;
extern cursor_position cursor_pos;

void Filament_Cutter_Init(stepper_motor *motor, dc_motor* dc_motor)
{
	FC_params.filament_density = Filament_density_PLA;
	FC_params.filament_diameter = Filament_diameter_175;
	FC_params.sample_quantities = 1;
	FC_params.target_weight = Sample_weight_5g;
	FC_params.current_length_cm = 0;
	FC_params.target_qty = 0;
	FC_params.current_qty = 0;
	FC_params.ACTIVE_START_FLAG = 0;
	FC_params.temp_qty_last_val = 0;
	FC_params.temp_qty = 1;


	FC_struct.motor = motor;
	FC_struct.dc_motor = dc_motor;
	FC_struct.mode = STANDBY;
	FC_struct.parameters = FC_params;
}



void motors_update(stepper_motor *motor, dc_motor* dc_motor)
{
	switch(FC_struct.mode)
	{
	case STANDBY:
		DC_stop(&DC_motor);

		if(FC_struct.parameters.current_qty != FC_struct.parameters.target_qty)
		{
			FC_struct.mode = EXTRUDE;
			FC_struct.parameters.current_qty++;
		}

		else
		{
			FC_struct.parameters.ACTIVE_START_FLAG = 0;
		}

		break;

	case EXTRUDE:
		HAL_GPIO_WritePin(EXTRD_SLEEP_GPIO_Port, EXTRD_SLEEP_Pin, GPIO_PIN_SET);
		stepper_extrude_weight(motor);
		break;

	case CUTTING:
		stepper_stop(&extruder);
		DC_set_angle(dc_motor, 360, 100, RIGHT);

		break;
	}
}
