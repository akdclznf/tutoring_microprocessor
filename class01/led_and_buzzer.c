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

	DDRA |= _BV(0);  // PORTA�� 0�� ���� ������� ����

	// LED �Ǵ� ���� 1�ʸ��� �۵�
	while(1) {  // ���� �ݺ���
		PORTA |= _BV(0);  // led or buzzer ON
		_delay_ms(1000);  // 1�� ���
		PORTA &= ~(_BV(0));  // led or buzzer OFF
		_delay_ms(1000);  // 1�� ���
	}
}
