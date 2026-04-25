#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>

#define MAXSIZE 20

char buf[MAXSIZE];
int wt = 0;

/* ---------- UART6 + PORT P ---------- */
void config_uart_port(void) {
    SYSCTL_RCGCGPIO_R |= 0x02000;
    SYSCTL_RCGCUART_R |= 0x40;
    wt++;

    GPIO_PORTP_DEN_R   |= 0x03;
    GPIO_PORTP_DIR_R   |= 0x02;
    GPIO_PORTP_AFSEL_R |= 0x03;
    GPIO_PORTP_PCTL_R  &= ~0xFF;
    GPIO_PORTP_PCTL_R  |= 0x11;

    UART6_CTL_R &= ~0x01;
    UART6_IBRD_R = 8;
    UART6_FBRD_R = 44;
    UART6_LCRH_R = 0x60;
    UART6_CTL_R  = 0x0301;
}

/* ---------- PORT M (LEDs) ---------- */
void config_leds(void) {
    SYSCTL_RCGCGPIO_R |= 0x800;   // Port M
    wt++;

    GPIO_PORTM_DIR_R |= 0x0F;     // PM0–PM3 output
    GPIO_PORTM_DEN_R |= 0x0F;
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

/* ---------- COMMAND DECODER ---------- */
void decode_cmd(char *cmd) {

    if (cmd[0] != 'l' || cmd[1] != 'e' || cmd[2] != 'd')
        return;

    char op  = cmd[3];
    char num = cmd[4];

    if (num < '0' || num > '3')
        return;

    int bit = num - '0';

    if (op == '+')
        GPIO_PORTM_DATA_R |=  (1 << bit);
    else if (op == '-')
        GPIO_PORTM_DATA_R &= ~(1 << bit);
}

/* ---------- MAIN ---------- */
void main(void) {
    int i;
    char c;

    config_uart_port();
    config_leds();

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
        decode_cmd(buf);
    }
}
