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
	fields[PLANT_NAME_FIELD].label.y = 1;
	fields[PLANT_NAME_FIELD].label.x = 1;
	fields[PLANT_NAME_FIELD].label.l = 20;
	fields[PLANT_NAME_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[PLANT_NAME_FIELD].label.l_value, lpad("Plant Name", 20, ' '), fields[PLANT_NAME_FIELD].label.l);
	fields[PLANT_NAME_FIELD].value.y = fields[PLANT_NAME_FIELD].label.y;
	fields[PLANT_NAME_FIELD].value.x = fields[PLANT_NAME_FIELD].label.x + fields[PLANT_NAME_FIELD].label.l + 1;
	fields[PLANT_NAME_FIELD].value.l = 30;
	fields[PLANT_NAME_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[PLANT_NAME_FIELD].value.c_value, spaces, fields[PLANT_NAME_FIELD].value.l);
	fields[PLANT_NAME_FIELD].uom.y = -1;
	
	fields[LATIN_NAME_FIELD].label.y = fields[LATIN_NAME_FIELD - 1].label.y + 1;
	fields[LATIN_NAME_FIELD].label.x = 1;
	fields[LATIN_NAME_FIELD].label.l = 20;
	fields[LATIN_NAME_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[LATIN_NAME_FIELD].label.l_value, lpad("Latin Name", 20, ' '), fields[LATIN_NAME_FIELD].label.l);
	fields[LATIN_NAME_FIELD].value.y = fields[LATIN_NAME_FIELD].label.y;
	fields[LATIN_NAME_FIELD].value.x = fields[LATIN_NAME_FIELD].label.x + fields[LATIN_NAME_FIELD].label.l + 1;
	fields[LATIN_NAME_FIELD].value.l = 30;
	fields[LATIN_NAME_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[LATIN_NAME_FIELD].value.c_value, spaces, fields[LATIN_NAME_FIELD].value.l);
	fields[LATIN_NAME_FIELD].uom.y = -1;

	fields[HEIGHT_FIELD].label.y = fields[HEIGHT_FIELD - 1].label.y + 1;
	fields[HEIGHT_FIELD].label.x = 1;
	fields[HEIGHT_FIELD].label.l = 20;
	fields[HEIGHT_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[HEIGHT_FIELD].label.l_value, lpad("Height", 20, ' '), fields[HEIGHT_FIELD].label.l);
	fields[HEIGHT_FIELD].value.y = fields[HEIGHT_FIELD].label.y;
	fields[HEIGHT_FIELD].value.x = fields[HEIGHT_FIELD].label.x + fields[HEIGHT_FIELD].label.l + 1;
	fields[HEIGHT_FIELD].value.l = 5;
	fields[HEIGHT_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[HEIGHT_FIELD].value.c_value, spaces, fields[HEIGHT_FIELD].value.l);
	fields[HEIGHT_FIELD].uom.y = fields[HEIGHT_FIELD].value.y;
	fields[HEIGHT_FIELD].uom.x = fields[HEIGHT_FIELD].value.x + fields[HEIGHT_FIELD].value.l + 1;
	fields[HEIGHT_FIELD].uom.l = 6;
	fields[HEIGHT_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[HEIGHT_FIELD].uom.u_value, lpad("inches", 6, ' '), fields[HEIGHT_FIELD].uom.l);

	fields[WIDTH_FIELD].label.y = fields[WIDTH_FIELD - 1].label.y + 1;
	fields[WIDTH_FIELD].label.x = 1;
	fields[WIDTH_FIELD].label.l = 20;
	fields[WIDTH_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[WIDTH_FIELD].label.l_value, lpad("Width", 20, ' '), fields[WIDTH_FIELD].label.l);
	fields[WIDTH_FIELD].value.y = fields[WIDTH_FIELD].label.y;
	fields[WIDTH_FIELD].value.x = fields[WIDTH_FIELD].label.x + fields[WIDTH_FIELD].label.l + 1;
	fields[WIDTH_FIELD].value.l = 5;
	fields[WIDTH_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[WIDTH_FIELD].value.c_value, spaces, fields[WIDTH_FIELD].value.l);
	fields[WIDTH_FIELD].uom.y = fields[WIDTH_FIELD].value.y;
	fields[WIDTH_FIELD].uom.x = fields[WIDTH_FIELD].value.x + fields[WIDTH_FIELD].value.l + 1;
	fields[WIDTH_FIELD].uom.l = 6;
	fields[WIDTH_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[WIDTH_FIELD].uom.u_value, lpad("inches", 6, ' '), fields[WIDTH_FIELD].uom.l);

	fields[PLANTING_DEPTH_FIELD].label.y = fields[PLANTING_DEPTH_FIELD - 1].label.y + 1;
	fields[PLANTING_DEPTH_FIELD].label.x = 1;
	fields[PLANTING_DEPTH_FIELD].label.l = 20;
	fields[PLANTING_DEPTH_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[PLANTING_DEPTH_FIELD].label.l_value, lpad("Planting Depth", 20, ' '), fields[PLANTING_DEPTH_FIELD].label.l);
	fields[PLANTING_DEPTH_FIELD].value.y = fields[PLANTING_DEPTH_FIELD].label.y;
	fields[PLANTING_DEPTH_FIELD].value.x = fields[PLANTING_DEPTH_FIELD].label.x + fields[PLANTING_DEPTH_FIELD].label.l + 1;
	fields[PLANTING_DEPTH_FIELD].value.l = 5;
	fields[PLANTING_DEPTH_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[PLANTING_DEPTH_FIELD].value.c_value, spaces, fields[PLANTING_DEPTH_FIELD].value.l);
	fields[PLANTING_DEPTH_FIELD].uom.y = fields[PLANTING_DEPTH_FIELD].value.y;
	fields[PLANTING_DEPTH_FIELD].uom.x = fields[PLANTING_DEPTH_FIELD].value.x + fields[PLANTING_DEPTH_FIELD].value.l + 1;
	fields[PLANTING_DEPTH_FIELD].uom.l = 6;
	fields[PLANTING_DEPTH_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[PLANTING_DEPTH_FIELD].uom.u_value, lpad("inches", 6, ' '), fields[PLANTING_DEPTH_FIELD].uom.l);
	
	fields[SEED_SIZE_FIELD].label.y = fields[SEED_SIZE_FIELD - 1].label.y + 1;
	fields[SEED_SIZE_FIELD].label.x = 1;
	fields[SEED_SIZE_FIELD].label.l = 20;
	fields[SEED_SIZE_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[SEED_SIZE_FIELD].label.l_value, lpad("Seed Size", 20, ' '), fields[SEED_SIZE_FIELD].label.l);
	fields[SEED_SIZE_FIELD].value.y = fields[SEED_SIZE_FIELD].label.y;
	fields[SEED_SIZE_FIELD].value.x = fields[SEED_SIZE_FIELD].label.x + fields[SEED_SIZE_FIELD].label.l + 1;
	fields[SEED_SIZE_FIELD].value.l = SEED_SIZE_LENGTH;
	fields[SEED_SIZE_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[SEED_SIZE_FIELD].value.c_value, spaces, fields[SEED_SIZE_FIELD].value.l);
	fields[SEED_SIZE_FIELD].uom.y = fields[SEED_SIZE_FIELD].value.y;
	fields[SEED_SIZE_FIELD].uom.x = fields[SEED_SIZE_FIELD].value.x + fields[SEED_SIZE_FIELD].value.l + 1;
	fields[SEED_SIZE_FIELD].uom.l = 6;
	fields[SEED_SIZE_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[SEED_SIZE_FIELD].uom.u_value, lpad("inches", 6, ' '), fields[SEED_SIZE_FIELD].uom.l);
	
	fields[SEED_NEED_LIGHT_FIELD].label.y = fields[SEED_NEED_LIGHT_FIELD - 1].label.y + 1;
	fields[SEED_NEED_LIGHT_FIELD].label.x = 1;
	fields[SEED_NEED_LIGHT_FIELD].label.l = 20;
	fields[SEED_NEED_LIGHT_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[SEED_NEED_LIGHT_FIELD].label.l_value, lpad("Seed Needs Light", 20, ' '), fields[SEED_NEED_LIGHT_FIELD].label.l);
	fields[SEED_NEED_LIGHT_FIELD].value.y = fields[SEED_NEED_LIGHT_FIELD].label.y;
	fields[SEED_NEED_LIGHT_FIELD].value.x = fields[SEED_NEED_LIGHT_FIELD].label.x + fields[SEED_NEED_LIGHT_FIELD].label.l + 1;
	fields[SEED_NEED_LIGHT_FIELD].value.l = SEED_NEED_LIGHT_LENGTH;
	fields[SEED_NEED_LIGHT_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[SEED_NEED_LIGHT_FIELD].value.c_value, spaces, fields[SEED_NEED_LIGHT_FIELD].value.l);
	fields[SEED_NEED_LIGHT_FIELD].uom.y = fields[SEED_NEED_LIGHT_FIELD].value.y;
	fields[SEED_NEED_LIGHT_FIELD].uom.x = fields[SEED_NEED_LIGHT_FIELD].value.x + fields[SEED_NEED_LIGHT_FIELD].value.l + 3;
	fields[SEED_NEED_LIGHT_FIELD].uom.l = 6;
	fields[SEED_NEED_LIGHT_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[SEED_NEED_LIGHT_FIELD].uom.u_value, lpad("yes|no", 6, ' '), fields[SEED_NEED_LIGHT_FIELD].uom.l);
	
	fields[SEED_SCARIFICATION_FIELD].label.y = fields[SEED_SCARIFICATION_FIELD - 1].label.y + 1;
	fields[SEED_SCARIFICATION_FIELD].label.x = 1;
	fields[SEED_SCARIFICATION_FIELD].label.l = 20;
	fields[SEED_SCARIFICATION_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[SEED_SCARIFICATION_FIELD].label.l_value, lpad("Seed Scarification", 20, ' '), fields[SEED_SCARIFICATION_FIELD].label.l);
	fields[SEED_SCARIFICATION_FIELD].value.y = fields[SEED_SCARIFICATION_FIELD].label.y;
	fields[SEED_SCARIFICATION_FIELD].value.x = fields[SEED_SCARIFICATION_FIELD].label.x + fields[SEED_SCARIFICATION_FIELD].label.l + 1;
	fields[SEED_SCARIFICATION_FIELD].value.l = SEED_SCARIFICATION_LENGTH;
	fields[SEED_SCARIFICATION_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[SEED_SCARIFICATION_FIELD].value.c_value, spaces, fields[SEED_SCARIFICATION_FIELD].value.l);
	fields[SEED_SCARIFICATION_FIELD].uom.y = fields[SEED_SCARIFICATION_FIELD].value.y;
	fields[SEED_SCARIFICATION_FIELD].uom.x = fields[SEED_SCARIFICATION_FIELD].value.x + fields[SEED_SCARIFICATION_FIELD].value.l + 3;
	fields[SEED_SCARIFICATION_FIELD].uom.l = 6;
	fields[SEED_SCARIFICATION_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[SEED_SCARIFICATION_FIELD].uom.u_value, lpad("yes|no", 6, ' '), fields[SEED_SCARIFICATION_FIELD].uom.l);
	
	fields[PH_FIELD].label.y = fields[PH_FIELD - 1].label.y + 1;
	fields[PH_FIELD].label.x = 1;
	fields[PH_FIELD].label.l = 20;
	fields[PH_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[PH_FIELD].label.l_value, lpad("pH", 20, ' '), fields[PH_FIELD].label.l);
	fields[PH_FIELD].value.y = fields[PH_FIELD].label.y;
	fields[PH_FIELD].value.x = fields[PH_FIELD].label.x + fields[PH_FIELD].label.l + 1;
	fields[PH_FIELD].value.l = PH_LENGTH;
	fields[PH_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[PH_FIELD].value.c_value, spaces, fields[PH_FIELD].value.l);
	fields[PH_FIELD].uom.y = -1;
	
	fields[EC_FIELD].label.y = fields[EC_FIELD - 1].label.y + 1;
	fields[EC_FIELD].label.x = 1;
	fields[EC_FIELD].label.l = 20;
	fields[EC_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[EC_FIELD].label.l_value, lpad("EC", 20, ' '), fields[EC_FIELD].label.l);
	fields[EC_FIELD].value.y = fields[EC_FIELD].label.y;
	fields[EC_FIELD].value.x = fields[EC_FIELD].label.x + fields[EC_FIELD].label.l + 1;
	fields[EC_FIELD].value.l = EC_LENGTH;
	fields[EC_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[EC_FIELD].value.c_value, spaces, fields[EC_FIELD].value.l);
	fields[EC_FIELD].uom.y = -1;
	
	fields[DAY_LIGHT_INTERVAL_FIELD].label.y = fields[DAY_LIGHT_INTERVAL_FIELD - 1].label.y + 1;
	fields[DAY_LIGHT_INTERVAL_FIELD].label.x = 1;
	fields[DAY_LIGHT_INTERVAL_FIELD].label.l = 20;
	fields[DAY_LIGHT_INTERVAL_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[DAY_LIGHT_INTERVAL_FIELD].label.l_value, lpad("Day Light Interval", 20, ' '), fields[DAY_LIGHT_INTERVAL_FIELD].label.l);
	fields[DAY_LIGHT_INTERVAL_FIELD].value.y = fields[DAY_LIGHT_INTERVAL_FIELD].label.y;
	fields[DAY_LIGHT_INTERVAL_FIELD].value.x = fields[DAY_LIGHT_INTERVAL_FIELD].label.x + fields[DAY_LIGHT_INTERVAL_FIELD].label.l + 1;
	fields[DAY_LIGHT_INTERVAL_FIELD].value.l = DAY_LIGHT_INTERVAL_LENGTH;
	fields[DAY_LIGHT_INTERVAL_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[DAY_LIGHT_INTERVAL_FIELD].value.c_value, spaces, fields[DAY_LIGHT_INTERVAL_FIELD].value.l);
	fields[DAY_LIGHT_INTERVAL_FIELD].uom.y = fields[DAY_LIGHT_INTERVAL_FIELD].value.y;
	fields[DAY_LIGHT_INTERVAL_FIELD].uom.x = fields[DAY_LIGHT_INTERVAL_FIELD].value.x + fields[DAY_LIGHT_INTERVAL_FIELD].value.l + 1;
	fields[DAY_LIGHT_INTERVAL_FIELD].uom.l = 8;
	fields[DAY_LIGHT_INTERVAL_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[DAY_LIGHT_INTERVAL_FIELD].uom.u_value, lpad("mol/m2/s", fields[DAY_LIGHT_INTERVAL_FIELD].uom.l, ' '), fields[DAY_LIGHT_INTERVAL_FIELD].uom.l);
	
	fields[PHOTOPERIOD_HOURS_FIELD].label.y = fields[PHOTOPERIOD_HOURS_FIELD - 1].label.y + 1;
	fields[PHOTOPERIOD_HOURS_FIELD].label.x = 1;
	fields[PHOTOPERIOD_HOURS_FIELD].label.l = 20;
	fields[PHOTOPERIOD_HOURS_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[PHOTOPERIOD_HOURS_FIELD].label.l_value, lpad("Photoperiod", 20, ' '), fields[PHOTOPERIOD_HOURS_FIELD].label.l);
	fields[PHOTOPERIOD_HOURS_FIELD].value.y = fields[PHOTOPERIOD_HOURS_FIELD].label.y;
	fields[PHOTOPERIOD_HOURS_FIELD].value.x = fields[PHOTOPERIOD_HOURS_FIELD].label.x + fields[PHOTOPERIOD_HOURS_FIELD].label.l + 1;
	fields[PHOTOPERIOD_HOURS_FIELD].value.l = PHOTOPERIOD_HOURS_LENGTH;
	fields[PHOTOPERIOD_HOURS_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[PHOTOPERIOD_HOURS_FIELD].value.c_value, spaces, fields[PHOTOPERIOD_HOURS_FIELD].value.l);
	fields[PHOTOPERIOD_HOURS_FIELD].uom.y = fields[PHOTOPERIOD_HOURS_FIELD].value.y;
	fields[PHOTOPERIOD_HOURS_FIELD].uom.x = fields[PHOTOPERIOD_HOURS_FIELD].value.x + fields[PHOTOPERIOD_HOURS_FIELD].value.l + 1;
	fields[PHOTOPERIOD_HOURS_FIELD].uom.l = 5;
	fields[PHOTOPERIOD_HOURS_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[PHOTOPERIOD_HOURS_FIELD].uom.u_value, lpad("hours", fields[PHOTOPERIOD_HOURS_FIELD].uom.l, ' '), fields[PHOTOPERIOD_HOURS_FIELD].uom.l);
	
	fields[LIGHT_LOWER_FIELD].label.y = 14;
	fields[LIGHT_LOWER_FIELD].label.x = 1;
	fields[LIGHT_LOWER_FIELD].label.l = 20;
	fields[LIGHT_LOWER_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[LIGHT_LOWER_FIELD].label.l_value, lpad("Light", 20, ' '), fields[LIGHT_LOWER_FIELD].label.l);
	fields[LIGHT_LOWER_FIELD].value.y = 14;
	fields[LIGHT_LOWER_FIELD].value.x = 22;
	fields[LIGHT_LOWER_FIELD].value.l = LIGHT_LOWER_LENGTH;
	fields[LIGHT_LOWER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[LIGHT_LOWER_FIELD].value.c_value, spaces, fields[LIGHT_LOWER_FIELD].value.l);
	fields[LIGHT_LOWER_FIELD].uom.y = 13;
	fields[LIGHT_LOWER_FIELD].uom.x = 22;
	fields[LIGHT_LOWER_FIELD].uom.l = 5;
	fields[LIGHT_LOWER_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[LIGHT_LOWER_FIELD].uom.u_value, lpad("Lower", fields[LIGHT_LOWER_FIELD].uom.l, ' '), fields[LIGHT_LOWER_FIELD].uom.l);
	
	fields[LIGHT_OPTIMAL_FIELD].label.y = -1;
	
	fields[LIGHT_OPTIMAL_FIELD].value.y = 14;
	fields[LIGHT_OPTIMAL_FIELD].value.x = 29;
	fields[LIGHT_OPTIMAL_FIELD].value.l = LIGHT_OPTIMAL_LENGTH;
	fields[LIGHT_OPTIMAL_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[LIGHT_OPTIMAL_FIELD].value.c_value, spaces, fields[LIGHT_OPTIMAL_FIELD].value.l);
	fields[LIGHT_OPTIMAL_FIELD].uom.y = 13;
	fields[LIGHT_OPTIMAL_FIELD].uom.x = 28;
	fields[LIGHT_OPTIMAL_FIELD].uom.l = 7;
	fields[LIGHT_OPTIMAL_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[LIGHT_OPTIMAL_FIELD].uom.u_value, lpad("Optimal", fields[LIGHT_OPTIMAL_FIELD].uom.l, ' '), fields[LIGHT_OPTIMAL_FIELD].uom.l);
	
	fields[LIGHT_UPPER_FIELD].label.y = -1;
	
	fields[LIGHT_UPPER_FIELD].value.y = 14;
	fields[LIGHT_UPPER_FIELD].value.x = 36;
	fields[LIGHT_UPPER_FIELD].value.l = LIGHT_UPPER_LENGTH;
	fields[LIGHT_UPPER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[LIGHT_UPPER_FIELD].value.c_value, spaces, fields[LIGHT_UPPER_FIELD].value.l);
	fields[LIGHT_UPPER_FIELD].uom.y = 13;
	fields[LIGHT_UPPER_FIELD].uom.x = 36;
	fields[LIGHT_UPPER_FIELD].uom.l = 5;
	fields[LIGHT_UPPER_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[LIGHT_UPPER_FIELD].uom.u_value, lpad("Upper", fields[LIGHT_UPPER_FIELD].uom.l, ' '), fields[LIGHT_UPPER_FIELD].uom.l);

	fields[NITROGEN_LOWER_FIELD].label.y = 15;
	fields[NITROGEN_LOWER_FIELD].label.x = 1;
	fields[NITROGEN_LOWER_FIELD].label.l = 20;
	fields[NITROGEN_LOWER_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[NITROGEN_LOWER_FIELD].label.l_value, lpad("Nitrogen (N)", 20, ' '), fields[NITROGEN_LOWER_FIELD].label.l);
	fields[NITROGEN_LOWER_FIELD].value.y = 15;
	fields[NITROGEN_LOWER_FIELD].value.x = 22;
	fields[NITROGEN_LOWER_FIELD].value.l = NITROGEN_LOWER_LENGTH;
	fields[NITROGEN_LOWER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[NITROGEN_LOWER_FIELD].value.c_value, spaces, fields[NITROGEN_LOWER_FIELD].value.l);
	fields[NITROGEN_LOWER_FIELD].uom.y = 14;
	fields[NITROGEN_LOWER_FIELD].uom.x = 42;
	fields[NITROGEN_LOWER_FIELD].uom.l = 9;
	fields[NITROGEN_LOWER_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[NITROGEN_LOWER_FIELD].uom.u_value, lpad("umol/m2/s", fields[NITROGEN_LOWER_FIELD].uom.l, ' '), fields[NITROGEN_LOWER_FIELD].uom.l);
	
	fields[NITROGEN_OPTIMAL_FIELD].label.y = -1;

	fields[NITROGEN_OPTIMAL_FIELD].value.y = 15;
	fields[NITROGEN_OPTIMAL_FIELD].value.x = 29;
	fields[NITROGEN_OPTIMAL_FIELD].value.l = NITROGEN_OPTIMAL_LENGTH;
	fields[NITROGEN_OPTIMAL_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[NITROGEN_OPTIMAL_FIELD].value.c_value, spaces, fields[NITROGEN_OPTIMAL_FIELD].value.l);
	fields[NITROGEN_OPTIMAL_FIELD].uom.y = -1;
	
	fields[NITROGEN_UPPER_FIELD].label.y = -1;

	fields[NITROGEN_UPPER_FIELD].value.y = 15;
	fields[NITROGEN_UPPER_FIELD].value.x = 36;
	fields[NITROGEN_UPPER_FIELD].value.l = NITROGEN_UPPER_LENGTH;
	fields[NITROGEN_UPPER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[NITROGEN_UPPER_FIELD].value.c_value, spaces, fields[NITROGEN_UPPER_FIELD].value.l);
	fields[NITROGEN_UPPER_FIELD].uom.y = 15;
	fields[NITROGEN_UPPER_FIELD].uom.x = 42;
	fields[NITROGEN_UPPER_FIELD].uom.l = 3;
	fields[NITROGEN_UPPER_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[NITROGEN_UPPER_FIELD].uom.u_value, lpad("ppm", fields[NITROGEN_UPPER_FIELD].uom.l, ' '), fields[NITROGEN_UPPER_FIELD].uom.l);
	
	fields[PHOSPHORUS_LOWER_FIELD].label.y = 16;
	fields[PHOSPHORUS_LOWER_FIELD].label.x = 1;
	fields[PHOSPHORUS_LOWER_FIELD].label.l = 20;
	fields[PHOSPHORUS_LOWER_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[PHOSPHORUS_LOWER_FIELD].label.l_value, lpad("Phosphorus (P)", 20, ' '), fields[PHOSPHORUS_LOWER_FIELD].label.l);
	fields[PHOSPHORUS_LOWER_FIELD].value.y = 16;
	fields[PHOSPHORUS_LOWER_FIELD].value.x = 22;
	fields[PHOSPHORUS_LOWER_FIELD].value.l = PHOSPHORUS_LOWER_LENGTH;
	fields[PHOSPHORUS_LOWER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[PHOSPHORUS_LOWER_FIELD].value.c_value, spaces, fields[PHOSPHORUS_LOWER_FIELD].value.l);
	fields[PHOSPHORUS_LOWER_FIELD].uom.y = -1;
	
	fields[PHOSPHORUS_OPTIMAL_FIELD].label.y = -1;

	fields[PHOSPHORUS_OPTIMAL_FIELD].value.y = 16;
	fields[PHOSPHORUS_OPTIMAL_FIELD].value.x = 29;
	fields[PHOSPHORUS_OPTIMAL_FIELD].value.l = PHOSPHORUS_OPTIMAL_LENGTH;
	fields[PHOSPHORUS_OPTIMAL_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[PHOSPHORUS_OPTIMAL_FIELD].value.c_value, spaces, fields[PHOSPHORUS_OPTIMAL_FIELD].value.l);
	fields[PHOSPHORUS_OPTIMAL_FIELD].uom.y = -1;
	
	fields[PHOSPHORUS_UPPER_FIELD].label.y = -1;

	fields[PHOSPHORUS_UPPER_FIELD].value.y = 16;
	fields[PHOSPHORUS_UPPER_FIELD].value.x = 36;
	fields[PHOSPHORUS_UPPER_FIELD].value.l = PHOSPHORUS_UPPER_LENGTH;
	fields[PHOSPHORUS_UPPER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[PHOSPHORUS_UPPER_FIELD].value.c_value, spaces, fields[PHOSPHORUS_UPPER_FIELD].value.l);
	fields[PHOSPHORUS_UPPER_FIELD].uom.y = 16;
	fields[PHOSPHORUS_UPPER_FIELD].uom.x = 42;
	fields[PHOSPHORUS_UPPER_FIELD].uom.l = 3;
	fields[PHOSPHORUS_UPPER_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[PHOSPHORUS_UPPER_FIELD].uom.u_value, lpad("ppm", fields[PHOSPHORUS_UPPER_FIELD].uom.l, ' '), fields[PHOSPHORUS_UPPER_FIELD].uom.l);

	fields[POTASSIUM_LOWER_FIELD].label.y = 17;
	fields[POTASSIUM_LOWER_FIELD].label.x = 1;
	fields[POTASSIUM_LOWER_FIELD].label.l = 20;
	fields[POTASSIUM_LOWER_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[POTASSIUM_LOWER_FIELD].label.l_value, lpad("Potassium (K)", 20, ' '), fields[POTASSIUM_LOWER_FIELD].label.l);
	fields[POTASSIUM_LOWER_FIELD].value.y = 17;
	fields[POTASSIUM_LOWER_FIELD].value.x = 22;
	fields[POTASSIUM_LOWER_FIELD].value.l = POTASSIUM_LOWER_LENGTH;
	fields[POTASSIUM_LOWER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[POTASSIUM_LOWER_FIELD].value.c_value, spaces, fields[POTASSIUM_LOWER_FIELD].value.l);
	fields[POTASSIUM_LOWER_FIELD].uom.y = -1;
	
	fields[POTASSIUM_OPTIMAL_FIELD].label.y = -1;

	fields[POTASSIUM_OPTIMAL_FIELD].value.y = 17;
	fields[POTASSIUM_OPTIMAL_FIELD].value.x = 29;
	fields[POTASSIUM_OPTIMAL_FIELD].value.l = POTASSIUM_OPTIMAL_LENGTH;
	fields[POTASSIUM_OPTIMAL_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[POTASSIUM_OPTIMAL_FIELD].value.c_value, spaces, fields[POTASSIUM_OPTIMAL_FIELD].value.l);
	fields[POTASSIUM_OPTIMAL_FIELD].uom.y = -1;
	
	fields[POTASSIUM_UPPER_FIELD].label.y = -1;

	fields[POTASSIUM_UPPER_FIELD].value.y = 17;
	fields[POTASSIUM_UPPER_FIELD].value.x = 36;
	fields[POTASSIUM_UPPER_FIELD].value.l = POTASSIUM_UPPER_LENGTH;
	fields[POTASSIUM_UPPER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[POTASSIUM_UPPER_FIELD].value.c_value, spaces, fields[POTASSIUM_UPPER_FIELD].value.l);
	fields[POTASSIUM_UPPER_FIELD].uom.y = 17;
	fields[POTASSIUM_UPPER_FIELD].uom.x = 42;
	fields[POTASSIUM_UPPER_FIELD].uom.l = 3;
	fields[POTASSIUM_UPPER_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[POTASSIUM_UPPER_FIELD].uom.u_value, lpad("ppm", fields[POTASSIUM_UPPER_FIELD].uom.l, ' '), fields[POTASSIUM_UPPER_FIELD].uom.l);

	fields[GERMINATION_LOWER_FIELD].label.y = 18;
	fields[GERMINATION_LOWER_FIELD].label.x = 1;
	fields[GERMINATION_LOWER_FIELD].label.l = 20;
	fields[GERMINATION_LOWER_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[GERMINATION_LOWER_FIELD].label.l_value, lpad("Germination", 20, ' '), fields[GERMINATION_LOWER_FIELD].label.l);
	fields[GERMINATION_LOWER_FIELD].value.y = 18;
	fields[GERMINATION_LOWER_FIELD].value.x = 22;
	fields[GERMINATION_LOWER_FIELD].value.l = GERMINATION_LOWER_LENGTH;
	fields[GERMINATION_LOWER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[GERMINATION_LOWER_FIELD].value.c_value, spaces, fields[GERMINATION_LOWER_FIELD].value.l);
	fields[GERMINATION_LOWER_FIELD].uom.y = -1;
	
	fields[GERMINATION_NORMAL_FIELD].label.y = -1;

	fields[GERMINATION_NORMAL_FIELD].value.y = 18;
	fields[GERMINATION_NORMAL_FIELD].value.x = 29;
	fields[GERMINATION_NORMAL_FIELD].value.l = GERMINATION_NORMAL_LENGTH;
	fields[GERMINATION_NORMAL_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[GERMINATION_NORMAL_FIELD].value.c_value, spaces, fields[GERMINATION_NORMAL_FIELD].value.l);
	fields[GERMINATION_NORMAL_FIELD].uom.y = -1;
	
	fields[GERMINATION_UPPER_FIELD].label.y = -1;

	fields[GERMINATION_UPPER_FIELD].value.y = 18;
	fields[GERMINATION_UPPER_FIELD].value.x = 36;
	fields[GERMINATION_UPPER_FIELD].value.l = GERMINATION_UPPER_LENGTH;
	fields[GERMINATION_UPPER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[GERMINATION_UPPER_FIELD].value.c_value, spaces, fields[GERMINATION_UPPER_FIELD].value.l);
	fields[GERMINATION_UPPER_FIELD].uom.y = 18;
	fields[GERMINATION_UPPER_FIELD].uom.x = 42;
	fields[GERMINATION_UPPER_FIELD].uom.l = 4;
	fields[GERMINATION_UPPER_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[GERMINATION_UPPER_FIELD].uom.u_value, lpad("days", fields[GERMINATION_UPPER_FIELD].uom.l, ' '), fields[GERMINATION_UPPER_FIELD].uom.l);
	
	fields[TRANSPLANTING_LOWER_FIELD].label.y = 19;
	fields[TRANSPLANTING_LOWER_FIELD].label.x = 1;
	fields[TRANSPLANTING_LOWER_FIELD].label.l = 20;
	fields[TRANSPLANTING_LOWER_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[TRANSPLANTING_LOWER_FIELD].label.l_value, lpad("Transplanting", 20, ' '), fields[TRANSPLANTING_LOWER_FIELD].label.l);
	fields[TRANSPLANTING_LOWER_FIELD].value.y = 19;
	fields[TRANSPLANTING_LOWER_FIELD].value.x = 22;
	fields[TRANSPLANTING_LOWER_FIELD].value.l = TRANSPLANTING_LOWER_LENGTH;
	fields[TRANSPLANTING_LOWER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[TRANSPLANTING_LOWER_FIELD].value.c_value, spaces, fields[TRANSPLANTING_LOWER_FIELD].value.l);
	fields[TRANSPLANTING_LOWER_FIELD].uom.y = -1;
	
	fields[TRANSPLANTING_OPTIMAL_FIELD].label.y = -1;

	fields[TRANSPLANTING_OPTIMAL_FIELD].value.y = 19;
	fields[TRANSPLANTING_OPTIMAL_FIELD].value.x = 29;
	fields[TRANSPLANTING_OPTIMAL_FIELD].value.l = TRANSPLANTING_OPTIMAL_LENGTH;
	fields[TRANSPLANTING_OPTIMAL_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[TRANSPLANTING_OPTIMAL_FIELD].value.c_value, spaces, fields[TRANSPLANTING_OPTIMAL_FIELD].value.l);
	fields[TRANSPLANTING_OPTIMAL_FIELD].uom.y = -1;
	
	fields[TRANSPLANTING_UPPER_FIELD].label.y = -1;

	fields[TRANSPLANTING_UPPER_FIELD].value.y = 19;
	fields[TRANSPLANTING_UPPER_FIELD].value.x = 36;
	fields[TRANSPLANTING_UPPER_FIELD].value.l = TRANSPLANTING_UPPER_LENGTH;
	fields[TRANSPLANTING_UPPER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[TRANSPLANTING_UPPER_FIELD].value.c_value, spaces, fields[TRANSPLANTING_UPPER_FIELD].value.l);
	fields[TRANSPLANTING_UPPER_FIELD].uom.y = 19;
	fields[TRANSPLANTING_UPPER_FIELD].uom.x = 42;
	fields[TRANSPLANTING_UPPER_FIELD].uom.l = 4;
	fields[TRANSPLANTING_UPPER_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[TRANSPLANTING_UPPER_FIELD].uom.u_value, lpad("days", fields[TRANSPLANTING_UPPER_FIELD].uom.l, ' '), fields[TRANSPLANTING_UPPER_FIELD].uom.l);

	fields[MATURITY_LOWER_FIELD].label.y = 20;
	fields[MATURITY_LOWER_FIELD].label.x = 1;
	fields[MATURITY_LOWER_FIELD].label.l = 20;
	fields[MATURITY_LOWER_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[MATURITY_LOWER_FIELD].label.l_value, lpad("Maturity", 20, ' '), fields[MATURITY_LOWER_FIELD].label.l);
	fields[MATURITY_LOWER_FIELD].value.y = 20;
	fields[MATURITY_LOWER_FIELD].value.x = 22;
	fields[MATURITY_LOWER_FIELD].value.l = MATURITY_LOWER_LENGTH;
	fields[MATURITY_LOWER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[MATURITY_LOWER_FIELD].value.c_value, spaces, fields[MATURITY_LOWER_FIELD].value.l);
	fields[MATURITY_LOWER_FIELD].uom.y = -1;
	
	fields[MATURITY_OPTIMAL_FIELD].label.y = -1;

	fields[MATURITY_OPTIMAL_FIELD].value.y = 20;
	fields[MATURITY_OPTIMAL_FIELD].value.x = 29;
	fields[MATURITY_OPTIMAL_FIELD].value.l = MATURITY_OPTIMAL_LENGTH;
	fields[MATURITY_OPTIMAL_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[MATURITY_OPTIMAL_FIELD].value.c_value, spaces, fields[MATURITY_OPTIMAL_FIELD].value.l);
	fields[MATURITY_OPTIMAL_FIELD].uom.y = -1;
	
	fields[MATURITY_UPPER_FIELD].label.y = -1;

	fields[MATURITY_UPPER_FIELD].value.y = 20;
	fields[MATURITY_UPPER_FIELD].value.x = 36;
	fields[MATURITY_UPPER_FIELD].value.l = MATURITY_UPPER_LENGTH;
	fields[MATURITY_UPPER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[MATURITY_UPPER_FIELD].value.c_value, spaces, fields[MATURITY_UPPER_FIELD].value.l);
	fields[MATURITY_UPPER_FIELD].uom.y = 20;
	fields[MATURITY_UPPER_FIELD].uom.x = 42;
	fields[MATURITY_UPPER_FIELD].uom.l = 4;
	fields[MATURITY_UPPER_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[MATURITY_UPPER_FIELD].uom.u_value, lpad("days", fields[MATURITY_UPPER_FIELD].uom.l, ' '), fields[MATURITY_UPPER_FIELD].uom.l);
