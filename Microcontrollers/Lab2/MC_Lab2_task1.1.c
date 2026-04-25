#include <stdint.h>
#include "inc/tm4c1294ncpt.h"

// --- Helper Function for Delay ---
// This fixes the "loop outside main" issue.
// We use 'volatile' to stop the compiler from deleting the loop (Slide 10).
void delay_software(unsigned long loops) {
    volatile unsigned long i;
    for (i = 0; i < loops; i++) {
        // Empty body - just burning CPU cycles
    }
}

int main(void) {
    // ==========================================
    // 1. SETUP CODE
    // ==========================================
    
    // A. Enable Clocks (Port L and M)
    SYSCTL_RCGCGPIO_R |= (1 << 10) | (1 << 11); 

    // B. Wait for peripherals to be ready
    while( (SYSCTL_PRGPIO_R & ((1<<10) | (1<<11))) == 0 );

    // C. Configure Port L (Pin 0) as Sync INPUT
    GPIO_PORTL_DEN_R |= (1 << 0);    
    GPIO_PORTL_DIR_R &= ~(1 << 0);   
    GPIO_PORTL_AFSEL_R &= ~(1 << 0); 

    // D. Configure Port M (Pins 0-7) as LED OUTPUTS
    GPIO_PORTM_DEN_R |= 0xFF;        
    GPIO_PORTM_DIR_R |= 0xFF;       
    GPIO_PORTM_AFSEL_R &= ~0xFF;     

    // ==========================================
    // 2. TIMING VARIABLES
    // ==========================================
    unsigned long POSITION_DELAY = 100000; // Tweak this to move bar L/R
    unsigned long WIDTH_DELAY = 5000;      // Tweak this for thickness

    while(1) {
        // --- STEP 1: Wait for Left Swing to Start ---
        // Wait for PL0 Rising Edge (0 -> 1)
        while( (GPIO_PORTL_DATA_R & 0x01) == 0 ); 

        // --- STEP 2: Draw on Left Swing ---
        delay_software(POSITION_DELAY);      // Move to position
        GPIO_PORTM_DATA_R = 0xFF;            // LEDs ON
        delay_software(WIDTH_DELAY);         // Draw Width
        GPIO_PORTM_DATA_R = 0x00;            // LEDs OFF

        // --- STEP 3: Wait for Right Swing to Start ---
        // Wait for PL0 Falling Edge (1 -> 0)
        while( (GPIO_PORTL_DATA_R & 0x01) != 0 );

        // --- STEP 4: Draw on Right Swing ---
        delay_software(POSITION_DELAY);      // Move to position
        GPIO_PORTM_DATA_R = 0xFF;            // LEDs ON
        delay_software(WIDTH_DELAY);         // Draw Width
        GPIO_PORTM_DATA_R = 0x00;            // LEDs OFF
    }
}
