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

int main(void) {
	MCUCR = 0x0;  // ��Ʈ ����� ����
	XMCRB = 0x0;

	DDRA |= _BV(0);  // dc ������ + ����
	DDRA |= _BV(1);  // dc ������ - ����

	PORTA |= _BV(0);  // dc ���� ȸ��
	PORTA &= ~(_BV(1));
	_delay_ms(4000);
	PORTA &= ~(_BV(0));  // ����

	PORTA |= _BV(1);  // dc ���� �ݴ�� ȸ��
	PORTA &= ~(_BV(0));
	_delay_ms(4000);
	PORTA &= ~(_BV(1));  // ����
}
