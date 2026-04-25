#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>

#define MAXSIZE 20

char buf[MAXSIZE];
int wt = 0;

/* ---------- PORT P (UART6) ---------- */
void config_port(void) {
    SYSCTL_RCGCGPIO_R |= 0x02000;     // Port P clock
    wt++;

    GPIO_PORTP_DEN_R   |= 0x03;       // PP0, PP1 digital
    GPIO_PORTP_DIR_R   |= 0x02;       // PP1 output, PP0 input
    GPIO_PORTP_AFSEL_R |= 0x03;       // Alternate function
    GPIO_PORTP_PCTL_R  &= ~0xFF;
    GPIO_PORTP_PCTL_R  |= 0x11;       // UART6 RX/TX
}

/* ---------- UART6 ---------- */
void config_uart(void) {
    SYSCTL_RCGCUART_R |= 0x40;        // UART6 clock
    wt++;

    UART6_CTL_R &= ~0x01;             // Disable UART

    UART6_IBRD_R = 8;                 // 115200 baud
    UART6_FBRD_R = 44;
    UART6_LCRH_R = 0x60;              // 8N1
    UART6_CTL_R  = 0x0301;            // RXE + TXE + UARTEN
}

/* ---------- UART TX ---------- */
void uart_tx(char c) {
    while (UART6_FR_R & 0x20);
    UART6_DR_R = c;
}

/* ---------- UART RX ---------- */
char uart_rx(void) {
    while (UART6_FR_R & 0x10);
    return UART6_DR_R & 0xFF;
}

/* ---------- MAIN ---------- */
void main(void) {
    int i;
    char c;

    config_port();
    config_uart();

    while (1) {

        uart_tx('\r');
        uart_tx('\n');
        uart_tx('>');

        i = 0;

        while (1) {
            c = uart_rx();

            if (c == 0x0D || i == MAXSIZE - 1)
                break;

            buf[i++] = c;
        }

        buf[i] = '\0';
        printf("%s", buf);
    }
}
