#ifndef _H_S50229
#define _H_S50229

#define DISPLAYS 16
#define NUMLEDS 15

void init50229(void);

void select50229(unsigned char display);
void setDisplay(int display, const char * str);
void setFullDisplay(const char * str);

unsigned char keyscan50229();                          /* returns button number */

#endif
