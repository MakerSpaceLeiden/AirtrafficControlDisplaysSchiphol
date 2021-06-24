#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "iocompat.h"
#include "pins.h"

#include "buttons50229.h"

static const char *_labels[KEY_ROWS][KEY_COLS] = {
	{ "4",  "3",  "2",  "1"    },		// empty rows buttons
	{ "36L","18L","18R","CLR"  }, 
	{ "22", "27", "36R","06"   },
	{ "UP", "WIS","RVR","RD06" },
};

void 
init50229_buttons(void)
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
const char * butt_scan()
{
	// Put the 0's of the output onto all wires to see if
	// anything is pressed.
	//
	DDR(KEY_R) |= KEY_R_MASK;
	unsigned char v = KEY_C & KEY_C_MASK;

	if (KEY_C_MASK == v)
		return 0;

	// Scan for the pressed key (in a very naive way; this should
	// be a binary search, etc).
	//
	unsigned char col = 255;

	for(unsigned i = 0; i < KEY_COLS; i++) {
		if ((v & (1 <<(KEY_C_FROM + i))) == 0) { 
			col = i;
			break;
		}
	};
	unsigned char row = 255;

	// one by one - make them low; leaving the others in HiZ (input) state
	// to prvent shortcircuits.
	//
	for(unsigned i = 0; i < KEY_ROWS; i++) {
		DDR(KEY_R)= (DDR(KEY_R)& ~KEY_R_MASK) |  (1<<(KEY_R_FROM + i));

                _delay_us(2);
		if (!(KEY_C & (1<<(KEY_C_FROM + col)))) {
			row = i;
			break;
		};
	};
	if (row == 255 || col == 255)
		return 0;

	return _labels[row][col];
};

