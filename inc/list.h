struct Date {
	int year;
	int month;
	int day;
};

struct Time {
	int hours;
	int minutes;
};

struct Node {
	struct Date date;
	struct Time time;
	char name[10];
	char loc[10];
	struct Node *next;
};

struct Node *parse(char *buf);
int list_create(struct Node *node);
int list_delete(struct Node *node);
int list_modify(struct Node *node);
