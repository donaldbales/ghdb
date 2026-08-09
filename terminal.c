#include <gdbm.h>
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ghdb.h"
/*
struct RECORD
{
	char plant_name[PLANT_NAME_LENGTH + 1];
	char latin_name[LATIN_NAME_LENGTH + 1];
	char height[HEIGHT_LENGTH + 1];
	char width[WIDTH_LENGTH + 1];
	char planting_depth[PLANTING_DEPTH + 1];
};
*/
int rtof(struct FIELD fields[], int num_fields, struct RECORD *record)
{
	/*
	strncpy(fields[PLANT_NAME_FIELD].value.c_value, record->plant_name, PLANT_NAME_LENGTH);
	strncpy(fields[LATIN_NAME_FIELD].value.c_value, record->latin_name, LATIN_NAME_LENGTH);
	strncpy(fields[HEIGHT_FIELD].value.c_value, record->height, HEIGHT_LENGTH);
	strncpy(fields[WIDTH_FIELD].value.c_value, record->width, WIDTH_LENGTH);
	strncpy(fields[PLANTING_DEPTH_FIELD].value.c_value, record->planting_depth, PLANTING_DEPTH_LENGTH);
	*/
	strcpy(fields[PLANT_NAME_FIELD].value.c_value, record->plant_name);
	strcpy(fields[LATIN_NAME_FIELD].value.c_value, record->latin_name);
	strcpy(fields[HEIGHT_FIELD].value.c_value, record->height);
	strcpy(fields[WIDTH_FIELD].value.c_value, record->width);
	strcpy(fields[PLANTING_DEPTH_FIELD].value.c_value, record->planting_depth);

	return 0;
}

int ftor(struct FIELD fields[], int num_fields, struct RECORD *record)
{
	/*
	strncpy(record->plant_name, fields[PLANT_NAME_FIELD].value.c_value, PLANT_NAME_LENGTH);
	strncpy(record->latin_name, fields[LATIN_NAME_FIELD].value.c_value, LATIN_NAME_LENGTH);
	strncpy(record->height, fields[HEIGHT_FIELD].value.c_value, HEIGHT_LENGTH);
	strncpy(record->width, fields[WIDTH_FIELD].value.c_value, WIDTH_LENGTH);
	strncpy(record->planting_depth, fields[PLANTING_DEPTH_FIELD].value.c_value, PLANTING_DEPTH_LENGTH);
	*/
	strcpy(record->plant_name, fields[PLANT_NAME_FIELD].value.c_value);
	strcpy(record->latin_name, fields[LATIN_NAME_FIELD].value.c_value);
	strcpy(record->height, fields[HEIGHT_FIELD].value.c_value);
	strcpy(record->width, fields[WIDTH_FIELD].value.c_value);
	strcpy(record->planting_depth, fields[PLANTING_DEPTH_FIELD].value.c_value);

	return 0;
}

int xerror(char *message)
{
	int i = 0;
	int l = 75;
	char *local = malloc(l + 1);
	struct CURSOR curmax;
	getmaxyx(stdscr, curmax.y, curmax.x);
	strcpy(local, message);
	for (i=strlen(local); i<l; i++)
	{
		*(local+i) = ' ';
	}
	*(local+l) = '\0';

	(void) move(curmax.y - 1, 3);
	(void) attroff(COLOR_PAIR(1) | A_REVERSE);
	(void) attron(COLOR_PAIR(1) | A_BOLD);
	(void) printw("%s", local);
	(void) move(curmax.y - 1, curmax.x - 4);
	(void) attroff(COLOR_PAIR(1) | A_BOLD);
	free(local);
	refresh();
	//sleep(1);

	return 0;
}

