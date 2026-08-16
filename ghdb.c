#include <gdbm.h>
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "ghdb.h"

/*
gcc keymouse.c -c -g -Wall
gcc keyboard.c -c -g -Wall
gcc terminal.c -c -g -Wall
gcc ghdb.c -c -g -Wall
gcc ghdb.o terminal.o keyboard.o keymouse.o -lncursesw_g -o ghdb

rm -f trace; env NCURSES_TRACE=1 ghdb 2>ghdb.err; cat ghdb.err
*/

/*
    TODO?
    Use left or right arrow keys to insert text in a field?
X   Save the binary data to a file
X   Read the binary data from a file
X   Save the delimited data to a file
X   Read the delimited data from a file
X   Scroll to the next record
*/

int main()
{
	return terminal();
}
