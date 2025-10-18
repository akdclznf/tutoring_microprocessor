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

	DDRA |= _BV(0);  // PORTA의 0번 핀을 출력으로 설정

	// LED 또는 부저 1초마다 작동
	while(1) {  // 무한 반복문
		PORTA |= _BV(0);  // led or buzzer ON
		_delay_ms(1000);  // 1초 대기
		PORTA &= ~(_BV(0));  // led or buzzer OFF
		_delay_ms(1000);  // 1초 대기
	}
}