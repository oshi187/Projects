#include <stdint.h>
#include "inc/tm4c1294ncpt.h"

// ==========================================
// 1. HELPERS & FONT TABLE
// ==========================================

// Simple Delay
void delay_software(unsigned long loops) {
    volatile unsigned long i;
    for (i = 0; i < loops; i++) {}
}

// Font Table: 10 digits (Rows), 5 Columns each (Bytes)
// 0x01 is the Bottom LED, 0x80 is the Top LED.
const uint8_t FONT[10][5] = {
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}  // 9
};

// Measurement Function (Using Timer0B on PD1)
// Returns distance in CM, or 0 if error.
uint32_t get_distance(void) {
    // 1. Trigger Pulse (PD0) - SWAPPED
    GPIO_PORTD_AHB_DATA_R &= ~(1<<0); 
    delay_software(20);
    GPIO_PORTD_AHB_DATA_R |= (1<<0);  
    delay_software(20);               
    GPIO_PORTD_AHB_DATA_R &= ~(1<<0); 

    // 2. Wait for Rising Edge (Start on Timer B)
    TIMER0_ICR_R = (1<<10); // Clear Capture B Flag (Bit 10)
    
    int safety = 50000; 
    // Wait for CBERIS (Bit 10)
    while((TIMER0_RIS_R & (1<<10)) == 0) {
        safety--;
        if(safety == 0) return 0; // Timeout
    }
    
    uint32_t start = TIMER0_TBR_R; // Read Timer B
    TIMER0_ICR_R = (1<<10);        // Clear Flag

    // 3. Wait for Falling Edge (End on Timer B)
    safety = 50000;
    while((TIMER0_RIS_R & (1<<10)) == 0) {
        safety--;
        if(safety == 0) return 0; // Timeout
    }
    
    uint32_t end = TIMER0_TBR_R; // Read Timer B

    // 4. Calculate
    uint32_t diff = (start - end) & 0xFFFF;
    return (diff / 940); // Calibration for cm
}

// Function to draw a single digit
void draw_digit(int num) {
    if(num > 9) return;
    int col;
    for(col = 0; col < 5; col++) {
        GPIO_PORTM_DATA_R = FONT[num][col]; // Draw Column
        delay_software(1500);               // Width of pixel
    }
    GPIO_PORTM_DATA_R = 0x00; // Turn off
    delay_software(3000);     // Space between digits
}

int main(void) {
    // ==========================================
    // 2. HARDWARE SETUP (Swapped Config)
    // ==========================================
    
    // Enable Clocks
    SYSCTL_RCGCGPIO_R |= (1<<3) | (1<<10) | (1<<11);
    SYSCTL_RCGCTIMER_R |= 0x01;
    while((SYSCTL_PRGPIO_R & (1<<3)) == 0);

    // Port M (LEDs) -> Output
    GPIO_PORTM_DEN_R = 0xFF;
    GPIO_PORTM_DIR_R = 0xFF;

    // Port L (Sync) -> Input
    GPIO_PORTL_DEN_R |= 0x01;
    GPIO_PORTL_DIR_R &= ~0x01;

    // Port D (Sensor) - SWAPPED
    // PD0 Output (Trigger)
    GPIO_PORTD_AHB_DIR_R |= (1<<0);
    GPIO_PORTD_AHB_DEN_R |= (1<<0);
    
    // PD1 Input (Echo - Timer0B)
    GPIO_PORTD_AHB_DIR_R &= ~(1<<1);
    GPIO_PORTD_AHB_DEN_R |= (1<<1);
    GPIO_PORTD_AHB_AFSEL_R |= (1<<1);
    
    // PCTL for PD1 (Bits 4-7) -> Set to 3
    GPIO_PORTD_AHB_PCTL_R &= ~0x000000F0;
    GPIO_PORTD_AHB_PCTL_R |= 0x00000030; // Connect to Timer0B

    // Timer Configuration (Timer 0B)
    TIMER0_CTL_R &= ~(1<<8);     // Disable Timer B
    TIMER0_CFG_R = 0x04;         // 16-bit Split Mode
    
    // Timer B Mode: Capture(3) + Edge(1) + Down(0) -> 0x07
    TIMER0_TBMR_R = 0x07;        
    TIMER0_TBPR_R = 0xFF;        // Prescaler B
    TIMER0_TBILR_R = 0xFFFF;     // Max Load B
    
    // Event B: Both Edges (Bits 11:10 -> 0x3)
    TIMER0_CTL_R |= (0x3 << 10); 
    TIMER0_CTL_R |= (1<<8);      // Enable Timer B

    // State Variable
    uint32_t measured_dist = 0;

    // ==========================================
    // 3. MAIN LOOP
    // ==========================================
    while(1) {
        
        // --- PHASE 1: MOVING LEFT (Measurement) ---
        while((GPIO_PORTL_DATA_R & 0x01) == 0);

        uint32_t new_dist = get_distance();
        
        // Simple noise filter
        if(new_dist > 0 && new_dist < 99) {
            measured_dist = new_dist;
        }

        // --- PHASE 2: MOVING RIGHT (Display) ---
        while((GPIO_PORTL_DATA_R & 0x01) != 0);

        // A. Position Delay (Center the text)
        delay_software(80000); 

        // B. Draw Tens Digit
        draw_digit(measured_dist / 10);

        // C. Draw Ones Digit
        draw_digit(measured_dist % 10);
        
        // D. Ensure LEDs are off
        GPIO_PORTM_DATA_R = 0x00;
    }
}
