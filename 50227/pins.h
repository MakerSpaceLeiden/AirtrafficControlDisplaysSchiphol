/* for the 50227 boards; with 16 displays of the type PD4435 and 2 demuxes. ATMEL161 based.
 *
 * See https://wiki.makerspaceleiden.nl/mediawiki/index.php/RadarBoardsSchipholControl
 */
#ifndef _H_PD_PINS
#define _H_PD_PINS

#include <avr/iom161.h>

#define _MAP(type,name)              type ## name 
#define _SET(type,name,bit)          type ## name  |= _BV(bit)    
#define _CLEAR(type,name,bit)        type ## name  &= ~ _BV(bit)        
#define _TOGGLE(type,name,bit)       type ## name  ^= _BV(bit)    
#define _GET(type,name,bit)          ((type ## name >> bit) &  1)
#define _PUT(type,name,bit,value)    type ## name = ( type ## name & ( ~ _BV(bit)) ) | ( ( 1 & (unsigned char)value ) << bit )

#define DDR(port)	    _MAP(DDR,port)
#define PORT(port)	    _MAP(PORT,port)
#define PIN(port)	    _MAP(PIN,port)
#define OUTPUT(pin)         _SET(DDR,pin)    
#define INPUT(pin)          _CLEAR(DDR,pin)    
#define HIGH(pin)           _SET(PORT,pin)
#define LOW(pin)            _CLEAR(PORT,pin)    
#define SET(pin,value)      _PUT(PORT,pin,value)
#define TOGGLE(pin)         _TOGGLE(PORT,pin)    
#define READ(pin)           _GET(PIN,pin)

/* PD4435 display */

#define PD44_RST      E,2

#define PD44_WR      D,6
#define PD44_RD      D,7

#define PD44_A0      C,0
#define PD44_A1      C,1
#define PD44_A2      C,2

#define PD44_DATA PORTA

#define PD44_D0   A,0
#define PD44_D1   A,1
#define PD44_D2   A,2
#define PD44_D3   A,3
#define PD44_D4   A,4
#define PD44_D5   A,5
#define PD44_D6   A,6
#define PD44_D7   A,7

/* DISPlay_SELect lines.
 * Demultiplexer address lines (SNx4HC138) -- see PD datasheet for
 * how this is used to break out to (here 6) displays.
 */
#define DISP_SEL_A0  C,3 
#define DISP_SEL_A1  C,4
#define DISP_SEL_A2  C,5

/* Display enable lines, also for adressing */ 
#define DISP_SEL_UPPER2  C,6 /* E1 for upper 2 rows */
#define DISP_SEL_LOWER2  C,7 /* E1 for lower 2 rows */

/*
 *  Push buttons on 50227 board are in a key matrix
 *       PD2 PD3 PD4	PD5
 * PB3      UP  WIS	RVR RD 06
 * PB2 	    22  27  36R  06
 * PB1 	    36L 18L 18R  CLR
 * PB0      4   3   2    1 
 * 
 * Down is between PD4 and PD5, Up between PD2 and PB3
 */
#define KEY_ROWS	(4)
#define KEY_COLS	(4)
#define NUMKEYS		(17) //including UP and DOWN

#define KEY_R  		B
#define KEY_R_FROM 	(0)
#define KEY_R_MASK	(0xF << KEY_R_FROM)

#define KEY_R0 B,0
#define KEY_R1 B,1
#define KEY_R2 B,2
#define KEY_R3 B,3

#define KEY_C  		D
#define KEY_C_FROM	(2)
#define KEY_C_MASK 	(0xF << KEY_C_FROM)

#define KEY_C0 D,2
#define KEY_C1 D,3
#define KEY_C2 D,4
#define KEY_C3 D,5
 
 /* LEDs
  * The LED's under the switchcaps are driven trough the 2nd demux and a few flipflops
  */
#define NUMLEDS 15

#define LED_R0 A,0
#define LED_R1 A,1 
#define LED_R2 A,2
#define LED_R3 A,3 
#define LED_R4 A,4
#define LED_SEL0 C,3 
#define LED_SEL1 C,4 
#define LED_SEL2 D,6 

#define LED_nE1 C,6
#define LED_nE2 C,7
#define LED_E3 C,5
 
#endif
