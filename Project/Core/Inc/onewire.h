/*
 * onewire.h
 *
 *  Created on: Mar 14, 2025
 *      Author: skd
 */

#ifndef INC_ONEWIRE_H_
#define INC_ONEWIRE_H_

#include <stdint.h>
#include "stm32g0xx_hal.h"
#include "hardware.h"

#define ONE_WIRE_UART_TIMEOUT 10

#define ONE_WIRE_RESET 		0xF0
#define ONE_WIRE_PRESENCE 	0xC0


#define ONE_WIRE_WRITE_1 	0xFF
#define ONE_WIRE_WRITE_0 	0xE0

#define ONE_WIRE_READ		0xFF

uint8_t one_wire_init(UART_HandleTypeDef* uart);
uint8_t one_wire_deinit(UART_HandleTypeDef* uart);

uint8_t one_wire_set_fast();
uint8_t one_wire_set_slow();

uint8_t one_wire_reset_presence();
uint8_t one_wire_write_byte(uint8_t data);
uint8_t one_wire_read_byte();

#endif /* INC_ONEWIRE_H_ */
