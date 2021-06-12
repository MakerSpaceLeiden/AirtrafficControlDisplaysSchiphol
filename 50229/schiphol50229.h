#ifndef _H_S50229
#define _H_S50229

#define DISPLAYS 16

void init50229(void);
void select50229(unsigned char display);
void setDisplay(int display, char * str);
void setFullDisplay(char * str);

#endif
