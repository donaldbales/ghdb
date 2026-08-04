#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "ghdb.h"

/*
(2) Create datafile

(3) Add Records

	Data Entry Mode
	------------------
	(ENTER) Add record
	(9) Exit to modify
	(12) Table/Range
	(16) Exit

(4) Modify Records

	Find Mode
	------------------
	(ENTER) Find record
	(2)	First record
	(4)	Previous record
	(5)	Next record
	(9) Add record
	(12) Table/Range
	(16) Exit

	Data Modification Mode
	------------------
	(ENTER) Modify record
	(1)	Find
	(16) Exit

(5) Delete Records

	Find Mode
	------------------
	(ENTER) Find record
	(2)	First record
	(4)	Previous record
	(5)	Next record
	(9) Add record
	(12) Table/Range
	(16) Exit

	Data Deletion Mode
	------------------
	(ENTER) Delete record
	(1)	Find
	(16) Exit

(6) Display Data File

	Find Mode
	------------------
	(ENTER) Find record
	(2)	First record
	(4)	Previous record
	(5)	Next record
	(9) Add record
	(12) Table/Range
	(16) Exit

(7) Import
(8) Export

(9) Run INQUIRY

(16) Exit
*/

int database()
{
	return 0;
}