#ifndef _H_SERIAL
#define _H_SERIAL

void UART_init();
void UART_send_char(unsigned char c);
void UART_send(char *str);

#define NODATA (-1)
int UART_get(void); 

#endif

