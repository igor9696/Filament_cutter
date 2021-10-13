/*
 * menu_LCD.c
 *
 *  Created on: 23 lip 2021
 *      Author: DELL
 */

#include "main.h"
#include "menu_LCD.h"
#include "ST7920_SERIAL.h"
#include "encoder.h"
#include "filament_cutter.h"


extern filament_cutter FC_struct;
extern dc_motor DC_motor;
extern encoder_button enc_btn;

void update_first_layer(cursor_position* curr_position)
{
	if(curr_position->FL_position != curr_position->FL_prev_position)
	{
		switch(curr_position->FL_position)
		{
			case DEFAULT:
			DC_stop(&DC_motor);
			clear_screen();
			default_screen();
			break;

			case START:
			default_screen();

			// highlighting "START"
			ST7920_GraphicMode(1);
			DrawFilledRectangle(88, 48, 80, 16);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

			case ACTIVE_START:
			clear_screen();
			active_start_screen();
			break;

			case STOP:
			clear_screen();
			active_start_screen();
			//start_screen_update(FC_struct.parameters.target_weight, FC_struct.parameters.current_qty, FC_struct.parameters.target_qty);
			// highlighting
			ST7920_GraphicMode(1);
			DrawFilledRectangle(48, 50, 35, 16);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

			case SETTINGS:
			default_screen();
			// highlighting "SETTINGS"
			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 16, 80, 16);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

			default:

			break;

		}

		curr_position->FL_prev_position = curr_position->FL_position;
	}
}


void update_second_layer(cursor_position* curr_position)
{
	if(curr_position->SL_position != curr_position->SL_prev_position)
	{

		switch(curr_position->SL_position)
		{
		case FIL_DIA:
			clear_screen();
			settings_screen();

			// highlighting
			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 0, 79, 14);
			ST7920_Update();
			ST7920_GraphicMode(0);

			break;

		case FIL_DEN:
			clear_screen();
			settings_screen();

			// highlighting
			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 15, 79, 15);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case SAMPLE_WEIGHT:
			clear_screen();
			settings_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 30, 79, 15);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case QUANTITY:
			clear_screen();
			settings_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 48, 79, 15);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case BACK:
			clear_screen();
			settings_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(80, 48, 79, 15);
			ST7920_Update();
			ST7920_GraphicMode(0);

		default:
			break;
		}

		curr_position->SL_prev_position = curr_position->SL_position;
	}

}


void update_third_layer(cursor_position* curr_position)
{
	if(curr_position->TL_position != curr_position->TL_prev_position)
	{
		switch(curr_position->TL_position)
		{
		case DIAMETER_175:
			clear_screen();
			diameter_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 0, 79, 14);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case DIAMETER_285:
			clear_screen();
			diameter_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 15, 79, 15);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case DENSITY_PLA:
			clear_screen();
			density_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 0, 79, 14);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case DENSITY_ABS:
			clear_screen();
			density_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 15, 79, 14);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case DENSITY_PETG:
			clear_screen();
			density_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 30, 79, 14);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case WEIGHT:
			clear_screen();
			weight_screen();
			break;

		case QTY:
			clear_screen();
			quantity_screen();
			break;

		default:
			break;

		}

		curr_position->TL_prev_position = curr_position->TL_position;

	}

}



void menu_update(cursor_position* curr_position)
{

	update_enc(curr_position);

	if(curr_position->current_layer == FIRST_LAYER)
	{

		update_first_layer(curr_position);

		if(curr_position->FL_position == ACTIVE_START)
		{
			start_screen_update(FC_struct.parameters.target_weight, FC_struct.parameters.current_qty, FC_struct.parameters.target_qty);
		}


	}

	else if(curr_position->current_layer == SECOND_LAYER)
	{
		update_second_layer(curr_position);
	}

	else if(curr_position->current_layer == THIRD_LAYER)
	{
		update_third_layer(curr_position);
		// print temporary quantity value on screen
		if(curr_position->TL_position == QTY)
		{
			quantity_screen_update();
		}

		else if(curr_position->TL_position == WEIGHT)
		{
			weight_screen_update();
		}
	}
}


void Init_menu(cursor_position* curr_position)
{
	curr_position->FL_position = DEFAULT;
	curr_position->SL_position = FIL_DIA;
	curr_position->TL_position = DIAMETER_175;
	curr_position->FL_prev_position = START;
	curr_position->SL_prev_position = NONE;
	curr_position->TL_prev_position = NONE;
	curr_position->current_layer = FIRST_LAYER;
	menu_update(curr_position);
}



