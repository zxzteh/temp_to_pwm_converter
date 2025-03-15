/*
 * debug_logger.c
 *
 *  Created on: Mar 14, 2025
 *      Author: skd
 */

#include "debug_logger.h"


void debug_print(MsgType type, const char *format, ...) {
    if (type > LOG_LEVEL) {
        return;
    }

    char raw_buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(raw_buffer, sizeof(raw_buffer), format, args);
    va_end(args);

    const char *prefix = "";
    switch (type) {
        case INF:
            prefix = "[INF] ";
            break;
        case WRN:
            prefix = "[WRN] ";
            break;
        case ERR:
            prefix = "[ERR] ";
            break;
        default:
            prefix = "[NTP] ";  //  NO_TYPE
            break;
    }

    char formatted_buffer[256];
    snprintf(formatted_buffer, sizeof(formatted_buffer), "%s%s\r\n", prefix, raw_buffer);

    HAL_UART_Transmit(&UART_DEBUG, (uint8_t*)formatted_buffer, strlen(formatted_buffer), UART_DEBUG_TIMEOUT);
}

