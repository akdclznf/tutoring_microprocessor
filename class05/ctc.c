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

volatile int_count = 0;  // int_count를 전역 변수로 선언

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

	// CTC mode 설정 : WGM01(TCCR0 3번) : 1, WGM00(TCCR0 6번) : 0
	TCCR0 |= _BV(3);
	TCCR0 &= ~(_BV(6));

	// 분주 1024 설정 : CS02(TCCR0 0번) : 1, CS01(TCCR0 1번) : 1, CS00(TCCR0 2번) : 1
	TCCR0 |= _BV(0);
	TCCR0 |= _BV(1);
	TCCR0 |= _BV(2);

	OCR0 = 127;  // OCR 값 지정

	TIMSK |= _BV(1);  // COMP0 인터럽트 허용

	sei();  // 전역 인터럽트 허용

	while(1) {
	}
}
