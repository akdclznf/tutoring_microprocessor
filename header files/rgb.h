#define OCR1_R OCR1A  // 16bit 타이머/카운터 핀에 할당
#define OCR1_G OCR1B
#define OCR1_B OCR1C

void set_color(int red, int green, int blue) {  // 각 핀의 최대값으로 255 지정
	OCR1_R = red & 255;
	OCR1_G = green & 255;
	OCR1_B = blue & 255;
}

void init_rgb_timer() {  // 16bit 카운터로 8bit(256) 연산을 보기 쉽게 하기 위해 fast pwm 설정
	TCCR1A |= _BV(WGM10);
	TCCR1A |= _BV(COM1A1);
	TCCR1A |= _BV(COM1B1);
	TCCR1A |= _BV(COM1C1);
	TCCR1B |= _BV(WGM12);
	TCCR1B |= _BV(CS11);
	set_color(0, 0, 0);
}