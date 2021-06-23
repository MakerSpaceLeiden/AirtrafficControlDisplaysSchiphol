#ifndef _H_LEDS
#define _H_LEDS

void init50229_leds(void);

void leds_clear(void);
void leds_update(void);
	
void led_set(unsigned char at, unsigned char onoff);

#endif
