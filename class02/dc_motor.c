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
	MCUCR = 0x0;  // 포트 입출력 설정
	XMCRB = 0x0;

	DDRA |= _BV(0);  // dc 모터의 + 연결
	DDRA |= _BV(1);  // dc 모터의 - 연결

	PORTA |= _BV(0);  // dc 모터 회전
	PORTA &= ~(_BV(1));
	_delay_ms(4000);
	PORTA &= ~(_BV(0));  // 정지

	PORTA |= _BV(1);  // dc 모터 반대로 회전
	PORTA &= ~(_BV(0));
	_delay_ms(4000);
	PORTA &= ~(_BV(1));  // 정지
}
