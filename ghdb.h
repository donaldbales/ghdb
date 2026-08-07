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
	char o_value[2001];
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

//define                0x00000001 // PF1
#define FIRST_RECORD    0x00000002 // PF2
//define                0x00000004 // PF3
#define PREVIOUS_RECORD 0x00000008 // PF4
#define NEXT_RECORD     0x00000010 // PF5
//define                0x00000020 // PF6
//define                0x00000040 // PF7
//define                0x00000080 // PF8
#define UPDATE_RECORD   0x00000100 // PF9
//define                0x00000200 // PF10
//define                0x00000400 // PF11
//define                0x00000800 // PF12
//define                0x00001000 // PF13
//define                0x00002000 // PF14
//define                0x00004000 // PF15
//define                0x00008000 // PF16
//define                0x00010000 // PF17
//define                0x00020000 // PF18
//define                0x00040000 // PF19
//define                0x00080000 // PF20
//define                0x00100000 // PF21
//define                0x00200000 // PF22
//define                0x00400000 // PF23
//define                0x00800000 // PF24
#define ENTER           0x01000000
//define                0x02000000
//define                0x04000000
//define                0x08000000
#define SELECT_MODE     0x10000000
#define INSERT_MODE     0x20000000
#define UPDATE_MODE     0x40000000
#define DELETE_MODE     0x80000000

int terminal();
int keyboard(struct FIELD fields[], int num_fields, struct CURSOR curmax, struct CURSOR cursor);
int keymouse(struct FIELD fields[], int num_fields);
int database();

#endif