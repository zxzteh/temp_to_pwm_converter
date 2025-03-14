/*
 * hardware.c
 *
 *  Created on: Mar 14, 2025
 *      Author: skd
 */

#include "hardware.h"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
	if (htim == &TIM_MEASURE) {  //  1s elapsed
		static uint8_t cnt = 0;
		cnt++;
		one_wire_write_byte(0xFF);
    }
}
