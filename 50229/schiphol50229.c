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
	OUTPUT(MPLEX_A);
	OUTPUT(MPLEX_B);
	OUTPUT(MPLEX_C);
	OUTPUT(MPLEX_D);
}

void select50229(unsigned char c) {
	if(c < 8) {
		SET(MPLEX_A,(c>>0)&1);
		SET(MPLEX_B,(c>>1)&1);
		SET(MPLEX_C,(c>>2)&1);
	} else {
		SET(MPLEX_C,1);
		SET(MPLEX_D,(c>>2)&1);
	};
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
