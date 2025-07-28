/*
 * uart.h
 *
 *  Created on: Jul 27, 2025
 *      Author: Paarth
 */

#ifndef STM32F1XX_HAL_DRIVER_INC_UART_H_
#define STM32F1XX_HAL_DRIVER_INC_UART_H_

#include <stdint.h>

void uart_init();
void uart_send_char(char c);
void uart_send_string(char *str);
int uart_receive_char(void);
void uart_receive_string(char *buffer, int max_length);

#endif /* STM32F1XX_HAL_DRIVER_INC_UART_H_ */
