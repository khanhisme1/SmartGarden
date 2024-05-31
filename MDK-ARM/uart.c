#include "stm32f4xx.h"
#include <stdio.h>
#include "uart.h"

void UART_Init(void) {
	// Clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	
	// Analog mode
    GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1; 

	// Alternative function to 7
    GPIOA->AFR[0] |= 0x00007700;

	//Baud rate 9600 with 16Mhz clock bus
    USART2->BRR = 0x683;

	// Enable uart
    USART2->CR1 |= USART_CR1_UE;

	// Enable TX and RX
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
	
	// Enable RX interrupt
    USART2->CR1 |= USART_CR1_RXNEIE;
	
	// Set USART2 interrupt priority and enable it
	NVIC_SetPriority(38, 5);
    NVIC_EnableIRQ(38);
}

void UART_Transmit(char c) {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

void UART_Transmit_String(char* str) {
    while (*str) {
        UART_Transmit(*str++);
    }
}

void UART_Transmit_Float(float humidity, float temperature) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%g %g\n", humidity, temperature);
    UART_Transmit_String(buffer);
}

void USART2_IRQHandler(void) {
    if (USART2->SR & USART_SR_RXNE) {
        // Read received character
        char received_char = USART2->DR;
        
        // Echo the received character back
        UART_Transmit(received_char);
    }
}
