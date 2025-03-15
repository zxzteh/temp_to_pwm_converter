/*
 * hardware.c
 *
 *  Created on: Mar 14, 2025
 *      Author: skd
 */

#include "hardware.h"
#include "ds18b20.h"

static void set_pwm(float temperature) {
	temperature *= 100;  //  shifting out two digits from comma
	uint16_t pulse_val = (uint16_t)temperature;
	TIM_PWM.Instance->CCR1 = pulse_val;
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &TIM_MEASURE && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {  //  800 ms elapsed

    	static uint8_t queue_flag = 1;
    	if (queue_flag) {
        	queue_flag = 0;
			debug_print(INF, "Starting measure. . .");
        	return;
    	}

    	ds18b20_read_temperature_callback();
		float temp = 0.0f;
		uint8_t status = sensor_get_temp(&temp);

		if (!status) {
			set_pwm(temp);
			char temp_string[128] = {};
			float_to_str(temp, temp_string, 2);
			debug_print(INF, "temp = %s degrees", temp_string);
		} else {
			debug_print(ERR, "Can't acquire data from sensor");
		}
    }

    if (htim == &TIM_MEASURE && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {  //  1000 ms elapsed
    	ds18b20_read_temperature_request();
    }
}

void HAL_PWR_PVDCallback(void) {
    debug_print(ERR, "Voltage is low!");
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_PVDO);
}


uint8_t calculate_crc_hw(uint8_t* data, uint8_t len) {
    __HAL_CRC_DR_RESET(&CRC_UNIT);
    uint32_t crc = HAL_CRC_Calculate(&CRC_UNIT, (uint32_t*)data, len);
    return (uint8_t)crc;
}


