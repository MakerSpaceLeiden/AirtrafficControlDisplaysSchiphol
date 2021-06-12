
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "iocompat.h"

#include "pins.h"
#include "serial.h"

#define UBR 51 /* 9600 - 8N1 defaults*/

void UART_init() {
    UCSR0A = 0;
    UCSR0B = (1 << 4) + (1 << 3); // RX/TX on.
#ifdef UBRR0H
    UBRR0H = (UBR >> 8) & 0x0F;
    UBRR0L = (UBR >> 0) & 0xFF;
#else
    UBRRH = (UBR >> 8) & 0x0F;
    UBRR0 = (UBR >> 0) & 0xFF;
#endif

}

void inline UART_send_char(unsigned char c) {
    /* wait for the register to clear */
    while ((UCSR0A & (1 << 5)) == 0) {};
    UDR0 = c;
}

void UART_send(char *str) {
    for (char *p = str; *p; p++) UART_send_char(*p);
}

int UART_get(void) {
    if (UCSR0A & (1 << 7))
        return UDR0;
    return NODATA;
};

