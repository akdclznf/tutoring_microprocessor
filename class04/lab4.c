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

#include "uart0.h"

volatile uint16_t light = 0;  // 조도 센서 값 저장할 변수
volatile uint16_t water = 0;  // 수위 센서
volatile uint16_t soil = 0;  // 수분 센서

// 조도 센서 함수
uint16_t light_level(void) {
	ADMUX  = 0x40;  // ADC0 (PF0) 선택
	_delay_ms(100);  // ADMUX 선택 딜레이
	ADCSRA |= _BV(6);  // ADC 변환 시작
	return ADCW;  // 값 반환
}

// 수위 센서 함수
uint16_t water_level(void) {
	ADMUX  = 0x41;  // ADC1 (PF1) 선택
	_delay_ms(100);
	ADCSRA |= _BV(6);
	return ADCW;
}

// 수분 센서 함수
uint16_t soil_level(void) {
	ADMUX  = 0x42;  // ADC2 (PF2) 선택
	_delay_ms(100);
	ADCSRA |= _BV(6);
	return ADCW;
}

int main(void) {
	_delay_ms(500);  // 부팅 시간

	MCUCR =0x0;  // port 입출력 설정
	XMCRB =0x0;

	init_printf();  // printf 사용

	// ADC 설정
	ADCSRA |= _BV(7);  // ADC 허용

	ADCSRA |= _BV(5);  // free running mode

	ADCSRA |= _BV(2);  // prescaler 128
	ADCSRA |= _BV(1);
	ADCSRA |= _BV(0);

	_delay_ms(500);  // 안정화 대기

	while (1) {
		light = light_level();  // 조도 센서 값 저장
		water = water_level();  // 수위 센서
		soil = soil_level();  // 수분 센서
		printf("Light Level = %u\r\n", light);
		printf("Water Level = %u\r\n", water);
		printf("Soil Level = %u\r\n", soil);
		_delay_ms(1000);
	}
}
