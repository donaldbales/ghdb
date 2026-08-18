#include <errno.h>
#include <gdbm.h>
#include <ncurses.h>
#include <regex.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "ghdb.h"

/*
 Unfortunately global
 */
GDBM_FILE gdbm_file = NULL;

/*
 Free singly-linked list
 */
struct FREE_LIST
{
	struct FREE_LIST *prev;
};
struct FREE_LIST *free_list = NULL;

/*
 Key doubly-linked list
 */
struct KEY_LIST 
{
	struct KEY_LIST *prev;
	char *key;
	struct KEY_LIST *next;
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
int ghdb_close()
{
	if (gdbm_file != NULL)
	{
		gdbm_close(gdbm_file);
		gdbm_file = NULL;
	}
	
	return 0;
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
		(void) endwin();
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
		(void) endwin();
		exit(EXIT_FAILURE);
	}
	else if (retcode == 1)
	{
		fprintf(stderr, "can't delete from database: %s\n", gdbm_strerror (gdbm_errno));
	}

	return 0;
}

int ghdb_export()
{
	int n = 0;
	ghdb_close();
	pid_t pid = fork(); // Create a new process

	if (pid < 0)
	{
		perror("Fork failed");
		return 1;
	}
	if (pid == 0) 
	{
		// Child process
		fprintf(stderr, "Backing up data to file: ghdb.asc.\n");
		n = execl("/usr/bin/gdbmtool", "/usr/bin/gdbmtool", "ghdb.gdbm", "export", "ghdb.asc", "ascii", (char *)NULL);
		fprintf(stderr, "Backing up data to file: n=%d.\n", n);
		if (n == -1)
		{
			fprintf(stderr, "ghdb_export: %s\n", strerror(errno));
		}
		(void) endwin();
		exit(EXIT_FAILURE);
	}
	else
	{
		// Parent process
		wait(NULL); // Wait for child to finish
		fprintf(stderr, "Child process completed.\n");
		xerror("Backup to ghdb.asc completed successfully.");
	}

	return n;
}

