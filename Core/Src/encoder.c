/*
 * encoder.c
 *
 *  Created on: Jul 27, 2021
 *      Author: DELL
 */


#include "encoder.h"
#include "main.h"
#include "menu_LCD.h"
#include "L298_dc.h"
#include "stepper.h"
#include "filament_cutter.h"

static encoder_button enc_btn;
extern stepper_motor extruder;
extern dc_motor DC_motor;
extern filament_cutter FC_struct;

void encoder_init(GPIO_TypeDef* GPIO_BTN_PORT, uint16_t Button_Pin, uint32_t debounce_time)
{
	enc_btn.PORT = GPIO_BTN_PORT;
	enc_btn.PIN = Button_Pin;
	enc_btn.debounce_time = debounce_time;
	enc_btn.BTN_state = DEFA;
	enc_btn.last_tick = 0;

	ENC_Button_RegisterPressCallback(&ENC_Button_PressedTask);

	__HAL_TIM_SET_AUTORELOAD(_ENC_TIMER, 19);
	HAL_TIM_Encoder_Start(_ENC_TIMER, TIM_CHANNEL_ALL);
}


uint32_t enc_get_counter()
{
	return __HAL_TIM_GET_COUNTER(_ENC_TIMER);
}


void ENC_Button_RegisterPressCallback(void *callback)
{
	enc_btn.ButtonPressed = callback;

}


void ENC_Button_DebounceRoutine(cursor_position* cur_postion)
{
	if((HAL_GetTick() - enc_btn.last_tick) > enc_btn.debounce_time)
	{
		if(HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == GPIO_PIN_RESET)
		{
			enc_btn.BTN_state = PRESSED;
			if(enc_btn.ButtonPressed != NULL)
			{
				enc_btn.ButtonPressed(cur_postion);
			}
		}

		else
		{
			enc_btn.BTN_state = DEFA;
		}

	}

}


void ENC_Button_IdleRoutine()
{
	if(HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == GPIO_PIN_RESET)
	{
		enc_btn.last_tick = HAL_GetTick();
		enc_btn.BTN_state = DEBOUNCE;
	}
}


void ENC_Button_PressedRoutine()
{
	if(HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == GPIO_PIN_SET)
	{
		enc_btn.BTN_state = DEFA;
	}
}


void ENC_Button_Action(cursor_position* cur_postion)
{
	switch(enc_btn.BTN_state)
	{
	case DEFA:
		ENC_Button_IdleRoutine();
		break;
	case DEBOUNCE:
		ENC_Button_DebounceRoutine(cur_postion);
		break;
	case PRESSED:
		ENC_Button_PressedRoutine();
		break;
	}
}



//void ENC_Button_PressedTask(encoder_button* enc_btn, cursor_position* cursor_pos)
void ENC_Button_PressedTask(cursor_position* cursor_pos)
{
	if(cursor_pos->current_layer == FIRST_LAYER)
	{
		if(cursor_pos->FL_position == SETTINGS)
		{
			cursor_pos->current_layer = SECOND_LAYER;
			__HAL_TIM_SET_COUNTER(_ENC_TIMER, 0);
		}

		// START pressed
		else if(cursor_pos->FL_position == START)
		{
			FC_struct.mode = EXTRUDE;

			__HAL_TIM_SET_COUNTER(_ENC_TIMER, 0);
			cursor_pos->FL_position = DEFAULT;
		}

		else if(cursor_pos->FL_position == STOP)
		{
			stepper_stop(&extruder);
			DC_stop(&DC_motor);

			__HAL_TIM_SET_COUNTER(_ENC_TIMER, 0);
			cursor_pos->FL_position = DEFAULT;
		}
	}

	else if(cursor_pos->current_layer == SECOND_LAYER)
	{
		if(cursor_pos->SL_position == BACK)
		{
			cursor_pos->current_layer = FIRST_LAYER;
			cursor_pos->FL_position = DEFAULT;
		}

		else if(cursor_pos->SL_position == FIL_DIA)
		{
			cursor_pos->current_layer = THIRD_LAYER;
			cursor_pos->TL_position = DIAMETER_175;
		}

		else if(cursor_pos->SL_position == FIL_DEN)
		{
			cursor_pos->current_layer = THIRD_LAYER;
			cursor_pos->TL_position = DENSITY_PLA;
		}
	}

	else if(cursor_pos->current_layer == THIRD_LAYER)
	{
		if(cursor_pos->TL_position == DIAMETER_175)
		{
			cursor_pos->current_layer = SECOND_LAYER;
			cursor_pos->SL_position = FIL_DIA;
			// do smth
			// save data to struct

		}

		else if(cursor_pos->TL_position == DIAMETER_285)
		{
			cursor_pos->current_layer = SECOND_LAYER;
			cursor_pos->SL_position = FIL_DIA;
			// do smth
			// save data to struct
		}

		else if(cursor_pos->TL_position == DENSITY_PLA)
		{
			cursor_pos->current_layer = SECOND_LAYER;
			cursor_pos->SL_position = FIL_DEN;
			// do smth
			// save data to struct
		}

		else if(cursor_pos->TL_position == DENSITY_ABS)
		{
			cursor_pos->current_layer = SECOND_LAYER;
			cursor_pos->SL_position = FIL_DEN;
		}

		else if(cursor_pos->TL_position == DENSITY_PETG)
		{
			cursor_pos->current_layer = SECOND_LAYER;
			cursor_pos->TL_position = FIL_DEN;
		}

		else if(cursor_pos->TL_position == WEIGHT)
		{
			cursor_pos->current_layer = SECOND_LAYER;
			cursor_pos->TL_position = DIAMETER_175;
		}

		else if(cursor_pos->TL_position == QTY)
		{
			cursor_pos->current_layer = SECOND_LAYER;
			cursor_pos->TL_position = DENSITY_PLA;
		}


	}


}
