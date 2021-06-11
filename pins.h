/* for the 50228 boards; with 6 displays of the type PD4435.
 * See https://wiki.makerspaceleiden.nl/mediawiki/index.php/RadarBoardsSchipholControl
 */
#ifndef _H_PD_PINS
#define _H_PD_PINS

#include <avr/iom161.h>

#define _SET(type,name,bit)          type ## name  |= _BV(bit)    
#define _CLEAR(type,name,bit)        type ## name  &= ~ _BV(bit)        
#define _TOGGLE(type,name,bit)       type ## name  ^= _BV(bit)    
#define _GET(type,name,bit)          ((type ## name >> bit) &  1)
#define _PUT(type,name,bit,value)    type ## name = ( type ## name & ( ~ _BV(bit)) ) | ( ( 1 & (unsigned char)value ) << bit )

#define OUTPUT(pin)         _SET(DDR,pin)    
#define INPUT(pin)          _CLEAR(DDR,pin)    
#define HIGH(pin)           _SET(PORT,pin)
#define LOW(pin)            _CLEAR(PORT,pin)    
#define SET(pin,value)      _PUT(PORT,pin,value)
#define TOGGLE(pin)         _TOGGLE(PORT,pin)    
#define READ(pin)           _GET(PIN,pin)

/* PD4435 display */

#define PD44_CE1      C,6
#define PD44_RST      E,2

#define PD44_WR      D,7
#define PD44_RD      D,6

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

/* Multiplexer address lines (SNx4HC138) -- see PD datasheet for
 * how this is used to break out to (here 6) displays.
 */
#define MPLEX_A      C,3 // C,3
#define MPLEX_B      C,4 // C,4
#define MPLEX_C      C,5 // C,5

#endif
