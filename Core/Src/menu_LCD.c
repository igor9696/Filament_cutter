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


void update_first_layer(cursor_position* curr_position)
{
	if(curr_position->FL_position != curr_position->FL_prev_position)
	{
		switch(curr_position->FL_position)
		{
			case DEFAULT:
			DC_stop(&DC_motor);
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();

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
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();
			active_start_screen();


			break;


			case STOP:
			active_start_screen();

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
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();


			settings_screen();

			// highlighting
			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 0, 79, 14);
			ST7920_Update();
			ST7920_GraphicMode(0);

			break;

		case FIL_DEN:
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();

			// highlighting
			settings_screen();
			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 15, 79, 15);
			ST7920_Update();
			ST7920_GraphicMode(0);

			break;

		case SAMPLE_WEIGHT:
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();


			settings_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 30, 79, 15);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case QUANTITY:
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();


			settings_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 48, 79, 15);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case BACK:
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();


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
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();
			diameter_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 0, 79, 14);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case DIAMETER_285:
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();
			diameter_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 15, 79, 15);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case DENSITY_PLA:
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();
			density_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 0, 79, 14);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case DENSITY_ABS:
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();
			density_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 15, 79, 14);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case DENSITY_PETG:
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();
			density_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 30, 79, 14);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case WEIGHT_5g:
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();
			weight_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 0, 79, 14);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case WEIGHT_25g:
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();
			weight_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 15, 79, 14);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case WEIGHT_50g:
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();
			weight_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 30, 79, 16);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case WEIGHT_100g:
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();
			weight_screen();

			ST7920_GraphicMode(1);
			DrawFilledRectangle(0, 48, 79, 14);
			ST7920_Update();
			ST7920_GraphicMode(0);
			break;

		case QTY:
			ST7920_GraphicMode(1);
			ST7920_Clear();
			ST7920_GraphicMode(0);
			ST7920_Clear();

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
	}


	else if(curr_position->current_layer == SECOND_LAYER)
	{
		update_second_layer(curr_position);

	}

	else if(curr_position->current_layer == THIRD_LAYER)
	{
		update_third_layer(curr_position);
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

	uint8_t enc_value;
	enc_value = enc_get_counter();

	if(curr_position->current_layer == FIRST_LAYER)
	{

		if(!(FC_struct.mode != STANDBY))
		{

			if((enc_value >=0) & (enc_value < 4))
			{
				curr_position->FL_position = DEFAULT;
			}

			else if((enc_value >= 4) & (enc_value < 8))
			{
				curr_position->FL_position = SETTINGS;
			}

			else if((enc_value >= 8) & (enc_value < 12))
			{
				curr_position->FL_position = START;
			}
		}

		else if((enc_value >= 8) & (enc_value < 20))
		{
			curr_position->FL_position = STOP;
		}

	}

	else if(curr_position->current_layer == SECOND_LAYER)
	{

		if((enc_value >=0) & (enc_value < 4))
		{
			curr_position->SL_position = FIL_DIA;
		}

		else if((enc_value >= 4) & (enc_value < 8))
		{
			curr_position->SL_position = FIL_DEN;
		}

		else if((enc_value >= 8) & (enc_value < 12))
		{
			curr_position->SL_position = SAMPLE_WEIGHT;
		}

		else if((enc_value >= 12) & (enc_value < 16))
		{
			curr_position->SL_position = QUANTITY;
		}

		else if((enc_value >= 16) & (enc_value < 21))
		{
			curr_position->SL_position = BACK;
		}


	}

	else if(curr_position->current_layer == THIRD_LAYER)
	{
		if(curr_position->SL_position == FIL_DIA)
		{
			if((enc_value >= 0) & (enc_value < 4))
			{
				curr_position->TL_position = DIAMETER_175;
			}

			else if((enc_value >= 4) & (enc_value <= 8))
			{
				curr_position->TL_position = DIAMETER_285;

			}
		}

		else if(curr_position->SL_position == FIL_DEN)
		{

			if((enc_value >= 0) & (enc_value < 4))
			{
				curr_position->TL_position = DENSITY_PLA;
			}

			else if((enc_value >= 4) & (enc_value < 8))
			{
				curr_position->TL_position = DENSITY_ABS;

			}
			else if((enc_value >= 8) & (enc_value <= 20))
			{
				curr_position->TL_position = DENSITY_PETG;
			}

		}

		else if(curr_position->SL_position == SAMPLE_WEIGHT)
		{
			if((enc_value >= 0) & (enc_value < 4))
			{
				curr_position->TL_position = WEIGHT_5g;
			}

			else if((enc_value >= 4) & (enc_value < 8))
			{
				curr_position->TL_position = WEIGHT_25g;

			}
			else if((enc_value >= 8) & (enc_value < 12))
			{
				curr_position->TL_position = WEIGHT_50g;
			}

			else if((enc_value >= 12) & (enc_value < 20))
			{
				curr_position->TL_position = WEIGHT_100g;
			}

		}


		else if(curr_position->SL_position == QUANTITY)
		{
				// do smth
		}


	}


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
	//ST7920_SendString(3,0,"STOP");
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

	if(FC_struct.parameters.filament_density == Filament_diameter_285)
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
	if(FC_struct.parameters.target_weight == Sample_weight_5g)
	{
		ST7920_SendString(0, 0, "5g <<");
		ST7920_SendString(1, 0, "25g");
		ST7920_SendString(2, 0, "50g");
		ST7920_SendString(3, 0, "100g");
	}

	if(FC_struct.parameters.target_weight == Sample_weight_25g)
	{
		ST7920_SendString(0, 0, "5g");
		ST7920_SendString(1, 0, "25g <<");
		ST7920_SendString(2, 0, "50g");
		ST7920_SendString(3, 0, "100g");
	}

	if(FC_struct.parameters.target_weight == Sample_weight_50g)
	{
		ST7920_SendString(0, 0, "5g");
		ST7920_SendString(1, 0, "25g");
		ST7920_SendString(2, 0, "50g <<");
		ST7920_SendString(3, 0, "100g");
	}

	if(FC_struct.parameters.target_weight == Sample_weight_100g)
	{
		ST7920_SendString(0, 0, "5g");
		ST7920_SendString(1, 0, "25g");
		ST7920_SendString(2, 0, "50g");
		ST7920_SendString(3, 0, "100g <<");
	}
}

void active_start_screen()
{
	ST7920_SendString(0, 0, "Working...");
	ST7920_SendString(1, 0, "0/500 [cm]");
	ST7920_SendString(3, 3, "STOP");
}


void quantity_screen()
{
	ST7920_SendString(0, 0, "QTY: ");
}
