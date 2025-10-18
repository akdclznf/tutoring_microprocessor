// UART0 초기화
void uart0_init(void) {
	DDRE &= ~(_BV(0));  // Rx0
	DDRE |= _BV(1);  // Tx0
	UCSR0B = 0x00;  // interrupt disable while setting baud rate
	UCSR0A = 0x00;  // Asynchronous Normal Mode
	UCSR0C = 0x06;  // 비동기 방식, No parity bit, 1 stop bit 
	UBRR0L = 0x67;  // set baud rate 0x67 for 9600bps, 0x33 for 19200, 0x19 for 38400,  0x6 for 115200bps
	UBRR0H = 0x00;
	UCSR0B = 0x08;  // transmitter enable only
}

int uart0_putchar(char c) {
	UCSR0A = UCSR0A | 0x40;
	UDR0 = c;
	while (!(UCSR0A & 0x40));
}

int uart0_getchar(void) {
	char c;
	while (!(UCSR0A & 0x80));
	c = UDR0;
	uart0_putchar(c);
	return c;
}

void init_printf(void) {
	uart0_init();
	fdevopen(uart0_putchar, uart0_getchar);
}
