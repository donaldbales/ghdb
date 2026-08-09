#include <gdbm.h>
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "ghdb.h"

/*
  Get the length of the field value from the screen buffer
*/
int fldlen(const int y, const int x, const int l) 
{
	int len = 0;
	int i = 0;
	for (i=x+l; i>=x; i--)
	{
		int c = (mvinch(y, i) & A_CHARTEXT);
		if (c > 32) {
			len = i - x + 1;
			break;
		}
	}

	char local[2001];
	sprintf(local, "fldlen: '%d'", len);
	xerror(local);

	return len;
}

/*
  Return the field value from the screen buffer
*/
char *getfld(const int y, const int x, const int l)
{
	int i = 0;
	char *field;
	field = malloc(l + 1);
	if (field == NULL)
	{
		exit(EXIT_FAILURE);
	}
	char *f = field;
	for (i=x; i<x + l; i++) 
	{
		int c = (mvinch(y, i) & A_CHARTEXT);
		fprintf(stderr, "*getfld: c='%d', i='%d'\n", c, i);

		char local[2001];
		sprintf(local, "*getfld: c='%c', i='%d'", c, i);
		xerror(local);
		//sleep(1);

		*(f++) = c;
	}
	*(f) = '\0';

	char local[2001];
	sprintf(local, "*getfld: '%s'", field);
	xerror(local);

	return field;
}

