#include "blinz.h"

// Define led pulse output pins
#define LED_DDR        DDRB
#define LED_PORT       PORTB
#define LED_BIT        5  // Uno Digital Pin 13

int main(void) {
    LED_DDR |= (1<<LED_BIT);

    for (;;) {
        LED_PORT |= (1<<LED_BIT);
        _delay_ms(500);

        LED_PORT &= ~(1<<LED_BIT);
        _delay_ms(500);
    }

    return 0; // not reachable
} // end of main
