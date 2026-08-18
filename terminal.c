#include <gdbm.h>
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ghdb.h"

/*
 Move record values to the virtual screen field values
 */
int rtof(struct FIELD fields[], int num_fields, struct RECORD *record)
{
//	fprintf(stderr, "rtof:\n");
	strcpy(fields[PLANT_NAME_FIELD].value.c_value, record->plant_name);
	strcpy(fields[LATIN_NAME_FIELD].value.c_value, record->latin_name);
	strcpy(fields[HEIGHT_FIELD].value.c_value, record->height);
	strcpy(fields[WIDTH_FIELD].value.c_value, record->width);
	strcpy(fields[PLANTING_DEPTH_FIELD].value.c_value, record->planting_depth);
	strcpy(fields[SEED_SIZE_FIELD].value.c_value, record->seed_size);
	strcpy(fields[SEED_NEED_LIGHT_FIELD].value.c_value, record->seed_need_light);
	strcpy(fields[SEED_SCARIFICATION_FIELD].value.c_value, record->seed_scarification);
	strcpy(fields[PH_FIELD].value.c_value, record->ph);
	strcpy(fields[EC_FIELD].value.c_value, record->ec);
	strcpy(fields[DAY_LIGHT_INTERVAL_FIELD].value.c_value, record->day_light_interval);
	strcpy(fields[PHOTOPERIOD_HOURS_FIELD].value.c_value, record->photoperiod_hours);
	strcpy(fields[LIGHT_LOWER_FIELD].value.c_value, record->light_lower);
	strcpy(fields[LIGHT_OPTIMAL_FIELD].value.c_value, record->light_optimal);
	strcpy(fields[LIGHT_UPPER_FIELD].value.c_value, record->light_upper);
	strcpy(fields[NITROGEN_LOWER_FIELD].value.c_value, record->nitrogen_lower);
	strcpy(fields[NITROGEN_OPTIMAL_FIELD].value.c_value, record->nitrogen_optimal);
	strcpy(fields[NITROGEN_UPPER_FIELD].value.c_value, record->nitrogen_upper);
	strcpy(fields[PHOSPHORUS_LOWER_FIELD].value.c_value, record->phosphorus_lower);
	strcpy(fields[PHOSPHORUS_OPTIMAL_FIELD].value.c_value, record->phosphorus_optimal);
	strcpy(fields[PHOSPHORUS_UPPER_FIELD].value.c_value, record->phosphorus_upper);
	strcpy(fields[POTASSIUM_LOWER_FIELD].value.c_value, record->potassium_lower);
	strcpy(fields[POTASSIUM_OPTIMAL_FIELD].value.c_value, record->potassium_optimal);
	strcpy(fields[POTASSIUM_UPPER_FIELD].value.c_value, record->potassium_upper);
	strcpy(fields[GERMINATION_LOWER_FIELD].value.c_value, record->germination_lower);
	strcpy(fields[GERMINATION_NORMAL_FIELD].value.c_value, record->germination_normal);
	strcpy(fields[GERMINATION_UPPER_FIELD].value.c_value, record->germination_upper);
	strcpy(fields[TRANSPLANTING_LOWER_FIELD].value.c_value, record->transplanting_lower);
	strcpy(fields[TRANSPLANTING_OPTIMAL_FIELD].value.c_value, record->transplanting_optimal);
	strcpy(fields[TRANSPLANTING_UPPER_FIELD].value.c_value, record->transplanting_upper);
	strcpy(fields[MATURITY_LOWER_FIELD].value.c_value, record->maturity_lower);
	strcpy(fields[MATURITY_OPTIMAL_FIELD].value.c_value, record->maturity_optimal);
	strcpy(fields[MATURITY_UPPER_FIELD].value.c_value, record->maturity_upper);
	strcpy(fields[FROST_TOLERANCE_FIELD].value.c_value, record->frost_tolerance);
	strcpy(fields[FLOWERING_FIELD].value.c_value, record->flowering);
	strcpy(fields[POLLINATION_PRIMARY_FIELD].value.c_value, record->pollination_primary);
	strcpy(fields[POLLINATION_SECONDARY_FIELD].value.c_value, record->pollination_secondary);

	return 0;
}

/*
 Move the virtual screen field values to the record values
 */
