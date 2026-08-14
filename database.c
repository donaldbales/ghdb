#include <errno.h>
#include <gdbm.h>
#include <ncurses.h>
#include <regex.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "ghdb.h"

/*
 Free singly-linked list
 */
struct FREE_LIST
{
	void *prev;
};
struct FREE_LIST *free_list = NULL;

/*
 Key doubly-linked list
 */
struct KEY_LIST 
{
	void *prev;
	char *key;
	void *next;
};
struct KEY_LIST *ghdb_key = NULL;

/*
 Alpha comparator
 */
static int compare_char_arrays(const void *a, const void *b)
{
	return strcmp(*(const char**)a, *(const char**)b); 
}

char *trim(char *string)
{
	for (int i=strlen(string) - 1; i >= 0; i--)
	{
		//printf("%d, '%c'\n", i, *(string+i));
		if (*(string+i) == ' ')
		{
			*(string+i) = '\0';
		}
		else
		{
			break;
		}
	}

	return string;
}

int like(char *re, char *str, int *patterns, int *matches)
{
	char *buffer;
	char *pattern;
	char *string;
	int errors = 0;
	int n = 0;
	int nbuffer = 1024;
	int nregmatch = 1;
	regex_t compiled;
	regmatch_t regmatch[nregmatch];

	pattern = malloc(strlen(re) + 1);
	strcpy(pattern, re);
	trim(pattern);
	if (strlen(pattern) > 0)
	{
		n = regcomp(&compiled, pattern, REG_EXTENDED | REG_ICASE | REG_NEWLINE);
		if (n == 0)
		{
			fprintf(stderr, "pattern compiled successfully\n");
		}
		else
		{
			buffer = malloc(nbuffer + 1);
			regerror(n, &compiled, buffer, nbuffer);
			fprintf(stderr, "pattern compilation error: %s\n", buffer);
			xerror(buffer);
			free(buffer);
			errors++;
		}

		if (n == 0)
		{
			(*patterns)++;
			string = malloc(strlen(str) + 1);
			strcpy(string, str);
			trim(string);
			n = regexec(&compiled, string, nregmatch, regmatch, 0);
			if (n == 0)
			{
				(*matches)++;
				fprintf(stderr, "it matches\n");
			}
			else if (n == REG_NOMATCH)
			{
				fprintf(stderr, "no match\n");
			}
			else
			{
				buffer = malloc(nbuffer + 1);
				regerror(n, &compiled, buffer, nbuffer);
				fprintf(stderr, "pattern match error: %s\n", buffer);
				xerror(buffer);
				free(buffer);
				errors++;
			}
			free(string);
		}
		regfree(&compiled);
	}
	free(pattern);

	return errors;
}

