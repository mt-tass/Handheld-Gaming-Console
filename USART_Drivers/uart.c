/*
 * uart.c
 *
 *  Created on: Jul 27, 2025
 *      Author: Paarth
 */

#include "stm32f1xx.h"  // Include the header file for the STM32F1 series
#include <stdlib.h>

#define UART_BAUDRATE 9600
#define Timeout 100000

#define UART_TX_PIN 2  // PA2 -> USART2_TX
#define UART_RX_PIN 3  // PA3 -> USART2_RX

void uart_init(){
	// Enable clock for GPIOA
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	// Enable clock for USART2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Configure PA2 (USART2_TX) Alternate Function mode
	GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);
	GPIOA->CRL |= GPIO_CRL_MODE2_1 | GPIO_CRL_CNF2_1;

	//PA3 as Input Floating
	GPIOA->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
	GPIOA->CRL |= GPIO_CRL_CNF3_0;  // Input floating

    //Configure Baudrate
	USART2->BRR = 0x1D4C;	//72MHz and 9600 Baudrate

    //Configure Data Length
    USART2->CR1 &= ~USART_CR1_M;	//8 bit
    USART2->CR2 &= ~USART_CR2_STOP;	//1 stop bit
    USART2->CR1 &= ~USART_CR1_PCE;	//No Parity

    // Enable USART2, enable transmitter and receiver
    USART2->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);
}

// Sending one Byte
void uart_send_char(char c) {
    while (!(USART2->SR & USART_SR_TXE));  // wait for flag
    USART2->DR = c;  // send data
}

// sending more than one byte actually string
void uart_send_string(char *str) {
    while (*str) {
        uart_send_char(*str++);
    }
}

// receiving one byte
int uart_receive_char(void) {
	int timeout_counter = 0;
	while (!(USART2->SR & USART_SR_RXNE)) {
	    timeout_counter++;
	    if (timeout_counter >= Timeout) {
	        return -1;
	    }
	}  //wait for flag
    return (int)USART2->DR;  // receive data
}

// receiving more than one byte with buffer
void uart_receive_string(char *buffer, int max_length) {
    int i = 0;
    int c;
    while (i < max_length - 1) {
        c = uart_receive_char();
        if (c == -1) break;		//For timeout
        if (c == '\r' || c == '\n') break;  // stop receiving when click enter
        buffer[i++] = (char)c;
    }
    buffer[i] = '\0';  //add null character at the end of the string
}
