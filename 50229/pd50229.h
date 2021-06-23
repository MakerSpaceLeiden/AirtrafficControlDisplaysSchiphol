#ifndef _H_S50229
#define _H_S50229

#define DISPLAYS 16
#define LEDS 15

void init50229(void);
void select50229(unsigned char display);
void setDisplay(int display, const char * str);
void setFullDisplay(const char * str);
void setLED(unsigned char LED);
void WriteLED(unsigned char LED, unsigned char value);
void WriteLEDs(unsigned char col, unsigned char rows);

#endif
