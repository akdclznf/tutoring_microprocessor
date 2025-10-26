// UART1 초기화
void uart1_init(void) {
	DDRD &= ~(_BV(2));  // Rx1핀 입력으로 설정
	DDRD |= _BV(3);  // Tx1핀 출력으로 설정
	UCSR1B = 0x00;  // uart 초기화 중 인터럽트 비활성화
	UCSR1A = 0x00;  // Asynchronous Normal Mode (기본값)
	UCSR1C = 0x06;  // 비동기 방식, No parity bit, 1 stop bit, 8비트 데이터
	UBRR1L = 0x67;  // set baud rate 0x67 for 9600bps, 0x33 for 19200, 0x19 for 38400,  0x6 for 115200bps
	UBRR1H = 0x00;
	UCSR1B = 0x08;  // 송신 허용
}

int uart1_putchar(char d) {
	UCSR1A = _BV(6);
	UDR1 = d;
	while(1) {
		if ((UCSR1A & _BV(6)) == _BV(6)) {  // 송신 완료까지 대기
			break;
		}
	}
}

void uart1_puts(const char *s) {
    while (*s) {
        uart1_putchar(*s);  // 하나씩 출력
        s++;
    }
}
