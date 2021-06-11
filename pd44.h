#ifndef _H_PD44
#define _H_PD44

void pd44_init(void);

void pd44_sendCtrl(unsigned char val);
void pd44_sendByte(unsigned char addr, unsigned char val);

void pd44_reset(void);
void pd44_cls(void);
void pd44_brigthness(unsigned char val);
void pd44_lamptest();

void pd44_sendChar(unsigned char addr, char c);

#endif
