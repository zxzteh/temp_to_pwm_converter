/*
 * hardware.h
 *
 *  Created on: Mar 14, 2025
 *      Author: skd
 */

#ifndef INC_HARDWARE_H_
#define INC_HARDWARE_H_

#include "stm32g0xx_hal.h"
#include "main.h"

extern TIM_HandleTypeDef TIM_MEASURE;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* INC_HARDWARE_H_ */