void update_enc(cursor_position* curr_position)
{

	uint16_t enc_value;
	enc_value = enc_get_counter();
	enc_value /= 4;

	if(curr_position->current_layer == FIRST_LAYER)
	{
		__HAL_TIM_SET_AUTORELOAD(_ENC_TIMER, 11);
		if(enc_get_counter() > 11)
		{
			__HAL_TIM_SET_COUNTER(_ENC_TIMER, 0);
			enc_value = enc_get_counter();
			enc_value /= 4;
		}

		if(!FC_struct.parameters.ACTIVE_START_FLAG)
		{
			if(enc_value == 0)
			{
				curr_position->FL_position = DEFAULT;
			}

			else if(enc_value == 1)
			{
				curr_position->FL_position = SETTINGS;
			}

			else if(enc_value == 2)
			{
				curr_position->FL_position = START;
			}
		}

		else if (enc_value > 1)
		{
			curr_position->FL_position = STOP;
		}
	}

	else if(curr_position->current_layer == SECOND_LAYER)
	{
		__HAL_TIM_SET_AUTORELOAD(_ENC_TIMER, 19);

		if(enc_get_counter() > 19)
		{
			__HAL_TIM_SET_COUNTER(_ENC_TIMER, 0);
			enc_value = enc_get_counter();
			enc_value /= 4;
		}

		if(enc_value == 0)
		{
			curr_position->SL_position = BACK;
		}

		if(enc_value == 1)
		{
			curr_position->SL_position = FIL_DIA;
		}

		if(enc_value == 2)
		{
			curr_position->SL_position = FIL_DEN;
		}

		if(enc_value == 3)
		{
			curr_position->SL_position = SAMPLE_WEIGHT;
		}

		if(enc_value == 4)
		{
			curr_position->SL_position = QUANTITY;
		}

	}

	else if(curr_position->current_layer == THIRD_LAYER)
	{
		if(curr_position->SL_position == FIL_DIA)
		{
			if(enc_get_counter() > 7)
			{
				__HAL_TIM_SET_COUNTER(_ENC_TIMER, 0);
				__HAL_TIM_SET_AUTORELOAD(_ENC_TIMER, 7);
				enc_value = enc_get_counter();
				enc_value /= 4;
			}

			if(enc_value == 0)
			{
				curr_position->TL_position = DIAMETER_175;
			}

			if(enc_value == 1)
			{
				curr_position->TL_position = DIAMETER_285;
			}
		}

		else if(curr_position->SL_position == FIL_DEN)
		{
			__HAL_TIM_SET_AUTORELOAD(_ENC_TIMER, 11);
			if(enc_get_counter() > 12)
			{
				__HAL_TIM_SET_COUNTER(_ENC_TIMER, 0);
				enc_value = enc_get_counter();
				enc_value /= 4;
			}

			if(enc_value == 0)
			{
				curr_position->TL_position = DENSITY_PLA;
			}

			if(enc_value == 1)
			{
				curr_position->TL_position = DENSITY_ABS;
			}

			if(enc_value == 2)
			{
				curr_position->TL_position = DENSITY_PETG;
			}

		}

		else if(curr_position->SL_position == SAMPLE_WEIGHT)
		{
			__HAL_TIM_SET_AUTORELOAD(_ENC_TIMER, 600);

			if(enc_value == 0)
			{
				enc_value = 1;
				FC_struct.parameters.temp_weight = enc_value;
			}

			else
			{
				FC_struct.parameters.temp_weight = enc_value;
			}
		}


		else if(curr_position->SL_position == QUANTITY)
		{
			__HAL_TIM_SET_AUTORELOAD(_ENC_TIMER, 101);

			if(enc_value == 0)
			{
				enc_value = 1;
				FC_struct.parameters.temp_qty = enc_value;
			}

			else
			{
				FC_struct.parameters.temp_qty = enc_value;
			}
		}
	}


}

void clear_screen()
{
	ST7920_GraphicMode(1);
	ST7920_Clear();
	ST7920_GraphicMode(0);
	ST7920_Clear();
}

void default_screen()
{
	ST7920_SendString(0,0,"FILAMENT CUTTER");
	ST7920_GraphicMode(1);
	ST7920_Clear();
	DrawRectangle(0, 0, 127, 14);
	ST7920_Update();
	ST7920_GraphicMode(0);
	ST7920_SendString(1,0,"1.Settings");
	ST7920_SendString(3,5," START");
}



void settings_screen()
{
	ST7920_SendString(0,0,"1.Diameter");
	ST7920_SendString(1,0,"2.Density");
	ST7920_SendString(2,0,"3.Weight");
	ST7920_SendString(3,0,"4.QTY");
	ST7920_SendString(3,5,"5.BACK");

}


void diameter_screen()
{
	if(FC_struct.parameters.filament_diameter == Filament_diameter_175)
	{
		ST7920_SendString(0, 0, "1.75 mm <<");
		ST7920_SendString(1, 0, "2.85 mm");
	}

	else
	{
		ST7920_SendString(0, 0, "1.75 mm");
		ST7920_SendString(1, 0, "2.85 mm <<");
	}
}


