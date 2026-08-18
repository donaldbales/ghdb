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
	char o_value[2001]; // Later use to determine if a record has changed
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

#define PLANT_NAME_FIELD               0
#define LATIN_NAME_FIELD               1
#define HEIGHT_FIELD                   2
#define WIDTH_FIELD                    3
#define PLANTING_DEPTH_FIELD           4
#define SEED_SIZE_FIELD                5
#define SEED_NEED_LIGHT_FIELD          6
#define SEED_SCARIFICATION_FIELD       7
#define PH_FIELD                       8
#define EC_FIELD                       9
#define DAY_LIGHT_INTERVAL_FIELD      10
#define PHOTOPERIOD_HOURS_FIELD       11
#define LIGHT_LOWER_FIELD             12
#define LIGHT_OPTIMAL_FIELD           13
#define LIGHT_UPPER_FIELD             14
#define NITROGEN_LOWER_FIELD          15
#define NITROGEN_OPTIMAL_FIELD        16
#define NITROGEN_UPPER_FIELD          17
#define PHOSPHORUS_LOWER_FIELD        18
#define PHOSPHORUS_OPTIMAL_FIELD      19
#define PHOSPHORUS_UPPER_FIELD        20
#define POTASSIUM_LOWER_FIELD         21
#define POTASSIUM_OPTIMAL_FIELD       22
#define POTASSIUM_UPPER_FIELD         23
#define GERMINATION_LOWER_FIELD       24
#define GERMINATION_NORMAL_FIELD      25
#define GERMINATION_UPPER_FIELD       26
#define TRANSPLANTING_LOWER_FIELD     27
#define TRANSPLANTING_OPTIMAL_FIELD   28
#define TRANSPLANTING_UPPER_FIELD     29
#define MATURITY_LOWER_FIELD          30
#define MATURITY_OPTIMAL_FIELD        31
#define MATURITY_UPPER_FIELD          32
#define FROST_TOLERANCE_FIELD         33
#define FLOWERING_FIELD               34
#define POLLINATION_PRIMARY_FIELD     35
#define POLLINATION_SECONDARY_FIELD   36

#define PLANT_NAME_LENGTH             30
#define LATIN_NAME_LENGTH             30
#define HEIGHT_LENGTH                  5
#define WIDTH_LENGTH                   5
#define PLANTING_DEPTH_LENGTH          5
#define SEED_SIZE_LENGTH               5
#define SEED_NEED_LIGHT_LENGTH         3
#define SEED_SCARIFICATION_LENGTH      3
#define PH_LENGTH                      5
#define EC_LENGTH                      5
#define DAY_LIGHT_INTERVAL_LENGTH      5
#define PHOTOPERIOD_HOURS_LENGTH       5
#define LIGHT_LOWER_LENGTH             5
#define LIGHT_OPTIMAL_LENGTH           5
#define LIGHT_UPPER_LENGTH             5
#define NITROGEN_LOWER_LENGTH          5
#define NITROGEN_OPTIMAL_LENGTH        5
#define NITROGEN_UPPER_LENGTH          5
#define PHOSPHORUS_LOWER_LENGTH        5
#define PHOSPHORUS_OPTIMAL_LENGTH      5
#define PHOSPHORUS_UPPER_LENGTH        5
#define POTASSIUM_LOWER_LENGTH         5
#define POTASSIUM_OPTIMAL_LENGTH       5
#define POTASSIUM_UPPER_LENGTH         5
#define GERMINATION_LOWER_LENGTH       5
#define GERMINATION_NORMAL_LENGTH      5
#define GERMINATION_UPPER_LENGTH       5
#define TRANSPLANTING_LOWER_LENGTH     5
#define TRANSPLANTING_OPTIMAL_LENGTH   5
#define TRANSPLANTING_UPPER_LENGTH     5
#define MATURITY_LOWER_LENGTH          5
#define MATURITY_OPTIMAL_LENGTH        5
#define MATURITY_UPPER_LENGTH          5
#define FROST_TOLERANCE_LENGTH         3
#define FLOWERING_LENGTH               5
#define POLLINATION_PRIMARY_LENGTH    30
#define POLLINATION_SECONDARY_LENGTH  30

