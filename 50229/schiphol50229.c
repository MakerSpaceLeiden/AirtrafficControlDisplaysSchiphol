#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "iocompat.h"	
#include "pins.h"	

#include "schiphol50229.h"	
#include "pd44.h"	

static uint16_t LEDs;

void init50229(void) {
	OUTPUT(DISP_SEL_A0);
	OUTPUT(DISP_SEL_A1);
	OUTPUT(DISP_SEL_A2);
	OUTPUT(DISP_SEL_UPPER2);
	OUTPUT(DISP_SEL_LOWER2);
	OUTPUT(LED_E3);
	OUTPUT(LED_nE2);
	OUTPUT(LED_nE1);
	OUTPUT(LED_R0);
	OUTPUT(LED_R1);
	OUTPUT(LED_R2);
	OUTPUT(LED_R3);
	OUTPUT(LED_R4);
	OUTPUT(LED_SEL0);
	OUTPUT(LED_SEL1);
	OUTPUT(LED_SEL2);
	
	LOW(LED_R0);
	LOW(LED_R1);
	LOW(LED_R2);
	LOW(LED_R3);
	LOW(LED_R4);
	
	/*input + pull-up*/
	INPUT(KEY_C0);
	INPUT(KEY_C1);
	INPUT(KEY_C2);
	INPUT(KEY_C3);
	HIGH(KEY_C0);
	HIGH(KEY_C1);
	HIGH(KEY_C2);
	HIGH(KEY_C3);
	
	OUTPUT(KEY_R0);
	OUTPUT(KEY_R1);
	OUTPUT(KEY_R2);
	OUTPUT(KEY_R3);
}

void select50229(unsigned char c) {
	SET(DISP_SEL_A0,(c>>0)&1);
	SET(DISP_SEL_A1,(c>>1)&1);
	SET(DISP_SEL_A2,(c>>2)&1);
	SET(DISP_SEL_LOWER2,(c>>3)&1);
	SET(DISP_SEL_UPPER2,((c>>3)^1));
};

void setDisplay(int display, char * str) {
	select50229(display);
	
        for(int i = 0; i < 4; i++) {
		char c = *str;
		if (c) str++;
		if (!c) c = ' ';
		pd44_sendChar(3-i,c);
	};
}

void setFullDisplay(char * str) {
	int len = 0;
	for(char * p = str; *p; len++, p++) {};

	for(int d = 0; d < DISPLAYS && d*4 < len; d++) 
		setDisplay(d, str + d * 4);
}

unsigned char readLED(unsigned char LED){
	LED = LED%NUMLEDS;
	return ((LEDs>>LED)&1);
	};

void writeLED(unsigned char LED, unsigned char value){ 
	uint16_t shiftout;
	LED=LED%NUMLEDS;
	if(value==0){
		LEDs = LEDs&~(1<<LED);
	} 
	else
	{
		LEDs=LEDs|(1<<LED);
	}
	shiftout=LEDs;
	
	for(unsigned char i=0;i<3;i++){
		writeLEDs(i,shiftout&0x1F);
		shiftout=shiftout>>5;
	}
};

void writeLEDs(unsigned char col, unsigned char rows){
	LOW(LED_nE1);
	LOW(LED_nE2);
	LOW(LED_E3);
	LOW(LED_SEL2);
	SET(LED_R0,(rows>>0)&1);
	SET(LED_R1,(rows>>1)&1);
	SET(LED_R2,(rows>>2)&1);
	SET(LED_R3,(rows>>3)&1);
	SET(LED_R4,(rows>>4)&1);
	SET(LED_SEL0,(col>>0)&1);
	SET(LED_SEL1,(col>>1)&1);
	HIGH(LED_E3);
	LOW(LED_E3);
};

unsigned char keyscan50229(){
	/*TODO: ? better debouncing */
	
	INPUT(KEY_C3); /* restore everything upset for that one key not in the matrix */
	HIGH(KEY_C3);
	OUTPUT(KEY_R0);
	OUTPUT(KEY_R1);
	OUTPUT(KEY_R2);
	OUTPUT(KEY_R3);
	/*begin scan of matrix*/
	 LOW(KEY_R0);
	HIGH(KEY_R1);
	HIGH(KEY_R2);
	HIGH(KEY_R3);
	_delay_ms(5);
	if(0==READ(KEY_C0)) return 3;
	if(0==READ(KEY_C1)) return 2;
	if(0==READ(KEY_C2)) return 1;
	if(0==READ(KEY_C3)) return 0;
	HIGH(KEY_R0);
	 LOW(KEY_R1);
	HIGH(KEY_R2);
	HIGH(KEY_R3);
	_delay_ms(5);
	if(0==READ(KEY_C0)) return 7;
	if(0==READ(KEY_C1)) return 6;
	if(0==READ(KEY_C2)) return 5;
	if(0==READ(KEY_C3)) return 4;
	HIGH(KEY_R0);
	HIGH(KEY_R1);
	 LOW(KEY_R2);
	HIGH(KEY_R3);
	_delay_ms(5);
	if(0==READ(KEY_C0)) return 11;
	if(0==READ(KEY_C1)) return 10;
	if(0==READ(KEY_C2)) return 9;
	if(0==READ(KEY_C3)) return 8;
	HIGH(KEY_R0);
	HIGH(KEY_R1);
	HIGH(KEY_R2);
	 LOW(KEY_R3);
	_delay_ms(5);
	if(0==READ(KEY_C0)) return 15;
	if(0==READ(KEY_C1)) return 14;
	if(0==READ(KEY_C2)) return 13;
	if(0==READ(KEY_C3)) return 12;
	
	/*scan the one button not in the matrix: */
	HIGH(KEY_R3);
	INPUT(KEY_R0);
	INPUT(KEY_R1);
	INPUT(KEY_R2);
	INPUT(KEY_R3);
	
	LOW(KEY_C3);
	OUTPUT(KEY_C3);
	_delay_ms(5);
	if(0==READ(KEY_C2)) return 16;
	
	return 0xFF;
	}; 
