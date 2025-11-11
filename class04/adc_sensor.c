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

void main(void) {
	_delay_ms(500);  // 부팅 시간

	uint16_t adc_data = 0;  // ADC값 저장할 변수 선언

	MCUCR = 0x0;  // port 입출력 설정
	XMCRB = 0x0;

	init_printf();

	// ADC 설정
	ADMUX  = 0x40;  // ADC0 (PF0) 선택
	_delay_ms(100);  // ADMUX 선택 딜레이

	ADCSRA |= _BV(5);  // free running mode

	ADCSRA |= _BV(0);  // prescaler 128
	ADCSRA |= _BV(1);
	ADCSRA |= _BV(2);

	ADCSRA |= _BV(7);  // ADC 허용
	
	ADCSRA |= _BV(6);  // ADC 변환 시작

	_delay_ms(500);  // 안정화 대기

	while (1) {
		adc_data = ADCW;  // ADC 값 읽고 변수에 저장
		printf("Adc Data = %d\r\n", adc_data);
		_delay_ms(1000);
	}
}
