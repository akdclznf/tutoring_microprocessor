#define BYTE unsigned char

volatile BYTE LCD, lcd_addr;

// LCD 초기화
void twi_write(unsigned char address, unsigned char data) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);  // START

	while (!(TWCR & (1 << TWINT)));  // TWINT 플래그가 설정될 때까지 기다림
	while ((TWSR & 0xF8) != 0x08);  // START 조건을 기다림

	TWDR = (address << 1) & 0xFE;  // 7비트 주소 + 0(쓰기)
	TWCR = (1 << TWINT) | (1 << TWEN);  // 주소 전송

	while (!(TWCR & (1 << TWINT)));
	while ((TWSR & 0xF8) != 0x18);  // SLA+W ACK을 기다림

	TWDR = data; // 데이터 전송
	TWCR = (1 << TWINT) | (1 << TWEN);  // 데이터 전송

	while (!(TWCR & (1 << TWINT)));
	while ((TWSR & 0xF8) != 0x28);  // 데이터 ACK을 기다림

	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);  // STOP 조건
}

void LCD_command(BYTE command) {
	LCD = (command & 0xF0) | 0x0C;  // MSB 4비트, 명령 모드, 쓰기 모드, enable high
	twi_write(lcd_addr, LCD); 
	_delay_us(50);
	LCD &= ~(_BV(2));  // enable 토글
	twi_write(lcd_addr, LCD); 
	_delay_us(50);

	LCD = (command & 0x0F) << 4;  // LSB 4비트
	LCD = LCD | 0x0C;  // 명령 모드, 쓰기 모드, enable high
	twi_write(lcd_addr, LCD); 
	_delay_us(50);
	LCD &= ~(_BV(2));  // enable 토글
	twi_write(lcd_addr, LCD); 
	_delay_us(50);

	_delay_ms(1);
}

void LCD_data(BYTE data) {
	LCD = (data & 0xF0) | 0x0D;  // MSB 4비트, 데이터 모드, 쓰기 모드, enable high
	twi_write(lcd_addr, LCD); 
	_delay_us(50);
	LCD &= ~(_BV(2));  // enable 토글
	twi_write(lcd_addr, LCD); 
	_delay_us(50);

	LCD = (data & 0x0F) << 4;  // LSB 4비트
	LCD = LCD | 0x0D;  // 데이터 모드, 쓰기 모드, enable high
	twi_write(lcd_addr, LCD); 
	_delay_us(50);
	LCD &= ~(_BV(2));  // enable 토글
	twi_write(lcd_addr, LCD); 
	_delay_us(50);

	_delay_ms(1);
}

void LCD_string(BYTE command, BYTE *string) {
	LCD_command(command);  // 문자열 시작 위치 설정

	while (*string != '\0') {  // 문자열 표시 
		LCD_data(*string);
		string++;
	}
}

void LCD_nibble(BYTE lcd_addr) {
	_delay_ms(20);
	twi_write(lcd_addr, 0x3C);  // 명령 모드, 쓰기 모드, enable high
	_delay_us(50);
	twi_write(lcd_addr, 0x38);  // enable 토글
	_delay_us(50);

	_delay_ms(4);
	twi_write(lcd_addr, 0x3C);  // 명령 모드, 쓰기 모드, enable high
	_delay_us(50);
	twi_write(lcd_addr, 0x38);  // enable 토글
	_delay_us(50);

	_delay_ms(1);
	twi_write(lcd_addr, 0x2C);  // 명령 모드, 쓰기 모드, enable high
	_delay_us(50);
	twi_write(lcd_addr, 0x28);  // enable 토글
	_delay_us(50);
	twi_write(lcd_addr, 0x2C); 
	_delay_us(50);
}

void LCD_initialize(void) {
	LCD_command(0x3C);  // 기능 설정 (8비트, 16x2라인, 5x7 점)
	_delay_ms(2);
	LCD_command(0x08);  // 디스플레이 설정 (디스플레이 켬, 커서 끔, 깜박임 끔)
	_delay_ms(2);
	LCD_command(0x01);  // 화면 지우기
	_delay_ms(2);
	LCD_command(0x06);  // 입력 모드 설정 (증가, 이동 없음)
	_delay_ms(2);
	LCD_command(0x02);  // 홈으로 돌아가기
	_delay_ms(2);
}

void LCD_setting() {
	lcd_addr = 0x27;  // LCD I2C 주소

	// TWI (I2C) 초기화
	TWSR = 0x01;  // 프리스케일러 4 설정
	TWBR = 18;  // SCL 주파수 100 kHz로 설정
}
