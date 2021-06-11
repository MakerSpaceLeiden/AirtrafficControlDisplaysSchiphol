#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "iocompat.h"	
#include "pins.h"

static void _sendByte(unsigned char addr, unsigned char val) {
	HIGH(PD44_CE1);
	HIGH(PD44_RD);
	HIGH(PD44_WR);

	SET(PD44_A0, ((addr >> 0) & 1));
	SET(PD44_A1, ((addr >> 1) & 1));
	SET(PD44_A2, ((addr >> 2) & 1));


        PD44_DATA = val;

	LOW(PD44_WR);
	_delay_ms(1);
	HIGH(PD44_WR);
	_delay_ms(1);

	LOW(PD44_RD);
	LOW(PD44_CE1);
}

void pd44_init(void) {
	OUTPUT(PD44_WR);
	OUTPUT(PD44_CE1);

	OUTPUT(PD44_RD);
	LOW(PD44_RD);

	OUTPUT(PD44_A2);
	OUTPUT(PD44_A1);
	OUTPUT(PD44_A0);

	OUTPUT(PD44_D0);
	OUTPUT(PD44_D1);
	OUTPUT(PD44_D2);
	OUTPUT(PD44_D3);
	OUTPUT(PD44_D4);
	OUTPUT(PD44_D5);
	OUTPUT(PD44_D6);
	OUTPUT(PD44_D7);

	OUTPUT(PD44_RST);
}

void pd44_sendByte(unsigned char addr, unsigned char val) {
	_sendByte(addr | 4, val);
}

void pd44_sendChar(unsigned char addr, char c) {

	if (c >= '0' && c <='9') 
		c = 0x30 + c - '0';
	else if (c >= '@' && c <='Z') 
		c = 0x40 + c - '@';
	else if (c >= 'a' && c <='z') 
		c = 0x61 + c - 'a';
        else if (c == ' ')
		c = 0x20;
        else
                c = 0x7F;

	_sendByte(addr | 4, c);
}

void pd44_sendCtrl(unsigned char addr, unsigned char val) {
	pd44_sendByte(addr & ~4, val);
}

void pd44_reset(void) {
	LOW(PD44_RST);
	HIGH(PD44_RST);
}

void pd44_cls(void) {
	pd44_sendCtrl(0, 0x80);
}

void pd44_brigthness(unsigned char val) {
	pd44_sendCtrl(0 & ~4, val & 0x7);
}

void pd44_lamptest(unsigned char val) {
	pd44_sendCtrl(0 & ~4, val & 0x40);
}