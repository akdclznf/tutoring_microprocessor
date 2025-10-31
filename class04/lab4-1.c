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

void main(void) {
	_delay_ms(500);  // ���� �ð�

	uint16_t adc_data = 0;  // ADC�� ������ ���� ����

	MCUCR =0x0;  // port ����� ����
	XMCRB =0x0;

	init_printf();

	// ADC ����
	ADMUX  = 0x40;  // ADC0 (PF0) ����
	_delay_ms(100);  // ADMUX ���� ������

	ADCSRA |= _BV(5);  // free running mode

	ADCSRA |= _BV(2);  // prescaler 128
	ADCSRA |= _BV(1);
	ADCSRA |= _BV(0);

	ADCSRA |= _BV(7);  // ADC ���
	ADCSRA |= _BV(6);  // ADC ��ȯ ����

	_delay_ms(500);  // ����ȭ ���

	while (1) {
		adc_data = ADCW;  // ADC �� �а� ������ ����
		printf("Adc Data = %d\r\n", adc_data);
		_delay_ms(1000);
	}
}
