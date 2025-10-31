// UART0 �ʱ�ȭ
void uart0_init(void) {
	DDRE &= ~(_BV(0));  // Rx0�� �Է����� ����
	DDRE |= _BV(1);  // Tx0�� ������� ����
	UCSR0B = 0x00;  // uart �ʱ�ȭ �� ���ͷ�Ʈ ��Ȱ��ȭ
	UCSR0A = 0x00;  // uart �ʱ�ȭ, Normal Mode (�⺻��)
	UCSR0C = 0x06;  // �񵿱� ���, No parity bit, 1 stop bit, 8��Ʈ ������
	UBRR0L = 0x67;  // set baud rate 0x67 for 9600bps, 0x33 for 19200, 0x19 for 38400,  0x6 for 115200bps
	UBRR0H = 0x00;
	UCSR0B = 0x18;  // �ۼ��� ���
}

int uart0_getchar(void) {
	char c;
	while(1) {
		if ((UCSR0A &  _BV(7)) ==  _BV(7)) {  // ���� �Ϸ���� ��� (�ڵ� �ʱ�ȭ)
			break;
		}
	}
	c = UDR0;  // ������ �б�
	uart0_putchar(c);  // ����
	return c;
}

void uart0_putchar(char c) {
	UCSR0A |= _BV(6);  // �۽� �Ϸ� �÷��� �ʱ�ȭ
	UDR0 = c;  // �۽� ���� ����
	while(1) {
		if ((UCSR0A & _BV(6)) ==  _BV(6)) {  // �۽� �Ϸ���� ���
			break;
		}
	}
}

void init_printf(void) {
	uart0_init();
	fdevopen(uart0_putchar, uart0_getchar);  // printf�� �ۼ��� �Լ� ����
}