int ghdb_import()
{
	int n = 0;
	ghdb_close();
	pid_t pid = fork(); // Create a new process

	if (pid < 0)
	{
		perror("Fork failed");
		return 1;
	}
	if (pid == 0) 
	{
		// Child process
		fprintf(stderr, "Restoring data from file: ghdb.asc.\n");
		n = execl("/usr/bin/gdbmtool", "/usr/bin/gdbmtool", "ghdb.gdbm", "import", "ghdb.asc", "replace", (char *)NULL);
		fprintf(stderr, "Restoring data from file: n=%d.\n", n);
		if (n == -1)
		{
			fprintf(stderr, "ghdb_export: %s\n", strerror(errno));
		}
		(void) endwin();
		exit(EXIT_FAILURE);
	}
	else
	{
		// Parent process
		wait(NULL); // Wait for child to finish
		fprintf(stderr, "Child process completed.\n");
		xerror("Restore from ghdb.asc completed successfully.");
	}

	return n;
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
		(void) endwin();
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
		(void) endwin();
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
	char* gdbm_file_filename = "ghdb.gdbm"; 

	if (gdbm_file == NULL)
	{
		gdbm_file = gdbm_open(gdbm_file_filename, 0, GDBM_WRCREAT | GDBM_NUMSYNC, (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH), NULL);
	}
	if (gdbm_file == NULL)
	{
		fprintf(stderr, "can't open the database: %s\n", gdbm_strerror (gdbm_errno));
		(void) endwin();
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
	char string[75];
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
		return 1;
	}
	else
	{
		xerror(gdbm_db_strerror(gdbm_file));
		return -1;
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
			return 1;
		}
		else
		{
			xerror(gdbm_db_strerror(gdbm_file));
			return -1;
		}
	}
	else
	{
		xerror("Last record. Press (ENTER) to update.");
		return 1;
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
		(void) endwin();
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
		(void) endwin();
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
	int i = 0;

	for (i=0; i<PLANT_NAME_LENGTH; i++)
	{
		record->plant_name[i]= ' ';
	}
	record->plant_name[PLANT_NAME_LENGTH] = '\0';

	for (i=0; i<LATIN_NAME_LENGTH; i++)
	{
		record->latin_name[i]= ' ';
	}
	record->latin_name[LATIN_NAME_LENGTH] = '\0';

	for (i=0; i<HEIGHT_LENGTH; i++)
	{
		record->height[i]= ' ';
	}
	record->height[HEIGHT_LENGTH] = '\0';

	for (i=0; i<WIDTH_LENGTH; i++)
	{
		record->width[i]= ' ';
	}
	record->width[WIDTH_LENGTH] = '\0';

	for (i=0; i<PLANTING_DEPTH_LENGTH; i++)
	{
		record->planting_depth[i]= ' ';
	}
	record->planting_depth[PLANTING_DEPTH_LENGTH] = '\0';

	for (i=0; i<SEED_SIZE_LENGTH; i++)
	{
		record->seed_size[i]= ' ';
	}
	record->seed_size[SEED_SIZE_LENGTH] = '\0';

	for (i=0; i<SEED_NEED_LIGHT_LENGTH; i++)
	{
		record->seed_need_light[i]= ' ';
	}
	record->seed_need_light[SEED_NEED_LIGHT_LENGTH] = '\0';

	for (i=0; i<SEED_SCARIFICATION_LENGTH; i++)
	{
		record->seed_scarification[i]= ' ';
	}
	record->seed_scarification[SEED_SCARIFICATION_LENGTH] = '\0';

	for (i=0; i<PH_LENGTH; i++)
	{
		record->ph[i]= ' ';
	}
	record->ph[PH_LENGTH] = '\0';

	for (i=0; i<EC_LENGTH; i++)
	{
		record->ec[i]= ' ';
	}
	record->ec[EC_LENGTH] = '\0';

	for (i=0; i<DAY_LIGHT_INTERVAL_LENGTH; i++)
	{
		record->day_light_interval[i]= ' ';
	}
	record->day_light_interval[DAY_LIGHT_INTERVAL_LENGTH] = '\0';

	for (i=0; i<PHOTOPERIOD_HOURS_LENGTH; i++)
	{
		record->photoperiod_hours[i]= ' ';
	}
	record->photoperiod_hours[PHOTOPERIOD_HOURS_LENGTH] = '\0';

	for (i=0; i<LIGHT_LOWER_LENGTH; i++)
	{
		record->light_lower[i]= ' ';
	}
	record->light_lower[LIGHT_LOWER_LENGTH] = '\0';

	for (i=0; i<LIGHT_OPTIMAL_LENGTH; i++)
	{
		record->light_optimal[i]= ' ';
	}
	record->light_optimal[LIGHT_OPTIMAL_LENGTH] = '\0';

	for (i=0; i<LIGHT_UPPER_LENGTH; i++)
	{
		record->light_upper[i]= ' ';
	}
	record->light_upper[LIGHT_UPPER_LENGTH] = '\0';

	for (i=0; i<NITROGEN_LOWER_LENGTH; i++)
	{
		record->nitrogen_lower[i]= ' ';
	}
	record->nitrogen_lower[NITROGEN_LOWER_LENGTH] = '\0';

	for (i=0; i<NITROGEN_OPTIMAL_LENGTH; i++)
	{
		record->nitrogen_optimal[i]= ' ';
	}
	record->nitrogen_optimal[NITROGEN_OPTIMAL_LENGTH] = '\0';

	for (i=0; i<NITROGEN_UPPER_LENGTH; i++)
	{
		record->nitrogen_upper[i]= ' ';
	}
	record->nitrogen_upper[NITROGEN_UPPER_LENGTH] = '\0';

	for (i=0; i<PHOSPHORUS_LOWER_LENGTH; i++)
	{
		record->phosphorus_lower[i]= ' ';
	}
	record->phosphorus_lower[PHOSPHORUS_LOWER_LENGTH] = '\0';

	for (i=0; i<PHOSPHORUS_OPTIMAL_LENGTH; i++)
	{
		record->phosphorus_optimal[i]= ' ';
	}
	record->phosphorus_optimal[PHOSPHORUS_OPTIMAL_LENGTH] = '\0';

	for (i=0; i<PHOSPHORUS_UPPER_LENGTH; i++)
	{
		record->phosphorus_upper[i]= ' ';
	}
	record->phosphorus_upper[PHOSPHORUS_UPPER_LENGTH] = '\0';

	for (i=0; i<POTASSIUM_LOWER_LENGTH; i++)
	{
		record->potassium_lower[i]= ' ';
	}
	record->potassium_lower[POTASSIUM_LOWER_LENGTH] = '\0';

	for (i=0; i<POTASSIUM_OPTIMAL_LENGTH; i++)
	{
		record->potassium_optimal[i]= ' ';
	}
	record->potassium_optimal[POTASSIUM_OPTIMAL_LENGTH] = '\0';

	for (i=0; i<POTASSIUM_UPPER_LENGTH; i++)
	{
		record->potassium_upper[i]= ' ';
	}
	record->potassium_upper[POTASSIUM_UPPER_LENGTH] = '\0';

	for (i=0; i<GERMINATION_LOWER_LENGTH; i++)
	{
		record->germination_lower[i]= ' ';
	}
	record->germination_lower[GERMINATION_LOWER_LENGTH] = '\0';

	for (i=0; i<GERMINATION_NORMAL_LENGTH; i++)
	{
		record->germination_normal[i]= ' ';
	}
	record->germination_normal[GERMINATION_NORMAL_LENGTH] = '\0';

	for (i=0; i<GERMINATION_UPPER_LENGTH; i++)
	{
		record->germination_upper[i]= ' ';
	}
	record->germination_upper[GERMINATION_UPPER_LENGTH] = '\0';

	for (i=0; i<TRANSPLANTING_LOWER_LENGTH; i++)
	{
		record->transplanting_lower[i]= ' ';
	}
	record->transplanting_lower[TRANSPLANTING_LOWER_LENGTH] = '\0';

	for (i=0; i<TRANSPLANTING_OPTIMAL_LENGTH; i++)
	{
		record->transplanting_optimal[i]= ' ';
	}
	record->transplanting_optimal[TRANSPLANTING_OPTIMAL_LENGTH] = '\0';

	for (i=0; i<TRANSPLANTING_UPPER_LENGTH; i++)
	{
		record->transplanting_upper[i]= ' ';
	}
	record->transplanting_upper[TRANSPLANTING_UPPER_LENGTH] = '\0';

	for (i=0; i<MATURITY_LOWER_LENGTH; i++)
	{
		record->maturity_lower[i]= ' ';
	}
	record->maturity_lower[MATURITY_LOWER_LENGTH] = '\0';

	for (i=0; i<MATURITY_OPTIMAL_LENGTH; i++)
	{
		record->maturity_optimal[i]= ' ';
	}
	record->maturity_optimal[MATURITY_OPTIMAL_LENGTH] = '\0';

	for (i=0; i<MATURITY_UPPER_LENGTH; i++)
	{
		record->maturity_upper[i]= ' ';
	}
	record->maturity_upper[MATURITY_UPPER_LENGTH] = '\0';

	for (i=0; i<FROST_TOLERANCE_LENGTH; i++)
	{
		record->frost_tolerance[i]= ' ';
	}
	record->frost_tolerance[FROST_TOLERANCE_LENGTH] = '\0';

	for (i=0; i<FLOWERING_LENGTH; i++)
	{
		record->flowering[i]= ' ';
	}
	record->flowering[FLOWERING_LENGTH] = '\0';

	for (i=0; i<POLLINATION_PRIMARY_LENGTH; i++)
	{
		record->pollination_primary[i]= ' ';
	}
	record->pollination_primary[POLLINATION_PRIMARY_LENGTH] = '\0';

	for (i=0; i<POLLINATION_SECONDARY_LENGTH; i++)
	{
		record->pollination_secondary[i]= ' ';
	}
	record->pollination_secondary[POLLINATION_SECONDARY_LENGTH] = '\0';

	return 0;
}

