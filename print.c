#include "print.h"

void printPgmString(const char *s)
{
  char c;
  while ((c = pgm_read_byte_near(s++)))
    serial_write(c);
}

void printString(const char *s)
{
  while (*s)
    serial_write(*s++);
}

void dPrintString(const char *s)
{
  if (DEBUG)
    while (*s)
      serial_write(*s++);
}
