#include <gdbm.h>
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
	}
	*(out+outlen) = '\0';
	for (i = outlen - 1, --inlen; i >= 0 && inlen >= 0; --i, --inlen)
	{
		*(out+i) = *(in+inlen);
	}
	return out;
}

int formscrn(struct FIELD fields[], int num_fields)
{
	char spaces[2001];
	for (int i=0; i<sizeof(spaces) - 1; i++) {
		spaces[i] = 32;  // space
	}
	spaces[2000] = 0;
	struct LABEL label = { -1, -1, -1, 0, "" };
	struct VALUE value = { -1, -1, -1, 0, "", "" };
	struct FIELD field = { label, value };
	/* initialize the fields */
	for (int i=0; i<num_fields; i++)
	{
		fields[i] = field;
	}
	if (has_colors())
	{
		(void) start_color();
		(void) init_pair(1, COLOR_YELLOW, COLOR_BLACK);
		(void) attrset(COLOR_PAIR(1));
	}
	/* set up the fields */
	fields[0].label.y = 1;
	fields[0].label.x = 1;
	fields[0].label.l = 20;
	fields[0].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[0].label.l_value, lpad("Plant Name", 20, ' '), fields[0].label.l);
	fields[0].value.y = fields[0].label.y;
	fields[0].value.x = fields[0].label.x + fields[0].label.l + 1;
	fields[0].value.l = 30;
	fields[0].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[0].value.c_value, spaces, fields[0].value.l);
	fields[0].uom.y = -1;
	
	fields[1].label.y = fields[1 - 1].label.y + 1;
	fields[1].label.x = 1;
	fields[1].label.l = 20;
	fields[1].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[1].label.l_value, lpad("Latin Name", 20, ' '), fields[1].label.l);
	fields[1].value.y = fields[1].label.y;
	fields[1].value.x = fields[1].label.x + fields[1].label.l + 1;
	fields[1].value.l = 30;
	fields[1].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[1].value.c_value, spaces, fields[1].value.l);
	fields[1].uom.y = -1;

	fields[2].label.y = fields[2 - 1].label.y + 1;
	fields[2].label.x = 1;
	fields[2].label.l = 20;
	fields[2].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[2].label.l_value, lpad("Height", 20, ' '), fields[2].label.l);
	fields[2].value.y = fields[2].label.y;
	fields[2].value.x = fields[2].label.x + fields[2].label.l + 1;
	fields[2].value.l = 5;
	fields[2].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[2].value.c_value, spaces, fields[2].value.l);
	fields[2].uom.y = fields[2].value.y;
	fields[2].uom.x = fields[2].value.x + fields[2].value.l + 1;
	fields[2].uom.l = 6;
	fields[2].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[2].uom.u_value, lpad("inches", 6, ' '), fields[2].uom.l);

	fields[3].label.y = fields[3 - 1].label.y + 1;
	fields[3].label.x = 1;
	fields[3].label.l = 20;
	fields[3].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[3].label.l_value, lpad("Width", 20, ' '), fields[3].label.l);
	fields[3].value.y = fields[3].label.y;
	fields[3].value.x = fields[3].label.x + fields[3].label.l + 1;
	fields[3].value.l = 5;
	fields[3].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[3].value.c_value, spaces, fields[3].value.l);
	fields[3].uom.y = fields[3].value.y;
	fields[3].uom.x = fields[3].value.x + fields[3].value.l + 1;
	fields[3].uom.l = 6;
	fields[3].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[3].uom.u_value, lpad("inches", 6, ' '), fields[3].uom.l);

	fields[4].label.y = fields[4 - 1].label.y + 1;
	fields[4].label.x = 1;
	fields[4].label.l = 20;
	fields[4].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[4].label.l_value, lpad("Planting Depth", 20, ' '), fields[4].label.l);
	fields[4].value.y = fields[4].label.y;
	fields[4].value.x = fields[4].label.x + fields[4].label.l + 1;
	fields[4].value.l = 5;
	fields[4].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[4].value.c_value, spaces, fields[4].value.l);
	fields[4].uom.y = fields[4].value.y;
	fields[4].uom.x = fields[4].value.x + fields[4].value.l + 1;
	fields[4].uom.l = 6;
	fields[4].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[4].uom.u_value, lpad("inches", 6, ' '), fields[4].uom.l);
	
	return 0;
}