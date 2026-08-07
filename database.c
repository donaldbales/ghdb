#include <errno.h>
#include <gdbm.h>
#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "ghdb.h"

/*
256.128.64.32.16.8 4 2 1

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

void *ghdb_open() 
{
	static GDBM_FILE gdbf = NULL;
	char* gdbf_filename = "ghdb.gbm"; 
/*
	datum key;
	datum content;
	datum contentin;
	char *pk;
	char *datein = "01/01/1980";
	char *dateout; 
	char *p;
	int i = 0;
	int n = -1;
	int s = -1;
	size_t DATE = 30;
	struct RECORD r;
	struct RECORD *rptr;
	struct RECORD w;
	struct RECORD *wptr;
	struct tm datetm;
	time_t seconds_since_epoch = -1;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: gdbm_file [FILE]\n");
		exit(EXIT_FAILURE);
	}
*/
	gdbf = gdbm_open(gdbf_filename, 0, GDBM_WRCREAT, (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH), NULL);
	if (gdbf == NULL)
	{
		fprintf(stderr, "can't open database: %s\n", gdbm_strerror (gdbm_errno));
		exit(EXIT_FAILURE);
	}
	return gdbf;
}

int database()
{
	/*
	Initiate sequential access to the database dbf. The returned value is the first key
	accessed in the database. If the dptr field in the returned datum is NULL, inspect the
	gdbm_errno variable (see Chapter 20 [Variables], page 44). The value of GDBM_ITEM_
	NOT_FOUND means that the database contains no data. Other value means an error
	occurred.
	On success, dptr points to a memory block obtained from malloc, which holds the
	key value. The caller is responsible for freeing this memory block when no longer
	needed.
	*/

	//datum gdbm_firstkey (GDBM FILE dbf)

	/*
	This function continues iteration over the keys in dbf, initiated by gdbm_firstkey.
	The parameter prev holds the value returned from a previous call to gdbm_nextkey
	or gdbm_firstkey.
	The function returns next key from the database. If the dptr field in the returned
	datum is NULL inspect the gdbm_errno variable (see Chapter 20 [Variables], page 44).
	The value of GDBM_ITEM_NOT_FOUND means that all keys in the database has been
	visited. Any other value means an error occurred.
	Otherwise, dptr points to a memory block obtained from malloc, which holds the
	key value. The caller is responsible for freeing this memory block when no longer
	needed.
	*/

	//datum gdbm_nextkey (GDBM FILE dbf, datum prev)



	/*
	If key is found, returns true (1). If it is not found, returns false (0) and sets
	gdbm_errno to GDBM_NO_ERROR (0).
	On error, returns 0 and sets gdbm_errno to a non-0 error code.
	*/

	//int gdbm_exists(gdbf, datum key);
	
	/*
	Deletes the data associated with the given key, if it exists in the database dbf.
	The parameters are:
	dbf The pointer returned by gdbm_open.
	datum key
	The search key.
	The function returns -1 if the item is not present or if an error is encountered.
	Examine the gdbm_errno variable or the return from gdbm_last_errno (dbf) to
	know the reason.
	The return of 0 marks a successful delete.
	*/

	//int gdbm_delete (GDBM FILE dbf, datum key)

	/*
	Synchronizes the changes in dbf with its disk file. The parameter is a pointer returned
	by gdbm_open.
	This function would usually be called after a complete set of changes have been made
	to the database and before some long waiting time. This set of changes should preserve
	application-level invariants. In other words, call gdbm_sync only when the database
	is in a consistent state with regard to the application logic, a state from which you
	are willing and able to recover. You can think about all database operations between
	two consecutive gdbm_sync calls as constituting a single transaction. See Section 17.3
	[Synchronizing the Database], page 31, for a detailed discussion about how to properly
	select the synchronization points.
	The gdbm_close function automatically calls the equivalent of gdbm_sync so no call
	is needed if the database is to be closed immediately after the set of changes have
	been made.
	Gdbm_sync returns 0 on success. On error, it sets gdbm_errno and system errno
	variables to the codes describing the error and returns -1.
	*/
	
	//int gdbm_sync (GDBM FILE dbf)

	/*

	*/
	
	//int gdbm_recover (GDBM FILE dbf, gdbm recovery *rcvr, int flags)


	return 0;
}

























