#include <stdint.h>
#include <stdio.h>
#include "inc/tm4c1294ncpdt.h"

// =========================================================
// GLOBAL VARIABLES (must be volatile because used in ISRs)
// =========================================================
volatile uint32_t start_time = 0;
volatile uint32_t end_time = 0;
volatile uint32_t pulse_width = 0;
volatile uint32_t distance_cm = 0;

volatile uint8_t echo_state = 0;        // 0=rising, 1=falling
volatile uint8_t sync_falling = 0;

volatile uint32_t led_timer_ticks = 0;
volatile uint32_t display_ticks = 0;

// =========================================================
// TIMER1A ISR — Generate Ultrasonic Trigger Pulse
// =========================================================
void Timer1A_Handler(void)
{
    TIMER1_ICR_R = 1;    // Clear timeout interrupt

    // 10us trigger pulse on PD0
    GPIO_PORTD_AHB_DATA_R |= (1<<0);
    int i;
    for(i=0; i<30; i++);  // small delay (~10µs)
    GPIO_PORTD_AHB_DATA_R &= ~(1<<0);
}

// =========================================================
// TIMER0B ISR — Measure Echo Pulse Width Using Capture Mode
// =========================================================
void Timer0B_Handler(void)
{
    TIMER0_ICR_R = (1<<10); // Clear capture event interrupt

    if (echo_state == 0)
    {
        start_time = TIMER0_TBR_R; // Rising edge timestamp
        echo_state = 1;
    }
    else
    {
        end_time = TIMER0_TBR_R;   // Falling edge timestamp
        pulse_width = (start_time - end_time) & 0xFFFF;
        distance_cm = pulse_width / 940;  // Calibration
        echo_state = 0;
    }
}

// =========================================================
// GPIO Port L ISR — Detect SYNC Falling Edge
// =========================================================
void GPIOPortL_Handler(void)
{
    GPIO_PORTL_ICR_R = 1;  // Clear interrupt flag
    sync_falling = 1;      // Mark new sweep cycle
}

// =========================================================
// TIMER2A ISR — Pendulum LED Display Logic
// =========================================================
void Timer2A_Handler(void)
{
    TIMER2_ICR_R = 1;  // Clear timeout flag

    // Falling edge → prepare new LED bar duration
    if(sync_falling)
    {
        display_ticks = distance_cm * 20;  // scaling factor
        led_timer_ticks = 0;
        sync_falling = 0;
    }

    // Increment tick counter each ISR call
    led_timer_ticks++;

    // LED ON while tick is smaller than display width
    if (led_timer_ticks < display_ticks)
        GPIO_PORTM_DATA_R = 0x18;   // LED bar (2 LEDs)
    else
        GPIO_PORTM_DATA_R = 0x00;   // OFF
}

// =========================================================
// MAIN INITIALIZATION + MAIN LOOP
// =========================================================
int main(void)
{
    // Enable GPIO + Timer Clocks
    SYSCTL_RCGCGPIO_R |= (1<<3) | (1<<10) | (1<<11); // Ports D, L, M
    SYSCTL_RCGCTIMER_R |= (1<<0) | (1<<1) | (1<<2);  // Timers 0,1,2

    while(!(SYSCTL_PRGPIO_R & (1<<3))); // Port D ready

    // ---------------------------
    // PORT M — LED OUTPUTS
    // ---------------------------
    GPIO_PORTM_DEN_R = 0xFF;
    GPIO_PORTM_DIR_R = 0xFF;

    // ---------------------------
    // PORT L — SYNC INPUT + Interrupt
    // ---------------------------
    GPIO_PORTL_DEN_R |= 1;      // PL0
    GPIO_PORTL_DIR_R &= ~1;

    GPIO_PORTL_IS_R &= ~1;      // Edge sensitive
    GPIO_PORTL_IBE_R |= 1;      // Both edges
    GPIO_PORTL_IM_R |= 1;       // Enable interrupt

    NVIC_EN1_R |= (1<<21);      // NVIC IRQ 53 for Port L (53-32 = 21)

    // ---------------------------
    // PORT D — Ultrasonic Sensor
    // ---------------------------
    GPIO_PORTD_AHB_DIR_R |= (1<<0);    // PD0 = Trigger Output
    GPIO_PORTD_AHB_DEN_R |= (1<<0);

    GPIO_PORTD_AHB_DIR_R &= ~(1<<1);   // PD1 = Echo Input
    GPIO_PORTD_AHB_DEN_R |= (1<<1);
    GPIO_PORTD_AHB_AFSEL_R |= (1<<1);
    GPIO_PORTD_AHB_PCTL_R &= ~0xF0;
    GPIO_PORTD_AHB_PCTL_R |= 0x30;     // PD1 -> T0CCP1 (Timer0B Capture)

    // ---------------------------
    // TIMER1A — Trigger Generator
    // ---------------------------
    TIMER1_CTL_R &= ~1;
    TIMER1_CFG_R = 0;
    TIMER1_TAMR_R = 0x02;       // Periodic
    TIMER1_TAILR_R = 1600000;   // 100 ms @ 16MHz
    TIMER1_IMR_R |= 1;
    NVIC_EN0_R |= (1<<21);      // IRQ 21
    TIMER1_CTL_R |= 1;

    // ---------------------------
    // TIMER0B — Echo Measurement
    // ---------------------------
    TIMER0_CTL_R &= ~(1<<8);
    TIMER0_CFG_R = 0x04;       // 16-bit
    TIMER0_TBMR_R = 0x07;      // Capture, Edge-time
    TIMER0_TBILR_R = 0xFFFF;
    TIMER0_TBPR_R = 0xFF;
    TIMER0_CTL_R |= (3<<10);   // Both edges
    TIMER0_IMR_R |= (1<<10);   // Capture event interrupt
    NVIC_EN0_R |= (1<<20);     // IRQ 20
    TIMER0_CTL_R |= (1<<8);

    // ---------------------------
    // TIMER2A — Pendulum LED Control
    // ---------------------------
    TIMER2_CTL_R &= ~1;
    TIMER2_CFG_R = 0;
    TIMER2_TAMR_R = 0x02;       // Periodic
    TIMER2_TAILR_R = 1600;      // ~100 µs interval
    TIMER2_IMR_R |= 1;
    NVIC_EN0_R |= (1<<23);      // IRQ 23
    TIMER2_CTL_R |= 1;

    // ---------------------------
    // MAIN LOOP — NOTHING HERE!
    // (ALL WORK IS INTERRUPT-DRIVEN)
    // ---------------------------
    while(1)
    {
        // Do nothing — all timing is in interrupts
    }
}
