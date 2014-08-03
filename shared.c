#include "cedit.h"


/*
 * matches line[cur+i] with full buffer
 * always save to call - validates input
 * returns 0 if no match was found and len if there was a match
 */
size_t match_line_buf(Line *l, size_t cur, char *buf)
{
	size_t len;
	len = strlen(buf);
	if((cur + len) <= l->blen){
		return match_buf_buf(l->c, cur, buf, 0, len);
	}
	return 0;
}

/*
 * matches buf1[cur1+i] with buf2[cur2+i] i=0...len-1
 * does not care about lenght of buffer
 * returns 0 of no match was found and len if there was a match
 */
size_t match_buf_buf(char *buf1, size_t cur1, char *buf2, size_t cur2, size_t len)
{
	size_t i;
	for(i = 0; i < len; i++){
		if(buf1[cur1+i] != buf2[cur2+i]){
			return 0;
		}
	}
	return len;
}

/*
 * calculates the byteposition of a single character in an utf-8 char array
 */
size_t utf8_bytepos(char *buf, size_t pos, size_t blen)
{
	size_t i;
	size_t bpos;

	bpos = 0;
	for(i = 0; i<pos && bpos <= blen ; i++){
		bpos += tb_utf8_char_length(buf[bpos]);
	}
	return bpos;
}

/*
 * calculates display position of position in a line
 */
size_t dispos(Line *line, size_t pos)
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
char *getFiletype()
{
	size_t i;
	size_t len;
	char buf[10];
	char *ftype;

	if(CF->path == 0) return 0;
	len = strlen(CF->path)-1;

	for(i = len; i >= 0; i--){
		if((len-i) > 9) break;
		if(CF->path[i] == '.' && i != len){
			ftype = malloc((len-i));
			ftype[0] = '\0';
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

size_t isSpecial(char c)
{
	if((32  <= c && c <= 47 ) || (58  <= c && c <= 64 )||
	   (91  <= c && c <= 94 ) || (123 <= c && c <= 126)||
	   (c == 9) || (c == 96)) return 1;
	return 0;
}

