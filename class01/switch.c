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

int main(void) {

	MCUCR = 0x0;  // ��Ʈ ����� ����
	XMCRB = 0x0;

	init_printf();  // printf ��� (uart0.h�� ����)

	DDRA &= ~(_BV(0));  // PORTA�� 0�� ���� �Է����� ����

	PORTA |= _BV(0);  // PORTA�� 0�� �ɿ� Ǯ�� ���� Ȱ��

	// ����ġ�� ������ "on" ���
	while(1) {  // ���� �ݺ���
		if ((PINA & _BV(0)) == 0) {  // 0������ ���� 1���� ���Ͽ��� �� ����(0)�̸� ���ǹ� ����
			printf("on");  // "on" ���
			_delay_ms(1000);  // 1�� ���
		}
	}
}
