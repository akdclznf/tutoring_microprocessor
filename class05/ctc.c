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

volatile int_count = 0;

ISR(TIMER0_COMP_vect) {  // 약 1초마다 ON 출력
	int_count ++;
	if (int_count >= 122) {
		printf("ON\n\r");
		int_count = 0;
	}
}

int main(void) {
	_delay_ms(100);  // 부팅시간

	MCUCR = 0x0;
	XMCRB = 0x0;

	init_printf();

	TCCR0 &= ~(_BV(6));  // CTC mode
	TCCR0 |= _BV(3);

	TCCR0 |= _BV(2);  // 분주 1024
	TCCR0 |= _BV(1);
	TCCR0 |= _BV(0);

	OCR0 = 127;  // 지정 값

	TIMSK |= _BV(1);  // COMP0 인터럽트 허용

	sei();  // 전역 인터럽트 허용

	while(1) {
	}
}