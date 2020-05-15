#include <stdarg.h>
#include <stdio.h>
#include "hw/io.h"

/*
 * HW-independent serial output function that works like printf
 */
void sc_serial_printf(char *format, ...)
{
    char d[128];
    va_list args;
    va_start(args, format);
    vsnprintf(d, 128, format, args);
    hw_uart_puts(d); // call HW-dependent implementation for serial output
}
