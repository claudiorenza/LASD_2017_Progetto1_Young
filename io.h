#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <stdio.h>

#include "random.h"

#define MAX_string 30		//lunghezza massima delle stringhe

void io_getString(char *string, int length);

int io_getInteger();

char io_getChar();

void io_clearScreen();

void io_pressKey();

#endif
