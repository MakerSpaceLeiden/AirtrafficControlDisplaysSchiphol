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

char brightness = 3;

ISR (TIMER1_OVF_vect)	
{
}

void update_brightness(){
    for (int displ = 0; displ < DISPLAYS; displ++) {
        select50229(displ);
        pd44_brigthness(brightness);
    };
}

void read_keys(){
	unsigned char key;
	key=keyscan50229();
	if(key!=0xFF){
	#if 0 /* to show key number */
		char * str = "    ";
		str[1] = key / 10 + '0';
		str[2] = key % 10 + '0';
		setDisplay(0, str);
	#endif
	if(key==16){
		if(brightness>0) brightness--;
			update_brightness();
		}
	if(key==15){
		if(brightness<3) brightness++;
			update_brightness();
		}
	if(key<NUMLEDS){
		 writeLED(key,!readLED(key)); /* togle led on buttonpress if it is a button with LED*/
	}
	_delay_ms(100);	
	}
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
    writeLEDs(0,0);
    writeLEDs(1,0);
    writeLEDs(2,0);
    
	for(int LED = 0; LED < NUMLEDS; LED++) { 
		writeLED(LED,1);
		_delay_ms(100);
    }
    
	for(int LED = 0; LED < NUMLEDS; LED++) { 
		writeLED(LED,0);
		_delay_ms(100);
    }

    for(int displ = 0; displ < DISPLAYS; displ++) {
	char * str = "    ";
	str[1] = displ / 10 + '0';
	str[2] = displ % 10 + '0';
        select50229(displ);
        pd44_brigthness(brightness);
	setDisplay(displ, str);
    } 
    //for(;;){};
    _delay_ms(1000);
   
   for(int displ = 0; displ < DISPLAYS; displ++) {
        select50229(displ);
        pd44_brigthness(brightness);
		pd44_sendChar(3, 'A' );
		pd44_sendChar(2, 'B' );
		pd44_sendChar(1, 'C' );
		pd44_sendChar(0, 'D' );
    };
   
	//for(;;){};
   _delay_ms(1000);
   
        
    //char str[] = "  Makerspace        Leiden            all your bases are belong to us";
    //char str[] = "1234567890abcdef"; /* simpler test */
    char str[]   = "  Hello World from     MakerSpace Leiden          On 50229 display   ";     
    
    int at = 0;

    for (int i = 0; ; i++) {
        read_keys();
        for(int displ = 0; displ < DISPLAYS; displ++) {
			select50229(displ);
			pd44_sendChar(3, str[ ( displ*4 + i + 0 ) % (sizeof(str)-1) ]);
			pd44_sendChar(2, str[ ( displ*4 + i + 1 ) % (sizeof(str)-1) ]);
			pd44_sendChar(1, str[ ( displ*4 + i + 2 ) % (sizeof(str)-1) ]);
			pd44_sendChar(0, str[ ( displ*4 + i + 3 ) % (sizeof(str)-1) ]);
		};
		//for(;;){};
        _delay_ms(100);
        //writeLED(i%NUMLEDS,i%2); /* automagic blinkenlichten */
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
