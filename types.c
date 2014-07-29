/*
 * Line structure: calling newLine() will return an empty line containing only
 * zeros
 */
typedef struct Line Line;
struct Line {
	char *c;
	size_t blen;
	size_t clen;
	size_t mlen;
	Line *next;
	Line *prev;
};
Line *newLine()
{
	Line *line;
	line = malloc(sizeof(Line));
	line->c = malloc(LINESIZE);
	line->c[0] = 0;
	line->blen = 0;
	line->clen = 0;
	line->mlen = LINESIZE;
	line->next = 0;
	line->prev = 0;
	return line;
}

/*
 * Filepos structure: calling newFilepos() will return an empty Filepos
 */
typedef struct Filepos Filepos;
struct Filepos {
	Line *l;
	size_t p;
};
Filepos *newFilepos()
{
	Filepos *fp;
	fp = malloc(sizeof(Filepos));
	fp->l = 0;
	fp->p = 0;
	return fp;
}

/*
 * File structure: calling newFile() will return a File with an empty Line and
 * a cursor and anchor pointing to the first line. next, prev and path will
 * be zero
 */
typedef struct File File;
struct File {
	Line *first;
	Filepos *cur;
	Filepos *anc;
	File *next;
	File *prev;
	char *path;
	char *type;
};
File *newFile()
{
	File *file;
	file = malloc(sizeof(File));
	file->first = newLine();
	file->cur = newFilepos();
	file->cur->l = file->first;
	file->anc = newFilepos();
	file->anc->l = file->first;
	file->next = 0;
	file->prev = 0;
	file->path = 0;
	file->type = 0;
	return file;
}

typedef struct Cmd Cmd;
struct Cmd{
	char *msg;		// message to be displayed
	char *type;		// type
	char *c;		// content
	char input;		// input method 0=full input 1=only numbers
	size_t cur;		// cursor
	size_t anc;		// anchor
	size_t blen;		// byte lenght
	size_t clen;		// content lenght
	size_t mlen;		// lenght of malloced line
};
Cmd *newCmd()
{
	Cmd *cmd;
	cmd = malloc(sizeof(Cmd));
	cmd->msg = 0;
	cmd->type = 0;
	cmd->c = malloc(LINESIZE);
	cmd->c[0] = 0;
	cmd->input = 0;
	cmd->cur = 0;
	cmd->anc = 0;
	cmd->blen = 0;
	cmd->clen = 0;
	cmd->mlen = 0;
	return cmd;
}
