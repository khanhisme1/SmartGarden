#include "stm32f4xx.h"

void UART_Init(void);
void UART_Transmit(char c);
void UART_Transmit_String(char* str);
void UART_Transmit_Uint8(uint8_t num);
void UART_Transmit_Float(float humidity, float temperature);
