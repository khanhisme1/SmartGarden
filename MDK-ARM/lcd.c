#include "stm32f4xx.h"
#include "i2c.h"
#include "lcd.h"
#include "delay.h"
#include <stdio.h>

void LCD_SendCmd(uint8_t cmd) {
    uint8_t data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd&0xf0);
    data_l = ((cmd<<4)&0xf0);
    data_t[0] = data_u|0x0C;	//en=1, rs=0
    data_t[1] = data_u|0x08;	//en=0, rs=0
    data_t[2] = data_l|0x0C;	//en=0, rs=0
    data_t[3] = data_l|0x08;	//en=1, rs=0
    for(int i=0; i<4; i++)
    {
        I2C_SendData(LCD_ADDR, data_t[i]);
        delay_us(20);
    }
}

void LCD_SendData(uint8_t data) {
    uint8_t data_u, data_l;
    uint8_t data_t[4];
    data_u = (data&0xf0);
    data_l = ((data<<4)&0xf0);
    data_t[0] = data_u|0x0D;	//en=1, rs=1
    data_t[1] = data_u|0x09;	//en=0, rs=1
    data_t[2] = data_l|0x0D;	//en=1, rs=1
    data_t[3] = data_l|0x09;	//en=0, rs=1
    for(int i=0; i<4; i++)
    {
        I2C_SendData(LCD_ADDR, data_t[i]);
        delay_us(20);
    }
}

void LCD_Init(void) {
    LCD_SendCmd(0x33);	//LCD 4-bit mode
		delay_us(10000);
    LCD_SendCmd(0x32);	//LCD 4-bit mode
		delay_us(2000);
    LCD_SendCmd(0x06);	//Cursor position auto increment after a character write
		delay_us(2000);
    LCD_SendCmd(0x0C);	//No cursor and cursor blinking
		delay_us(2000);
    LCD_SendCmd(0x28);	//2 line mode and 5x8 mode
		delay_us(2000);
    LCD_SendCmd(0x01);	//Clear screen
    delay_us(2000);
}

void LCD_PutString(char *string, int line) {
	if (line == 1) {
		LCD_SendCmd(0x80);	//First line
	} else {
		LCD_SendCmd(0xC0);	//Second line
	}
    while (*string) {
        LCD_SendData((uint8_t)(*string));
        string++;
    }
}

void LCD_PutStringFloat(char *string, float value, int line) {
    char buffer[32];
    sprintf(buffer, "%s%.1f", string, value);
    LCD_PutString(buffer, line);
}
