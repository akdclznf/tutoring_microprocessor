volatile uint16_t light = 0;

// ���� ���� �ʱ�ȭ
void adc_light(void) {
	// ADC �ʱ�ȭ: AVCC�� ���� �������� ����, ADC1 ä�� ����
	ADMUX = (1 << REFS0);  // AVCC ���� ����, ADC1 ä��
	ADMUX &= ~(_BV(4));
	ADMUX &= ~(_BV(3));
	ADMUX &= ~(_BV(2));
	ADMUX |= _BV(1);
	ADMUX |= _BV(0);
	// ADC ���� �������� �ʱ�ȭ
	ADCSRA = 0;
	ADCSRA |= _BV(7); // ADC Ȱ��ȭ (ADEN = 1)
	ADCSRA |= _BV(6); // ADC ��ȯ ���� (ADSC = 1)

	// ADC prescaler ����
	ADCSRA |= _BV(2);  // prescaler = 128
	ADCSRA |= _BV(1);
	ADCSRA |= _BV(0);

	// ADC ��ȯ ����
	ADCSRA |= _BV(6);  // ADSC = 1; ��ȯ ����

	// ADC ��ȯ �Ϸ� ���
	while (ADCSRA & _BV(6));  // ADSC�� 1�̸� ��ȯ ��, 0�̸� ��ȯ �Ϸ�

	// ��ȯ �Ϸ� �� ��� �б�
	light = ADCL | (ADCH << 8);  // 10��Ʈ ���� �б�

	printf("Light: %d\n", light);

	// 1000ms ���
	_delay_ms(1000);
	
}
