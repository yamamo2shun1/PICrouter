#include "Compiler.h"
#include "myDelay.h"

void delayUs(unsigned int usec)
{
    unsigned int i, max;
    max = usec * 80 / Fosc;
    for(i = 0; i < max; i++) {}
}

void delayMs(unsigned int msec)
{
    unsigned int i;
    for(i = 0; i < msec; i++)
        delayUs(1000);
}
