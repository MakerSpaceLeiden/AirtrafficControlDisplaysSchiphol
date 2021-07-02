#ifndef _H_LEDS
#define _H_LEDS

#include "pins.h" // for NUMLEDS define

void init50227_leds(void);

void leds_clear(void);
void leds_update(void);
	
void led_set(unsigned char at, unsigned char onoff);
unsigned char led_get(unsigned char at);
void led_toggle(unsigned char at);


#endif
