#ifndef __GHDB_H
#define __GHDB_H

struct LABEL {
	int y;
	int x;
	int l;
	int fac;
	char l_value[31];
};

struct VALUE {
	int y;
	int x;
	int l;
	int fac;
	char c_value[2001];
	char o_value[2001]; // Later use to deter,mine if a record has changed
};

struct UOM {
	int y;
	int x;
	int l;
	int fac;
	char u_value[31];
};

struct FIELD {
	struct LABEL label;
	struct VALUE value;
	struct UOM uom;
};

struct CURSOR {
	int y;
	int x;
};

#define PLANT_NAME_FIELD      0
#define LATIN_NAME_FIELD      1
#define HEIGHT_FIELD          2
#define WIDTH_FIELD           3
#define PLANTING_DEPTH_FIELD  4

#define PLANT_NAME_LENGTH    30
#define LATIN_NAME_LENGTH    30
#define HEIGHT_LENGTH         5
#define WIDTH_LENGTH          5
#define PLANTING_DEPTH_LENGTH 5

struct RECORD
{
	char plant_name[PLANT_NAME_LENGTH + 1];
	char latin_name[LATIN_NAME_LENGTH + 1];
	char height[HEIGHT_LENGTH + 1];
	char width[WIDTH_LENGTH + 1];
	char planting_depth[PLANTING_DEPTH_LENGTH + 1];
};
#define NUM_FIELDS 5

//define                0x00000001 // PF1
#define FIRST_RECORD    0x00000002 // PF2
#define FIND_RECORD     0x00000004 // PF3
#define PREVIOUS_RECORD 0x00000008 // PF4
#define NEXT_RECORD     0x00000010 // PF5
//define                0x00000020 // PF6
#define IMPORT_MODE     0x00000040 // PF7
#define EXPORT_MODE     0x00000080 // PF8
//define                0x00000100 // PF9
//define                0x00000200 // PF10
//define                0x00000400 // PF11
//define                0x00000800 // PF12
//define                0x00001000 // PF13
//define                0x00002000 // PF14
//define                0x00004000 // PF15
#define EXIT_MODE       0x00008000 // PF16
//define                0x00010000 // PF17
//define                0x00020000 // PF18
//define                0x00040000 // PF19
//define                0x00080000 // PF20
//define                0x00100000 // PF21
//define                0x00200000 // PF22
//define                0x00400000 // PF23
//define                0x00800000 // PF24
#define ENTER           0x01000000
#define UPDATE_RECORD   0x02000000
//define                0x04000000
//define                0x08000000
#define SELECT_MODE     0x10000000
#define INSERT_MODE     0x20000000
#define UPDATE_MODE     0x40000000
#define DELETE_MODE     0x80000000

int fldlen(const int y, const int x, const int l);
int formscrn(struct FIELD fields[], int num_fields);
int ftor(struct FIELD fields[], int num_fields, struct RECORD *record);
int ghdb_close();
int ghdb_delete(struct RECORD *record);
int ghdb_export();
int ghdb_import();
int ghdb_insert(struct RECORD *record);
int ghdb_select(struct RECORD *record);
int ghdb_select_first(struct RECORD *record);
int ghdb_select_next(struct RECORD *record);
int ghdb_select_previous(struct RECORD *record);
int ghdb_update(struct RECORD *record);
int init_record(struct RECORD *record);
int keymouse(struct FIELD fields[], int num_fields);
int paint(struct FIELD fields[], int num_fields, struct CURSOR *cursor, unsigned action);
int rtof(struct FIELD fields[], int num_fields, struct RECORD *record);
int terminal();
int tsv_export();
int tsv_import();
int xerror(const char *message);
unsigned keyboard(struct FIELD fields[], int num_fields, struct CURSOR *cursor, unsigned action);
void *ghdb_open();

#endif