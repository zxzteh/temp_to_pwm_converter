/*
 * hardware.c
 *
 *  Created on: Mar 14, 2025
 *      Author: skd
 */

#include "hardware.h"
#include "ds18b20.h"

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &TIM_MEASURE && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {  //  800 ms elapsed
    	ds18b20_read_temperature_callback();
		float temp = 0.0f;
		if ( !sensor_get_temp(&temp) ) {
			char temp_string[128] = {};
			float_to_str(temp, temp_string, 2);
			debug_print(INF, "temp = %s degrees", temp_string);
		}
    }

    if (htim == &TIM_MEASURE && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {  //  1000 ms elapsed
    	ds18b20_read_temperature_request();
    }
}


uint8_t calculate_crc_hw(uint8_t* data, uint8_t len) {
    __HAL_CRC_DR_RESET(&CRC_UNIT);
    uint32_t crc = HAL_CRC_Calculate(&CRC_UNIT, (uint32_t*)data, len);
    return (uint8_t)crc;
}
