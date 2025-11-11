#define F_CPU 16000000UL  // 클럭 주파수 설정

#include <avr/io.h>  // 라이브러리, 헤더파일 불러오기
#include <util/delay.h>
#include <compat/twi.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/signal.h>
#include <stdlib.h>
#include <math.h>
#include <avr/pgmspace.h>

// dht11 연결 핀 설정
#define DHT11_DDR DDRC
#define DHT11_PORT PORTC
#define DHT11_PIN PINC
#define DHT11_INPUTPIN PC0

#include "uart0.h"
#include "dht11.h"  // dht11.h 헤더 파일 가져오기

int main(void) {
	_delay_ms(100);  // 부팅시간

	MCUCR = 0x0;
	XMCRB = 0x0;

	uint8_t temp = 0;  // 온습도 값 저장할 변수 선언
	uint8_t humidity = 0;

	init_printf();

	// 온도 에러값 소거
	dht11_getdata(0, &temp);  // temp 변수에 온도 값 가져오기
	dht11_getdata(1, &humidity);  // humi 변수에 습도 값 가져오기
	_delay_ms(2000);  // dht11의 데이터 읽는 주기는 2초 이상이 안정적

	while(1) {
		// 온습도 값 읽기
		dht11_getdata(0, &temp);
		dht11_getdata(1, &humidity);
		_delay_ms(2000);

		// 온습도 값 출력
		printf("Temperature : %d\n\r", temp);
		printf("Humidity : %d\n\r", humidity);
	}
}
