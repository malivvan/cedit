#include "cedit.h"

/*
 * calculates the byteposition of a single character in an utf-8 char array
 */
size_t misc_utf8_bytepos(char *buf, size_t pos)
{
	size_t i;
	size_t bpos;

	bpos = 0;
	for(i=0;i<pos&&buf[bpos]!=0;i++) bpos += tb_utf8_char_length(buf[bpos]);
	return bpos;
}

/*
 * calculates display position of position in a line
 */
size_t misc_dispos(Line *line, size_t pos)
{
	size_t len;
	size_t bcnt;
	size_t pcnt;
	size_t vcnt;
	size_t tpos;

	vcnt = 0;
	bcnt = 0;
	pcnt = 0;
	tpos = TABSIZE;

	while(bcnt < line->blen && pcnt < pos){
		len = tb_utf8_char_length(line->c[bcnt]);

		if(len == 1 && line->c[bcnt] == '\t'){
			vcnt += tpos;
			tpos = 0;
		} else {
			tpos--;
			vcnt++;
		}

		pcnt++;
		bcnt += len;
		if(tpos == 0) tpos = TABSIZE;
	}
	return vcnt;
}

/*
 * returns char array of 10 chars or less until a dot was found
 */
char *misc_filetype()
{
	size_t i;
	size_t len;
	char buf[10];
	char *ftype;

	if(CF->path == 0) return 0;
	len = strlen(CF->path)-1;

	for(i = len; i >= 0; i--){
		if((len-i) > 9) break;
		if(CF->path[i] == '.'){
			ftype = malloc((len-i));
			strncpy(ftype, buf, (len-i));
			return ftype;
		}
		buf[len-i] = CF->path[i];
	}

	return 0;
}


/*
 * function to detect Numbers and Letters
 */
size_t isNumber(char c)
{
	if (48 <= c && c <= 57) return 1;
	return 0;
}

size_t isLetter(char c)
{
	if((65 <= c && c <= 90) || (97 <= c && c <= 122)) return 1;
	return 0;
}


