#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "iocompat.h"
#include "pins.h"

#include "buttons50229.h"

void init50229_buttons(void)
{
	// All in HiZ mode (to prevent shortcircuit when user presses two buttons).
	INPUT(KEY_R0);
	INPUT(KEY_R1);
	INPUT(KEY_R2);
	INPUT(KEY_R3);

	// Make all the rows low - so that the buttons are seen
	// when pressed.
	//
        PORTB &= ~KEY_R_MASK; 

	INPUT(KEY_C0);
	INPUT(KEY_C1);
	INPUT(KEY_C2);
	INPUT(KEY_C3);

	// enable pull ups -- so we see a '1' when they are not
	// pressed; and the value of the row when pressed.
	//
	SET(KEY_C0, 1); 
	SET(KEY_C1, 1);
	SET(KEY_C2, 1);
	SET(KEY_C3, 1);
}

// we cannot use any interrupts - as those are ony wired to port A and C.
unsigned char butt_scan()
{
	unsigned char col = 255;
	unsigned char row = 255;

	// Put the 0's of the output onto all wires to see if
	// anything is pressed.
	//
	DDR(KEY_R) |= KEY_R_MASK;
	_delay_us(2);
	unsigned char v = PIN(KEY_C) & KEY_C_MASK;

	if (KEY_C_MASK == v) 
		goto check_specials;

	// Scan for the pressed key (in a very naive way; this should
	// be a binary search, etc).
	//

	for(unsigned i = 0; i < KEY_COLS; i++) {
		if ((v & (1 <<(KEY_C_FROM + i))) == 0) { 
			col = i;
			break;
		}
	};

	// one by one - make them low; leaving the others in HiZ (input) state
	// to prvent shortcircuits.
	//
	for(unsigned i = 0; i < KEY_ROWS; i++) {
		DDR(KEY_R)= (DDR(KEY_R)& ~KEY_R_MASK) |  (1<<(KEY_R_FROM + i));

                _delay_us(2);
		if (!(PIN(KEY_C) & (1<<(KEY_C_FROM + col)))) {
			row = i;
			break;
		};
	};

	if (row < KEY_ROWS && col < KEY_COLS)
		return row * KEY_COLS + (KEY_ROWS - col -1);

check_specials:
	DDR(KEY_R) &= ~KEY_R_MASK;

	// check the special button wired between col 2 and col 3.
	//
	SET(KEY_C3,0);
	OUTPUT(KEY_C3); /* after disabling pull-up/setting low */

	_delay_us(2);
	v = READ(KEY_C2);

	INPUT(KEY_C3); 
	SET(KEY_C3,1);  /* after setting back to input! (prevent shorts) */

	if (v == 0)
		return 16;

	return 255;
};


const char * butt_scan2label(unsigned char at) {
	static const char *_labels[KEY_ROWS][KEY_COLS] = {
		{ "1", "2", "3", "3", },
	        { "CLR", "18R", "18L", "36L", },
		{ "06", "36R", "27", "22", },
		{ "RD06", "RVR", "WIS", "UP" },
	};
	if (at < 15)
		return _labels[ at / KEY_COLS][ at % KEY_COLS ];
	if (at == 16)
		return "DWN";
	return "";
}
