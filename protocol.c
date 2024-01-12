#include "protocol.h"

void print_welcome_message()
{
    printPgmString(PSTR("Blinz started!"));
    printPgmString(PSTR("\r\n"));
}

void protocol_main_loop()
{
    static char line[LINE_BUFFER_SIZE];
    uint8_t c = 0;
    uint8_t char_counter = 0;

    for (;;)
    {
        while ((c = serial_read()) != SERIAL_NO_DATA)
        {
            if ((c == '\n') || (c == '\r'))
            {                           // End of line reached
                line[char_counter] = 0; // Set string termination character.
                // printPgmString(PSTR("Ahoj jak se mas?"));
                printString(line);
                printPgmString(PSTR("\n"));
                char_counter = 0;
            }
            else
            { // still not reached end of line
                if (char_counter >= (LINE_BUFFER_SIZE - 1))
                {
                    // Detect line buffer overflow. Report error and reset line buffer.
                    char_counter = 0;
                    // printPgmString(PSTR("\r\n"));
                }
                else if (c >= 'a' && c <= 'z')
                { // Upcase lowercase
                    line[char_counter++] = c - 'a' + 'A';
                }
                else
                {
                    LED_PORT ^= (1 << LED_BIT);
                    line[char_counter++] = c;
                }
            }
        }
    }
}