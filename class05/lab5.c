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

#define DHT11_DDR DDRB
#define DHT11_PORT PORTB
#define DHT11_PIN PINB
#define DHT11_INPUTPIN PB0

#include "uart0.h"
#include "uart1.h"
#include "dht11.h"
#include "lcd.h"

int main(void) {
	_delay_ms(100);  // 부팅시간

	MCUCR = 0x0;
	XMCRB = 0x0;

	uint8_t temp = 0;
	uint8_t humidity = 0;
	
	char message_a[10] = {0};  // 10바이트 문자 저장
	char message_b[10] = {0};

	init_printf();
	uart1_init();

	// LCD 초기화
	LCD_port();
	LCD_setting();
	LCD_nibble(lcd_addr);
	LCD_initialize();
	LCD_command(0x01);  // 화면 지우기
	_delay_ms(10);

	LCD_string(0x80, "Temp : ");
	LCD_string(0x87, "--");
	LCD_command(0x89);
	LCD_data(0xDF);
	LCD_string(0x8a, "C");
	LCD_string(0xC0, "Humi : ");
	LCD_string(0xc7, "--");
	LCD_string(0xca, "%");

	// 온도 에러값 소거
	dht11_getdata(0, &temp);
	dht11_getdata(1, &humidity);
	_delay_ms(2000);

	while(1) {
		// 온습도 값 읽기
		dht11_getdata(0, &temp);
		dht11_getdata(1, &humidity);
		_delay_ms(2000);

		// 온습도 값 출력
		printf("Temperature : %d\n\r", temp);
		printf("Humidity : %d\n\r", humidity);
		_delay_ms(100);

		itoa(temp, message_a, 10);  // 숫자를 10바이트 문자로 변환
		itoa(humidity, message_b, 10);

		uart1_puts("Temperature : ");
		uart1_puts(message_a);
		uart1_puts("\n\r");
		uart1_puts("Humidity : ");
		uart1_puts(message_b);
		uart1_puts("\n\r");

		LCD_command(0x01);  // 화면 지우기
		_delay_ms(10);
		
		if (temp == DHT11_ERROR) {
			LCD_string(0x87, "--");
		}
		else {
			LCD_string(0x87, message_a);
		}
		if (humidity == DHT11_ERROR) {
			LCD_string(0xc7, "--");
		}
		else {
			LCD_string(0xc7, message_b);
		}
		LCD_string(0x80, "Temp : ");
		LCD_command(0x89);
		LCD_data(0xDF);
		LCD_string(0x8a, "C");
		LCD_string(0xC0, "Humi : ");
		LCD_string(0xca, "%");
	}
}