/*
	fields[KNOTTS_MATURITY_LOWER_FIELD].label.y = fields[KNOTTS_MATURITY_LOWER_FIELD - 1].label.y + 1;
	fields[KNOTTS_MATURITY_LOWER_FIELD].label.x = 1;
	fields[KNOTTS_MATURITY_LOWER_FIELD].label.l = 20;
	fields[KNOTTS_MATURITY_LOWER_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[KNOTTS_MATURITY_LOWER_FIELD].label.l_value, lpad("Width", 20, ' '), fields[KNOTTS_MATURITY_LOWER_FIELD].label.l);
	fields[KNOTTS_MATURITY_LOWER_FIELD].value.y = fields[KNOTTS_MATURITY_LOWER_FIELD].label.y;
	fields[KNOTTS_MATURITY_LOWER_FIELD].value.x = fields[KNOTTS_MATURITY_LOWER_FIELD].label.x + fields[KNOTTS_MATURITY_LOWER_FIELD].label.l + 1;
	fields[KNOTTS_MATURITY_LOWER_FIELD].value.l = KNOTTS_MATURITY_LOWER_LENGTH;
	fields[KNOTTS_MATURITY_LOWER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[KNOTTS_MATURITY_LOWER_FIELD].value.c_value, spaces, fields[KNOTTS_MATURITY_LOWER_FIELD].value.l);
	fields[KNOTTS_MATURITY_LOWER_FIELD].uom.y = fields[KNOTTS_MATURITY_LOWER_FIELD].value.y;
	fields[KNOTTS_MATURITY_LOWER_FIELD].uom.x = fields[KNOTTS_MATURITY_LOWER_FIELD].value.x + fields[KNOTTS_MATURITY_LOWER_FIELD].value.l + 1;
	fields[KNOTTS_MATURITY_LOWER_FIELD].uom.l = 6;
	fields[KNOTTS_MATURITY_LOWER_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[KNOTTS_MATURITY_LOWER_FIELD].uom.u_value, lpad("inches", 6, ' '), fields[KNOTTS_MATURITY_LOWER_FIELD].uom.l);
	
	fields[KNOTTS_UPPER_FIELD].label.y = fields[KNOTTS_UPPER_FIELD - 1].label.y + 1;
	fields[KNOTTS_UPPER_FIELD].label.x = 1;
	fields[KNOTTS_UPPER_FIELD].label.l = 20;
	fields[KNOTTS_UPPER_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[KNOTTS_UPPER_FIELD].label.l_value, lpad("Width", 20, ' '), fields[KNOTTS_UPPER_FIELD].label.l);
	fields[KNOTTS_UPPER_FIELD].value.y = fields[KNOTTS_UPPER_FIELD].label.y;
	fields[KNOTTS_UPPER_FIELD].value.x = fields[KNOTTS_UPPER_FIELD].label.x + fields[KNOTTS_UPPER_FIELD].label.l + 1;
	fields[KNOTTS_UPPER_FIELD].value.l = KNOTTS_UPPER_LENGTH;
	fields[KNOTTS_UPPER_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[KNOTTS_UPPER_FIELD].value.c_value, spaces, fields[KNOTTS_UPPER_FIELD].value.l);
	fields[KNOTTS_UPPER_FIELD].uom.y = fields[KNOTTS_UPPER_FIELD].value.y;
	fields[KNOTTS_UPPER_FIELD].uom.x = fields[KNOTTS_UPPER_FIELD].value.x + fields[KNOTTS_UPPER_FIELD].value.l + 1;
	fields[KNOTTS_UPPER_FIELD].uom.l = 6;
	fields[KNOTTS_UPPER_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[KNOTTS_UPPER_FIELD].uom.u_value, lpad("inches", 6, ' '), fields[KNOTTS_UPPER_FIELD].uom.l);
	
	fields[KNOTTS_FROST_TOLERANCE_FIELD].label.y = fields[KNOTTS_FROST_TOLERANCE_FIELD - 1].label.y + 1;
	fields[KNOTTS_FROST_TOLERANCE_FIELD].label.x = 1;
	fields[KNOTTS_FROST_TOLERANCE_FIELD].label.l = 20;
	fields[KNOTTS_FROST_TOLERANCE_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[KNOTTS_FROST_TOLERANCE_FIELD].label.l_value, lpad("Width", 20, ' '), fields[KNOTTS_FROST_TOLERANCE_FIELD].label.l);
	fields[KNOTTS_FROST_TOLERANCE_FIELD].value.y = fields[KNOTTS_FROST_TOLERANCE_FIELD].label.y;
	fields[KNOTTS_FROST_TOLERANCE_FIELD].value.x = fields[KNOTTS_FROST_TOLERANCE_FIELD].label.x + fields[KNOTTS_FROST_TOLERANCE_FIELD].label.l + 1;
	fields[KNOTTS_FROST_TOLERANCE_FIELD].value.l = KNOTTS_FROST_TOLERANCE_LENGTH;
	fields[KNOTTS_FROST_TOLERANCE_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[KNOTTS_FROST_TOLERANCE_FIELD].value.c_value, spaces, fields[KNOTTS_FROST_TOLERANCE_FIELD].value.l);
	fields[KNOTTS_FROST_TOLERANCE_FIELD].uom.y = fields[KNOTTS_FROST_TOLERANCE_FIELD].value.y;
	fields[KNOTTS_FROST_TOLERANCE_FIELD].uom.x = fields[KNOTTS_FROST_TOLERANCE_FIELD].value.x + fields[KNOTTS_FROST_TOLERANCE_FIELD].value.l + 1;
	fields[KNOTTS_FROST_TOLERANCE_FIELD].uom.l = 6;
	fields[KNOTTS_FROST_TOLERANCE_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[KNOTTS_FROST_TOLERANCE_FIELD].uom.u_value, lpad("inches", 6, ' '), fields[KNOTTS_FROST_TOLERANCE_FIELD].uom.l);
	
	fields[KNOTTS_TRANSPLANTABLE_FIELD].label.y = fields[KNOTTS_TRANSPLANTABLE_FIELD - 1].label.y + 1;
	fields[KNOTTS_TRANSPLANTABLE_FIELD].label.x = 1;
	fields[KNOTTS_TRANSPLANTABLE_FIELD].label.l = 20;
	fields[KNOTTS_TRANSPLANTABLE_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[KNOTTS_TRANSPLANTABLE_FIELD].label.l_value, lpad("Width", 20, ' '), fields[KNOTTS_TRANSPLANTABLE_FIELD].label.l);
	fields[KNOTTS_TRANSPLANTABLE_FIELD].value.y = fields[KNOTTS_TRANSPLANTABLE_FIELD].label.y;
	fields[KNOTTS_TRANSPLANTABLE_FIELD].value.x = fields[KNOTTS_TRANSPLANTABLE_FIELD].label.x + fields[KNOTTS_TRANSPLANTABLE_FIELD].label.l + 1;
	fields[KNOTTS_TRANSPLANTABLE_FIELD].value.l = KNOTTS_TRANSPLANTABLE_LENGTH;
	fields[KNOTTS_TRANSPLANTABLE_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[KNOTTS_TRANSPLANTABLE_FIELD].value.c_value, spaces, fields[KNOTTS_TRANSPLANTABLE_FIELD].value.l);
	fields[KNOTTS_TRANSPLANTABLE_FIELD].uom.y = fields[KNOTTS_TRANSPLANTABLE_FIELD].value.y;
	fields[KNOTTS_TRANSPLANTABLE_FIELD].uom.x = fields[KNOTTS_TRANSPLANTABLE_FIELD].value.x + fields[KNOTTS_TRANSPLANTABLE_FIELD].value.l + 1;
	fields[KNOTTS_TRANSPLANTABLE_FIELD].uom.l = 6;
	fields[KNOTTS_TRANSPLANTABLE_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[KNOTTS_TRANSPLANTABLE_FIELD].uom.u_value, lpad("inches", 6, ' '), fields[KNOTTS_TRANSPLANTABLE_FIELD].uom.l);
	
	fields[FLOWERING_FIELD].label.y = fields[FLOWERING_FIELD - 1].label.y + 1;
	fields[FLOWERING_FIELD].label.x = 1;
	fields[FLOWERING_FIELD].label.l = 20;
	fields[FLOWERING_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[FLOWERING_FIELD].label.l_value, lpad("Width", 20, ' '), fields[FLOWERING_FIELD].label.l);
	fields[FLOWERING_FIELD].value.y = fields[FLOWERING_FIELD].label.y;
	fields[FLOWERING_FIELD].value.x = fields[FLOWERING_FIELD].label.x + fields[FLOWERING_FIELD].label.l + 1;
	fields[FLOWERING_FIELD].value.l = FLOWERING_LENGTH;
	fields[FLOWERING_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[FLOWERING_FIELD].value.c_value, spaces, fields[FLOWERING_FIELD].value.l);
	fields[FLOWERING_FIELD].uom.y = fields[FLOWERING_FIELD].value.y;
	fields[FLOWERING_FIELD].uom.x = fields[FLOWERING_FIELD].value.x + fields[FLOWERING_FIELD].value.l + 1;
	fields[FLOWERING_FIELD].uom.l = 6;
	fields[FLOWERING_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[FLOWERING_FIELD].uom.u_value, lpad("inches", 6, ' '), fields[FLOWERING_FIELD].uom.l);
	
	fields[POLLINATION_PRIMARY_FIELD].label.y = fields[POLLINATION_PRIMARY_FIELD - 1].label.y + 1;
	fields[POLLINATION_PRIMARY_FIELD].label.x = 1;
	fields[POLLINATION_PRIMARY_FIELD].label.l = 20;
	fields[POLLINATION_PRIMARY_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[POLLINATION_PRIMARY_FIELD].label.l_value, lpad("Width", 20, ' '), fields[POLLINATION_PRIMARY_FIELD].label.l);
	fields[POLLINATION_PRIMARY_FIELD].value.y = fields[POLLINATION_PRIMARY_FIELD].label.y;
	fields[POLLINATION_PRIMARY_FIELD].value.x = fields[POLLINATION_PRIMARY_FIELD].label.x + fields[POLLINATION_PRIMARY_FIELD].label.l + 1;
	fields[POLLINATION_PRIMARY_FIELD].value.l = POLLINATION_PRIMARY_LENGTH;
	fields[POLLINATION_PRIMARY_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[POLLINATION_PRIMARY_FIELD].value.c_value, spaces, fields[POLLINATION_PRIMARY_FIELD].value.l);
	fields[POLLINATION_PRIMARY_FIELD].uom.y = fields[POLLINATION_PRIMARY_FIELD].value.y;
	fields[POLLINATION_PRIMARY_FIELD].uom.x = fields[POLLINATION_PRIMARY_FIELD].value.x + fields[POLLINATION_PRIMARY_FIELD].value.l + 1;
	fields[POLLINATION_PRIMARY_FIELD].uom.l = 6;
	fields[POLLINATION_PRIMARY_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[POLLINATION_PRIMARY_FIELD].uom.u_value, lpad("inches", 6, ' '), fields[POLLINATION_PRIMARY_FIELD].uom.l);
	
	fields[POLLINATION_SECONDARY_FIELD].label.y = fields[POLLINATION_SECONDARY_FIELD - 1].label.y + 1;
	fields[POLLINATION_SECONDARY_FIELD].label.x = 1;
	fields[POLLINATION_SECONDARY_FIELD].label.l = 20;
	fields[POLLINATION_SECONDARY_FIELD].label.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[POLLINATION_SECONDARY_FIELD].label.l_value, lpad("Width", 20, ' '), fields[POLLINATION_SECONDARY_FIELD].label.l);
	fields[POLLINATION_SECONDARY_FIELD].value.y = fields[POLLINATION_SECONDARY_FIELD].label.y;
	fields[POLLINATION_SECONDARY_FIELD].value.x = fields[POLLINATION_SECONDARY_FIELD].label.x + fields[POLLINATION_SECONDARY_FIELD].label.l + 1;
	fields[POLLINATION_SECONDARY_FIELD].value.l = POLLINATION_SECONDARY_LENGTH;
	fields[POLLINATION_SECONDARY_FIELD].value.fac = (COLOR_PAIR(1) | A_REVERSE);
	strncpy(fields[POLLINATION_SECONDARY_FIELD].value.c_value, spaces, fields[POLLINATION_SECONDARY_FIELD].value.l);
	fields[POLLINATION_SECONDARY_FIELD].uom.y = fields[POLLINATION_SECONDARY_FIELD].value.y;
	fields[POLLINATION_SECONDARY_FIELD].uom.x = fields[POLLINATION_SECONDARY_FIELD].value.x + fields[POLLINATION_SECONDARY_FIELD].value.l + 1;
	fields[POLLINATION_SECONDARY_FIELD].uom.l = 6;
	fields[POLLINATION_SECONDARY_FIELD].uom.fac = (COLOR_PAIR(1) | A_NORMAL);
	strncpy(fields[POLLINATION_SECONDARY_FIELD].uom.u_value, lpad("inches", 6, ' '), fields[POLLINATION_SECONDARY_FIELD].uom.l);
*/
	return 0;
}