int ftor(struct FIELD fields[], int num_fields, struct RECORD *record)
{
//	fprintf(stderr, "ftor:\n");
	strcpy(record->plant_name, fields[PLANT_NAME_FIELD].value.c_value);
	strcpy(record->latin_name, fields[LATIN_NAME_FIELD].value.c_value);
	strcpy(record->height, fields[HEIGHT_FIELD].value.c_value);
	strcpy(record->width, fields[WIDTH_FIELD].value.c_value);
	strcpy(record->planting_depth, fields[PLANTING_DEPTH_FIELD].value.c_value);
	strcpy(record->seed_size, fields[SEED_SIZE_FIELD].value.c_value);
	strcpy(record->seed_need_light, fields[SEED_NEED_LIGHT_FIELD].value.c_value);
	strcpy(record->seed_scarification, fields[SEED_SCARIFICATION_FIELD].value.c_value);
	strcpy(record->ph, fields[PH_FIELD].value.c_value);
	strcpy(record->ec, fields[EC_FIELD].value.c_value);
	strcpy(record->day_light_interval, fields[DAY_LIGHT_INTERVAL_FIELD].value.c_value);
	strcpy(record->photoperiod_hours, fields[PHOTOPERIOD_HOURS_FIELD].value.c_value);
	strcpy(record->light_lower, fields[LIGHT_LOWER_FIELD].value.c_value);
	strcpy(record->light_optimal, fields[LIGHT_OPTIMAL_FIELD].value.c_value);
	strcpy(record->light_upper, fields[LIGHT_UPPER_FIELD].value.c_value);
	strcpy(record->nitrogen_lower, fields[NITROGEN_LOWER_FIELD].value.c_value);
	strcpy(record->nitrogen_optimal, fields[NITROGEN_OPTIMAL_FIELD].value.c_value);
	strcpy(record->nitrogen_upper, fields[NITROGEN_UPPER_FIELD].value.c_value);
	strcpy(record->phosphorus_lower, fields[PHOSPHORUS_LOWER_FIELD].value.c_value);
	strcpy(record->phosphorus_optimal, fields[PHOSPHORUS_OPTIMAL_FIELD].value.c_value);
	strcpy(record->phosphorus_upper, fields[PHOSPHORUS_UPPER_FIELD].value.c_value);
	strcpy(record->potassium_lower, fields[POTASSIUM_LOWER_FIELD].value.c_value);
	strcpy(record->potassium_optimal, fields[POTASSIUM_OPTIMAL_FIELD].value.c_value);
	strcpy(record->potassium_upper, fields[POTASSIUM_UPPER_FIELD].value.c_value);
	strcpy(record->germination_lower, fields[GERMINATION_LOWER_FIELD].value.c_value);
	strcpy(record->germination_normal, fields[GERMINATION_NORMAL_FIELD].value.c_value);
	strcpy(record->germination_upper, fields[GERMINATION_UPPER_FIELD].value.c_value);
	strcpy(record->transplanting_lower, fields[TRANSPLANTING_LOWER_FIELD].value.c_value);
	strcpy(record->transplanting_optimal, fields[TRANSPLANTING_OPTIMAL_FIELD].value.c_value);
	strcpy(record->transplanting_upper, fields[TRANSPLANTING_UPPER_FIELD].value.c_value);
	strcpy(record->maturity_lower, fields[MATURITY_LOWER_FIELD].value.c_value);
	strcpy(record->maturity_optimal, fields[MATURITY_OPTIMAL_FIELD].value.c_value);
	strcpy(record->maturity_upper, fields[MATURITY_UPPER_FIELD].value.c_value);
	strcpy(record->frost_tolerance, fields[FROST_TOLERANCE_FIELD].value.c_value);
	strcpy(record->flowering, fields[FLOWERING_FIELD].value.c_value);
	strcpy(record->pollination_primary, fields[POLLINATION_PRIMARY_FIELD].value.c_value);
	strcpy(record->pollination_secondary, fields[POLLINATION_SECONDARY_FIELD].value.c_value);

	return 0;
}

/*
 STDSCR message
 */
int xerror(const char *message)
{
//	fprintf(stderr, "xerror:\n");
	int i = 0;
	int l = 75;
	char *local = malloc(l + 1);
	struct CURSOR cursor;
	getyx(stdscr, cursor.y, cursor.x);
	struct CURSOR curmax;
	getmaxyx(stdscr, curmax.y, curmax.x);
	strcpy(local, message);
	for (i=strlen(local); i<l; i++)
	{
		*(local+i) = ' ';
	}
	*(local+l) = '\0';

	(void) move(curmax.y - 1, 0);
	(void) attroff(COLOR_PAIR(1) | A_REVERSE);
	(void) attron(COLOR_PAIR(1) | A_BOLD);
	(void) printw("%s", local);
	(void) move(curmax.y - 1, curmax.x - 4);
	(void) attroff(COLOR_PAIR(1) | A_BOLD);
	free(local);
	(void) move(cursor.y, cursor.x);
	refresh();

	return 0;
}

