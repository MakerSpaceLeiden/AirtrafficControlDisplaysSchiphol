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

    for(int j = 0; j < 8; j++) {
         select50228(j);
         pd44_reset();
         pd44_brigthness(3);
    } 
	
    char str[] = "Hello MSL       ";

    for (int i = 0; ; i++) {
      for(int j = 0; j < 8; j++) {
        select50228(j);
	str[10] = j + '0';
	pd44_sendChar(3, str[ ( i + 0 ) % (sizeof(str)-1) ]);
	pd44_sendChar(2, str[ ( i + 1 ) % (sizeof(str)-1) ]);
	pd44_sendChar(1, str[ ( i + 2 ) % (sizeof(str)-1) ]);
	pd44_sendChar(0, str[ ( i + 3 ) % (sizeof(str)-1) ]);
      };
	_delay_ms(1500); 
//        sleep_mode();
    }

    return (0);
}