struct RECORD
{
	char plant_name[PLANT_NAME_LENGTH + 1];
	char latin_name[LATIN_NAME_LENGTH + 1];
	char height[HEIGHT_LENGTH + 1];
	char width[WIDTH_LENGTH + 1];
	char planting_depth[PLANTING_DEPTH_LENGTH + 1];
	char seed_size[SEED_SIZE_LENGTH + 1];
	char seed_need_light[SEED_NEED_LIGHT_LENGTH + 1];
	char seed_scarification[SEED_SCARIFICATION_LENGTH + 1];
	char ph[PH_LENGTH + 1];
	char ec[EC_LENGTH + 1];
	char day_light_interval[DAY_LIGHT_INTERVAL_LENGTH + 1];
	char photoperiod_hours[PHOTOPERIOD_HOURS_LENGTH + 1];
	char light_lower[LIGHT_LOWER_LENGTH + 1];
	char light_optimal[LIGHT_OPTIMAL_LENGTH + 1];
	char light_upper[LIGHT_UPPER_LENGTH + 1];
	char nitrogen_lower[NITROGEN_LOWER_LENGTH + 1];
	char nitrogen_optimal[NITROGEN_OPTIMAL_LENGTH + 1];
	char nitrogen_upper[NITROGEN_UPPER_LENGTH + 1];
	char phosphorus_lower[PHOSPHORUS_LOWER_LENGTH + 1];
	char phosphorus_optimal[PHOSPHORUS_OPTIMAL_LENGTH + 1];
	char phosphorus_upper[PHOSPHORUS_UPPER_LENGTH + 1];
	char potassium_lower[POTASSIUM_LOWER_LENGTH + 1];
	char potassium_optimal[POTASSIUM_OPTIMAL_LENGTH + 1];
	char potassium_upper[POTASSIUM_UPPER_LENGTH + 1];
	char germination_lower[GERMINATION_LOWER_LENGTH + 1];
	char germination_normal[GERMINATION_NORMAL_LENGTH + 1];
	char germination_upper[GERMINATION_UPPER_LENGTH + 1];
	char transplanting_lower[TRANSPLANTING_LOWER_LENGTH + 1];
	char transplanting_optimal[TRANSPLANTING_OPTIMAL_LENGTH + 1];
	char transplanting_upper[TRANSPLANTING_UPPER_LENGTH + 1];
	char maturity_lower[MATURITY_LOWER_LENGTH + 1];
	char maturity_optimal[MATURITY_OPTIMAL_LENGTH + 1];
	char maturity_upper[MATURITY_UPPER_LENGTH + 1];
	char frost_tolerance[FROST_TOLERANCE_LENGTH + 1];
	char flowering[FLOWERING_LENGTH + 1];
	char pollination_primary[POLLINATION_PRIMARY_LENGTH + 1];
	char pollination_secondary[POLLINATION_SECONDARY_LENGTH + 1];
};

#define NUM_FIELDS 37

#define ADD_RECORD      0x00000001 // PF1
#define FIRST_RECORD    0x00000002 // PF2
#define FIND_RECORD     0x00000004 // PF3
#define PREVIOUS_RECORD 0x00000008 // PF4
#define NEXT_RECORD     0x00000010 // PF5
#define DELET_RECORD    0x00000020 // PF6
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
int keymouse(struct FIELD fields[], int num_fields, struct CURSOR *cursor);
int paint(struct FIELD fields[], int num_fields, struct CURSOR *cursor, unsigned action);
int rtof(struct FIELD fields[], int num_fields, struct RECORD *record);
int terminal();
int tsv_export();
int tsv_import();
int xerror(const char *message);
unsigned keyboard(struct FIELD fields[], int num_fields, struct CURSOR *cursor, unsigned action);
void *ghdb_open();
char *getfld(const int y, const int x, const int l);

#endif