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

volatile uint16_t light = 0;  // ���� ���� �� ������ ����
volatile uint16_t water = 0;  // ���� ����
volatile uint16_t soil = 0;  // ���� ����

// ���� ���� �Լ�
uint16_t light_level(void) {
	ADMUX  = 0x40;  // ADC0 (PF0) ����
	_delay_ms(100);  // ADMUX ���� ������
	ADCSRA |= _BV(6);  // ADC ��ȯ ����
	return ADCW;  // �� ��ȯ
}

// ���� ���� �Լ�
uint16_t water_level(void) {
	ADMUX  = 0x41;  // ADC1 (PF1) ����
	_delay_ms(100);
	ADCSRA |= _BV(6);
	return ADCW;
}

// ���� ���� �Լ�
uint16_t soil_level(void) {
	ADMUX  = 0x42;  // ADC2 (PF2) ����
	_delay_ms(100);
	ADCSRA |= _BV(6);
	return ADCW;
}

int main(void) {
	_delay_ms(500);  // ���� �ð�

	MCUCR =0x0;  // port ����� ����
	XMCRB =0x0;

	init_printf();  // printf ���

	// ADC ����
	ADCSRA |= _BV(7);  // ADC ���

	ADCSRA |= _BV(5);  // free running mode

	ADCSRA |= _BV(2);  // prescaler 128
	ADCSRA |= _BV(1);
	ADCSRA |= _BV(0);

	_delay_ms(500);  // ����ȭ ���

	while (1) {
		light = light_level();  // ���� ���� �� ����
		water = water_level();  // ���� ����
		soil = soil_level();  // ���� ����
		printf("Light Level = %u\r\n", light);
		printf("Water Level = %u\r\n", water);
		printf("Soil Level = %u\r\n", soil);
		_delay_ms(1000);
	}
}
