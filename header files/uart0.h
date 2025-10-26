// UART0 초기화
void uart0_init(void) {
	DDRE &= ~(_BV(0));  // Rx0핀 입력으로 설정
	DDRE |= _BV(1);  // Tx0핀 출력으로 설정
	UCSR0B = 0x00;  // uart 초기화 중 인터럽트 비활성화
	UCSR0A = 0x00;  // Asynchronous Normal Mode (기본값)
	UCSR0C = 0x06;  // 비동기 방식, No parity bit, 1 stop bit, 8비트 데이터
	UBRR0L = 0x67;  // set baud rate 0x67 for 9600bps, 0x33 for 19200, 0x19 for 38400,  0x6 for 115200bps
	UBRR0H = 0x00;
	UCSR0B = 0x18;  // 송수신 허용
}

int uart0_getchar(void) {
	char c;
	while(1) {
		if ((UCSR0A &  _BV(7)) ==  _BV(7)) {  // 수신 완료까지 대기 (자동 초기화)
			break;
		}
	}
	c = UDR0;  // 데이터 읽기
	uart0_putchar(c);  // 에코
	return c;
}

void uart0_putchar(char c) {
	UCSR0A |= _BV(6);  // 송신 완료 플래그 초기화
	UDR0 = c;  // 송진 문자 저장
	while(1) {
		if ((UCSR0A & _BV(6)) ==  _BV(6)) {  // 송신 완료까지 대기
			break;
		}
	}
}

void init_printf(void) {
	uart0_init();
	fdevopen(uart0_putchar, uart0_getchar);  // printf에 송수신 함수 연결
}