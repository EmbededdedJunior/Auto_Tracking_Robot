/*
 * Auto_Tracking_Robot_ATmega128.c
 *
 * Created: 2024-11-13 오후 2:22:20
 * Author : jung
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//=====================================================================//
//ADC 체널 0번 1번 사용 설정
#define FIRST_ADC_INPUT 0
#define LAST_ADC_INPUT 1
unsigned int adc_data[LAST_ADC_INPUT-FIRST_ADC_INPUT+1];
#define ADC_VREF_TYPE 0x00
//=================================================================
//통신관련 전처리 문구
#define U08 unsigned char
//=====통신중 수신받은 코드 값 저장
unsigned char Bt_Data=0;
int ir_l_v=0,ir_r_v=0;
int ir_l_sum=0,ir_r_sum=0;
int max_ir=900,i=0;
//=====================================================================//

// 인터럽트 서비스 루틴 선언(AD Converter + USART0 INT) 
ISR(USART0_RX_vect);
ISR(ADC_vect);
//=====================================================================//

// ADC interrupt service routine
ISR(ADC_vect)
{
	cli();
	static unsigned char input_index=0;
	// Read the AD conversion result
	adc_data[input_index]=ADCW;
	// Select next ADC input
	if (++input_index > (LAST_ADC_INPUT-FIRST_ADC_INPUT))
	input_index=0;
	ADMUX=(FIRST_ADC_INPUT | (ADC_VREF_TYPE & 0xff))+input_index;
	// Delay needed for the stabilization of the ADC input voltage
	_delay_us(10);
	// AD 변환시작
	ADCSRA|=0x40;
	
	sei();
}

//=====================================================================//

//=====================================================================//
// USART0 - Bluetooth 신호보내기/
//=====================================================================//
void TX0_char(char c)
{ 
	while(!(UCSR0A&0x20)); 
	UDR0=c; 
}

//char RX0_CHAR(void){ while(!(UCSR0A&0x80)); return UDR0; }      // 수신함수
void TX0_STR(char *s)
{ 
	while(*s)TX0_char(*s++); 
}

//cp로 부터 자료가 들어오면 인터럽트 발생 들어온 자료를 Bluetooth_Data변수에 저장
ISR(USART0_RX_vect)
{
	// 수신 인터럽트
	cli();
	Bt_Data = UDR0;
	sei();
}

void main(void){

	unsigned char str[30];   //str 문자배열

	// ADC 초기화
	// ADC Clock frequency: 1000.000 kHz
	// ADC Voltage Reference: AREF pin
	ADMUX=FIRST_ADC_INPUT | (ADC_VREF_TYPE & 0xff);
	ADCSRA=0xCC;

	// UART0 initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART0 Receiver: On
	// USART0 Transmitter: On
	// USART0 Mode: Asynchronous
	// USART0 Baud Rate: 9600
	DDRE=0b11111110; // RXD0=PORTE.0=입력으로설정, TXD0=PORTE.1=출력으로설정
	UCSR0A=0x00;
	UCSR0B=0x98;
	UCSR0C=0x06;
	UBRR0H=0x00;
	//UBRR0L=0x67; //9600
	UBRR0L=0x19; //38400

	//==================
	// Global 인터럽트 허용
	sei();
	//==================
	while(1){
		
		for(i=0;i<=50;i++){
			ir_l_v   = adc_data[0];
			ir_r_v   = adc_data[1];
			ir_l_sum=ir_l_sum+ir_l_v;
			ir_r_sum=ir_r_sum+ir_r_v;
			_delay_ms(2);
		}
		
		ir_l_v=ir_l_sum/50;
		ir_l_v=1024-ir_l_v;
		ir_r_v=ir_r_sum/50;
		ir_r_v=1024-ir_r_v;
		ir_l_sum=0;
		ir_r_sum=0;
		
		if((ir_r_v<=max_ir)||(ir_l_v<=max_ir)){
			TX0_char('a');
			_delay_ms(1000);
			}else{
			TX0_char('b');
			_delay_ms(1000);
		}
		//delay_ms(100);
	}
	
	return 0;
}