int find_like(struct RECORD *record)
{
	int patterns = 0;
	int matches = 0;
	char *re;
	char *str;
	struct RECORD *local_record;
	datum content;
	datum key;
	struct KEY_LIST *p = NULL;
	struct KEY_LIST *first = NULL;
	struct KEY_LIST *prev = NULL;
	struct KEY_LIST *local_key = NULL;
	GDBM_FILE gdbm_file = ghdb_open();

	fprintf(stderr, "find_like(): build key list.\n");
	local_record = malloc(sizeof(struct RECORD));
	init_record(local_record);
	ghdb_key = NULL;
	ghdb_select_first(local_record);
	free(local_record);
	local_record = NULL;
	p = ghdb_key;
	if (p != NULL)
	{
		xerror("Searching...");
		do
		{
			fprintf(stderr, "find_like(): get a record from the database.\n");
			local_record = malloc(sizeof(struct RECORD));
			init_record(local_record);
			fprintf(stderr, "find_like():ghdb_key='%p'\n", p);
			key.dptr = ((struct KEY_LIST *)p)->key;
			key.dsize = PLANT_NAME_LENGTH;
			fprintf(stderr, "find_like(): key.dptr='%s'\n", key.dptr);
			fprintf(stderr, "find_like(): key.dsize='%d'\n", key.dsize);
			content = gdbm_fetch(gdbm_file, key);
			if (content.dptr != NULL)
			{
				memcpy(local_record, content.dptr, content.dsize);
				xerror("");
			}
			else if (gdbm_errno == GDBM_ITEM_NOT_FOUND)
			{
				fprintf(stderr, "find_like(): record not found.\n");
				xerror("Record not found.");
			}
			else
			{
				xerror(gdbm_db_strerror(gdbm_file));
			}

			fprintf(stderr, "find_like(): create the re.\n");
			re = malloc(strlen(record->plant_name) + 1);
			strcpy(re, record->plant_name);
			if (strlen(trim(re)) > 0)
			{
				patterns = 0;
				matches = 0;
				fprintf(stderr, "find_like(): create the str.\n");
				str = malloc(strlen(local_record->plant_name) + 1);
				strcpy(str, local_record->plant_name);
				fprintf(stderr, "find_like(): Before like.\n");
				like(re, str, &patterns, &matches);
				fprintf(stderr, "find_like(): After like, patterns=%d, matches=%d.\n\n", patterns, matches);
				free(str);
			}
			free(local_record);
			free(re);


			if (patterns == matches)
			{
				fprintf(stderr, "find_like(): Found a record: '%s'\n", p->key);
				local_key = malloc(sizeof(struct KEY_LIST));
				if (first == NULL)
				{
					first = local_key;
				}
				local_key->prev = prev;
				local_key->key = p->key;
				local_key->next = NULL;
				if (prev != NULL)
				{
					((struct KEY_LIST *)prev)->next = local_key;
				}
				prev = local_key;
			}
			p = p->next;
			fprintf(stderr, "find_like(): the next key is %p\n", p);
		}
		while (p != NULL);
	}
	xerror("Done.");
	// NOTE: free all the ghdk_key structs
	ghdb_key = first;

	return 0;
}

/*
 Close
 */
void ghdb_close(GDBM_FILE gdbm_file)
{
	if (gdbm_close(gdbm_file) == 0)
	{
		gdbm_file = NULL;
	}
	else
	{
		fprintf(stderr, "can't close the database: %s\n", gdbm_strerror (gdbm_errno));
		exit(EXIT_FAILURE);
	}
	return;
}

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

	//int gdbm_delete (GDBM FILE dbf, datum key)

	*/
int ghdb_delete(struct RECORD *record)
{
	GDBM_FILE gdbm_file = ghdb_open();
	char *pk;
	datum key;
	int retcode = -1;

	pk = malloc(PLANT_NAME_LENGTH + 1);
	if (pk == NULL)
	{
		exit(EXIT_FAILURE);
	}
	strncpy(pk, record->plant_name, PLANT_NAME_LENGTH);

	key.dsize = PLANT_NAME_LENGTH;
	// I think you can recode this to use record->plant_name!
	key.dptr = record->plant_name;
	pk = NULL;

	retcode = gdbm_delete(gdbm_file, key);
	free(pk);
	if (retcode == 0)
	{
		xerror("Record deleted.");
	}
	if (retcode == -1)
	{
		fprintf(stderr, "can't delete from database: %s\n", gdbm_strerror (gdbm_errno));
		exit(EXIT_FAILURE);
	}
	else if (retcode == 1)
	{
		fprintf(stderr, "can't delete from database: %s\n", gdbm_strerror (gdbm_errno));
	}

	return 0;
}

/*
	typedef struct
	{
		char *dptr;
		int dsize;
	} datum;
 */
