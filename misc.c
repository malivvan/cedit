#include "cedit.h"

/*
 * calculates the byteposition of a single character in an utf-8 char array
 */
size_t misc_utf8_bytepos(char *buf, size_t pos)//REVIEWED
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
