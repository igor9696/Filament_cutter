/*
 * encoder.h
 *
 *  Created on: Jul 27, 2021
 *      Author: DELL
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "tim.h"
#include "menu_LCD.h"

#define _ENC_TIMER &htim3

struct encoder_button;

typedef void (*ENCButtonPressed)(struct encoder_button*, cursor_position*);

typedef enum BTN_STATE {
	DEFA, DEBOUNCE, PRESSED,
} BTN_STATE;

typedef struct encoder_button {

	GPIO_TypeDef *PORT;
	uint16_t PIN;
	uint32_t last_tick;
	uint32_t debounce_time;
	BTN_STATE BTN_state;

	ENCButtonPressed ButtonPressed;

} encoder_button;

void encoder_init(encoder_button *enc_btn, GPIO_TypeDef *GPIO_BTN_PORT,
		uint16_t Button_Pin, uint32_t debounce_time);
uint32_t enc_get_counter();
void ENC_Button_Action(encoder_button *enc_btn, cursor_position *cur_postion);
void ENC_Button_RegisterPressCallback(encoder_button *enc_btn, void *callback);
void ENC_Button_PressedTask(encoder_button *enc_btn,
		cursor_position *cursor_pos);

#endif /* INC_ENCODER_H_ */
