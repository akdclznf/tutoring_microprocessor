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

// 전역 변수 설정
volatile int i = 0;
volatile uint16_t count_overflow = 0;

int main(void) {
	_delay_ms(500);  // 부팅 시간

	MCUCR = 0x0;  // port 입출력 설정
	XMCRB = 0x0;

	init_printf();  // printf 사용

	DDRE &= ~(_BV(7));  // 스위치 입력 설정

	TCCR0 &= ~(_BV(3));  // oc0 normal mode
	TCCR0 &= ~(_BV(6));

	TCCR0 |= _BV(2);  // oc0 prescaler 64
	TCCR0 &= ~(_BV(1));
	TCCR0 &= ~(_BV(0));

	TCNT0 = 0;  // 카운트 초기화

	SREG |= _BV(7);  // 전역 INT 허용

	TIMSK |= _BV(0);  // 타이머0 OVF INT 허용

	EIMSK |= _BV(7);  // INT7 외부 인터럽트 사용
	EICRB = EICRB | 0b11000000;  // rising edge trigger

	i = 0;  // 오동작 방지

	_delay_ms(500);  // 안정화 시간

	while(1) {
		printf("i : %d\n\r", i);  // 1초마다 변수 출력
		_delay_ms(1000);
	}
}

ISR(TIMER0_OVF_vect) {
	count_overflow ++;  // 인터럽트 횟수 증가(약 1ms마다 인터럽트 발생)
}

ISR(INT7_vect) {
	if (count_overflow >= 200) {  // 약 200ms 동안은 인터럽트 재발생 금지(채터링 방지)
		count_overflow = 0;  // 인터럽트 횟수 초기화
		i++;
	}
}
