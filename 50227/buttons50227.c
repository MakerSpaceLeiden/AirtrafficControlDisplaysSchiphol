#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "iocompat.h"
#include "pins.h"

#include "buttons50227.h"

void init50227_buttons(void)
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

        SET(KEY_C2,0);
        OUTPUT(KEY_C2); /* after disabling pull-up/setting low */
	SET(KEY_C1,1);
	SET(KEY_C3,1);
        _delay_us(2);
        unsigned char exec = READ(KEY_C1);
        unsigned char l3 = READ(KEY_C3);

        INPUT(KEY_C2); 
        SET(KEY_C2,1);  /* after setting back to input! (prevent shorts) */

	if (exec == 0)
		return 19;
	if (l3 == 0)
		return 20;

	SET(KEY_R1,0);
	OUTPUT(KEY_R1);

	SET(KEY_R0,1);
	SET(KEY_R2,1);
	_delay_us(2);
	unsigned char v06 = READ(KEY_R0); // for '06'
	unsigned char v22 = READ(KEY_R2); // for '22'
	INPUT(KEY_R1); 
	SET(KEY_R0,0);
	SET(KEY_R2,0);

	if (v06 == 0)
		return 16;
	if (v22 == 0)
		return 17;

	SET(KEY_R2,0);
	OUTPUT(KEY_R2);
	SET(KEY_R3,1);
	_delay_us(2);
	unsigned char corr = READ(KEY_R3); // for '22'
	INPUT(KEY_R2); 
	SET(KEY_R3,0);

	if (corr == 0)
		return 18;

	return 255;
};


const char * butt_scan2label(unsigned char at) {
	static const char *_labels[KEY_ROWS][KEY_COLS] = {
		{ "18R", "18L", "09", "HELI", },
	        { "L1", "36L", "36R", "27", },
		{ "R2", "R1", "18C", "24", },
		{ "04", "R3", "L2", "36C" },
	};
	static const char *_collabels[] = {
		"06", "22", "CORR", "EXE", "L3", 
	};

	if (at < 16)
		return _labels[ at / KEY_COLS][ at % KEY_COLS ];

	if (at >= 15 && at < 21)
		return _collabels[ at - 16 ];

	static char out[5];
	out[0] = '0' + at/100;
	out[1] = '0' + (at/10) % 10;
	out[2] = '0' + at % 10;
	out[3] = '?';
	out[4] = 0;
	return out;
}
