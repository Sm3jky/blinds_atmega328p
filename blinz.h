// Main blitz include file

#ifndef blinz
#define blinz

// Grbl versioning system
#define BLINZ_VERSION "0.1"
#define BLINZ_VERSION_BUILD "20240112"

// Define standard libraries used by Blinz
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <math.h>
#include <inttypes.h>    
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Define the Blinz system include files
#include "config.h"
#include "cpu_map/atmega328p.h"
#include "serial.h"

#endif