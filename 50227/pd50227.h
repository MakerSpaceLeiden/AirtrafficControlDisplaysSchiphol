#ifndef _H_S50227
#define _H_S50227

#define DISPLAYS 6

void init50227(void);

void select50227(unsigned char display);
void setDisplay(int display, const char * str);
void setFullDisplay(const char * str);

unsigned char keyscan50227();                          /* returns button number */

#endif
