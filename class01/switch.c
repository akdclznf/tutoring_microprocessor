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

int main(void) {

	MCUCR = 0x0;  // 포트 입출력 설정
	XMCRB = 0x0;

	init_printf();  // printf 사용 (uart0.h에 포함)

	DDRA &= ~(_BV(0));  // PORTA의 0번 핀을 입력으로 설정

	PORTA |= _BV(0);  // PORTA의 0번 핀에 풀업 저항 활성

	// 스위치를 누르면 "on" 출력
	while(1) {  // 무한 반복문
		if ((PINA & _BV(0)) == 0) {  // 0번핀의 값과 1값을 비교하였을 때 거짓(0)이면 조건문 실행
			printf("on");  // "on" 출력
			_delay_ms(1000);  // 1초 대기
		}
	}
}