#define F_CPU 16000000UL  // 클럭 주파수 설정

#include <avr/io.h>  // 라이브러리, 헤더파일 불러오기
#include <util/delay.h>
#include <compat/twi.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/signal.h>
#include <stdlib.h>
#include <math.h>
#include <avr/pgmspace.h>

#include "lcd.h"

int main(void) {
	_delay_ms(100);  // 부팅시간

	MCUCR = 0x0;
	XMCRB = 0x0;

	// LCD 초기화
	LCD_port();
	LCD_setting();
	LCD_nibble(lcd_addr);
	LCD_initialize();
	LCD_command(0x01);  // 화면 지우기
	_delay_ms(10);

	while(1) {
		LCD_string(0x80, "Hello");
		LCD_string(0xC0, "World!");
		_delay_ms(1000);
		//LCD_command(0x01);
		//_delay_ms(10);
		LCD_string(0x80, "Hasung");
		LCD_string(0xC0, "Tutoring");
		_delay_ms(1000);
		//LCD_command(0x01);
		//_delay_ms(10);
	}
}