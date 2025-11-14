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

int main(void) {
	_delay_ms(100);  // 부팅 시간

	MCUCR = 0x0;  // 포트 입출력 설정
	XMCRB = 0x0;

	// fast pwm mode
	TCCR1A &= ~(_BV(0));
	TCCR1A |= _BV(1);
	TCCR1B |= _BV(3);
	TCCR1B |= _BV(4);

	// non-inverting mode
	TCCR1A &= ~(_BV(6));
	TCCR1A |= _BV(7);

	// prescaler 64
	TCCR1B |= _BV(0);
	TCCR1B |= _BV(1);
	TCCR1B &= ~(_BV(2));

	ICR1 = 4999;  // top 값

	DDRB |= _BV(5);  // servo motor

	_delay_ms(100);  // 안정 시간

	OCR1A = 250;  // high 비율, 1ms (0도)
	_delay_ms(1000);
	OCR1A = 375;  // 1.5ms (90도)
	_delay_ms(1000);
	OCR1A = 500;  // 2ms (180도)
	_delay_ms(1000);
}