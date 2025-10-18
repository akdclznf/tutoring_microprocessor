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

#include "rgb.h"

int main(void) {
	MCUCR = 0x0;  // 포트 입출력 설정
	XMCRB = 0x0;

	init_rgb_timer();  // fast pwm설정 (rgb.h 참조)

	DDRB |= _BV(5);  // 각 핀 출력으로 설정
	DDRB |= _BV(6);
	DDRB |= _BV(7);

	set_color(255, 0, 0);  // white
	_delay_ms(1000);
	set_color(255, 0, 0);  // red
	_delay_ms(1000);
	set_color(0, 255, 0);  // green
	_delay_ms(1000);
	set_color(0, 0, 255);  // blue
	_delay_ms(1000);
	set_color(0, 0, 0);  // off
}