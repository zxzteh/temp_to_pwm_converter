/*
 * hardware.c
 *
 *  Created on: Mar 14, 2025
 *      Author: skd
 */

#include "hardware.h"
#include "ds18b20.h"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
	if (htim == &TIM_MEASURE) {  //  1s elapsed
		one_wire_reset_presence();
		one_wire_write_byte(CMD_READ_ROM);
//		one_wire_write_byte(CMD_READ_SCRATCHPAD);

		uint8_t data[10];
		for(uint8_t i = 0; i < 8; i++) {
			data[i] = one_wire_read_byte();
		}

		for(uint8_t i = 0; i < 8; i++) {
			debug_print(INF, "n%d = %x", i, data[i]);
		}
		HAL_GPIO_TogglePin(DEBUG_PIN_GPIO_Port, DEBUG_PIN_Pin);
		uint8_t crc = ds18b20_calculate_crc_hw(data, 7);
		HAL_GPIO_TogglePin(DEBUG_PIN_GPIO_Port, DEBUG_PIN_Pin);
		debug_print(INF, "crc = %x", crc);

		debug_print(INF, "Callback end!");
    }

}

uint8_t calculate_crc_hw(uint8_t* data, uint8_t len) {
    __HAL_CRC_DR_RESET(&CRC_UNIT);
    uint32_t crc = HAL_CRC_Calculate(&CRC_UNIT, (uint32_t*)data, len);
    return (uint8_t)crc;
}
