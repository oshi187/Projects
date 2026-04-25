//=============================================================================

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "inc/tm4c1294ncpdt.h"

// --- Configuration ---
#define MAXSIZE 25

// --- System Modes ---
typedef enum {
    MODE_SCANNER,
    MODE_MANUAL
} system_mode_t;

// --- Global Variables (volatile for ISR safety) ---
volatile char rx_buffer[MAXSIZE];
volatile uint8_t rx_index = 0;
volatile uint8_t cmd_ready = 0;

volatile system_mode_t system_mode = MODE_SCANNER;
volatile uint8_t manual_led_state = 0x00;

// --- Function Prototypes ---
void config_hardware(void);
void UART6_OutChar(char data);
void UART6_OutString(char *pt);
void process_command(char *cmd);
void UART6_Handler(void);

//=============================================================================

int main(void) {
    config_hardware();

    UART6_OutString("\r\n--- Lab 5: Interrupt-Driven Scanner Hub ---");
    UART6_OutString("\r\nCommands: stat | led+0..3 | led-0..3");
    UART6_OutString("\r\n>");

    uint8_t scanner_pos = 0x01;
    uint8_t direction = 0;
    int d;

    while (1) {

        /* -------- LED CONTROL -------- */
        if (system_mode == MODE_SCANNER) {
            GPIO_PORTM_DATA_R = scanner_pos;

            for (d = 0; d < 400000; d++);   // visual delay

            if (direction == 0) {
                scanner_pos <<= 1;
                if (scanner_pos == 0x08) direction = 1;
            } else {
                scanner_pos >>= 1;
                if (scanner_pos == 0x01) direction = 0;
            }
        }
        else { // MODE_MANUAL
            GPIO_PORTM_DATA_R = manual_led_state;
        }

        /* -------- COMMAND HANDLING -------- */
        if (cmd_ready) {
            process_command((char*)rx_buffer);
            cmd_ready = 0;
            UART6_OutString("\r\n>");
        }
    }
}

//=============================================================================
// UART6 INTERRUPT HANDLER
//=============================================================================
void UART6_Handler(void) {
    char data;

    if (UART6_MIS_R & 0x10) {       // RX interrupt
        UART6_ICR_R = 0x10;         // clear interrupt

        data = UART6_DR_R & 0xFF;
        UART6_OutChar(data);        // echo

        if (data == 0x0D) {         // ENTER key
            rx_buffer[rx_index] = '\0';
            rx_index = 0;
            cmd_ready = 1;
        }
        else if (rx_index < (MAXSIZE - 1)) {
            rx_buffer[rx_index++] = data;
        }
    }
}

//=============================================================================
// HARDWARE INITIALIZATION
//=============================================================================
void config_hardware(void) {

    // Enable clocks (Port P, Port M, UART6)
    SYSCTL_RCGCGPIO_R |= 0x2800;
    SYSCTL_RCGCUART_R |= 0x40;
    while ((SYSCTL_PRGPIO_R & 0x2800) == 0);
    while ((SYSCTL_PRUART_R & 0x40) == 0);

    // Port P: UART6 RX/TX
    GPIO_PORTP_AFSEL_R |= 0x03;
    GPIO_PORTP_DEN_R   |= 0x03;
    GPIO_PORTP_PCTL_R  = (GPIO_PORTP_PCTL_R & 0xFFFFFF00) | 0x11;

    // Port M: LEDs PM0–PM3
    GPIO_PORTM_DIR_R |= 0x0F;
    GPIO_PORTM_DEN_R |= 0x0F;

    // UART6: 115200, 8N1
    UART6_CTL_R &= ~0x01;
    UART6_IBRD_R = 8;
    UART6_FBRD_R = 44;
    UART6_LCRH_R = 0x60;

    // UART RX Interrupt
    UART6_IM_R  |= 0x10;
    UART6_ICR_R = 0x10;

    // NVIC: UART6 = IRQ 59 → EN1 bit 27
    NVIC_EN1_R |= (1 << 27);

    __asm(" CPSIE I "); // global interrupts

    UART6_CTL_R |= 0x0301;
}

//=============================================================================
// UART OUTPUT
//=============================================================================
void UART6_OutChar(char data) {
    while (UART6_FR_R & 0x20);
    UART6_DR_R = data;
}

void UART6_OutString(char *pt) {
    while (*pt) UART6_OutChar(*pt++);
}

//=============================================================================
// COMMAND PROCESSING
//=============================================================================
void process_command(char *cmd) {

    if (strcmp(cmd, "stat") == 0) {
        system_mode = MODE_SCANNER;
        UART6_OutString("\r\n[STAT] Scanner mode active");
        return;
    }

    if (strncmp(cmd, "led", 3) == 0 && strlen(cmd) == 5) {
        char sign = cmd[3];
        char num  = cmd[4];

        if (num < '0' || num > '3') return;

        uint8_t bit = num - '0';
        system_mode = MODE_MANUAL;

        if (sign == '+') manual_led_state |=  (1 << bit);
        if (sign == '-') manual_led_state &= ~(1 << bit);
    }
}
