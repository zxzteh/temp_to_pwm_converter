/*
 * onewire.c
 *
 *  Created on: Mar 14, 2025
 *      Author: skd
 */

#include "onewire.h"

static UART_HandleTypeDef* one_wire_uart_ = NULL;

uint8_t one_wire_init(UART_HandleTypeDef* uart) {
	if (uart == NULL) return 1;

	one_wire_uart_ = uart;
    return 0;
}

uint8_t one_wire_deinit(UART_HandleTypeDef* uart);

uint8_t one_wire_set_fast() {
	if (one_wire_uart_ == NULL) return 1;

	one_wire_uart_->Init.BaudRate = 115200;
	return ( HAL_UART_Init(one_wire_uart_) );
}

uint8_t one_wire_set_slow() {
	if (one_wire_uart_ == NULL) return 1;

	one_wire_uart_->Init.BaudRate = 9600;
	return ( HAL_UART_Init(one_wire_uart_) );
}

uint8_t one_wire_reset_presence() {
	if (one_wire_uart_ == NULL) return 1;

	uint8_t buffer[1] = { ONE_WIRE_RESET };
	one_wire_set_slow();
	HAL_UART_Transmit(one_wire_uart_, buffer, sizeof(uint8_t), ONE_WIRE_UART_TIMEOUT);
	HAL_UART_Receive(one_wire_uart_, buffer, sizeof(uint8_t), ONE_WIRE_UART_TIMEOUT);
	one_wire_set_fast();
	return (buffer[0] != ONE_WIRE_PRESENCE);
}

static uint8_t one_wire_write_bit(uint8_t bit) {
	uint8_t tx_buffer[1] = { 0 };
	uint8_t rx_buffer[1] = { 0 };
	tx_buffer[0] = bit ? ONE_WIRE_WRITE_1 : ONE_WIRE_WRITE_0;
	HAL_UART_Transmit(one_wire_uart_, tx_buffer, sizeof(uint8_t), ONE_WIRE_UART_TIMEOUT);
	HAL_UART_Receive(one_wire_uart_, rx_buffer, sizeof(uint8_t), ONE_WIRE_UART_TIMEOUT);
	return (tx_buffer[0] != tx_buffer[0]);
}

static uint8_t one_wire_read_bit() {
	uint8_t buffer[1] = { ONE_WIRE_READ };
	HAL_UART_Transmit(one_wire_uart_, buffer, sizeof(uint8_t), ONE_WIRE_UART_TIMEOUT);
	HAL_UART_Receive(one_wire_uart_, buffer, sizeof(uint8_t), ONE_WIRE_UART_TIMEOUT);
	return buffer[0];
}

uint8_t one_wire_write_byte(uint8_t data) {
	if (one_wire_uart_ == NULL) return 1;

	for(uint8_t i = 0; i < 8; i++) {
		one_wire_write_bit(data & 0x01);
		data >>= 1;
	}
	return 0;
}

uint8_t one_wire_read_byte() {
	if (one_wire_uart_ == NULL) return 1;

	uint8_t data = 0;
	for(uint8_t i = 0; i < 8; i++) {
		data <<= 1;
		data |= one_wire_read_bit();
	}
	return data;
}



