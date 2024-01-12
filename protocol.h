#ifndef protocol_h
#define protocol_h

#include "blinz.h"

#ifndef LINE_BUFFER_SIZE
#define LINE_BUFFER_SIZE 80
#endif

void print_welcome_message();
void protocol_main_loop();

#endif