int keyboard(struct FIELD fields[], int num_fields, struct CURSOR curmax, struct CURSOR cursor, int action)
{
	for (;;)
	{
		int ch = getch();
		fprintf(stderr, "keyboard: %d (%c)\n", ch, ch);
		(void) getyx(stdscr, cursor.y, cursor.x);
		fprintf(stderr, "keyboard: y=%d x=%d\n", cursor.y, cursor.x);

		switch(ch)
		{
			case 9:
				fprintf(stderr, "keyboard: %s\n", "TAB");
				for (int i=0; i<num_fields; i++)
				{
					if (cursor.y == fields[i].value.y &&
						  cursor.x >= fields[i].value.x && 
						  cursor.x <= fields[i].value.x + fields[i].value.l)
					{
						char *p = getfld(fields[i].value.y, fields[i].value.x, fields[i].value.l);
						strcpy(fields[i].value.c_value, p);
						free(p);
						fprintf(stderr, "KEY_TAB: %s\n", fields[i].value.c_value);
						if (i<num_fields - 1)
						{
							(void) move(fields[i + 1].value.y, fields[i + 1].value.x + \
								 fldlen(fields[i + 1].value.y, fields[i + 1].value.x, fields[i + 1].value.l));
							//(void) attron(fields[i + 1].value.fac);
						}
						else
						{
							(void) move(fields[0].value.y, fields[0].value.x + \
								 fldlen(fields[0].value.y, fields[0].value.x, fields[0].value.l));
							//(void) attron(fields[0].value.fac);
						}

						break;
					}
				}
				break;

			case 10:
				fprintf(stderr, "keyboard: %s\n", "NEWLINE");
				break;

			case 13:
				fprintf(stderr, "keyboard: %s\n", "RETURN");
				for (int i=0; i<num_fields; i++)
				{
					if (cursor.y == fields[i].value.y &&
						  cursor.x >= fields[i].value.x && 
						  cursor.x <= fields[i].value.x + fields[i].value.l)
					{
						char *p = getfld(fields[i].value.y, fields[i].value.x, fields[i].value.l);
						strcpy(fields[i].value.c_value, p);
						free(p);
						fprintf(stderr, "RETURN: %s\n", fields[i].value.c_value);
					}
				}
				return action | ENTER;
				break;

			case KEY_CODE_YES:
				fprintf(stderr, "keyboard: %s\n", "KEY_CODE_YES");
				break;

			// same as KEY_BREAK
			//case KEY_MIN:
			//	fprintf(stderr, "keyboard: %s\n", "KEY_MIN");
			//	break;
	
			case KEY_BREAK:
				fprintf(stderr, "keyboard: %s\n", "KEY_BREAK");
				break;
	
			case KEY_SRESET:
				fprintf(stderr, "keyboard: %s\n", "KEY_SRESET");
				break;
	
			case KEY_RESET:
				fprintf(stderr, "keyboard: %s\n", "KEY_RESET");
				break;
	
			case KEY_DOWN:
				fprintf(stderr, "keyboard: %s\n", "KEY_DOWN");
				break;
	
			case KEY_UP:
				fprintf(stderr, "keyboard: %s\n", "KEY_UP");
				break;
	
			case KEY_LEFT:
				fprintf(stderr, "keyboard: %s\n", "KEY_LEFT");
				for (int i=0; i<num_fields; i++)
				{
					if (cursor.y == fields[i].value.y &&
						cursor.x >= fields[i].value.x && 
						cursor.x <= fields[i].value.x + fields[i].value.l)
					{
						(void) move(cursor.y, --cursor.x);
						(void) attron(fields[i].value.fac);
						//(void) printw("%c", 32);
						(void) move(cursor.y, cursor.x);
					}
				}
				break;
	
			case KEY_RIGHT:
				fprintf(stderr, "keyboard: %s\n", "KEY_RIGHT");
				for (int i=0; i<num_fields; i++)
				{
					if (cursor.y == fields[i].value.y &&
						cursor.x >= fields[i].value.x && 
						cursor.x <  fields[i].value.x + fields[i].value.l)
					{
						(void) move(cursor.y, ++cursor.x);
						(void) attron(fields[i].value.fac);
						//(void) printw("%c", 32);
						//(void) move(cursor.y, cursor.x);
					}
				}
				break;
	
			case KEY_HOME:
				fprintf(stderr, "keyboard: %s\n", "KEY_HOME");
				break;
	
			case KEY_BACKSPACE:
				fprintf(stderr, "keyboard: %s\n", "KEY_BACKSPACE");
				for (int i=0; i<num_fields; i++)
				{
					if (cursor.y == fields[i].value.y &&
						cursor.x >  fields[i].value.x && 
						cursor.x <= fields[i].value.x + fields[i].value.l)
					{
						(void) move(cursor.y, --cursor.x);
						(void) attron(fields[i].value.fac);
						(void) printw("%c", 32);
						(void) move(cursor.y, cursor.x);
						break;
					}
				}
				break;
	
			case KEY_F0:
				fprintf(stderr, "keyboard: %s\n", "KEY_F0");
				break;
	
			case KEY_F(1):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(1)");
				break;
	
			case KEY_F(2):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(2)");
				break;
	
			case KEY_F(3):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(3)");
				break;
	
			case KEY_F(4):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(4)");
				break;
	
			case KEY_F(5):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(5)");
				break;
	
			case KEY_F(6):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(6)");
				break;
	
			case KEY_F(7):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(7)");
				break;
	
			case KEY_F(8):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(8)");
				break;
	
			case KEY_F(9):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(9)");
				break;
	
			case KEY_F(10):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(10)");
				break;
	
			case KEY_F(11):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(11)");
				break;
	
			case KEY_F(12):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(12)");
				break;
	
			case KEY_F(13):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(13)");
				break;
	
			case KEY_F(14):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(14)");
				break;
	
			case KEY_F(15):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(15)");
				break;
	
			case KEY_F(16):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(16)");
				fprintf(stderr, "KEY_F(16)\n");
				/*
				for (int i=0; i<num_fields; i++)
				{
					if (cursor.y == fields[i].value.y &&
						cursor.x >= fields[i].value.x && 
						cursor.x <= fields[i].value.x + fields[i].value.l)
					{
						char *p = getfld(fields[i].value.y, fields[i].value.x, fields[i].value.l);
						strcpy(fields[i].value.c_value, p);
						free(p);
						fprintf(stderr, "KEY_F(16): %s\n", fields[i].value.c_value);
					}
				}
				for (int i=0; i<num_fields; i++)
				{
					fprintf(stderr, "KEY_F(16): %s: %s\n", fields[i].label.l_value, fields[i].value.c_value);
				}
				*/
				(void) endwin();
				exit(0);
				break;
	
			case KEY_F(17):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(17)");
				break;
	
			case KEY_F(18):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(18)");
				break;

			case KEY_F(19):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(19)");
				break;

			case KEY_F(20):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(20)");
				break;

			case KEY_F(21):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(21)");
				break;

			case KEY_F(22):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(22)");
				break;

			case KEY_F(23):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(23)");
				break;

			case KEY_F(24):
				fprintf(stderr, "keyboard: %s\n", "KEY_F(24)");
				break;

			case KEY_DL:
				fprintf(stderr, "keyboard: %s\n", "KEY_DL");
				break;
	
			case KEY_IL:
				fprintf(stderr, "keyboard: %s\n", "KEY_IL");
				break;
	
			case KEY_DC:
				fprintf(stderr, "keyboard: %s\n", "KEY_DC");
				break;
	
			case KEY_IC:
				fprintf(stderr, "keyboard: %s\n", "KEY_IC");
				break;
	
			case KEY_EIC:
				fprintf(stderr, "keyboard: %s\n", "KEY_EIC");
				break;
	
			case KEY_CLEAR:
				fprintf(stderr, "keyboard: %s\n", "KEY_CLEAR");
				break;
	
			case KEY_EOS:
				fprintf(stderr, "keyboard: %s\n", "KEY_EOS");
				break;
	
			case KEY_EOL:
				fprintf(stderr, "keyboard: %s\n", "KEY_EOL");
				break;
	
			case KEY_SF:
				fprintf(stderr, "keyboard: %s\n", "KEY_SF");
				break;
	
			case KEY_SR:
				fprintf(stderr, "keyboard: %s\n", "KEY_SR");
				break;
	
			case KEY_NPAGE:
				fprintf(stderr, "keyboard: %s\n", "KEY_NPAGE");
				break;
	
			case KEY_PPAGE:
				fprintf(stderr, "keyboard: %s\n", "KEY_PPAGE");
				break;
	
			case KEY_STAB:
				fprintf(stderr, "keyboard: %s\n", "KEY_STAB");
				break;
	
			case KEY_CTAB:
				fprintf(stderr, "keyboard: %s\n", "KEY_CTAB");
				break;
	
			case KEY_CATAB:
				fprintf(stderr, "keyboard: %s\n", "KEY_CATAB");
				break;
	
			case KEY_ENTER:
				fprintf(stderr, "keyboard: %s\n", "KEY_ENTER");
				break;
	
			case KEY_PRINT:
				fprintf(stderr, "keyboard: %s\n", "KEY_PRINT");
				break;
	
			case KEY_LL:
				fprintf(stderr, "keyboard: %s\n", "KEY_LL");
				break;
	
			case KEY_A1:
				fprintf(stderr, "keyboard: %s\n", "KEY_A1");
				break;
	
			case KEY_A3:
				fprintf(stderr, "keyboard: %s\n", "KEY_A3");
				break;
	
			case KEY_B2:
				fprintf(stderr, "keyboard: %s\n", "KEY_B2");
				break;
	
			case KEY_C1:
				fprintf(stderr, "keyboard: %s\n", "KEY_C1");
				break;
	
			case KEY_C3:
				fprintf(stderr, "keyboard: %s\n", "KEY_C3");
				break;
	
			case KEY_BTAB:
				fprintf(stderr, "keyboard: %s\n", "KEY_BTAB");
				for (int i=0; i<num_fields; i++)
				{
					if (cursor.y == fields[i].value.y &&
						cursor.x >= fields[i].value.x && 
						cursor.x <= fields[i].value.x + fields[i].value.l)
					{
						char *p = getfld(fields[i].value.y, fields[i].value.x, fields[i].value.l);
						strcpy(fields[i].value.c_value, p);
						free(p);
						fprintf(stderr, "KEY_BTAB: %s\n", fields[i].value.c_value);
						if (i>0)
						{
							(void) move(fields[i - 1].value.y, \
										fields[i - 1].value.x + \
								 fldlen(fields[i - 1].value.y, \
										fields[i - 1].value.x, \
										fields[i - 1].value.l));
							(void) attron(fields[i - 1].value.fac);
						}
						else
						{
							(void) move(fields[num_fields - 1].value.y, \
									    fields[num_fields - 1].value.x + \
								 fldlen(fields[num_fields - 1].value.y, \
								 		fields[num_fields - 1].value.x, \
								 		fields[num_fields - 1].value.l));
							(void) attron(fields[num_fields - 1].value.fac);
						}
						break;
					}
				}
				break;
	
			case KEY_BEG:
				fprintf(stderr, "keyboard: %s\n", "KEY_BEG");
				break;
	
			case KEY_CANCEL:
				fprintf(stderr, "keyboard: %s\n", "KEY_CANCEL");
				break;
	
			case KEY_CLOSE:
				fprintf(stderr, "keyboard: %s\n", "KEY_CLOSE");
				break;
	
			case KEY_COMMAND:
				fprintf(stderr, "keyboard: %s\n", "KEY_COMMAND");
				break;
	
			case KEY_COPY:
				fprintf(stderr, "keyboard: %s\n", "KEY_COPY");
				break;
	
			case KEY_CREATE:
				fprintf(stderr, "keyboard: %s\n", "KEY_CREATE");
				break;
	
			case KEY_END:
				fprintf(stderr, "keyboard: %s\n", "KEY_END");
				break;
	
			case KEY_EXIT:
				fprintf(stderr, "keyboard: %s\n", "KEY_EXIT");
				break;
	
			case KEY_FIND:
				fprintf(stderr, "keyboard: %s\n", "KEY_FIND");
				break;
	
			case KEY_HELP:
				fprintf(stderr, "keyboard: %s\n", "KEY_HELP");
				break;
	
			case KEY_MARK:
				fprintf(stderr, "keyboard: %s\n", "KEY_MARK");
				break;
	
			case KEY_MESSAGE:
				fprintf(stderr, "keyboard: %s\n", "KEY_MESSAGE");
				break;
	
			case KEY_MOVE:
				fprintf(stderr, "keyboard: %s\n", "KEY_MOVE");
				break;
	
			case KEY_NEXT:
				fprintf(stderr, "keyboard: %s\n", "KEY_NEXT");
				break;
	
			case KEY_OPEN:
				fprintf(stderr, "keyboard: %s\n", "KEY_OPEN");
				break;
	
			case KEY_OPTIONS:
				fprintf(stderr, "keyboard: %s\n", "KEY_OPTIONS");
				break;
	
			case KEY_PREVIOUS:
				fprintf(stderr, "keyboard: %s\n", "KEY_PREVIOUS");
				break;
	
			case KEY_REDO:
				fprintf(stderr, "keyboard: %s\n", "KEY_REDO");
				break;
	
			case KEY_REFERENCE:
				fprintf(stderr, "keyboard: %s\n", "KEY_REFERENCE");
				break;
	
			case KEY_REFRESH:
				fprintf(stderr, "keyboard: %s\n", "KEY_REFRESH");
				break;
	
			case KEY_REPLACE:
				fprintf(stderr, "keyboard: %s\n", "KEY_REPLACE");
				break;
	
			case KEY_RESTART:
				fprintf(stderr, "keyboard: %s\n", "KEY_RESTART");
				break;
	
			case KEY_RESUME:
				fprintf(stderr, "keyboard: %s\n", "KEY_RESUME");
				break;
	
			case KEY_SAVE:
				fprintf(stderr, "keyboard: %s\n", "KEY_SAVE");
				break;
	
			case KEY_SBEG:
				fprintf(stderr, "keyboard: %s\n", "KEY_SBEG");
				break;
	
			case KEY_SCANCEL:
				fprintf(stderr, "keyboard: %s\n", "KEY_SCANCEL");
				break;
	
			case KEY_SCOMMAND:
				fprintf(stderr, "keyboard: %s\n", "KEY_SCOMMAND");
				break;
	
			case KEY_SCOPY:
				fprintf(stderr, "keyboard: %s\n", "KEY_SCOPY");
				break;
	
			case KEY_SCREATE:
				fprintf(stderr, "keyboard: %s\n", "KEY_SCREATE");
				break;
	
			case KEY_SDC:
				fprintf(stderr, "keyboard: %s\n", "KEY_SDC");
				break;
	
			case KEY_SDL:
				fprintf(stderr, "keyboard: %s\n", "KEY_SDL");
				break;
	
			case KEY_SELECT:
				fprintf(stderr, "keyboard: %s\n", "KEY_SELECT");
				break;
	
			case KEY_SEND:
				fprintf(stderr, "keyboard: %s\n", "KEY_SEND");
				break;
	
			case KEY_SEOL:
				fprintf(stderr, "keyboard: %s\n", "KEY_SEOL");
				break;
	
			case KEY_SEXIT:
				fprintf(stderr, "keyboard: %s\n", "KEY_SEXIT");
				break;
	
			case KEY_SFIND:
				fprintf(stderr, "keyboard: %s\n", "KEY_SFIND");
				break;
	
			case KEY_SHELP:
				fprintf(stderr, "keyboard: %s\n", "KEY_SHELP");
				break;
	
			case KEY_SHOME:
				fprintf(stderr, "keyboard: %s\n", "KEY_SHOME");
				break;
	
			case KEY_SIC:
				fprintf(stderr, "keyboard: %s\n", "KEY_SIC");
				break;
	
			case KEY_SLEFT:
				fprintf(stderr, "keyboard: %s\n", "KEY_SLEFT");
				break;
	
			case KEY_SMESSAGE:
				fprintf(stderr, "keyboard: %s\n", "KEY_SMESSAGE");
				break;
	
			case KEY_SMOVE:
				fprintf(stderr, "keyboard: %s\n", "KEY_SMOVE");
				break;
	
			case KEY_SNEXT:
				fprintf(stderr, "keyboard: %s\n", "KEY_SNEXT");
				break;
	
			case KEY_SOPTIONS:
				fprintf(stderr, "keyboard: %s\n", "KEY_SOPTIONS");
				break;
	
			case KEY_SPREVIOUS:
				fprintf(stderr, "keyboard: %s\n", "KEY_SPREVIOUS");
				break;
	
			case KEY_SPRINT:
				fprintf(stderr, "keyboard: %s\n", "KEY_SPRINT");
				break;
	
			case KEY_SREDO:
				fprintf(stderr, "keyboard: %s\n", "KEY_SREDO");
				break;
	
			case KEY_SREPLACE:
				fprintf(stderr, "keyboard: %s\n", "KEY_SREPLACE");
				break;
	
			case KEY_SRIGHT:
				fprintf(stderr, "keyboard: %s\n", "KEY_SRIGHT");
				break;
	
			case KEY_SRSUME:
				fprintf(stderr, "keyboard: %s\n", "KEY_SRSUME");
				break;
	
			case KEY_SSAVE:
				fprintf(stderr, "keyboard: %s\n", "KEY_SSAVE");
				break;
	
			case KEY_SSUSPEND:
				fprintf(stderr, "keyboard: %s\n", "KEY_SSUSPEND");
				break;
	
			case KEY_SUNDO:
				fprintf(stderr, "keyboard: %s\n", "KEY_SUNDO");
				break;
	
			case KEY_SUSPEND:
				fprintf(stderr, "keyboard: %s\n", "KEY_SUSPEND");
				break;
	
			case KEY_UNDO:
				fprintf(stderr, "keyboard: %s\n", "KEY_UNDO");
				break;
	
			case KEY_MOUSE:
				fprintf(stderr, "keyboard: %s\n", "KEY_MOUSE");
				(void) keymouse(fields, num_fields);
				break;
	
			case KEY_RESIZE:
				fprintf(stderr, "keyboard: %s\n", "KEY_RESIZE");
				break;

			default:
				if (ch > 31 && ch < 128)
				{
					for (int i=0; i<num_fields; i++)
					{
						if (cursor.y == fields[i].value.y &&
							cursor.x >= fields[i].value.x && 
							cursor.x <  fields[i].value.x + fields[i].value.l)
						{
							(void) attron(fields[i].value.fac);
							(void) printw("%c", ch);
							break;
						}
					}
				}
				else 
				{
					fprintf(stderr, "keyboard: Unsupported key code %d\n", ch);
				}

		}

		(void) refresh();

	}

}