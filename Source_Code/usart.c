#include <htc.h>
#include <stdio.h>

// The standard function prototype for putch in XC8
void putch(char byte)  
{
    /* output one byte */
    while(!TXIF)    /* wait until the transmit buffer is empty */
        continue;
    TXREG = byte;   /* transmit the byte */
}

// The standard function prototype for getch in XC8
int getch(void) {
    /* retrieve one byte */
    while(!RCIF)    /* wait until the receive buffer is full */
        continue;
    return RCREG;   /* return the received character */
}

// Echo function if needed
int getch_echo(void) {
    char c;
    c = getch();   /* receive character */
    putch(c);      /* echo character back */
    return c;
}
