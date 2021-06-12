
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "iocompat.h"

#include "pins.h"
#include "pd44.h"
#include "schiphol50228.h"


volatile _Bool d_pressed, u_pressed;
char test_b[20];
int test_int = 0;
char brightness = 3;

ISR (TIMER1_OVF_vect) {
}

ISR(INT0_vect){
    u_pressed = 1;

}

ISR(INT1_vect){
    d_pressed = 1;
}

void UART_init() {
    UCSR0A = 0;
    UCSR0B = (1 << 4) + (1 << 3); // RX/TX on.
#define UBR 51 /* 9600 */
    UBRRH = (UBR >> 8) & 0x0F;
    UBRR0 = (UBR >> 0) & 0xFF;
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
    return -1;
};

void BUTTON_init(){
    INPUT(BUTT_UP);
    SET(BUTT_UP, 1); //enable internal pullup
    INPUT(BUTT_DOWN);
    SET(BUTT_DOWN, 1);
    GIMSK = (1 << 7) + (1 << 6); //enable interrupts 0 and 1
    MCUCR &= 0xF0; //clear MCUCR lower nibble
    MCUCR |= 0x0F; //set MCUCR to rising edge
}

void update_brightness(){
    for (int displ = 0; displ < 6; displ++) {
        select50228(displ);
        pd44_brigthness(brightness);
    };
}

int main(void) {
    UART_init();
    UART_send("\n\n\n" __FILE__ " " __DATE__ " " __TIME__ "\n\n");

    BUTTON_init();

    TCCR1A = TIMER1_PWM_INIT;
    TCCR1B |= TIMER1_CLOCKSOURCE;
    OCR = 0;
    DDROC = _BV (OC1);
    TIMSK = _BV (TOIE1);
    sei ();

    init50228();
    pd44_init();

    for (int displ = 0; displ < 6; displ++) {
        char *str = "No  ";
        str[3] = displ + '0';
        select50228(displ);
        pd44_brigthness(brightness);
        setDisplay(displ, str);
    }
    _delay_ms(2000);

    char str[] = "All your bases are belong to us         ";
    int at = 0;

    for (int i = 0; i < 50; i++) {
        for (int displ = 0; displ < 6; displ++) {
            select50228(displ);
            pd44_sendChar(3, str[(displ * 4 + i + 0) % (sizeof(str) - 1)]);
            pd44_sendChar(2, str[(displ * 4 + i + 1) % (sizeof(str) - 1)]);
            pd44_sendChar(1, str[(displ * 4 + i + 2) % (sizeof(str) - 1)]);
            pd44_sendChar(0, str[(displ * 4 + i + 3) % (sizeof(str) - 1)]);
        };
        _delay_ms(100); // sleep_mode();
    };

    UART_send("Ready for input\n");
    for (;;) {
        int c = UART_get();
        if (c > 0 && at < sizeof(str) - 1) {
            if (c == 10) {
                at = 0;
            } else if (c >= ' ') {
                str[at++] = c;
                str[at] = 0;
            };
            setFullDisplay(str);
        };
        if(d_pressed){
            _delay_ms(10);
            if(READ(BUTT_DOWN) == 1){
                if(brightness > 0) {
                    brightness--;
                    update_brightness();
                }
            }

            d_pressed = 0;
        }
        if(u_pressed){
            _delay_ms(10); //debounce
            if(READ(BUTT_UP) == 1){
                if(brightness<3) {
                    brightness++;
                    update_brightness();
                }

            }
            u_pressed=0;

        }
        UART_send("r");
        sprintf(test_b, "%d", brightness);
        UART_send(test_b);
        UART_send("O");
        UART_send("\n");
        _delay_ms(100);
    }
    return (0);
}
