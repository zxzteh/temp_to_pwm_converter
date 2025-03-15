/*
 * ds18b20.c
 *
 *  Created on: Mar 14, 2025
 *      Author: skd
 */

#include "ds18b20.h"

uint8_t ds18b20_calculate_crc_sw(uint8_t* data, uint8_t len) {
	uint8_t crc = 0;
	while (len--) {
		uint8_t byte = *data++;
		for (uint8_t i = 8; i; i--) {
			uint8_t temp = (crc ^ byte) & 0x01;
			crc >>= 1;
			if (temp) crc ^= 0x8C;
			byte >>= 1;
		}
	}
	return crc;
}

uint8_t ds18b20_calculate_crc_hw(uint8_t* data, uint8_t len) {
	uint8_t crc = 0;
	crc = calculate_crc_hw(data, len);
	return crc;
}