void density_screen()
{
	if(FC_struct.parameters.filament_density >= (Filament_density_PLA-0.001) && (FC_struct.parameters.filament_density <= Filament_density_PLA+0.001))
	{
		ST7920_SendString(0, 0, "1. PLA <<");
		ST7920_SendString(1, 0, "2. ABS");
		ST7920_SendString(2, 0, "3. PETG");
	}

	if(FC_struct.parameters.filament_density >= (Filament_density_ABS-0.001) && (FC_struct.parameters.filament_density <= Filament_density_ABS+0.001))
	{
		ST7920_SendString(0, 0, "1. PLA");
		ST7920_SendString(1, 0, "2. ABS <<");
		ST7920_SendString(2, 0, "3. PETG");
	}

	if(FC_struct.parameters.filament_density >= (Filament_density_PETG-0.001) && (FC_struct.parameters.filament_density <= Filament_density_PETG+0.001))
	{
		ST7920_SendString(0, 0, "1. PLA");
		ST7920_SendString(1, 0, "2. ABS");
		ST7920_SendString(2, 0, "3. PETG <<");
	}
}


void weight_screen()
{
	char weight_value[4];
	uint8_t weight_val_targ;
	if(FC_struct.parameters.target_weight == 0) weight_val_targ = 1;
	else weight_val_targ = FC_struct.parameters.target_weight;

	sprintf(weight_value, "%d", weight_val_targ);
	ST7920_SendString(0, 0, "Weight[g]: ");
	ST7920_SendString(2, 0, "Prev weight: ");
	ST7920_SendString(2, 6, weight_value);
}


void weight_screen_update()
{
	static uint8_t weight_prev_value;
	char weight_value[4];

	if(weight_prev_value != FC_struct.parameters.temp_weight)
	{
		clear_screen();
		weight_screen();

		weight_prev_value = FC_struct.parameters.temp_weight;
		sprintf(weight_value, "%d", FC_struct.parameters.temp_weight);
		ST7920_SendString(0, 5, weight_value);
	}

}

void active_start_screen()
{
	char qty_target_value[4];
	char qty_current_value[4];
	char weight_target_value[5];

	sprintf(qty_target_value, "%d", FC_struct.parameters.target_qty + 1);
	sprintf(qty_current_value, "%d", (FC_struct.parameters.current_qty + 1));
	sprintf(weight_target_value, "%d", FC_struct.parameters.target_weight);


	ST7920_SendString(0, 0, "Working...");
	ST7920_SendString(1, 0, "   %");
	ST7920_SendString(2, 0, "W: ");
	ST7920_SendString(2, 3, "QTY:  / ");
	ST7920_SendString(3, 3, "STOP");
	ST7920_SendString(2, 7, qty_target_value);
	ST7920_SendString(2, 5, qty_current_value);
	ST7920_SendString(2, 1, weight_target_value);
}

void start_screen_update(uint8_t target_weight, uint8_t current_qty, uint8_t target_qty)
{
	uint8_t percent_prev_value = 1;

	char qty_target_value[4];
	char qty_current_value[4];
	char weight_target_value[5];
	char cm_current_percent[6];

	FC_struct.parameters.current_length_cm = (float)(FC_struct.motor->slave_timer.htim->Instance->CNT) / (FC_struct.motor->slave_timer.htim->Instance->ARR);
	FC_struct.parameters.current_length_cm = truncf(FC_struct.parameters.current_length_cm * 100);

	if((((uint8_t)FC_struct.parameters.current_length_cm % 5) == 0) && (percent_prev_value != FC_struct.parameters.current_length_cm))
	{
		percent_prev_value = FC_struct.parameters.current_length_cm;
		clear_screen();
		active_start_screen();

		sprintf(qty_target_value, "%d", FC_struct.parameters.target_qty + 1);
		sprintf(qty_current_value, "%d", (FC_struct.parameters.current_qty + 1));
		sprintf(weight_target_value, "%d", FC_struct.parameters.target_weight);
		sprintf(cm_current_percent, "%d", (uint8_t)FC_struct.parameters.current_length_cm);

		ST7920_SendString(2, 7, qty_target_value);
		ST7920_SendString(2, 5, qty_current_value);
		ST7920_SendString(2, 1, weight_target_value);
		ST7920_SendString(1, 0, cm_current_percent);
	}
}



void quantity_screen()
{
	char qty_value[4];
	uint8_t qty_val_targ;
	if(FC_struct.parameters.target_qty == 0) qty_val_targ = 1;
	else qty_val_targ = FC_struct.parameters.target_qty;

	sprintf(qty_value, "%d", qty_val_targ);
	ST7920_SendString(0, 0, "QTY: ");
	ST7920_SendString(2, 0, "PREV_QTY: ");
	ST7920_SendString(2, 5, qty_value);
}

void quantity_screen_update()
{
	static uint8_t qty_prev_value;
	char qty_value[4];

	if(qty_prev_value != FC_struct.parameters.temp_qty)
	{
		clear_screen();
		quantity_screen();

		qty_prev_value = FC_struct.parameters.temp_qty;
		sprintf(qty_value, "%d", FC_struct.parameters.temp_qty);
		ST7920_SendString(0, 2, qty_value);
	}

}
