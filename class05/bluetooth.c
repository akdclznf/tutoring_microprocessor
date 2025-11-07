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

#include "uart1.h"  // uart1 헤더 파일 가져오기

int main(void) {
	_delay_ms(100);  // 부팅시간

	MCUCR = 0x0;
	XMCRB = 0x0;

	uart1_init();  // uart1 초기화 함수 호출

	while(1) {
		uart1_puts("Hello World!\n\r");  // uart1으로 "Hello" 출력
		_delay_ms(1000);
	}
}