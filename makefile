#
# gcc ncurses4.c -g -o ncurses4 -lncursesw_g
# rm -f trace; env NCURSES_TRACE=1 ghdb 2>ghdb.err; cat ghdb.err
#

ghdb : ghdb.o terminal.o keyboard.o keymouse.o
	gcc ghdb.o terminal.o keyboard.o keymouse.o -l ncursesw_g -o ghdb

keymouse.o : ghdb.h keymouse.c
	gcc keymouse.c -c -g -Wall

keyboard.o : ghdb.h keyboard.c
	gcc keyboard.c -c -g -Wall

terminal.o : ghdb.h terminal.c
	gcc terminal.c -c -g -Wall

ghdb.o : ghdb.h ghdb.c
	gcc ghdb.c -c -g -Wall

.PHONY run : ghdb
	rm -f trace; env NCURSES_TRACE=1 ghdb 2>ghdb.err; cat ghdb.err
	