int ghdb_insert(struct RECORD *record)
{
	GDBM_FILE gdbm_file = ghdb_open();
	char *pk;
	datum key;
	datum content;
	int retcode = -1;
	void *prev;
	
	/* delete the doubly-linked list */
	if (ghdb_key)
	{
		// wind forward
		while (ghdb_key->next)
		{
			ghdb_key = ghdb_key->next;
		}
		// now free dynaically allocated storage going backwards
		while (ghdb_key)
		{
			prev = ghdb_key->prev;
			free(ghdb_key->key);
			free(ghdb_key);
			ghdb_key = prev;
		}
	}

	/* save the record */
	pk = malloc(PLANT_NAME_LENGTH + 1);
	if (pk == NULL)
	{
		exit(EXIT_FAILURE);
	}
	strncpy(pk, record->plant_name, PLANT_NAME_LENGTH);

	key.dsize = PLANT_NAME_LENGTH;
	// I think you can recode this to use record->plant_name
	key.dptr = (((void *)pk));
	pk = NULL;

	content.dsize = sizeof(struct RECORD);
	content.dptr = (((void *)record));

	retcode = gdbm_store(gdbm_file, key, content, GDBM_INSERT);
	free(pk);
	if (retcode == 0)
	{
		xerror("Record inserted.");
	}
	if (retcode == -1)
	{
		fprintf(stderr, "can't insert into database: %s\n", gdbm_strerror (gdbm_errno));
		exit(EXIT_FAILURE);
	}
	else if (retcode == 1)
	{
		fprintf(stderr, "can't insert into database: %s\n", gdbm_strerror (gdbm_errno));
	}

	return 0;
}

/*
 Open
 */
void *ghdb_open() 
{
	static GDBM_FILE gdbm_file = NULL;
	char* gdbm_file_filename = "ghdb.gbm"; 

	if (gdbm_file == NULL)
	{
		gdbm_file = gdbm_open(gdbm_file_filename, 0, GDBM_WRCREAT, (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH), NULL);
	}
	if (gdbm_file == NULL)
	{
		fprintf(stderr, "can't open the database: %s\n", gdbm_strerror (gdbm_errno));
		exit(EXIT_FAILURE);
	}

	return gdbm_file;
}

/*
 Select by key
 */
int ghdb_select(struct RECORD *record)
{
	datum content;
	datum key;
	GDBM_FILE gdbm_file = ghdb_open();
	int nrecords = 0;
	char string[50];
	struct KEY_LIST *p;

	fprintf(stderr, "Before find_like(record)\n");
	find_like(record);
	fprintf(stderr, "After find_like(record), ghdb_key=%p\n", ghdb_key);

	if (ghdb_key != NULL)
	{
		p = ghdb_key;
		fprintf(stderr, "p=%p\n", p);
		while (p != NULL)
		{
			nrecords++;
			fprintf(stderr, "p->next=%p\n", p->next);
			p = p->next;
		};

		fprintf(stderr, "After find_like(record), ghdb_key=%p, nrecords=%d\n", ghdb_key, nrecords);

		key.dptr = ghdb_key->key;
		fprintf(stderr, "ghdb_select(): key.dptr='%s'\n", key.dptr);
		key.dsize = PLANT_NAME_LENGTH;
		content = gdbm_fetch(gdbm_file, key);
		if (content.dptr != NULL)
		{

			memcpy(record, content.dptr, content.dsize);
			sprintf(string, "%d %s", nrecords, "records found. Press (ENTER) to update.");
			xerror(string);
		}
		else if (gdbm_errno == GDBM_ITEM_NOT_FOUND)
		{
			xerror("Record not found.");
		}
		else
		{
			xerror(gdbm_db_strerror(gdbm_file));
		}
	}
	else
	{
		xerror("Record not found.");
	}	

	fprintf(stderr, "Returning from ghdb_select()\n");

	return nrecords;
}

	/*
	Initiate sequential access to the database dbf. The returned value is the first key
	accessed in the database. If the dptr field in the returned datum is NULL, inspect the
	gdbm_errno variable (see Chapter 20 [Variables], page 44). The value of GDBM_ITEM_
	NOT_FOUND means that the database contains no data. Other value means an error
	occurred.
	On success, dptr points to a memory block obtained from malloc, which holds the
	key value. The caller is responsible for freeing this memory block when no longer
	needed.

	datum gdbm_firstkey (GDBM FILE dbf)

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

	datum gdbm_nextkey (GDBM FILE dbf, datum prev)

	*/
