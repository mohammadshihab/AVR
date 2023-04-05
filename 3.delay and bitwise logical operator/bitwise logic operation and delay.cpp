
#include <avr/io.h>
#include<util/delay.h>

int main(void)
{
	
	DDRB = 0xff;
	int i,j;
	i= 0b10001001;
	j= 0b10110100;
	/* Replace with your application code */
	while (1)
	{
		PORTB |= i; // here by default PORTB= 0b00000000;
		_delay_ms(500);
		PORTB = i & j ; 
		_delay_ms(500);
		
	}
	return 0;
}


