/*
 * ds18b20.h
 *
 *  Created on: Mar 14, 2025
 *      Author: skd
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include <stdint.h>
#include "hardware.h"
#include "onewire.h"

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

#define SCRATCHPAD_SIZE			9
#define ROM_SIZE				8

#define CRC8_POLYNOMIAL 0x31

typedef enum {
	SENSOR_STATE_NA,
	SENSOR_STATE_IDLE,
	SENSOR_STATE_CONVERSION,
	SENSOR_STATE_DATA_READY
} SensorState;

typedef enum {
	SENSOR_OK,
	SENSOR_TIMING_ERROR,
	SENSOR_CRC_ERROR
} SensorError;

typedef struct {
	float data;
	SensorState  sensorState;
	SensorError sensorError;
} sensor_t;

uint8_t ds18b20_init(UART_HandleTypeDef* uart);
void ds18b20_read_temperature_request();
void ds18b20_read_temperature_callback();
uint8_t sensor_get_temp();
uint8_t sensor_get_state();

#endif /* INC_DS18B20_H_ */



