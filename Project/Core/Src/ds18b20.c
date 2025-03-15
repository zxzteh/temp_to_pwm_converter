/*
 * ds18b20.c
 *
 *  Created on: Mar 14, 2025
 *      Author: skd
 */

#include "ds18b20.h"

static sensor_t sensor = {};

static uint8_t ds18b20_check_crc(uint8_t* data, uint8_t len);
static uint8_t ds18b20_calculate_crc_sw(uint8_t* data, uint8_t len);
static uint8_t ds18b20_calculate_crc_hw(uint8_t* data, uint8_t len);
static void ds18b20_read_rom(uint8_t* data);
static void ds18b20_read_scratchpad(uint8_t* data);
static float ds18b20_convert_temp(uint16_t);


uint8_t ds18b20_init(UART_HandleTypeDef* uart) {
	uint8_t data[ROM_SIZE];
	if (one_wire_init(uart)) {
		debug_print(ERR, "OneWire UART error");
		return 1;
	}

	if (one_wire_reset_presence()) {
		debug_print(ERR, "No device presence on bus");
		return 1;
	}

	ds18b20_read_rom(data);
	if (ds18b20_check_crc(data, sizeof(data))) {
		debug_print(ERR, "Read ROM CRC error");
		return 1;
	}

	sensor.sensorState = SENSOR_STATE_IDLE;
	debug_print(INF, "Sensor init success");
	return 0;
}

void ds18b20_read_temperature_request() {
	one_wire_reset_presence();
	if ( (sensor.sensorState != SENSOR_STATE_NA) && (sensor.sensorState != SENSOR_STATE_CONVERSION) ) {
		one_wire_write_byte(CMD_SKIP_ROM);
		one_wire_write_byte(CMD_CONVERT_T);
		sensor.sensorState = SENSOR_STATE_CONVERSION;
	} else {
		sensor.sensorError = SENSOR_TIMING_ERROR;
	}
}

void ds18b20_read_temperature_callback() {
	if (sensor.sensorState != SENSOR_STATE_CONVERSION) {
		sensor.sensorError = SENSOR_TIMING_ERROR;
		return;
	}

	uint8_t buffer[SCRATCHPAD_SIZE] = {};
	uint16_t temp_val = 0;
	uint16_t mask = 0xFFF;
	one_wire_reset_presence();

	ds18b20_read_scratchpad(buffer);
	if (!ds18b20_check_crc(buffer, sizeof(buffer))) {
		temp_val = ( buffer[0] | (buffer[1] << 8) ) & mask;
		sensor.data = ds18b20_convert_temp(temp_val);
		sensor.sensorState = SENSOR_STATE_DATA_READY;
	} else {
		sensor.sensorError = SENSOR_CRC_ERROR;
	}
}

uint8_t sensor_get_temp(float* data) {
	if (sensor.sensorState == SENSOR_STATE_DATA_READY && sensor.sensorError == SENSOR_OK) {
		*data = sensor.data;
		sensor.data = 0.0f;
		sensor.sensorState = SENSOR_STATE_IDLE;
		return 0;
	} else {
		sensor.sensorError = SENSOR_OK;
		return 1;
	}
}

static void ds18b20_read_rom(uint8_t* data) {
	one_wire_write_byte(CMD_READ_ROM);
	for(uint8_t i = 0; i < ROM_SIZE; i++) {
		data[i] = one_wire_read_byte();
	}
}

static void ds18b20_read_scratchpad(uint8_t* data) {
	one_wire_write_byte(CMD_SKIP_ROM);
	one_wire_write_byte(CMD_READ_SCRATCHPAD);
	for(uint8_t i = 0; i < SCRATCHPAD_SIZE; i++) {
		data[i] = one_wire_read_byte();
	}
}

static uint8_t ds18b20_check_crc(uint8_t* data, uint8_t len) {
	uint8_t real_crc = data[len - 1];
	uint8_t expected_crc = ds18b20_calculate_crc_hw(data, len - 1);
	return (real_crc != expected_crc);
}

static uint8_t __attribute__((unused)) ds18b20_calculate_crc_sw(uint8_t* data, uint8_t len) {
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

static uint8_t ds18b20_calculate_crc_hw(uint8_t* data, uint8_t len) {
	uint8_t crc = 0;
	crc = calculate_crc_hw(data, len);
	return crc;
}

static float ds18b20_convert_temp(uint16_t raw_temp) {
    int8_t int_temp = (int8_t)(raw_temp >> 4);
    uint8_t fraс_part = raw_temp & 0x0F;
    float frac_temp = fraс_part * 0.0625f;
    float res = (float)(int_temp + frac_temp);
    return res;
}

