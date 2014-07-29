#include "cedit.h"

/*
 * creates new Cmd structure and draws it
 */
void cmd_new(char *msg, char *type, char input)
{
	size_t c;
	size_t h;

	CMD = newCmd();
	CMD->msg = msg;
	CMD->type = type;
	CMD->input = input;

	draw_all();
}

/*
 * deletes the current Cmd structure
 */
void cmd_del()
{
	free(CMD->c);
	free(CMD);
	CMD = 0;
	draw_all();
}

/*
 * executes and deletes the current Cmd structure
 */
void cmd_exec()
{
	char *buf;
	char *type;
	size_t num;

	type = CMD->type;
	if(strlen(CMD->c) > 0){
		buf = malloc(strlen(CMD->c));
		buf[0] = '\0';
		strncpy(buf, CMD->c, strlen(CMD->c));
		if(CMD->input == 1) num = atoi(buf);
	} else {
		num = 0;
		buf = 0;
	}

	cmd_del();
	if (type == "savefile") file_save(buf);
	if (type == "openfile") file_open(buf);
	free(buf);
}

/*
 * adds an utf-8 character to the current Cmd struct by shifting all characters
 * from the right side of the cursor len to the right and writing in the gap
 * aftwards
 */
void cmd_input_add(uint32_t ch)
{
	char buf[6];
	size_t i;
	size_t len;
	size_t bpos;

	len = tb_utf8_unicode_to_char(buf, ch);
	bpos = misc_utf8_bytepos(CMD->c, CMD->cur, CMD->blen);
	cmd_ensure_cap(CMD->blen+len);

	for(i = CMD->blen+len; i >= bpos+len; i--)  CMD->c[i] = CMD->c[i-len];
	for(i = bpos; i < bpos+len; i++) CMD->c[i] = buf[i-bpos];

	CMD->blen += len;
	CMD->clen += 1;
	CMD->cur += 1;

	draw_all();
}

/*
 * removes an utf-8 character from the current Cmd struct by shifting all
 * characters from the right side of the cursor len to the left overwriting the
 * character
 */
void cmd_input_del()
{
	size_t i;
	size_t len;
	size_t bpos;

	bpos = misc_utf8_bytepos(CMD->c, CMD->cur-1, CMD->blen);
	len = tb_utf8_char_length(CMD->c[bpos]);

	for(i = bpos; i < CMD->blen; i++) CMD->c[i] = CMD->c[i+len];

	CMD->blen -= len;
	CMD->clen -= 1;
	CMD->cur -= 1;

	draw_all();
}

/*
 * cursor handling: right, left
 */
void cmd_input_left()
{
	if(0 < CMD->cur) CMD->cur--;
	draw_all();
}
void cmd_input_right()
{
	if(CMD->clen > CMD->cur) CMD->cur++;
	draw_all();
}

/*
 * ensures that there are always enough bytes to write new content to
 */
void cmd_ensure_cap(size_t cap)
{
	size_t i;
	i = 0;
	while (CMD->mlen+i < cap) i += LINESIZE;
	if (i > 0){
		CMD->mlen += i;
		CMD->c = realloc(CMD->c,CMD->mlen);
	}
}
