#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "iocompat.h"	

#include "pins.h"
#include "pd44.h"
#include "serial.h"
#include "schiphol50229.h"

ISR (TIMER1_OVF_vect)	
{
}

int main (void)
{
    UART_init();
    UART_send("\n\n\n" __FILE__ " " __DATE__ " " __TIME__ "\n\n"); 

    TCCR1A = TIMER1_PWM_INIT;
    TCCR1B |= TIMER1_CLOCKSOURCE;
    OCR = 0;
    DDROC = _BV (OC1);
    TIMSK = _BV (TOIE1);
    sei ();

    init50229();
    pd44_init();

	/* Clear displays first */
	for(int displ = 0; displ < DISPLAYS; displ++) { 
        select50229(displ);
        pd44_cls();
    }

    for(int displ = 0; displ < DISPLAYS; displ++) {
	char * str = "    ";
	str[1] = displ / 10 + '0';
	str[2] = displ % 10 + '0';
        select50229(displ);
        pd44_brigthness(3);
	setDisplay(displ, str);
    } 
	for(;;){};
    _delay_ms(1000);

    char str[] = "  Makerspace        Leiden            all your bases are belong to us";
    int at = 0;

    for (int i = 0; ; i++) {
       for(int displ = 0; displ < DISPLAYS; displ++) {
        select50229(displ);
	pd44_sendChar(3, str[ ( displ*4 + i + 0 ) % (sizeof(str)-1) ]);
	pd44_sendChar(2, str[ ( displ*4 + i + 1 ) % (sizeof(str)-1) ]);
	pd44_sendChar(1, str[ ( displ*4 + i + 2 ) % (sizeof(str)-1) ]);
	pd44_sendChar(0, str[ ( displ*4 + i + 3 ) % (sizeof(str)-1) ]);
        };
	for(;;){};
        _delay_ms(100); 
      };
    UART_send("Ready for input\n"); 
    for(;;) { 
      int c = UART_get();
      if (c>0 && at < sizeof(str) -1) {
		if (c == 10) {
			at = 0;
		} else if (c >= ' ') {
			str[at++] = c;
			str[at] = 0;
		};
		setFullDisplay(str);
	};
    }
    return (0);
}