/*
 Paint the screen
 */
int paint(struct FIELD fields[], int num_fields, struct CURSOR *cursor, unsigned action)
{	
//	fprintf(stderr, "paint:\n");
	struct CURSOR curmax;
	(void) getmaxyx(stdscr, curmax.y, curmax.x);
	if (curmax.y < 23 || curmax.x < 79)
	{
		(void) move(0, 0);
		(void) attron(COLOR_PAIR(1) | A_NORMAL);
		printw("%s", "The terminal window is too small, please adjust it to 24 x 80.");
		(void) move(0, curmax.x - 1);
		(void) attroff(COLOR_PAIR(1) | A_NORMAL);
		return -1;
	}

	char message[curmax.x];
	//                         1         2         3         4         5         6         7         8
	//                12345678901234567890123456789012345678901234567890123456789012345678901234567890
	char title[80] = "                           GREEN HOUSE PLANT DATABASE                 ";

	if (has_colors())
	{
		(void) start_color();
		(void) init_pair(1, COLOR_YELLOW, COLOR_BLACK);
		(void) attrset(COLOR_PAIR(1));
	}

	cursor->y = 0;
	cursor->x = 0;

	if      (action & DELETE_MODE)
	{
		(void) strcat(title, "DELETE");
	}
	else if (action & INSERT_MODE)
	{
		(void) strcat(title, "   ADD");
	}
	else if (action & SELECT_MODE)
	{
		(void) strcat(title, "  FIND");
	}
	else
	{
		(void) strcat(title, "  WTF?");
	}
	(void) move(cursor->y, cursor->x + 1);
	(void) attron(COLOR_PAIR(1) | A_NORMAL);
	printw("%s", title);
	(void) move(cursor->y, curmax.x - 4);
	(void) attroff(COLOR_PAIR(1) | A_NORMAL);

	for (int i=0; i<num_fields; i++)
	{
		if (fields[i].label.y != -1)
		{
			sprintf(message, "Label %d", i);
			(void) move(fields[i].label.y, fields[i].label.x);
			(void) attron(fields[i].label.fac);
			(void) printw("%s", fields[i].label.l_value);
			(void) move(fields[i].label.y, fields[i].label.x + fields[i].label.l);
			(void) attroff(fields[i].label.fac);
		}
		if (fields[i].value.y != -1)
		{
			sprintf(message, "Field %d", i);
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
		if (fields[i].uom.y != -1)
		{
			sprintf(message, "UOM %d", i);
			(void) move(fields[i].uom.y, fields[i].uom.x);
			(void) attron(fields[i].uom.fac);
			(void) printw("%s", fields[i].uom.u_value);
			(void) move(fields[i].uom.y, fields[i].uom.x + fields[i].uom.l);
			(void) attroff(fields[i].uom.fac);
		}
	}

	(void) move(curmax.y - 2, 0);
	(void) attron(COLOR_PAIR(1) | A_NORMAL);
	//                      1         2         3         4         5         6         7         8
	//             12345678901234567890123456789012345678901234567890123456789012345678901234567890
	(void) printw("(1)New  (2)First (3)Find (4)Prev (5)Next (6)Delete (7)Import (8)Export  (16)Exit");
	(void) move(curmax.y - 2, curmax.y - 2 + 9);
	(void) attroff(COLOR_PAIR(1) | A_NORMAL);
	
	(void) move(fields[0].value.y, fields[0].value.x + \
		 fldlen(fields[0].value.y, fields[0].value.x, fields[0].value.l));

	return 0;
}

int terminal()
{
//	fprintf(stderr, "terminal:\n");
	unsigned action = INSERT_MODE; // start out in insert mode
	int num_fields = NUM_FIELDS;
	int nrecords = 0;
	struct FIELD fields[num_fields];
	struct CURSOR cursor = { -1, -1 };
	struct RECORD record = { "", "", "", "", "" };
	mmask_t all = (mmask_t) ALL_MOUSE_EVENTS;

	/* initialize ncurses */
	(void) initscr();
	(void) cbreak();
	(void) nonl();
	(void) noecho();
	(void) keypad(stdscr, true);
	(void) mousemask(all, /*@i@*/ NULL);

	(void) formscrn(fields, num_fields);
	do
	{
		switch (action)
		{
			case (SELECT_MODE | FIND_RECORD):
//				fprintf(stderr, "SELECT_MODE | FIND_RECORD\n");
				(void) init_record(&record);
				(void) rtof(fields, num_fields, &record);
				(void) paint(fields, num_fields, &cursor, action);
				xerror("Press (ENTER) to execute query.");
				break;
			case (SELECT_MODE | ENTER):
//				fprintf(stderr, "SELECT_MODE | ENTER\n");
				(void) ftor(fields, num_fields, &record);
				nrecords = ghdb_select(&record);
				(void) rtof(fields, num_fields, &record);
				if (nrecords == 0)
				{
					action = (SELECT_MODE | FIND_RECORD);
				}
				else
				{
					action = (SELECT_MODE | FIRST_RECORD);
				}
				(void) paint(fields, num_fields, &cursor, action);
				break;
			case (SELECT_MODE | FIRST_RECORD):
//				fprintf(stderr, "SELECT_MODE | FIRST_RECORD\n");
				(void) init_record(&record);
				(void) rtof(fields, num_fields, &record);
				(void) ghdb_select_first(&record);
				(void) rtof(fields, num_fields, &record);
				(void) paint(fields, num_fields, &cursor, action);
				//xerror("Press (ENTER) to update.");
				break;
			case (SELECT_MODE | NEXT_RECORD):
//				fprintf(stderr, "SELECT_MODE | NEXT_RECORD\n");
				(void) ghdb_select_next(&record);
				(void) rtof(fields, num_fields, &record);
				(void) paint(fields, num_fields, &cursor, action);
				//xerror("Press (ENTER) to update.");
				break;
			case (SELECT_MODE | PREVIOUS_RECORD):
//				fprintf(stderr, "SELECT_MODE | PREVIOUS_RECORD\n");
				(void) ghdb_select_previous(&record);
				(void) rtof(fields, num_fields, &record);
				(void) paint(fields, num_fields, &cursor, action);
				//xerror("Press (ENTER) to update.");
				break;
			case (SELECT_MODE | UPDATE_RECORD):
//				fprintf(stderr, "SELECT_MODE | UPDATE_RECORD\n");
				(void) ftor(fields, num_fields, &record);
				(void) ghdb_update(&record);
				action = (SELECT_MODE | FIRST_RECORD);
				(void) paint(fields, num_fields, &cursor, action);
				break;
			case (INSERT_MODE):
//				fprintf(stderr, "INSERT_MODE\n");
				(void) init_record(&record);
				(void) rtof(fields, num_fields, &record);
				(void) paint(fields, num_fields, &cursor, action);
				xerror("Press (ENTER) to insert record.");
				break;
			case (INSERT_MODE | ENTER):
//				fprintf(stderr, "SELECT_MODE | ENTER\n");
				(void) ftor(fields, num_fields, &record);
				(void) ghdb_insert(&record);
				(void) init_record(&record);
				(void) rtof(fields, num_fields, &record);
				action = INSERT_MODE;
				(void) paint(fields, num_fields, &cursor, action);
				break;
			case (DELETE_MODE):
				(void) paint(fields, num_fields, &cursor, action);
				xerror("Press (ENTER) to confirm delete.");
				break;
			case (DELETE_MODE | ENTER):
				(void) ghdb_delete(&record);
				(void) init_record(&record);
				(void) rtof(fields, num_fields, &record);
				action = INSERT_MODE;
				(void) paint(fields, num_fields, &cursor, action);
				break;
			case (EXPORT_MODE):
				(void) tsv_export();
				(void) init_record(&record);
				(void) rtof(fields, num_fields, &record);
				action = INSERT_MODE;
				(void) paint(fields, num_fields, &cursor, action);
				xerror("Records exported as tab-separated values.");
				break;
			case (IMPORT_MODE):
				(void) tsv_import();
				(void) init_record(&record);
				(void) rtof(fields, num_fields, &record);
				action = INSERT_MODE;
				(void) paint(fields, num_fields, &cursor, action);
				xerror("Records imported from tab-separated values.");
				break;
			default:
				fprintf(stderr, "DEFAULT_MODE\n");
				xerror("ERROR: DEFAULT_MODE, DOES NOT EXIST.");
		}

		(void) refresh();
		action = keyboard(fields, num_fields, &cursor, action);
//		fprintf(stderr, "action=%d\n", action);
	} while (action > 0);

	return 0;
}