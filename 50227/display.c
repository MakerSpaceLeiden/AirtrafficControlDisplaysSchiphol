#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "iocompat.h"	

#include "pins.h"
#include "pd44.h"
#include "serial.h"

#include "pd50227.h"
#include "leds50227.h"
#include "buttons50227.h"

char brightness = 3;

ISR (TIMER1_OVF_vect)	
{
}

void update_brightness(){
    for (int displ = 0; displ < DISPLAYS; displ++) {
        select50227(displ);
        pd44_brigthness(brightness);
    };
}

void read_keys(){
	// unsigned char key;
	unsigned char at = butt_scan();
	if (at == 255)
		return;

	const char * but = butt_scan2label(at);
	setDisplay(0, but);

	if(at == 16 &&  brightness>1) {
		brightness--;
		update_brightness();
	}
	if(at ==15 && brightness<3) {
		brightness++;
		update_brightness();
	}
	led_toggle(at);
	_delay_ms(200);	
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
    init50227();
    init50227_leds();
    init50227_buttons();

	/* Clear displays first */
	for(int displ = 0; displ < DISPLAYS; displ++) { 
        select50227(displ);
        pd44_cls();
    }
	for(int LED = 0; LED < NUMLEDS; LED++) { 
		led_set(LED,1);
		_delay_ms(100);
    }
    
	for(int LED = 0; LED < NUMLEDS; LED++) { 
		led_set(LED,0);
		_delay_ms(100);
    }
		_delay_ms(1000);

    for(int displ = 0; displ < DISPLAYS; displ++) {
	char * str = "    ";
	str[1] = displ / 10 + '0';
	str[2] = displ % 10 + '0';
        select50227(displ);
        pd44_brigthness(brightness);
	setDisplay(displ, str);
    } 
    _delay_ms(2500);
  
#if 0 
   for(int displ = 0; displ < DISPLAYS; displ++) {
        select50227(displ);
        pd44_brigthness(brightness);
		pd44_sendChar(3, 'A' );
		pd44_sendChar(2, 'B' );
		pd44_sendChar(1, 'C' );
		pd44_sendChar(0, 'D' );
    };
  #endif 
   _delay_ms(2500);
        
    //char str[] = "  Makerspace        Leiden            all your bases are belong to us";
    //char str[] = "1234567890abcdef"; /* simpler test */
    char str[]   = "  Hello World from     MakerSpace Leiden          On 50227 display   ";     
    
    int at = 0;

    for (int i = 0; ; i++) {
        for(int displ = 0; displ < DISPLAYS; displ++) {
        	read_keys();
			select50227(displ);
			pd44_sendChar(3, str[ ( displ*4 + i + 0 ) % (sizeof(str)-1) ]);
			pd44_sendChar(2, str[ ( displ*4 + i + 1 ) % (sizeof(str)-1) ]);
			pd44_sendChar(1, str[ ( displ*4 + i + 2 ) % (sizeof(str)-1) ]);
			pd44_sendChar(0, str[ ( displ*4 + i + 3 ) % (sizeof(str)-1) ]);
		};
		//for(;;){};
        _delay_ms(100); 
        //led_set(i%NUMLEDS,i%2);
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
