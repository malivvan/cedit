typedef struct Line Line;
struct Line {
	char *c;		// content
	size_t blen;		// byte lenght
	size_t clen;		// content lenght
	size_t mlen;		// lenght of malloced line
	Line *next;		// ptr to next line
	Line *prev;		// ptr to prev line
};

typedef struct Filepos Filepos;
struct Filepos {
	Line *l;		// line
	size_t p;		// position in line
};

typedef struct File File;
struct File {
	Line *first;		// ptr to first line
	Filepos *cur;		// cursor
	Filepos *anc;		// anchor
	File *next;		// ptr to next file
	File *prev;		// ptr to prev file
	char *path;		// path
};

typedef struct Cmd Cmd;
struct Cmd{
	char *msg;		// message to be displayed
	char *type;		// type
	char *c;		// content
	size_t cur;		// cursor
	size_t anc;		// anchor
	size_t blen;		// byte lenght
	size_t clen;		// content lenght
	size_t mlen;		// lenght of malloced line
};
