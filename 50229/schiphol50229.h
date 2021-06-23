#ifndef _H_S50229
#define _H_S50229

#define DISPLAYS 16
#define NUMLEDS 15

void init50229(void);
void select50229(unsigned char display);
void setDisplay(int display, char * str);
void setFullDisplay(char * str);
void writeLED(unsigned char LED, unsigned char value); /* input LED (LED number) and 1 (on) or 0 (off) */
void writeLEDs(unsigned char col, unsigned char rows); /* input col 0,1,2 and rows (0-0x1F) to set one or more LED's in a row */
unsigned char readLED(unsigned char LED);              /* returns LED status */
unsigned char keyscan50229();                          /* returns button number */

#endif
