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

#include "uart0.h"

// ���� ���� ����
volatile int i = 0;
volatile uint16_t count_overflow = 0;

int main(void) {
	_delay_ms(500);  // ���� �ð�

	MCUCR =0x0;  // port ����� ����
	XMCRB =0x0;

	init_printf();  // printf ���

	DDRE &= ~(_BV(7));  // ����ġ �Է� ����

	TCCR0 &= ~(_BV(3));  // oc0 normal mode
	TCCR0 &= ~(_BV(6));

	TCCR0 |= _BV(2);  // oc0 prescaler 64
	TCCR0 &= ~(_BV(1));
	TCCR0 &= ~(_BV(0));

	TCNT0 = 0;  // ī��Ʈ �ʱ�ȭ

	SREG |= _BV(7);  // ���� INT ���

	TIMSK |= _BV(0);  // Ÿ�̸�0 OVF INT ���

	EIMSK |= _BV(7);  // INT7 �ܺ� ���ͷ�Ʈ ���
	EICRB = EICRB | 0b11000000;  // rising edge trigger

	i = 0;  // ������ ����

	_delay_ms(500);  // ����ȭ �ð�

	while(1) {
		printf("i: %d\n\r", i);  // 1�ʸ��� ���� ���
		_delay_ms(1000);
	}
}

ISR(TIMER0_OVF_vect) {
	count_overflow ++;  // ���ͷ�Ʈ Ƚ�� ����(�� 1ms���� ���ͷ�Ʈ �߻�)
}

ISR(INT7_vect) {
	if (count_overflow >= 200) {  // �� 200ms ������ ���ͷ�Ʈ ��߻� ����(ä�͸� ����)
		count_overflow = 0;  // ���ͷ�Ʈ Ƚ�� �ʱ�ȭ
		i++;
	}
}