int paint(struct FIELD fields[], int num_fields, struct CURSOR curmax, struct CURSOR *cursor)
{
	char message[curmax.x];

	if (has_colors())
	{
		(void) start_color();
		(void) init_pair(1, COLOR_YELLOW, COLOR_BLACK);
		(void) attrset(COLOR_PAIR(1));
	}

	cursor->x = 0;
	cursor->y = 0;

	//xerror("Upper left");
	(void) move(cursor->y, cursor->x);
	//refresh();
	//sleep(1);
	(void) attron(COLOR_PAIR(1) | A_NORMAL);
	printw("%d", cursor->y);
	//(void) move(cursor->y, cursor->x + 1);
	//(void) attroff(COLOR_PAIR(1) | A_NORMAL);

	//xerror("Upper right");
	(void) move(cursor->y, curmax.x - 3);
	//refresh();
	//sleep(1);
	(void) attron(COLOR_PAIR(1) | A_NORMAL);
	printw("%d", curmax.x - 1);
	(void) attroff(COLOR_PAIR(1) | A_NORMAL);

	//xerror("Lower left");
	(void) move(curmax.y - 1, cursor->x);
	//refresh();
	//sleep(1);
	(void) attron(COLOR_PAIR(1) | A_NORMAL);
	(void) printw("%d", curmax.y - 1);
	(void) move(curmax.y - 1, curmax.x - 1);
	(void) attroff(COLOR_PAIR(1) | A_NORMAL);
	
	//xerror("Lower right");
	(void) move(curmax.y - 1, curmax.x - 3);
	//refresh();
	//sleep(1);
	(void) attron(COLOR_PAIR(1) | A_NORMAL);
	(void) printw("%d", curmax.x - 1);
	(void) move(curmax.y - 1, curmax.x - 1);
	(void) attroff(COLOR_PAIR(1) | A_NORMAL);
	
	for (int i=0; i<num_fields; i++)
	{
		if (fields[i].label.y != -1)
		{
			sprintf(message, "Label %d", i);
			//xerror(message);
			(void) move(fields[i].label.y, fields[i].label.x);
			(void) attron(fields[i].label.fac);
			(void) printw("%s", fields[i].label.l_value);
			(void) move(fields[i].label.y, fields[i].label.x + fields[i].label.l);
			(void) attroff(fields[i].label.fac);
		}
		//refresh();
		//sleep(1);
		if (fields[i].value.y != -1)
		{
			sprintf(message, "Field %d", i);
			//xerror(message);
			(void) move(fields[i].value.y, fields[i].value.x);
			(void) attron(fields[i].value.fac);
			(void) printw("%s", fields[i].value.c_value);
			(void) move(fields[i].value.y, fields[i].value.x + fields[i].value.l);
			(void) attroff(fields[i].value.fac);
			if (cursor->y == 0)
			{
				cursor->y = fields[i].value.y;
				cursor->x = fields[i].value.x;
			}
		}
		//refresh();
		//sleep(1);
		if (fields[i].uom.y != -1)
		{
			sprintf(message, "UOM %d", i);
			//xerror(message);
			(void) move(fields[i].uom.y, fields[i].uom.x);
			(void) attron(fields[i].uom.fac);
			(void) printw("%s", fields[i].uom.u_value);
			(void) move(fields[i].uom.y, fields[i].uom.x + fields[i].uom.l);
			(void) attroff(fields[i].uom.fac);
		}
		//refresh();
		//sleep(1);
	}
	//xerror("PFKeys");
	(void) move(curmax.y - 2, 2);
	//refresh();
	//sleep(1);
	(void) attron(COLOR_PAIR(1) | A_NORMAL);
	(void) printw("(16) Exit");
	(void) move(curmax.y - 2, curmax.y - 2 + 9);
	(void) attroff(COLOR_PAIR(1) | A_NORMAL);
	
	//xerror("Position Cursor");
	(void) move(cursor->y, cursor->x);
	//refresh();
	//sleep(1);

	return 0;
}

int terminal()
{
	int action = INSERT_MODE;
	int num_fields = 5;
	struct FIELD fields[num_fields];
	struct CURSOR cursor = { -1, -1 };
	struct CURSOR curmax = { -1, -1 };
	struct RECORD record = { "", "", "", "", "" };
	mmask_t all = (mmask_t) ALL_MOUSE_EVENTS;


	(void) initscr();
	(void) cbreak();
	(void) nonl();
	(void) noecho();
	(void) keypad(stdscr, true);
	(void) mousemask(all, /*@i@*/ NULL);

	(void) getmaxyx(stdscr, curmax.y, curmax.x);
	fprintf(stderr, "TERMINAL: curmax.y=%d, curmax.x=%d\n", curmax.y, curmax.x);
	//(void) paint(fields, num_fields, &cursor);


	(void) formscrn(fields, num_fields);

	do
	{
		switch(action)
		{
			case (SELECT_MODE):
				break;
			case (SELECT_MODE | ENTER):
				break;
			case (SELECT_MODE | FIRST_RECORD):
				break;
			case (SELECT_MODE | NEXT_RECORD):
				break;
			case (SELECT_MODE | PREVIOUS_RECORD):
				break;
			case (SELECT_MODE | UPDATE_RECORD):
				break;
			case (INSERT_MODE):
				//xerror("INSERT_MODE");
				(void) init_record(&record);
				(void) rtof(fields, num_fields, &record);
				(void) paint(fields, num_fields, curmax, &cursor);
				break;
			case (INSERT_MODE | ENTER):
				//xerror("INSERT_MODE | ENTER");
				(void) ftor(fields, num_fields, &record);
				(void) ghdb_insert(&record);
				(void) init_record(&record);
				(void) rtof(fields, num_fields, &record);
				(void) paint(fields, num_fields, curmax, &cursor);
				action = INSERT_MODE;
				break;
			case (UPDATE_MODE):
				break;
			case (UPDATE_MODE | ENTER):
				break;
			case (DELETE_MODE):
				break;
			case (DELETE_MODE | ENTER):
				break;
		}

		(void) refresh();
	} while ((action = keyboard(fields, num_fields, curmax, cursor, action)) > 1);

	return 0;
}