int ghdb_select_first(struct RECORD *record)
{
	datum content;
	datum key;
	datum nextkey;
	gdbm_count_t ghdb_count;
	int i = 0;
	char message[80];
	struct KEY_LIST *k = NULL; 
	void *prev = NULL;
	GDBM_FILE gdbm_file = ghdb_open();
	gdbm_count(gdbm_file, &ghdb_count);
	sprintf(message, "First of %lld records. Press (ENTER) to update.", ghdb_count);
	xerror(message);

	/* create an array big enough to hold all the current keys */
	char *keys[ghdb_count];

	/* get all of the keys */
	key = gdbm_firstkey(gdbm_file);
	keys[0] = key.dptr;
	free_list = malloc(sizeof(struct FREE_LIST)); // use this to track GDBM mallocs
	free_list->prev = NULL;
	prev = NULL;
	for (i=1; i<ghdb_count; i++)
	{
		nextkey = gdbm_nextkey(gdbm_file, key);
		if (nextkey.dptr)
		{
			key = nextkey;
			keys[i] = key.dptr;
			prev = free_list;
			free_list = malloc(sizeof(struct FREE_LIST));
			free_list->prev = prev;
		}
	}

	/* sort the keys */
	qsort(keys, ghdb_count, sizeof(const char *), compare_char_arrays);

	/* create the doubly-linked list of the keys */
	prev = NULL;
	for (i=0; i<ghdb_count; i++)
	{
		k = malloc(sizeof(struct KEY_LIST));
		if (prev == NULL)
		{
			ghdb_key = k; // ghdb_key is assigned to the beginning of the list
		}
		else
		{
			((struct KEY_LIST *)prev)->next = k;
		} 
		k->prev = prev;
		k->key = malloc(PLANT_NAME_LENGTH + 1);
		strncpy(k->key, keys[i], PLANT_NAME_LENGTH);
		*(k->key+30) = '\0';
		k->next = NULL;
		prev = k;
	}

	// Free the GDBM malloc list now
	while (free_list)
	{
		prev = free_list->prev;
		free(free_list);
		free_list = prev;
	}

	/* get the first record using the key list */
	key.dptr = ((struct KEY_LIST *)ghdb_key)->key;
	key.dsize = PLANT_NAME_LENGTH;
	content = gdbm_fetch(gdbm_file, key);
	if (content.dptr != NULL)
	{
		memcpy(record, content.dptr, content.dsize);
		free(content.dptr);
	}
	else if (gdbm_errno == GDBM_ITEM_NOT_FOUND)
	{
		xerror("Record not found.");
	}
	else
	{
		xerror(gdbm_db_strerror(gdbm_file));
	}

	return 0;
}

int ghdb_select_next(struct RECORD *record)
{
	datum content;
	datum key;
	GDBM_FILE gdbm_file = ghdb_open();

	/* use the key list to get the next record */
	if (ghdb_key->next != NULL)
	{
		ghdb_key = ghdb_key->next;
		key.dptr = ((struct KEY_LIST *)ghdb_key)->key;
		key.dsize = PLANT_NAME_LENGTH;
		content = gdbm_fetch(gdbm_file, key);
		if (content.dptr != NULL)
		{
			memcpy(record, content.dptr, content.dsize);
			free(content.dptr);
			xerror("Press (ENTER) to update.");
		}
		else if (gdbm_errno == GDBM_ITEM_NOT_FOUND)
		{
			xerror("Record not found.");
		}
		else
		{
			xerror(gdbm_db_strerror(gdbm_file));
		}
	}
	else
	{
		xerror("Last record. Press (ENTER) to update.");
	}

	return 0;
}

