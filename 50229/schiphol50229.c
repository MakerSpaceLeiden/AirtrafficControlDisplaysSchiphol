#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "iocompat.h"	
#include "pins.h"	

#include "schiphol50229.h"	
#include "pd44.h"	

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


void WriteLED(unsigned char LED, unsigned char value){
	static uint16_t LEDs; 
	uint16_t shiftout;
	if(value==0){
		LEDs = LEDs&~(1<<LED);
	} 
	else
	{
		LEDs=LEDs|(1<<LED);
	}
	shiftout=LEDs;
	
	for(unsigned char i=0;i<3;i++){
		WriteLEDs(i,shiftout&0x1F);
		shiftout=shiftout>>5;
	}
};
void WriteLEDs(unsigned char col, unsigned char rows){
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
