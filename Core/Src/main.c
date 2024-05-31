#include "stm32f4xx.h"
#include "pwm.h"
#include "dht22.h"
#include "delay.h"
#include "uart.h"
#include "adc.h"
#include "gpio.h"
#include "i2c.h"
#include "lcd.h"
#include <stdio.h>

int main(void)
{
	// Initialize
	UART_Init();
	
	Timer2_Init();
	Timer3_Init();
	Timer4_Init();
	
	ADC_Init();
	
	GPIOB0_Init();
	GPIOC0_Init();
	GPIOC1_Init();
	GPIOC2_Init();
	
	I2C_Init();
	
	LCD_Init();

	uint8_t response;
	uint8_t humidity_first_byte;
	uint8_t humidity_second_byte;
	uint8_t temperature_first_byte;
	uint8_t temperature_second_byte;
	uint8_t sum;
	
	uint16_t raw_humidity;
	uint16_t raw_temperature;
	
	float humidity;
	float temperature;

	uint8_t temperature1;
	uint8_t humidity1;
	
	uint32_t adcValue0;
	uint32_t adcValue1;
	int count;
	uint8_t dirt_value;
	int flag = 0;
	while (1)
	{
		// Dirt sensor
		dirt_value = GPIOC2_Read();
		if (dirt_value == 1) {
			GPIOC1_On();
		} else {
			GPIOC1_Off();
		}
		
		// Light sensor
		adcValue1 = ADC_Read_Channel_1();
		PWM3_SetDutyCycle(adcValue1);

		// Humidity and temperature sensor
		DHT22_Start();
		response = DHT22_Check_Response();
		if (response == 2) {
			UART_Transmit_String("Error");
		} else {
			//Read 40 bit data from dht22
			humidity_first_byte = DHT22_Read();
			humidity_second_byte = DHT22_Read();
			temperature_first_byte = DHT22_Read();
			temperature_second_byte = DHT22_Read();
			sum = DHT22_Read();
			
			//Concat the 8 bit datas
			raw_humidity = (humidity_first_byte << 8) | humidity_second_byte;
			raw_temperature = (temperature_first_byte << 8) | temperature_second_byte;

			//Caculate the real data 
			humidity = raw_humidity / 10.0f;
			temperature = raw_temperature / 10.0f;
			
			//Threshold for fan and mist spray
			if (humidity < 70 || temperature > 30) {
				GPIOB0_Off();
				GPIOC0_On();
			} else {
				GPIOB0_On();
				GPIOC0_Off();
			}

			//Send data to LCD
			LCD_PutStringFloat("Hudmidity:", humidity, 1);
			LCD_PutStringFloat("Temperature:", temperature, 2);

			// Send data to PC
			UART_Transmit_Float(humidity, temperature);
		}
		
		// Check every 400ms
		delay_us(400000);
	}
}
