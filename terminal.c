#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "ghdb.h"

char *lpad(char *in, int outlen, char c)
{
	int i = 0;
	int inlen = strlen(in);
	char *out;
	if (outlen < 0)
	{
		return "";
	}
	out = malloc(outlen + 1);
	if (out == NULL)
	{
		fprintf(stderr, "lpad: Failed to allocate memory.\n");
		exit(1);
	}
	for (i = 0; i<outlen; i++)
	{
		*(out+i) = c;
		//printf("lpad: i=%d out=\"%s\"\n", i, out);
	}
	*(out+outlen) = '\0';
	for (i = outlen - 1, --inlen; i >= 0 && inlen >= 0; --i, --inlen)
	{
		*(out+i) = *(in+inlen);
		//printf("lpad: inlen=%d in=\"%s\" i=%d out=\"%s\"\n", inlen, in, i, out);
	}
	return out;
}

int terminal()
{
	mmask_t all = (mmask_t) ALL_MOUSE_EVENTS;
	//int r = 1;
	
	char spaces[2001];
	for (int i=0; i<sizeof(spaces) - 1; i++) {
		spaces[i] = 32;  // space
	}
	spaces[2000] = 0;

	int num_fields = 5;
	struct CURSOR cursor = { -1, -1 };
	struct CURSOR curmax = { -1, -1 };
	struct LABEL label = { -1, -1, -1, 0, "" };
	struct VALUE value = { -1, -1, -1, 0, "", "" };
	struct FIELD field = { label, value };
	struct FIELD fields[num_fields];
	//fprintf(stderr, "num_fields == %ld\n", num_fields);
	/* initialize the fields */
	for (int i=0; i<num_fields; i++)
	{
		fields[i] = field;
	}
	/* set up the fields */
	fields[0].label.y = 1;
	fields[0].label.x = 1;
	fields[0].label.l = 20;
	fields[0].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[0].label.l_value, lpad("First Name", 20, ' '), fields[0].label.l);
	fields[0].value.y = fields[0].label.y;
	fields[0].value.x = fields[0].label.x + fields[0].label.l + 1;
	fields[0].value.l = 30;
	fields[0].value.fac = (COLOR_PAIR(1) | A_UNDERLINE);
	strncpy(fields[0].value.c_value, spaces, fields[0].value.l);

	fields[1].label.y = fields[1 - 1].label.y + 1;
	fields[1].label.x = 1;
	fields[1].label.l = 20;
	fields[1].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[1].label.l_value, lpad("Last Name", 20, ' '), fields[1].label.l);
	fields[1].value.y = fields[1].label.y;
	fields[1].value.x = fields[1].label.x + fields[1].label.l + 1;
	fields[1].value.l = 30;
	fields[1].value.fac = (COLOR_PAIR(1) | A_UNDERLINE);
	strncpy(fields[1].value.c_value, spaces, fields[1].value.l);

	fields[2].label.y = fields[2 - 1].label.y + 1;
	fields[2].label.x = 1;
	fields[2].label.l = 20;
	fields[2].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[2].label.l_value, lpad("Birth Date", 20, ' '), fields[2].label.l);
	fields[2].value.y = fields[2].label.y;
	fields[2].value.x = fields[2].label.x + fields[2].label.l + 1;
	fields[2].value.l = 10;
	fields[2].value.fac = (COLOR_PAIR(1) | A_UNDERLINE);
	strncpy(fields[2].value.c_value, spaces, fields[2].value.l);

	fields[3].label.y = fields[3 - 1].label.y + 1;
	fields[3].label.x = 1;
	fields[3].label.l = 20;
	fields[3].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[3].label.l_value, lpad("Phone Number", 20, ' '), fields[3].label.l);
	fields[3].value.y = fields[3].label.y;
	fields[3].value.x = fields[3].label.x + fields[3].label.l + 1;
	fields[3].value.l = 15;
	fields[3].value.fac = (COLOR_PAIR(1) | A_UNDERLINE);
	strncpy(fields[3].value.c_value, spaces, fields[3].value.l);

	fields[4].label.y = fields[4 - 1].label.y + 1;
	fields[4].label.x = 1;
	fields[4].label.l = 20;
	fields[4].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[4].label.l_value, lpad("Email Address", 20, ' '), fields[4].label.l);
	fields[4].value.y = fields[4].label.y;
	fields[4].value.x = fields[4].label.x + fields[4].label.l + 1;
	fields[4].value.l = 60;
	fields[4].value.fac = (COLOR_PAIR(1) | A_UNDERLINE);
	strncpy(fields[4].value.c_value, spaces, fields[4].value.l);

	(void) initscr();
	(void) cbreak();
	(void) nonl();
	(void) noecho();
	(void) keypad(stdscr, true);
	(void) mousemask(all, /*@i@*/ NULL);

	if (has_colors())
	{
		(void) start_color();
		(void) init_pair(1, COLOR_YELLOW, COLOR_BLACK);
		(void) attrset(COLOR_PAIR(1));
	}

	// paint the screen using the fields
	for (int i=0; i<num_fields; i++)
	{
		if (fields[i].label.y != -1)
		{
			//(void) move(fields[i].value.y, 0);
			(void) move(fields[i].label.y, fields[i].label.x);
			(void) attron(fields[i].label.fac);
			(void) printw("%s", fields[i].label.l_value);
			(void) move(fields[i].label.y, fields[i].label.x + fields[i].label.l);
			(void) attroff(fields[i].label.fac);
		}
		if (fields[i].value.y != -1)
		{
			(void) move(fields[i].value.y, fields[i].value.x);
			(void) attron(fields[i].value.fac);
			(void) printw("%s", fields[i].value.c_value);
			(void) move(fields[i].value.y, fields[i].value.x + fields[i].value.l);
			(void) attroff(fields[i].value.fac);
			if (cursor.y == -1)
			{
				cursor.y = fields[i].value.y;
				cursor.x = fields[i].value.x;
			}
		}
	}
	(void) getmaxyx(stdscr, curmax.y, curmax.x);
	fprintf(stderr, "cursor max y, x = %d, %d", curmax.y, curmax.x);
	(void) move(curmax.y - 2, 2);
	(void) attron(COLOR_PAIR(1) | A_NORMAL);
	(void) printw("(16) Exit");
	//(void) attroff(COLOR_PAIR(1) | A_NORMAL);
	(void) move(cursor.y, cursor.x);
	(void) refresh();

	return keyboard(fields, num_fields, curmax, cursor);
}