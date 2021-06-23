#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "iocompat.h"	
#include "pins.h"

void _sendByte(unsigned char addr, unsigned char val) {
	HIGH(PD44_RD);
	HIGH(PD44_WR);

	SET(PD44_A0, ((addr >> 0) & 1));
	SET(PD44_A1, ((addr >> 1) & 1));
	SET(PD44_A2, ((addr >> 2) & 1));

        PD44_DATA = val;

	LOW(PD44_WR);
	HIGH(PD44_WR);

	LOW(PD44_RD);
}

void pd44_init(void) {
	OUTPUT(PD44_WR);

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
	HIGH(PD44_RST);
}

void inline pd44_sendByte(unsigned char addr, unsigned char val) {
	_sendByte(addr | 4, val);
}

void inline pd44_sendCtrl(unsigned char val) {
	_sendByte(0, val);
}

void pd44_sendChar(unsigned char addr, char c) {

	if (c < ' ' ||  c > 0x7E)
                c = 0x7F;

	_sendByte(addr | 4, c);
}

void pd44_reset(void) {
	LOW(PD44_RST);
	HIGH(PD44_RST);
}

void pd44_cls(void) {
	pd44_sendCtrl(0x80);
}

void pd44_brigthness(unsigned char val) {
	pd44_sendCtrl(val & 0x3);
}

void pd44_lamptest() {
	pd44_sendCtrl(1<<6);
}
