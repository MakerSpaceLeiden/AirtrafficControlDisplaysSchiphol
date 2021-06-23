#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "iocompat.h"	

#include "pins.h"
#include "pd44.h"
#include "serial.h"

#include "pd50229.h"
#include "leds50229.h"
#include "buttons50229.h"

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

    pd44_init();
    init50229();
    init50229_leds();
    init50229_buttons();

	/* Clear displays first */
	for(int displ = 0; displ < DISPLAYS; displ++) { 
        select50229(displ);
        pd44_cls();
    }
	for(int LED = 0; LED < LEDS; LED++) { 
		led_set(LED,1);
		_delay_ms(100);
    }
    
	for(int LED = 0; LED < LEDS; LED++) { 
		led_set(LED,0);
		_delay_ms(100);
    }

    for(int displ = 0; displ < DISPLAYS; displ++) {
	char * str = "    ";
	str[1] = displ / 10 + '0';
	str[2] = displ % 10 + '0';
        select50229(displ);
        pd44_brigthness(3);
	setDisplay(displ, str);
    } 
	//for(;;){};
    _delay_ms(500);
   
   for(int displ = 0; displ < DISPLAYS; displ++) {
        select50229(displ);
        pd44_brigthness(3);
		pd44_sendChar(3, 'A' );
		pd44_sendChar(2, 'B' );
		pd44_sendChar(1, 'C' );
		pd44_sendChar(0, 'D' );
    };
   
   //for(;;){};
   _delay_ms(500);
   
        
    //char str[] = "  Makerspace        Leiden            all your bases are belong to us";
    //char str[] = "1234567890abcdef"; /* simpler test */
    char str[]   = "  Hello World from     MakerSpace Leiden          On 50229 display   ";     
    
    int at = 0;

    for (int i = 0; ; i++) {
        for(int displ = 0; displ < DISPLAYS; displ++) {
		const char * b = butt_scan();
		if (b) {
			setDisplay(0,b);
			 _delay_ms(100);
		};
			select50229(displ);
			pd44_sendChar(3, str[ ( displ*4 + i + 0 ) % (sizeof(str)-1) ]);
			pd44_sendChar(2, str[ ( displ*4 + i + 1 ) % (sizeof(str)-1) ]);
			pd44_sendChar(1, str[ ( displ*4 + i + 2 ) % (sizeof(str)-1) ]);
			pd44_sendChar(0, str[ ( displ*4 + i + 3 ) % (sizeof(str)-1) ]);
		};
		//for(;;){};
        _delay_ms(100); 
        led_set(i%LEDS,i%2);
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
