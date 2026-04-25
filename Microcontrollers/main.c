/** Default Project for TM4C1294ncpdt
 *
 * File:    main.c
 * Author:
 * Date:
 * Version:
 *
 */

#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>

/*******************************************************************************************/

#define COUNTER_LIM 10000

/*******************************************************************************************/

void
print_counter()
{
    static int cnt = 0;

    printf("Counter value: %d", cnt++);
}

/*******************************************************************************************/

void
configure_led()
{
    SYSCTL_RCGCGPIO_R       |= SYSCTL_RCGCGPIO_R12; /* switch on clock for Port N*/
    GPIO_PORTN_DEN_R        |= 0x02;                /* digital I/O enable pin PN1 (APB)*/
    GPIO_PORTN_DIR_R        |= 0x02;                /*PortN 1 Output*/
    GPIO_PORTN_DATA_R        = 0x00;                /*PortN Output to zero**/
}

/*******************************************************************************************/



/*******************************************************************************************/

int main(void)
{
    unsigned char ledState = 0;
    int loopIdx            = 0;

    while(1)
    {
        /*Configure LED 1*/
        configure_led();

        /*Print and increment counter value*/
        print_counter();

        /*Toggle LED*/
        if (0x00 == ledState)
            ledState = 0x02;
        else
            ledState = 0x00;

        GPIO_PORTN_DATA_R = ledState;

        /*Delay*/
        for(loopIdx = 0; loopIdx < COUNTER_LIM; loopIdx++);
    }
}
