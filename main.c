#include "blinz.h"

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
