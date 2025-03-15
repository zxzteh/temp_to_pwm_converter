/*
 * ds18b20.h
 *
 *  Created on: Mar 14, 2025
 *      Author: skd
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include <stdint.h>

#define CMD_READ_ROM 			0x33
#define CMD_MATCH_ROM 			0x55
#define CMD_SKIP_ROM 			0xCC
#define CMD_ALARM_SEARCH 		0xEC
#define CMD_CONVERT_T 			0x44
#define CMD_WRITE_SCRATCHPAD 	0x4E
#define CMD_READ_SCRATCHPAD 	0xBE
#define CMD_COPY_SCRATCHPAD 	0x48
#define CMD_RECALL_EE		 	0xB8
#define CMD_READ_POWER_SUPPLY	0xB4

#define CRC8_POLYNOMIAL 0x31  //  X^8 + X^5 + X^4 + 1

uint8_t ds18b20_calculate_crc_sw(uint8_t* data, uint8_t len);
uint8_t ds18b20_calculate_crc_hw(uint8_t* data, uint8_t len);

#endif /* INC_DS18B20_H_ */



