#define BYTE unsigned char

volatile BYTE LCD, lcd_addr;

void twi_write(unsigned char address, unsigned char data) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

	while (!(TWCR & (1 << TWINT)));
	while ((TWSR & 0xF8) != 0x08);

	TWDR = (address << 1) & 0xFE;
	TWCR = (1 << TWINT) | (1 << TWEN);

	while (!(TWCR & (1 << TWINT)));
	while ((TWSR & 0xF8) != 0x18);

	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);

	while (!(TWCR & (1 << TWINT)));
	while ((TWSR & 0xF8) != 0x28);

	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void LCD_command(BYTE command) {
	LCD = (command & 0xF0) | 0x0C;
	twi_write(lcd_addr, LCD); 
	_delay_us(50);
	LCD &= ~(_BV(2));
	twi_write(lcd_addr, LCD); 
	_delay_us(50);

	LCD = (command & 0x0F) << 4;
	LCD = LCD | 0x0C;
	twi_write(lcd_addr, LCD); 
	_delay_us(50);
	LCD &= ~(_BV(2));
	twi_write(lcd_addr, LCD); 
	_delay_us(50);

	_delay_ms(1);
}

void LCD_data(BYTE data) {
	LCD = (data & 0xF0) | 0x0D;
	twi_write(lcd_addr, LCD); 
	_delay_us(50);
	LCD &= ~(_BV(2));
	twi_write(lcd_addr, LCD); 
	_delay_us(50);

	LCD = (data & 0x0F) << 4;
	LCD = LCD | 0x0D;
	twi_write(lcd_addr, LCD); 
	_delay_us(50);
	LCD &= ~(_BV(2));
	twi_write(lcd_addr, LCD); 
	_delay_us(50);

	_delay_ms(1);
}

void LCD_string(BYTE command, BYTE *string) {  // 문자열 출력
	LCD_command(command);  // 문자열 시작 위치 설정

	while (*string != '\0') {
		LCD_data(*string);
		string++;
	}
}

void LCD_nibble(BYTE lcd_addr) {
	_delay_ms(20);
	twi_write(lcd_addr, 0x3C);
	_delay_us(50);
	twi_write(lcd_addr, 0x38);
	_delay_us(50);

	_delay_ms(4);
	twi_write(lcd_addr, 0x3C);
	_delay_us(50);
	twi_write(lcd_addr, 0x38);
	_delay_us(50);

	_delay_ms(1);
	twi_write(lcd_addr, 0x2C);
	_delay_us(50);
	twi_write(lcd_addr, 0x28);
	_delay_us(50);
	twi_write(lcd_addr, 0x2C); 
	_delay_us(50);
}

void LCD_initialize(void) {
	LCD_command(0x3C);  // 기능 설정 (8비트, 16x2 라인, 5x7 점)
	_delay_ms(2);
	LCD_command(0x08);  // 디스플레이 설정 (디스플레이 ON, 커서 OFF, 깜박임 OFF)
	_delay_ms(2);
	LCD_command(0x01);  // 화면 지우기
	_delay_ms(2);
	LCD_command(0x06);  // 입력 모드 설정 (증가, 이동 없음)
	_delay_ms(2);
	LCD_command(0x02);  // 홈으로 돌아가기
	_delay_ms(2);
}

void LCD_setting(void) {
	lcd_addr = 0x27;  // LCD I2C 주소

	// I2C 초기화
	TWSR = 0x01;  // 분주 4로 설정
	TWBR = 18;  // SCL 주파수 100kHz로 설정
}

void LCD_port(void) {
	DDRD |= _BV(0);  // SCL, SDA 출력으로 설정
	DDRD |= _BV(1);

	PORTD &= ~(_BV(0));  // SCL, SDA LOW로 설정
	PORTD &= ~(_BV(1));
}