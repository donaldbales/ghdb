

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

struct FIELD {
	struct LABEL label;
	struct VALUE value;
};

struct CURSOR {
	int y;
	int x;
};

int terminal();
int keyboard(struct FIELD fields[], int num_fields, struct CURSOR curmax, struct CURSOR cursor);
int keymouse();
int database();
