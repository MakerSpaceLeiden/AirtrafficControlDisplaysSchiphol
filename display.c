#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "iocompat.h"	

#include "pins.h"
#include "pd44.h"
#include "schiphol50228.h"

ISR (TIMER1_OVF_vect)	
{
}

int main (void)
{

    TCCR1A = TIMER1_PWM_INIT;
    TCCR1B |= TIMER1_CLOCKSOURCE;
    OCR = 0;
    DDROC = _BV (OC1);
    TIMSK = _BV (TOIE1);
    sei ();

    init50228();
    pd44_init();

    for(int j = 0; j < 6; j++) {
         select50228(j);
         pd44_brigthness(3);
#if 0
         pd44_lamptest();
	 _delay_ms(500);
         pd44_cls();
	 pd44_sendChar(0, '0' + j);
#endif
    } 

    char str[] = "Hello MSL !                              How are we today ?                   ....";

    for (int i = 0; ; i++) {
      for(int j = 0; j < 6; j++) {
        select50228(j);
	pd44_sendChar(3, str[ ( j*4 + i + 0 ) % (sizeof(str)-1) ]);
	pd44_sendChar(2, str[ ( j*4 + i + 1 ) % (sizeof(str)-1) ]);
	pd44_sendChar(1, str[ ( j*4 + i + 2 ) % (sizeof(str)-1) ]);
	pd44_sendChar(0, str[ ( j*4 + i + 3 ) % (sizeof(str)-1) ]);
      };
      _delay_ms(150); // sleep_mode();
    }

    return (0);
}
