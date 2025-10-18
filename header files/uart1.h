// UART1 초기화
void uart1_init(void) {
	DDRD &= ~(_BV(2));  // Rx1
	DDRD |= _BV(3);  // Tx1
	UCSR1B = 0x00;  // interrupt disable while setting baud rate
	UCSR1A = 0x00;  // Asynchronous Normal Mode
	UCSR1C = 0x06;  // 비동기 방식, No parity bit, 1 stop bit 
	UBRR1L = 0x67;  // set baud rate 0x67 for 9600bps, 0x33 for 19200, 0x19 for 38400,  0x6 for 115200bps
	UBRR1H = 0x00;
	UCSR1B = 0x08;  // transmitter enable only
}

int uart1_putchar(char d) {
	UCSR1A = UCSR1A | 0x40;
	UDR1 = d;
	while (!(UCSR1A & 0x40));
}
/*
int uart1_getchar(void) {
	char d;
	while (!(UCSR1A & 0x80));
	d = UDR1;
	uart1_putchar(d);
	return d;
}
*/
void uart1_puts(const char *s) {
    while (*s) {
        uart1_putchar(*s);  // 하나씩 출력
        s++;
    }
}