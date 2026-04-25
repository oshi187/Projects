#include <stdint.h>
#include "inc/tm4c1294ncpdt.h"

// =====================================================
// Global variables used by ISRs
// =====================================================
volatile uint32_t start_time = 0;
volatile uint32_t end_time = 0;
volatile uint32_t pulse_width = 0;
volatile uint32_t distance_cm = 0;
volatile uint8_t  measurement_done = 0;

// =====================================================
// Simple inline delay inside ISR – just a few CPU cycles
// =====================================================
static inline void short_delay()
{
    for(volatile int i = 0; i < 30; i++);   // ~10 µs at 16 MHz
}

// =====================================================
// Timer1A ISR -- Generates Trigger Pulse
// =====================================================
void Timer1A_Handler(void)
{
    TIMER1_ICR_R = (1<<0); // Clear timeout interrupt

    // Create TRIGGER pulse on PD0
    GPIO_PORTD_AHB_DATA_R |= (1<<0);   // High
    short_delay();                     // ~10 us
    GPIO_PORTD_AHB_DATA_R &= ~(1<<0);  // Low
}

// =====================================================
// Timer0B ISR -- Measures Echo Pulse Width
// =====================================================
void Timer0B_Handler(void)
{
    TIMER0_ICR_R = (1<<10); // Clear capture event

    static uint8_t state = 0;

    if(state == 0)
    {
        start_time = TIMER0_TBR_R;  // Rising edge
        state = 1;
    }
    else
    {
        end_time = TIMER0_TBR_R;    // Falling edge
        pulse_width = (start_time - end_time) & 0xFFFF;
        distance_cm = pulse_width / 940;  // Rough scaling
        measurement_done = 1;
        state = 0;
    }
}

// =====================================================
// Initialization
// =====================================================
void init_GPIO_PD01(void)
{
    SYSCTL_RCGCGPIO_R |= (1<<3);
    while((SYSCTL_PRGPIO_R & (1<<3)) == 0);

    // PD0 = Output (Trigger)
    GPIO_PORTD_AHB_DIR_R |= (1<<0);
    GPIO_PORTD_AHB_DEN_R |= (1<<0);

    // PD1 = Timer0B Capture Input
    GPIO_PORTD_AHB_DIR_R &= ~(1<<1);
    GPIO_PORTD_AHB_DEN_R |= (1<<1);
    GPIO_PORTD_AHB_AFSEL_R |= (1<<1);
    GPIO_PORTD_AHB_PCTL_R &= ~0x000000F0;
    GPIO_PORTD_AHB_PCTL_R |=  0x00000030;  // PD1 = T0CCP1
}

void init_Timer1A_trigger(void)
{
    SYSCTL_RCGCTIMER_R |= (1<<1);
    while(!(SYSCTL_PRTIMER_R & (1<<1)));

    TIMER1_CTL_R &= ~1;         // Disable Timer1A
    TIMER1_CFG_R = 0x0;         // 32-bit mode
    TIMER1_TAMR_R = 0x02;       // Periodic mode
    TIMER1_TAILR_R = 1600000;   // 100 ms at 16 MHz
    TIMER1_IMR_R |= (1<<0);     // Timeout interrupt
    NVIC_EN0_R |= (1<<21);      // Enable IRQ21
    TIMER1_CTL_R |= 1;          // Start Timer1A
}

void init_Timer0B_capture(void)
{
    SYSCTL_RCGCTIMER_R |= (1<<0);
    while(!(SYSCTL_PRTIMER_R & (1<<0)));

    TIMER0_CTL_R &= ~(1<<8);    // Disable Timer0B
    TIMER0_CFG_R = 0x04;        // 16-bit
    TIMER0_TBMR_R = 0x07;       // Capture, edge-time
    TIMER0_TBILR_R = 0xFFFF;
    TIMER0_TBPR_R = 0xFF;

    TIMER0_CTL_R |= (0x3 << 10); // Both edges
    TIMER0_IMR_R |= (1<<10);     // Capture event interrupt

    NVIC_EN0_R |= (1<<20);       // Enable IRQ20

    TIMER0_CTL_R |= (1<<8);      // Enable Timer0B
}

// =====================================================
// MAIN
// =====================================================
int main(void)
{
    init_GPIO_PD01();
    init_Timer1A_trigger();
    init_Timer0B_capture();

    while(1)
    {
        if(measurement_done)
        {
            printf("Distance: %u cm\n", distance_cm);
            measurement_done = 0;
        }
    }
}
