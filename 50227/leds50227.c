#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "iocompat.h"
#include "pins.h"

#include "leds50227.h"

// 3 rowdata of 5 _LEDs; 15 in total.
//
static uint16_t	_LEDs = 0;

void 
init50227_leds(void)
{
	OUTPUT(LED_E3);
	OUTPUT(LED_nE2);
	OUTPUT(LED_nE1);
	OUTPUT(LED_R0);
	OUTPUT(LED_R1);
	OUTPUT(LED_R2);
	OUTPUT(LED_R3);
	OUTPUT(LED_R4);
	OUTPUT(LED_SEL0);
	OUTPUT(LED_SEL1);
	OUTPUT(LED_SEL2);

	LOW(LED_R0);
	LOW(LED_R1);
	LOW(LED_R2);
	LOW(LED_R3);
	LOW(LED_R4);

	leds_clear();
}

static void _led_update_col(unsigned char col, unsigned char rowdata)
{
	LOW(LED_nE1);
	LOW(LED_nE2);
	LOW(LED_E3);
	LOW(LED_SEL2);

	SET(LED_SEL0, (col >> 0) & 1);
	SET(LED_SEL1, (col >> 1) & 1);

	SET(LED_R0, (rowdata >> 0) & 1);
	SET(LED_R1, (rowdata >> 1) & 1);
	SET(LED_R2, (rowdata >> 2) & 1);
	SET(LED_R3, (rowdata >> 3) & 1);
	SET(LED_R4, (rowdata >> 4) & 1);

	HIGH(LED_E3);
	LOW(LED_E3);
};

void leds_clear(void)
{
	_LEDs = 0;
	leds_update();
}

void leds_update(void)
{
	for (unsigned char i = 0; i < 3; i++)
		_led_update_col(i, (_LEDs >> (i * 5)) & 0x1F);
}

void led_update(unsigned char at)
{
	int c = at / 5;
	_led_update_col(c, (_LEDs >> (c * 5)) & 0x1F);
}

unsigned char led_get(unsigned char at) 
{
        at %= NUMLEDS;
        return ((_LEDs>>at)&1);
}

void led_set(unsigned char at, unsigned char onoff)
{
	if (onoff)
		_LEDs |= (1 << at);
	else
		_LEDs &= ~(1 << at);

	_led_update_col(at/5, (_LEDs >> (((unsigned char)(at/5)) * 5)) & 0x1F);
}

void led_toggle(unsigned char at)
{
	led_set(at, !led_get(at));
}
