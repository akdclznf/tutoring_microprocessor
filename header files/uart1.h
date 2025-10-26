// UART1 �ʱ�ȭ
void uart1_init(void) {
	DDRD &= ~(_BV(2));  // Rx1�� �Է����� ����
	DDRD |= _BV(3);  // Tx1�� ������� ����
	UCSR1B = 0x00;  // uart �ʱ�ȭ �� ���ͷ�Ʈ ��Ȱ��ȭ
	UCSR1A = 0x00;  // Asynchronous Normal Mode (�⺻��)
	UCSR1C = 0x06;  // �񵿱� ���, No parity bit, 1 stop bit, 8��Ʈ ������
	UBRR1L = 0x67;  // set baud rate 0x67 for 9600bps, 0x33 for 19200, 0x19 for 38400,  0x6 for 115200bps
	UBRR1H = 0x00;
	UCSR1B = 0x08;  // �۽� ���
}

int uart1_putchar(char d) {
	UCSR1A = _BV(6);
	UDR1 = d;
	while(1) {
		if ((UCSR1A & _BV(6)) == _BV(6)) {  // �۽� �Ϸ���� ���
			break;
		}
	}
}

void uart1_puts(const char *s) {
    while (*s) {
        uart1_putchar(*s);  // �ϳ��� ���
        s++;
    }
}
