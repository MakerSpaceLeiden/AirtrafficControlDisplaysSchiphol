#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "schiphol50228.h"	
#include "iocompat.h"	
#include "pins.h"

void init50228(void) {
	OUTPUT(MPLEX_A);
	OUTPUT(MPLEX_B);
	OUTPUT(MPLEX_C);
}

void select50228(unsigned char c) {
#if 0
	SET(MPLEX_A,(c>>0)&1);
	SET(MPLEX_B,(c>>1)&1);
	SET(MPLEX_C,(c>>2)&1);
#else
	// Visible glitches - hardcode for now
	PORTC = (PORTC & ~((1<<3) + (1<<4) + (1<<5))) | ((c & 0x7) << 3);
#endif
};
