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

	select50228(0);
}

void select50228(int c) {
	SET(MPLEX_A,(c>>0)&1);
	SET(MPLEX_B,(c>>1)&1);
	SET(MPLEX_C,(c>>2)&1);
};