int tsv_export()
{
	char filename[9] = "ghdb.tsv";
	FILE *file;
	int n = 0;
	struct RECORD record;

	errno = 0;
	file = fopen(filename, "w");
	if (file == NULL)
	{
		fprintf(stderr, "Could not open %s: %s\n", filename, strerror(errno));
		(void) endwin();
		exit (EXIT_FAILURE);
	}
	fprintf(file, "PLANT_NAME\tLATIN_NAME\tHEIGHT\tWIDTH\tPLANTING_DEPTH\tSEED_SIZE\tSEED_NEED_LIGHT\tSEED_SCARIFICATION\tPH\tEC\tDAY_LIGHT_INTERVAL\tPHOTOPERIOD_HOURS\tLIGHT_LOWER\tLIGHT_OPTIMAL\tLIGHT_UPPER\tNITROGEN_LOWER\tNITROGEN_OPTIMAL\tNITROGEN_UPPER\tPHOSPHORUS_LOWER\tPHOSPHORUS_OPTIMAL\tPHOSPHORUS_UPPER\tPOTASSIUM_LOWER\tPOTASSIUM_OPTIMAL\tPOTASSIUM_UPPER\tGERMINATION_LOWER\tGERMINATION_NORMAL\tGERMINATION_UPPER\tTRANSPLANTING_LOWER\tTRANSPLANTING_OPTIMAL\tTRANSPLANTING_UPPER\tMATURITY_LOWER\tMATURITY_OPTIMAL\tMATURITY_UPPER\tFROST_TOLERANCE\tFLOWERING\tPOLLINATION_PRIMARY\tPOLLINATION_SECONDARY\n");

	(void) init_record(&record);
	n = ghdb_select_first(&record);
	while (n == 0)
	{
		//			.   1   2   3   4   5   6   7   8   9   0   1   2   3   4   5   6   7   8   9   0   1   2   3   4   5   6   7   8   9   0   1   2   3   4   5   6   7   
		fprintf(file, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
			trim(record.plant_name),
			trim(record.latin_name),
			trim(record.height),
			trim(record.width),
			trim(record.planting_depth),
			trim(record.seed_size),
			trim(record.seed_need_light),
			trim(record.seed_scarification),
			trim(record.ph),
			trim(record.ec),
			trim(record.day_light_interval),
			trim(record.photoperiod_hours),
			trim(record.light_lower),
			trim(record.light_optimal),
			trim(record.light_upper),
			trim(record.nitrogen_lower),
			trim(record.nitrogen_optimal),
			trim(record.nitrogen_upper),
			trim(record.phosphorus_lower),
			trim(record.phosphorus_optimal),
			trim(record.phosphorus_upper),
			trim(record.potassium_lower),
			trim(record.potassium_optimal),
			trim(record.potassium_upper),
			trim(record.germination_lower),
			trim(record.germination_normal),
			trim(record.germination_upper),
			trim(record.transplanting_lower),
			trim(record.transplanting_optimal),
			trim(record.transplanting_upper),
			trim(record.maturity_lower),
			trim(record.maturity_optimal),
			trim(record.maturity_upper),
			trim(record.frost_tolerance),
			trim(record.flowering),
			trim(record.pollination_primary),
			trim(record.pollination_secondary));

		n = ghdb_select_next(&record);
	}
	fclose(file);

	return 0;
}

