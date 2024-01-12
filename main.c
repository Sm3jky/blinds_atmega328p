#include "blinz.h"

ISR(TIMER1_COMPA_vect)
{
    // Toggle stavu LED
    LED_PORT ^= (1 << LED_BIT);
}

int main(void)
{
    serial_init(); // Setup serial baud rate and interrupts
    LED_DDR |= (1 << LED_BIT);
    // // Nastavení režimu CTC (WGM12) a předděliče 1024 (CS12, CS10)
    // TCCR0A |= (1 << WGM01);
    // TCCR0B |= (1 << CS02) | (1 << CS00);

    // // Nastavení hodnoty pro porovnání pro generování přerušení každých 0.25 sekundy (62499 pro 16MHz)
    // OCR0A = 255;

    // // Povolení přerušení pro Timer/Counter 0 Compare Match A
    // TIMSK0 |= (1 << OCIE0A);

    // // Globální povolení přerušení
    sei();

    for (;;)
    {
        // init all, before loop
        printPgmString(PSTR("Blinz started!"));
        printPgmString(PSTR("\r\n"));

        protocol_main_loop(); // loop
    }

    return 0; // not reachable
} // end of main
