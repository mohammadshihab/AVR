#include<avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
//#define LED_DDR   DDRB;
//#define LED_PORT  PORTB;

//initializing function for ADC//
static inline void initADC0(void){
	ADMUX =    ADMUX | (1<<REFS0);
	ADCSRA =  ADCSRA |  (1<<ADPS1)|(1<<ADPS0);
	ADCSRA =  ADCSRA |  (1<< ADEN);
}


//main function//
int main(void){
	//initialization in main function//
	int ledValue;
	int adcValue;
	int i;
	initADC0()
	PORTB = 0xff;
	
	
	//event loop//
	while(1){
		
		
		ADCSRA |=   (1<<ADSC);
		loop_until_bit_is_clear(ADCSRA,ADSC);
		adcValue=ADC;
		
		ledValue=(adcValue>>7);
		
		PORTB = 0;
		for (i=0; i<=ledValue; i++){
			PORTB |= (1<<i);
		}
		_delay_ms(50);
	}
	return(0);
}