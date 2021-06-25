#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "iocompat.h"	
#include "pins.h"	

#include "pd50227.h"	
#include "pd44.h"	

void init50227(void) {
	OUTPUT(DISP_SEL_A0);
	OUTPUT(DISP_SEL_A1);
	OUTPUT(DISP_SEL_A2);

	OUTPUT(DISP_SEL_UPPER2);
	OUTPUT(DISP_SEL_LOWER2);
}

void select50227(unsigned char c) {
	SET(DISP_SEL_A0,(c>>0)&1);
	SET(DISP_SEL_A1,(c>>1)&1);
	SET(DISP_SEL_A2,(c>>2)&1);

	SET(DISP_SEL_LOWER2,0); 
	SET(DISP_SEL_UPPER2,1);
};

void setDisplay(int display, const char * str) {
	select50227(display);
	
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
