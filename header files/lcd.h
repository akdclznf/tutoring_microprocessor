#define BYTE unsigned char

volatile BYTE LCD, lcd_addr;

// LCD �ʱ�ȭ
void twi_write(unsigned char address, unsigned char data) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);  // START

	while (!(TWCR & (1 << TWINT)));  // TWINT �÷��װ� ������ ������ ��ٸ�
	while ((TWSR & 0xF8) != 0x08);  // START ������ ��ٸ�

	TWDR = (address << 1) & 0xFE;  // 7��Ʈ �ּ� + 0(����)
	TWCR = (1 << TWINT) | (1 << TWEN);  // �ּ� ����

	while (!(TWCR & (1 << TWINT)));
	while ((TWSR & 0xF8) != 0x18);  // SLA+W ACK�� ��ٸ�

	TWDR = data; // ������ ����
	TWCR = (1 << TWINT) | (1 << TWEN);  // ������ ����

	while (!(TWCR & (1 << TWINT)));
	while ((TWSR & 0xF8) != 0x28);  // ������ ACK�� ��ٸ�

	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);  // STOP ����
}

void LCD_command(BYTE command) {
	LCD = (command & 0xF0) | 0x0C;  // MSB 4��Ʈ, ��� ���, ���� ���, enable high
	twi_write(lcd_addr, LCD); 
	_delay_us(50);
	LCD &= ~(_BV(2));  // enable ���
	twi_write(lcd_addr, LCD); 
	_delay_us(50);

	LCD = (command & 0x0F) << 4;  // LSB 4��Ʈ
	LCD = LCD | 0x0C;  // ��� ���, ���� ���, enable high
	twi_write(lcd_addr, LCD); 
	_delay_us(50);
	LCD &= ~(_BV(2));  // enable ���
	twi_write(lcd_addr, LCD); 
	_delay_us(50);

	_delay_ms(1);
}

void LCD_data(BYTE data) {
	LCD = (data & 0xF0) | 0x0D;  // MSB 4��Ʈ, ������ ���, ���� ���, enable high
	twi_write(lcd_addr, LCD); 
	_delay_us(50);
	LCD &= ~(_BV(2));  // enable ���
	twi_write(lcd_addr, LCD); 
	_delay_us(50);

	LCD = (data & 0x0F) << 4;  // LSB 4��Ʈ
	LCD = LCD | 0x0D;  // ������ ���, ���� ���, enable high
	twi_write(lcd_addr, LCD); 
	_delay_us(50);
	LCD &= ~(_BV(2));  // enable ���
	twi_write(lcd_addr, LCD); 
	_delay_us(50);

	_delay_ms(1);
}

void LCD_string(BYTE command, BYTE *string) {
	LCD_command(command);  // ���ڿ� ���� ��ġ ����

	while (*string != '\0') {  // ���ڿ� ǥ�� 
		LCD_data(*string);
		string++;
	}
}

void LCD_nibble(BYTE lcd_addr) {
	_delay_ms(20);
	twi_write(lcd_addr, 0x3C);  // ��� ���, ���� ���, enable high
	_delay_us(50);
	twi_write(lcd_addr, 0x38);  // enable ���
	_delay_us(50);

	_delay_ms(4);
	twi_write(lcd_addr, 0x3C);  // ��� ���, ���� ���, enable high
	_delay_us(50);
	twi_write(lcd_addr, 0x38);  // enable ���
	_delay_us(50);

	_delay_ms(1);
	twi_write(lcd_addr, 0x2C);  // ��� ���, ���� ���, enable high
	_delay_us(50);
	twi_write(lcd_addr, 0x28);  // enable ���
	_delay_us(50);
	twi_write(lcd_addr, 0x2C); 
	_delay_us(50);
}

void LCD_initialize(void) {
	LCD_command(0x3C);  // ��� ���� (8��Ʈ, 16x2����, 5x7 ��)
	_delay_ms(2);
	LCD_command(0x08);  // ���÷��� ���� (���÷��� ��, Ŀ�� ��, ������ ��)
	_delay_ms(2);
	LCD_command(0x01);  // ȭ�� �����
	_delay_ms(2);
	LCD_command(0x06);  // �Է� ��� ���� (����, �̵� ����)
	_delay_ms(2);
	LCD_command(0x02);  // Ȩ���� ���ư���
	_delay_ms(2);
}

void LCD_setting() {
	lcd_addr = 0x27;  // LCD I2C �ּ�

	// TWI (I2C) �ʱ�ȭ
	TWSR = 0x01;  // ���������Ϸ� 4 ����
	TWBR = 18;  // SCL ���ļ� 100 kHz�� ����
}
