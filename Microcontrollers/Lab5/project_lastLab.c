#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>

#define MAXSIZE 20

/* ---------- GLOBALS ---------- */
volatile char buf[MAXSIZE];
volatile int idx = 0;
volatile int cmd_ready = 0;
int wt = 0;

/* ---------- UART6 + PORT P ---------- */
void config_uart_port(void) {
    SYSCTL_RCGCGPIO_R |= 0x02000;   // enable Port P clock
    SYSCTL_RCGCUART_R |= 0x40;      // enable UART6 clock
    wt++;

    GPIO_PORTP_DEN_R   |= 0x03;     // PP0, PP1 digital enable
    GPIO_PORTP_DIR_R   |= 0x02;     // PP1 TX output
    GPIO_PORTP_AFSEL_R |= 0x03;     // alternate function
    GPIO_PORTP_PCTL_R  &= ~0xFF;
    GPIO_PORTP_PCTL_R  |= 0x11;     // UART6 RX/TX

    UART6_CTL_R &= ~0x01;           // disable UART6
    UART6_IBRD_R = 8;
    UART6_FBRD_R = 44;
    UART6_LCRH_R = 0x60;            // 8N1

    UART6_IM_R  |= 0x10;            // RX interrupt enable
    UART6_CTL_R |= 0x0301;          // RXE | TXE | UARTEN

    NVIC_EN1_R |= (1 << 27);        // UART6 IRQ = 59
}

/* ---------- PORT M (LEDs) ---------- */
void config_leds(void) {
    SYSCTL_RCGCGPIO_R |= 0x800;     // enable Port M clock
    wt++;

    GPIO_PORTM_DIR_R |= 0x0F;       // PM0–PM3 output
    GPIO_PORTM_DEN_R |= 0x0F;
}

/* ---------- UART TX ---------- */
void uart_tx(char c) {
    while (UART6_FR_R & 0x20);      // wait if TX FIFO full
    UART6_DR_R = c;
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

/* ---------- UART6 RX INTERRUPT HANDLER ---------- */
void UART6_Handler(void) {
    char c;

    if (UART6_MIS_R & 0x10) {       // RX interrupt active
        c = UART6_DR_R & 0xFF;

        if (c == 0x0D || idx == MAXSIZE - 1) {
            buf[idx] = '\0';
            idx = 0;
            cmd_ready = 1;
        } else {
            buf[idx++] = c;
        }

        UART6_ICR_R = 0x10;         // clear RX interrupt
    }
}

/* ---------- MAIN ---------- */
void main(void) {

    config_uart_port();
    config_leds();

    __enable_irq();

    uart_tx('\r');
    uart_tx('\n');
    uart_tx('>');

    while (1) {
        if (cmd_ready) {
            printf("%s", buf);
            decode_cmd((char *)buf);

            cmd_ready = 0;

            uart_tx('\r');
            uart_tx('\n');
            uart_tx('>');
        }
    }
}
