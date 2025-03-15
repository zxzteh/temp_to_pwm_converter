/*
 * debug_logger.h
 *
 *  Created on: Mar 14, 2025
 *      Author: skd
 */

#ifndef INC_DEBUG_LOGGER_H_
#define INC_DEBUG_LOGGER_H_

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "hardware.h"

#define LOG_LEVEL 3

#define UART_DEBUG_TIMEOUT 10

typedef enum {
	INF = 1,
	WRN = 2,
	ERR = 3
} MsgType;

void debug_print(MsgType type, const char *format, ...);

#endif /* INC_DEBUG_LOGGER_H_ */
