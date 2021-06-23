#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "iocompat.h"	
#include "pins.h"	

#include "pd50229.h"	
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
}

void select50229(unsigned char c) {
	SET(DISP_SEL_A0,(c>>0)&1);
	SET(DISP_SEL_A1,(c>>1)&1);
	SET(DISP_SEL_A2,(c>>2)&1);
	SET(DISP_SEL_LOWER2,(c>>3)&1);
	SET(DISP_SEL_UPPER2,((c>>3)^1));
};

void setDisplay(int display, const char * str) {
	select50229(display);
	
        for(int i = 0; i < 4; i++) {
		char c = *str;
		if (c) str++;
		if (!c) c = ' ';
		pd44_sendChar(3-i,c);
	};
}

void setFullDisplay(const char * str) {
	int len = 0;
	for(const char * p = str; *p; len++, p++) {};

	for(int d = 0; d < DISPLAYS && d*4 < len; d++) 
		setDisplay(d, str + d * 4);
}

