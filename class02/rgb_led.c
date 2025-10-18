#define F_CPU 16000000UL  // Ŭ�� ���ļ� ����

#include <avr/io.h>  // ���̺귯��, ������� �ҷ�����
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
	MCUCR = 0x0;  // ��Ʈ ����� ����
	XMCRB = 0x0;

	init_rgb_timer();  // fast pwm���� (rgb.h ����)

	DDRB |= _BV(5);  // �� �� ������� ����
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
