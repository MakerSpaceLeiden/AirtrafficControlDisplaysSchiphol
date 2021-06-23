#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "iocompat.h"	
#include "pins.h"	

#include "schiphol50228.h"	
#include "pd44.h"	

void init50228(void) {
	OUTPUT(MPLEX_A);
	OUTPUT(MPLEX_B);
	OUTPUT(MPLEX_C);
	OUTPUT(PD44_CE1);
	HIGH(PD44_CE1);
}

void select50228(unsigned char c) {
#if 0
	SET(MPLEX_A,(c>>0)&1);
	SET(MPLEX_B,(c>>1)&1);
	SET(MPLEX_C,(c>>2)&1);
#else
	// For now - until we understand what causes the glitches every 20-30 seconds.
	// As we can see above as a 0.2-0.4 uSecond spike.
	PORTC = (PORTC & ~((1<<3) + (1<<4) + (1<<5))) | ((c & 0x7) << 3);
#endif
};

void setDisplay(int display, char * str) {
	select50228(display);
	
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

	for(int d = 0; d < 6 && d*4 < len; d++) 
		setDisplay(d, str + d * 4);
}