int ghdb_select_previous(struct RECORD *record)
{
	datum content;
	datum key;
	GDBM_FILE gdbm_file = ghdb_open();

	/* use the key list to get the previous record */
	if (ghdb_key->prev != NULL)
	{
		ghdb_key = ghdb_key->prev;
		key.dptr = ((struct KEY_LIST *)ghdb_key)->key;
		key.dsize = PLANT_NAME_LENGTH;
		content = gdbm_fetch(gdbm_file, key);
		if (content.dptr != NULL)
		{
			memcpy(record, content.dptr, content.dsize);
			free(content.dptr);
			xerror("Press (ENTER) to update.");
		}
		else if (gdbm_errno == GDBM_ITEM_NOT_FOUND)
		{
			xerror("Record not found.");
		}
		else
		{
			xerror(gdbm_db_strerror(gdbm_file));
		}
	}
	else
	{
		xerror("First record. Press (ENTER) to update.");
	}

	return 0;
}

int ghdb_update(struct RECORD *record)
{
	GDBM_FILE gdbm_file = ghdb_open();
	char *pk;
	datum key;
	datum content;
	int retcode = -1;

	pk = malloc(PLANT_NAME_LENGTH + 1);
	if (pk == NULL)
	{
		exit(EXIT_FAILURE);
	}
	strncpy(pk, record->plant_name, PLANT_NAME_LENGTH);

	key.dsize = PLANT_NAME_LENGTH;
	// I think you can recode this to use record->plant_name
	key.dptr = (((void *)pk));
	pk = NULL;

	content.dsize = sizeof(struct RECORD);
	content.dptr = (((void *)record));

	retcode = gdbm_store(gdbm_file, key, content, GDBM_REPLACE);
	free(pk);
	if (retcode == 0)
	{
		xerror("Record updated.");
	}
	if (retcode == -1)
	{
		fprintf(stderr, "can't insert into database: %s\n", gdbm_strerror (gdbm_errno));
		exit(EXIT_FAILURE);
	}
	else if (retcode == 1)
	{
		fprintf(stderr, "can't insert into database: %s\n", gdbm_strerror (gdbm_errno));
	}

	return 0;
}

/*
	I didn't use these, yet.

	If key is found, returns true (1). If it is not found, returns false (0) and sets
	gdbm_errno to GDBM_NO_ERROR (0).
	On error, returns 0 and sets gdbm_errno to a non-0 error code.

	int gdbm_exists(gdbm_file, datum key);
	
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

	int gdbm_sync (GDBM FILE dbf)

	Check the database file dbf and fix eventual errors. The rcvr argument points to
	a structure that has input members, providing additional information to alter the
	behavior of gdbm_recover, and output members, which are used to return additional
	statistics about the recovery process (rcvr can be NULL if no such information is
	needed).
	Each input member has a corresponding flag bit, which must be set in flags, in order
	to instruct the function to use it.

	int gdbm_recover (GDBM FILE dbf, gdbm recovery *rcvr, int flags)
*/

/*
 Initialize a new record
 */
int init_record(struct RECORD *record)
{
	for (int i=0; i<PLANT_NAME_LENGTH; i++)
	{
		record->plant_name[i]= ' ';
	}
	record->plant_name[PLANT_NAME_LENGTH] = '\0';

	for (int i=0; i<LATIN_NAME_LENGTH; i++)
	{
		record->latin_name[i]= ' ';
	}
	record->latin_name[LATIN_NAME_LENGTH] = '\0';

	for (int i=0; i<HEIGHT_LENGTH; i++)
	{
		record->height[i]= ' ';
	}
	record->height[HEIGHT_LENGTH] = '\0';

	for (int i=0; i<WIDTH_LENGTH; i++)
	{
		record->width[i]= ' ';
	}
	record->width[WIDTH_LENGTH] = '\0';

	for (int i=0; i<PLANTING_DEPTH_LENGTH; i++)
	{
		record->planting_depth[i]= ' ';
	}
	record->planting_depth[PLANTING_DEPTH_LENGTH] = '\0';

	return 0;
}

