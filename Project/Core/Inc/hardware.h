/*
 * hardware.h
 *
 *  Created on: Mar 14, 2025
 *      Author: skd
 */

#ifndef INC_HARDWARE_H_
#define INC_HARDWARE_H_

#include "main.h"
#include "debug_logger.h"

extern TIM_HandleTypeDef TIM_MEASURE;
extern UART_HandleTypeDef UART_DEBUG;
extern CRC_HandleTypeDef CRC_UNIT;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
uint8_t calculate_crc_hw(uint8_t* data, uint8_t len);

#endif /* INC_HARDWARE_H_ */
