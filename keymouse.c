#include <gdbm.h>
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "ghdb.h"

int keymouse(struct FIELD fields[], int num_fields, struct CURSOR *cursor)
{
	int button_alt = 0;
	int button_ctrl = 0;
	int button_shift = 0;
	MEVENT mevent = { 0, 0, 0, 0, 0 };
	(void) getmouse(&mevent);
/*
	fprintf(stderr, "keymouse: mevent.x is %d\n", mevent.x);
	fprintf(stderr, "keymouse: mevent.y is %d\n", mevent.y);
	fprintf(stderr, "keymouse: mevent.bstate is %d\n", mevent.bstate);
*/
	if ((mevent.bstate & BUTTON_ALT) != 0)
	{
		button_alt = 1;
		fprintf(stderr, "keymouse: %s\n", "BUTTON_ALT");
	}
	if ((mevent.bstate & BUTTON_CTRL) != 0)
	{
		button_ctrl = 1;
		fprintf(stderr, "keymouse: %s\n", "BUTTON_CTRL");
	}
	if ((mevent.bstate & BUTTON_SHIFT) != 0)
	{
		button_shift = 1;
		fprintf(stderr, "keymouse: %s\n", "BUTTON_SHIFT");
	}

	if ((mevent.bstate & BUTTON1_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON1_CLICKED");
		if (button_alt == 1)
		{

		}
		else if (button_ctrl == 1)
		{

		}
		else if (button_shift == 1)
		{

		}
		else
		{
			for (int i=0; i<num_fields; i++)
			{
				if (cursor->y == fields[i].value.y &&
					cursor->x >= fields[i].value.x && 
					cursor->x <= fields[i].value.x + fields[i].value.l)
				{
					char *p = getfld(fields[i].value.y, fields[i].value.x, fields[i].value.l);
					strcpy(fields[i].value.c_value, p);
					free(p);
					fprintf(stderr, "BUTTON1_CLICKED: %s\n", fields[i].value.c_value);
					if (i<num_fields - 1)
					{
						(void) move(fields[i + 1].value.y, fields[i + 1].value.x + \
							 fldlen(fields[i + 1].value.y, fields[i + 1].value.x, fields[i + 1].value.l));
					}
					else
					{
						(void) move(fields[0].value.y, fields[0].value.x + \
							 fldlen(fields[0].value.y, fields[0].value.x, fields[0].value.l));
					}
					break;
				}
			}

			for (int i=0; i<num_fields; i++)
			{
				if (mevent.y == fields[i].value.y &&
					mevent.x >= fields[i].value.x && 
					mevent.x <= fields[i].value.x + fields[i].value.l)
				{
					(void) move(mevent.y, mevent.x);
					(void) attron(fields[i].value.fac);
				}
			}
			return 0;
		}
	}
	if ((mevent.bstate & BUTTON1_DOUBLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON1_DOUBLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON1_PRESSED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON1_PRESSED");
	}
	if ((mevent.bstate & BUTTON1_RELEASED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON1_RELEASED");
	}
	//if ((mevent.bstate & BUTTON1_RESERVED_EVENT) != 0)
	//{
	//	fprintf(stderr, "keymouse: %s\n", "BUTTON1_RESERVED_EVENT");
	//}
	if ((mevent.bstate & BUTTON1_TRIPLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON1_TRIPLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON2_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON2_CLICKED");
	}
	if ((mevent.bstate & BUTTON2_DOUBLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON2_DOUBLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON2_PRESSED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON2_PRESSED");
	}
	if ((mevent.bstate & BUTTON2_RELEASED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON2_RELEASED");
	}
	//if ((mevent.bstate & BUTTON2_RESERVED_EVENT) != 0)
	//{
	//	fprintf(stderr, "keymouse: %s\n", "BUTTON2_RESERVED_EVENT");
	//}
	if ((mevent.bstate & BUTTON2_TRIPLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON2_TRIPLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON3_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON3_CLICKED");
	}
	if ((mevent.bstate & BUTTON3_DOUBLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON3_DOUBLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON3_PRESSED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON3_PRESSED");
	}
	if ((mevent.bstate & BUTTON3_RELEASED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON3_RELEASED");
	}
	//if ((mevent.bstate & BUTTON3_RESERVED_EVENT) != 0)
	//{
	//	fprintf(stderr, "keymouse: %s\n", "BUTTON3_RESERVED_EVENT");
	//}
	if ((mevent.bstate & BUTTON3_TRIPLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON3_TRIPLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON4_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON4_CLICKED");
	}
	if ((mevent.bstate & BUTTON4_DOUBLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON4_DOUBLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON4_PRESSED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON4_PRESSED");
	}
	if ((mevent.bstate & BUTTON4_RELEASED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON4_RELEASED");
	}
	//if ((mevent.bstate & BUTTON4_RESERVED_EVENT) != 0)
	//{
	//	fprintf(stderr, "keymouse: %s\n", "BUTTON4_RESERVED_EVENT");
	//}
	if ((mevent.bstate & BUTTON4_TRIPLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON4_TRIPLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON5_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON5_CLICKED");
	}
	if ((mevent.bstate & BUTTON5_DOUBLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON5_DOUBLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON5_PRESSED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON5_PRESSED");
	}
	if ((mevent.bstate & BUTTON5_RELEASED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON5_RELEASED");
	}
	if ((mevent.bstate & BUTTON5_TRIPLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON5_TRIPLE_CLICKED");
	}
	/*if ((mevent.bstate & BUTTON6_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON6_CLICKED");
	}
	if ((mevent.bstate & BUTTON6_DOUBLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON6_DOUBLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON6_PRESSED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON6_PRESSED");
	}
	if ((mevent.bstate & BUTTON6_RELEASED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON6_RELEASED");
	}
	if ((mevent.bstate & BUTTON6_TRIPLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON6_TRIPLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON7_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON7_CLICKED");
	}
	if ((mevent.bstate & BUTTON7_DOUBLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON7_DOUBLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON7_PRESSED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON7_PRESSED");
	}
	if ((mevent.bstate & BUTTON7_RELEASED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON7_RELEASED");
	}
	if ((mevent.bstate & BUTTON7_TRIPLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON7_TRIPLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON8_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON8_CLICKED");
	}
	if ((mevent.bstate & BUTTON8_DOUBLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON8_DOUBLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON8_PRESSED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON8_PRESSED");
	}
	if ((mevent.bstate & BUTTON8_RELEASED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON8_RELEASED");
	}
	if ((mevent.bstate & BUTTON8_TRIPLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON8_TRIPLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON9_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON9_CLICKED");
	}
	if ((mevent.bstate & BUTTON9_DOUBLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON9_DOUBLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON9_PRESSED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON9_PRESSED");
	}
	if ((mevent.bstate & BUTTON9_RELEASED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON9_RELEASED");
	}
	if ((mevent.bstate & BUTTON9_TRIPLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON9_TRIPLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON10_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON10_CLICKED");
	}
	if ((mevent.bstate & BUTTON10_DOUBLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON10_DOUBLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON10_PRESSED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON10_PRESSED");
	}
	if ((mevent.bstate & BUTTON10_RELEASED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON10_RELEASED");
	}
	if ((mevent.bstate & BUTTON10_TRIPLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON10_TRIPLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON11_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON11_CLICKED");
	}
	if ((mevent.bstate & BUTTON11_DOUBLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON11_DOUBLE_CLICKED");
	}
	if ((mevent.bstate & BUTTON11_PRESSED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON11_PRESSED");
	}
	if ((mevent.bstate & BUTTON11_RELEASED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON11_RELEASED");
	}
	if ((mevent.bstate & BUTTON11_TRIPLE_CLICKED) != 0)
	{
		fprintf(stderr, "keymouse: %s\n", "BUTTON11_TRIPLE_CLICKED");
	}*/
	button_alt = 0;
	button_ctrl = 0;
	button_shift = 0;

	return 0;	
}