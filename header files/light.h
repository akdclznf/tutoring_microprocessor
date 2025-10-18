volatile uint16_t light = 0;

// 조도 센서 초기화
void adc_light(void) {

	// ADC 초기화
	ADMUX = (1 << REFS0);  // AVCC 기준 전압
	ADMUX &= ~(_BV(4));
	ADMUX &= ~(_BV(3));
	ADMUX &= ~(_BV(2));
	ADMUX |= _BV(1);
	ADMUX |= _BV(0);

	// ADC 제어 레지스터 초기화
	ADCSRA = 0;
	ADCSRA |= _BV(7);  // ADC 활성화 (ADEN = 1)
	ADCSRA |= _BV(6);  // ADC 변환 시작 (ADSC = 1)

	// ADC prescaler 설정
	ADCSRA |= _BV(2);  // prescaler = 128
	ADCSRA |= _BV(1);
	ADCSRA |= _BV(0);

	// ADC 변환 완료 대기
	while (ADCSRA & _BV(6));  // ADSC가 1이면 변환 중, 0이면 변환 완료

	// 변환 완료 후 결과 읽기
	light = ADCL | (ADCH << 8);  // 10비트 값을 읽기
}