int tsv_import()
{
	char filename[9] = "ghdb.tsv";
	FILE *file = NULL;
	char line[100 * NUM_FIELDS];
	
	char plant_name[100];
	char latin_name[100];
	char height[100];
	char width[100];
	char planting_depth[100];
	char seed_size[100];
	char seed_need_light[100];
	char seed_scarification[100];
	char ph[100];
	char ec[100];
	char day_light_interval[100];
	char photoperiod_hours[100];
	char light_lower[100];
	char light_optimal[100];
	char light_upper[100];
	char nitrogen_lower[100];
	char nitrogen_optimal[100];
	char nitrogen_upper[100];
	char phosphorus_lower[100];
	char phosphorus_optimal[100];
	char phosphorus_upper[100];
	char potassium_lower[100];
	char potassium_optimal[100];
	char potassium_upper[100];
	char germination_lower[100];
	char germination_normal[100];
	char germination_upper[100];
	char transplanting_lower[100];
	char transplanting_optimal[100];
	char transplanting_upper[100];
	char maturity_lower[100];
	char maturity_optimal[100];
	char maturity_upper[100];
	char frost_tolerance[100];
	char flowering[100];
	char pollination_primary[100];
	char pollination_secondary[100];
	
	int n = 0;
//	size_t size = 0;
	struct RECORD record;

	errno = 0;
	file = fopen(filename, "r");
	if (file == NULL)
	{
		fprintf(stderr, "Could not open %s: %s\n", filename, strerror(errno));
		(void) endwin();
		exit (EXIT_FAILURE);
	}
//	while ((n = getline(line, &size, file)) != -1)
	while (fgets(line, sizeof(line), file) != NULL)
	{
		plant_name[0] = '\0';
		latin_name[0] = '\0';
		height[0] = '\0';
		width[0] = '\0';
		planting_depth[0] = '\0';
		seed_size[0] = '\0';
		seed_need_light[0] = '\0';
		seed_scarification[0] = '\0';
		ph[0] = '\0';
		ec[0] = '\0';
		day_light_interval[0] = '\0';
		photoperiod_hours[0] = '\0';
		light_lower[0] = '\0';
		light_optimal[0] = '\0';
		light_upper[0] = '\0';
		nitrogen_lower[0] = '\0';
		nitrogen_optimal[0] = '\0';
		nitrogen_upper[0] = '\0';
		phosphorus_lower[0] = '\0';
		phosphorus_optimal[0] = '\0';
		phosphorus_upper[0] = '\0';
		potassium_lower[0] = '\0';
		potassium_optimal[0] = '\0';
		potassium_upper[0] = '\0';
		germination_lower[0] = '\0';
		germination_normal[0] = '\0';
		germination_upper[0] = '\0';
		transplanting_lower[0] = '\0';
		transplanting_optimal[0] = '\0';
		transplanting_upper[0] = '\0';
		maturity_lower[0] = '\0';
		maturity_optimal[0] = '\0';
		maturity_upper[0] = '\0';
		frost_tolerance[0] = '\0';
		flowering[0] = '\0';
		pollination_primary[0] = '\0';
		pollination_secondary[0] = '\0';
		fprintf(stderr, "tsv_import: getline=%d\n", n);
//						   1   2   3   4   5   6   7   8   9   0   1   2   3   4   5   6   7   8   9   0   1   2   3   4   5   6   7   8   9   0   1   2   3   4   5   6   7   
		n = sscanf(line, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
				plant_name,
				latin_name,
				height,
				width,
				planting_depth,
				seed_size,
				seed_need_light,
				seed_scarification,
				ph,
				ec,
				day_light_interval,
				photoperiod_hours,
				light_lower,
				light_optimal,
				light_upper,
				nitrogen_lower,
				nitrogen_optimal,
				nitrogen_upper,
				phosphorus_lower,
				phosphorus_optimal,
				phosphorus_upper,
				potassium_lower,
				potassium_optimal,
				potassium_upper,
				germination_lower,
				germination_normal,
				germination_upper,
				transplanting_lower,
				transplanting_optimal,
				transplanting_upper,
				maturity_lower,
				maturity_optimal,
				maturity_upper,
				frost_tolerance,
				flowering,
				pollination_primary,
				pollination_secondary);
		fprintf(stderr, "tsv_import: sscanf=%d\n", n);
		fprintf(stderr, "tsv_import: plant_name=%s\n", plant_name);
		fprintf(stderr, "tsv_import: latin_name=%s\n", latin_name);
		fprintf(stderr, "tsv_import: height=%s\n", height);
		fprintf(stderr, "tsv_import: width=%s\n", width);
		fprintf(stderr, "planting_depth=%s\n", planting_depth);
		fprintf(stderr, "seed_size=%s\n", seed_size);
		fprintf(stderr, "seed_need_light=%s\n", seed_need_light);
		fprintf(stderr, "seed_scarification=%s\n", seed_scarification);
		fprintf(stderr, "ph=%s\n", ph);
		fprintf(stderr, "ec=%s\n", ec);
		fprintf(stderr, "day_light_interval=%s\n", day_light_interval);
		fprintf(stderr, "photoperiod_hours=%s\n", photoperiod_hours);
		fprintf(stderr, "light_lower=%s\n", light_lower);
		fprintf(stderr, "light_optimal=%s\n", light_optimal);
		fprintf(stderr, "light_upper=%s\n", light_upper);
		fprintf(stderr, "nitrogen_lower=%s\n", nitrogen_lower);
		fprintf(stderr, "nitrogen_optimal=%s\n", nitrogen_optimal);
		fprintf(stderr, "nitrogen_upper=%s\n", nitrogen_upper);
		fprintf(stderr, "phosphorus_lower=%s\n", phosphorus_lower);
		fprintf(stderr, "phosphorus_optimal=%s\n", phosphorus_optimal);
		fprintf(stderr, "phosphorus_upper=%s\n", phosphorus_upper);
		fprintf(stderr, "potassium_lower=%s\n", potassium_lower);
		fprintf(stderr, "potassium_optimal=%s\n", potassium_optimal);
		fprintf(stderr, "potassium_upper=%s\n", potassium_upper);
		fprintf(stderr, "germination_lower=%s\n", germination_lower);
		fprintf(stderr, "germination_normal=%s\n", germination_normal);
		fprintf(stderr, "germination_upper=%s\n", germination_upper);
		fprintf(stderr, "transplanting_lower=%s\n", transplanting_lower);
		fprintf(stderr, "transplanting_optimal=%s\n", transplanting_optimal);
		fprintf(stderr, "transplanting_upper=%s\n", transplanting_upper);
		fprintf(stderr, "maturity_lower=%s\n", maturity_lower);
		fprintf(stderr, "maturity_optimal=%s\n", maturity_optimal);
		fprintf(stderr, "maturity_upper=%s\n", maturity_upper);
		fprintf(stderr, "frost_tolerance=%s\n", frost_tolerance);
		fprintf(stderr, "flowering=%s\n", flowering);
		fprintf(stderr, "pollination_primary=%s\n", pollination_primary);
		fprintf(stderr, "pollination_secondary=%s\n", pollination_secondary);
		/*
		if (n == NUM_FIELDS)
		{
			*/
			n = strcmp("PLANT_NAME", plant_name);
			fprintf(stderr, "tsv_import: strcmp=%d\n", n);
			if (n != 0)
			{
				(void) init_record(&record);
				memcpy(record.plant_name, plant_name, strlen(plant_name));
				memcpy(record.latin_name, latin_name, strlen(latin_name));
				memcpy(record.height, height, strlen(height));
				memcpy(record.width, width, strlen(width));
				memcpy(record.planting_depth, planting_depth, strlen(planting_depth));
				memcpy(record.seed_size, seed_size, strlen(seed_size));
				memcpy(record.seed_need_light, seed_need_light, strlen(seed_need_light));
				memcpy(record.seed_scarification, seed_scarification, strlen(seed_scarification));
				memcpy(record.ph, ph, strlen(ph));
				memcpy(record.ec, ec, strlen(ec));
				memcpy(record.day_light_interval, day_light_interval, strlen(day_light_interval));
				memcpy(record.photoperiod_hours, photoperiod_hours, strlen(photoperiod_hours));
				memcpy(record.light_lower, light_lower, strlen(light_lower));
				memcpy(record.light_optimal, light_optimal, strlen(light_optimal));
				memcpy(record.light_upper, light_upper, strlen(light_upper));
				memcpy(record.nitrogen_lower, nitrogen_lower, strlen(nitrogen_lower));
				memcpy(record.nitrogen_optimal, nitrogen_optimal, strlen(nitrogen_optimal));
				memcpy(record.nitrogen_upper, nitrogen_upper, strlen(nitrogen_upper));
				memcpy(record.phosphorus_lower, phosphorus_lower, strlen(phosphorus_lower));
				memcpy(record.phosphorus_optimal, phosphorus_optimal, strlen(phosphorus_optimal));
				memcpy(record.phosphorus_upper, phosphorus_upper, strlen(phosphorus_upper));
				memcpy(record.potassium_lower, potassium_lower, strlen(potassium_lower));
				memcpy(record.potassium_optimal, potassium_optimal, strlen(potassium_optimal));
				memcpy(record.potassium_upper, potassium_upper, strlen(potassium_upper));
				memcpy(record.germination_lower, germination_lower, strlen(germination_lower));
				memcpy(record.germination_normal, germination_normal, strlen(germination_normal));
				memcpy(record.germination_upper, germination_upper, strlen(germination_upper));
				memcpy(record.transplanting_lower, transplanting_lower, strlen(transplanting_lower));
				memcpy(record.transplanting_optimal, transplanting_optimal, strlen(transplanting_optimal));
				memcpy(record.transplanting_upper, transplanting_upper, strlen(transplanting_upper));
				memcpy(record.maturity_lower, maturity_lower, strlen(maturity_lower));
				memcpy(record.maturity_optimal, maturity_optimal, strlen(maturity_optimal));
				memcpy(record.maturity_upper, maturity_upper, strlen(maturity_upper));
				memcpy(record.frost_tolerance, frost_tolerance, strlen(frost_tolerance));
				memcpy(record.flowering, flowering, strlen(flowering));
				memcpy(record.pollination_primary, pollination_primary, strlen(pollination_primary));
				memcpy(record.pollination_secondary, pollination_secondary, strlen(pollination_secondary));

				n = ghdb_update(&record);
				fprintf(stderr, "tsv_import: ghdb_update=%d\n", n);
			}
/*		}*/

		//free(line);
		line[0] = '\0';
	}
	fclose(file);

	return 0;
}


















