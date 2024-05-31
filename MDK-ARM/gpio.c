#include "stm32f4xx.h"
#include "gpio.h"


//B0 as output to control mist spray module
void GPIOB0_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	GPIOB->MODER &= ~GPIO_MODER_MODER0;
	GPIOB->MODER |= GPIO_MODER_MODER0_0;
}

//C0 as output to control DC motor
void GPIOC0_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	GPIOC->MODER &= ~GPIO_MODER_MODER0;
	GPIOC->MODER |= GPIO_MODER_MODER0_0;
}

//C1 as output to control water module
void GPIOC1_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	GPIOC->MODER &= ~GPIO_MODER_MODER1;
	GPIOC->MODER |= GPIO_MODER_MODER1_1;
}

//C2 as input for reading dirt sensor
void GPIOC2_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	GPIOC->MODER &= ~GPIO_MODER_MODER2;
}


void GPIOB0_Off(void) {
	GPIOB->ODR &= ~GPIO_ODR_ODR_0;
}

void GPIOB0_On(void) {
	GPIOB->ODR |= GPIO_ODR_ODR_0;
}

void GPIOC0_Off(void) {
	GPIOC->ODR &= ~GPIO_ODR_ODR_0;
}

void GPIOC0_On(void) {
	GPIOC->ODR |= GPIO_ODR_ODR_0;
}

void GPIOC1_On(void) {
	GPIOC->ODR |= GPIO_ODR_ODR_1;
}

void GPIOC1_Off(void) {
	GPIOC->ODR &= ~GPIO_ODR_ODR_1;
}

uint8_t GPIOC2_Read(void) {
    uint8_t data = (GPIOC->IDR & GPIO_IDR_IDR_2) >> 2;
    return data;
}
