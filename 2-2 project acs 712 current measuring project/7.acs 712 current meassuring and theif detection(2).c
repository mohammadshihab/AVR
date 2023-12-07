
#include <avr/io.h>
#include <stdint.h>       // needed for uint8_t
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>



char arr[11] = "0123456789";

void   LCD_Command ( unsigned char cmnd ){
	PORTB = cmnd;
	//PORTD = 0b00000100;  /*RS=0 command reg.  ; RW=0 Write operation  ; EN=1*/
	PORTD = (1<<6)|(0<<5)|(0<<4)|(1 << 2)|(1<<3);
	
	
	_delay_us(1);
	//PORTD = 0b00000000;   /*RS=0 command reg.  ; RW=0 Write operation  ; EN=0*/
	PORTD = (0<<6)|(0<<5)|(0<<4)|(1 << 2)|(1<<3);
	
	_delay_ms(3);
}


void lcdChar( unsigned char  character_data ){
	DDRB = 0xff;
	DDRC = 0xff;
	PORTB = character_data;
	//PORTD = 0b00000101; /* RS=1 Data reg.;  RW=0 write operation  and EN=1 */
	PORTD = (1<<6)|(0<<5)|(1<<4)|(1 << 2)|(1<<3);
	
	
	_delay_us(1);
	//PORTD = 0b00000001; /* EN=0 */
	PORTD = (0<<6)|(0<<5)|(1<<4)|(1 << 2)|(1<<3);
	
	
	_delay_ms(50);
}


void lcdInit(void){
	DDRB = 0xff;
	DDRD = 0xff;
	_delay_ms(20);
	LCD_Command (0x38);		/* Initialization of 16X2 LCD in 8bit mode */
	LCD_Command (0x0C);		/* Display ON Cursor OFF */
	LCD_Command (0x06);		/* Auto Increment cursor */
	LCD_Command (0x01);		/* Clear display */
	LCD_Command (0x80);		/* Cursor at home position */
}


void lcdClear(){
	LCD_Command(0x01);
	LCD_Command(0x80);
}



//for writing an single char in a specific point
void LCD_String_xy (char row, char pos, unsigned char  character_data)    /* Send string to LCD with xy position */
{
	if (row == 0 && pos<20)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	
	
	else if (row == 1 && pos<20)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	
	


	else if (row == 2 && pos<20)
	LCD_Command((pos & 0x0F)|0x94);

	else if (row == 3 && pos<20)
	LCD_Command((pos & 0x0F)|0xD4);

	lcdChar(character_data);		/* Call LCD string function */
}




//  to write char line in xy position
void LCD_stringWrite_xyz (char row, char pos,  char  *character_dataa)/* Send string to LCD with xy position */
{
	if (row == 0 && pos<20)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<20)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	//lcdChar(character_dataa);		/* Call LCD string function */
	
	else if (row == 2 && pos<20)
	LCD_Command((pos & 0x0F)|0x94);

	else if (row == 3 && pos<20)
	LCD_Command((pos & 0x0F)|0xD4);
	
	
	
	int ii=0;
	for(ii=0; character_dataa[ii]!=0; ii++){
		lcdChar(character_dataa[ii]);
	}
}







// to write a variable in xy position
int LCD_varWrite_xyz(int row, int coll, int variable){
	
	
	LCD_String_xy(row, coll, arr[variable/1000%10]);
	LCD_String_xy(row,(coll+1), arr[variable/100%10]);
	LCD_String_xy(row,(coll+2),arr[variable/10%10]);
	LCD_String_xy(row, (coll+3), arr[variable/1%10]);
	
	
	
	
}







int ADCsingleREAD(uint8_t adctouse)
{
	int ADCval;

	ADMUX = adctouse;         // use #1 ADC
	ADMUX |= (1 << REFS0);    // use AVcc as the reference
	ADMUX &= ~(1 << ADLAR);   // clear for 10 bit resolution
	
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // 128 prescale for 8Mhz
	ADCSRA |= (1 << ADEN);    // Enable the ADC

	ADCSRA |= (1 << ADSC);    // Start the ADC conversion

	while(ADCSRA & (1 << ADSC));      // Thanks T, this line waits for the ADC to finish


	ADCval = ADCL;
	ADCval = (ADCH << 8) + ADCval;    // ADCH is read so ADC can be updated again
	// both ADCL and ADCH registers are stored in
	// a simple INT VARIABLE//

	return ADCval;
}





int acs7122(int pinNo){
	int  Ivall,Ival,ix,iy,iz;
	
	// for counting AC current
	
	iz=0;
	for(iy=0; iy<200; iy++){
		
		Ivall= ADCsingleREAD(pinNo);
		
		if (Ivall>iz)
		{
			iz=Ivall;
		}
		
		//_delay_ms(0.025);
	}
	
	Ival = (iz-486);
	
	
	
	return Ival;
}










int peroftd(void){
	int supc, loadc,perc;
	
	supc = acs7122(2);
	loadc = acs7122(3);
	
	perc = ((supc-loadc)*100/supc);
	
	return (perc);
	
	
	
}









int main(void)
{
	
	
	
	DDRD = 0b00001111;
	
	
	PORTD |= (1 << 2)|(1<<3);
	
	lcdInit();
	
	LCD_Command (0x01);

	
	int supplyc;
	
	
	int loadc;
	
	
	int tlp;
	
	
	
	
	

	while (1)
	{
		
		tlp = peroftd();
		
		supplyc= acs7122(2);
		
		
		loadc = acs7122(3);
		
		
		LCD_stringWrite_xyz(0,0,"SC=");
		
		
		LCD_varWrite_xyz(0,3,supplyc);
		LCD_stringWrite_xyz(0,7,"mA");
		
		LCD_stringWrite_xyz(1,0,"LC=");
		
		LCD_varWrite_xyz(1,3,loadc);
		LCD_stringWrite_xyz(1,7,"mA");
		
		
		
		if (supplyc>(1.03*loadc))
		{
			LCD_stringWrite_xyz(2,0,"Theft load detected   ");
			LCD_stringWrite_xyz(3,0,"W(%)=");
			LCD_varWrite_xyz(3,8,tlp);
		}
		
		else
		{
			LCD_stringWrite_xyz(2,0,"No theft load");
			LCD_stringWrite_xyz(3,0,"             ");
		}
		
		

		
	}
	
	return 0;
}








