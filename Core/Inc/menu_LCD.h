/*
 * menu_LCD.h
 *
 *  Created on: 23 lip 2021
 *      Author: DELL
 */

#ifndef INC_MENU_LCD_H_
#define INC_MENU_LCD_H_

#include "stdio.h"

typedef enum layer
{
	FIRST_LAYER,
	SECOND_LAYER,
	THIRD_LAYER,

}layer;


typedef enum first_layer
{
	DEFAULT,
	START,
	ACTIVE_START,
	STOP,
	SETTINGS,

}first_layer;


typedef enum second_layer
{
	BACK,
	FIL_DIA,
	FIL_DEN,
	SAMPLE_WEIGHT,
	QUANTITY,
	NONE

}second_layer;


typedef enum third_layer
{
	DIAMETER_175,
	DIAMETER_285,
	DENSITY_PLA,
	DENSITY_ABS,
	DENSITY_PETG,
	WEIGHT_5g,
	WEIGHT_25g,
	WEIGHT_50g,
	WEIGHT_100g,
	QTY,

}third_layer;



typedef struct cursor_position
{
	first_layer FL_position;
	first_layer FL_prev_position;
	second_layer SL_position;
	second_layer SL_prev_position;
	third_layer TL_position;
	third_layer TL_prev_position;

	layer current_layer;

}cursor_position;





void Init_menu(cursor_position* curr_position);
void update_enc(cursor_position* curr_position);
void menu_update(cursor_position* curr_position);
void default_screen();
void settings_screen();
void diameter_screen();
void density_screen();
void weight_screen();
void active_start_screen();
void quantity_screen();
void quantity_screen_update();
void clear_screen();

#endif /* INC_MENU_LCD_H_ */
