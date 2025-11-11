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

// dht11 핀 설정
#define DHT11_DDR DDRB
#define DHT11_PORT PORTB
#define DHT11_PIN PINB
#define DHT11_INPUTPIN PB2

// 헤더 파일 불러오기
#include "dht11.h"
#include "lcd.h"
#include "rgb.h"
#include "uart0.h"
#include "uart1.h"

volatile uint16_t light = 0;  // 조도 센서 값 저장할 변수
volatile uint16_t water = 0;  // 수위 센서
volatile uint16_t soil = 0;  // 수분 센서
volatile uint16_t count_overflow = 20598;  // 인터럽트 발생 횟수
volatile uint8_t pump_act = 0;  // 펌프 작동 플래그

// 조도 센서 함수
uint16_t light_level(void) {
	ADMUX = 0x40;  // ADC0 (PF0) 선택
	_delay_ms(100);  // ADMUX 선택 딜레이
	return ADCW;  // 값 반환
}

// 수위 센서 함수
uint16_t water_level(void) {
	ADMUX = 0x41;  // ADC1 (PF1) 선택
	_delay_ms(100);
	return ADCW;
}

// 수분 센서 함수
uint16_t soil_level(void) {
	ADMUX = 0x42;  // ADC2 (PF2) 선택
	_delay_ms(100);
	return ADCW;
}

// lcd 온습도 표시 틀
void lcd_dht11(void) {
	LCD_string(0x80, "Temp : ");
	LCD_command(0x89);
	LCD_data(0xDF);
	LCD_string(0x8a, "C");
	LCD_string(0xC0, "Humi : ");
	LCD_string(0xca, "%");
}

int main(void) {
	_delay_ms(100);  // 부팅시간

	MCUCR = 0x0;  // 포트 입출력 설정
	XMCRB = 0x0;

	uint8_t temp = 0;
	uint8_t humidity = 0;

	char message_a[10] = {0};  // 10바이트 문자 저장
	char message_b[10] = {0};

	init_printf();  // printf 사용 (uart0.h 참조)
	uart1_init();  // 문자 출력 사용 (uart1.h 참조)

	init_rgb_timer();  // fast pwm설정 (rgb.h 참조)

	DDRA |= _BV(2);  // pump
	DDRA |= _BV(3);
	DDRA |= _BV(6);  // fan
	DDRB |= _BV(5);  // rgb led
	DDRB |= _BV(6);
	DDRB |= _BV(7);

	// ADC set
	ADCSRA |= _BV(7);  // ADC 허용

	ADCSRA |= _BV(5);  // free running mode

	ADCSRA |= _BV(0);  // prescaler 128
	ADCSRA |= _BV(1);
	ADCSRA |= _BV(2);

	ADCSRA |= _BV(6);  // ADC 변환 시작

	SREG |= _BV(7);  // 전역 INT 허용

	// 16bit timer&counter3 set
	TCCR3A &= ~(_BV(0));  // WGM30	normal mode
	TCCR3A &= ~(_BV(1));  // WGM31
	TCCR3B &= ~(_BV(3));  // WGM32
	TCCR3B &= ~(_BV(4));  // WGM33

	TCCR3B |= _BV(0);  // CS30	1024 prescale
	TCCR3B &= ~(_BV(1));  // CS31 
	TCCR3B |= _BV(2);  // CS32

	TCNT3 = 0;  // 카운트 초기화

	ETIMSK |= _BV(2);  // 타이머3 OVF INT 허용

	// LCD 초기화
	LCD_port();
	LCD_setting();
	LCD_nibble(lcd_addr);
	LCD_initialize();
	LCD_command(0x01);  // 화면 지우기
	_delay_ms(10);

	LCD_string(0x87, "--");
	LCD_string(0xc7, "--");
	lcd_dht11();

	// 온도 에러값 소거
	dht11_getdata(0, &temp);
	dht11_getdata(1, &humidity);
	_delay_ms(2000);

	while(1) {
		light = light_level();  // 조도 센서 값 저장
		water = water_level();  // 수위 센서
		soil = soil_level();  // 수분 센서
		_delay_ms(100);

		// 온습도 값 읽기
		dht11_getdata(0, &temp);
		dht11_getdata(1, &humidity);
		_delay_ms(2000);

		// 값 터미널에 출력
		printf("Temperature : %d\n\r", temp);
		printf("Humidity : %d\n\r", humidity);
		printf("Light Level : %u\r\n", light);
		printf("Water Level : %u\r\n", water);
		printf("Soil Level : %u\r\n", soil);
		printf("Count : %d\n\r", count_overflow);
		_delay_ms(100);

		// 온습도 값을 10바이트 문자로 변환
		itoa(temp, message_a, 10);
		itoa(humidity, message_b, 10);

		// 블루투스로 연결된 기기에 출력
		uart1_puts("Temperature : ");
		uart1_puts(message_a);
		uart1_puts("\n\r");
		uart1_puts("Humidity : ");
		uart1_puts(message_b);
		uart1_puts("\n\r");

		LCD_command(0x01);  // 화면 지우기
		_delay_ms(10);
		
		// 물이 충분하면 lcd에 온습도 출력
		if (water > 300) {
			if (temp == DHT11_ERROR) {  // 에러 값
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
			lcd_dht11();
		}
		else {  // 물이 적으면 경고 문구 출력
			LCD_string(0x80, "Low water level");
			LCD_string(0xC0, "Please refill");
		}

		// 주위가 어두우면 led 점등
		if (light < 400) {
			set_color(0, 255, 0);
		}
		else {
			set_color(0, 0, 0);
		}

		// 토양이 마르면 펌프 작동
		if (soil > 900) {
			if (pump_act == 1) {
				pump_act = 0;  // 플래그 초기화

				PORTA |= _BV(3);  // 펌프 ON
				PORTA &= ~(_BV(2));
				_delay_ms(3000);

				PORTA &= ~(_BV(3));  // 펌프 OFF
			}
		}

		// 습도가 70% 보다 높으면 팬 작동
		if (humidity < 100) {
			if (humidity > 70) {
				PORTA |= _BV(6);
			}
			else {
				PORTA &= ~(_BV(6));
			}
		}
		else {
			PORTA &= ~(_BV(6));
		}
	}
}

// 펌프가 작동되면 약 24시간 동안 다시 작동하지 않음
ISR(TIMER3_OVF_vect) {
	count_overflow ++;  // 카운트 증가

	// 오버플로우 횟수에 따른 펌프 동작
	if (count_overflow >= 20600) {
		count_overflow = 0;  // 카운터 초기화
		pump_act = 1;  // 펌프 동작 플래그
	}
}