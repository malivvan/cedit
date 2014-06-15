#include "cedit.h"

static size_t h;
static size_t w;
static size_t offset;

/*
 * ensures vertical vieport
 */
void draw_ensure_viewport_v()
{
	Line *line;
	size_t i;
	size_t anc_pos;
	size_t cur_pos;

	/* get cursor and anchor position */
	line = CF->first;
	for(anc_pos=0;line!=0&&line!=CF->anc->l;anc_pos++)line=line->next;
	line = CF->first;
	for(cur_pos=0;line!=0&&line!=CF->cur->l;cur_pos++)line=line->next;

	/* move vertical viewport anchor */
	if(cur_pos < anc_pos) {
		for(i=0; i<anc_pos-cur_pos&& CF->anc->l->prev!=0; i++)
			CF->anc->l = CF->anc->l->prev;
	}
	if(anc_pos + (h-1) < cur_pos){
		for(i=0; i<cur_pos-(anc_pos+(h-1)) && CF->anc->l->next!=0; i++)
			CF->anc->l = CF->anc->l->next;
	}
}

void draw_ensure_viewport_h()
{
	size_t vpos;
	size_t vpos_cur;

	/* get visual cursor position */
	vpos_cur = misc_dispos(CF->cur->l,CF->cur->p);

	/* move horizontal viewport anchor */
	if(CF->anc->p > vpos_cur){
		CF->anc->p--;
		draw_ensure_viewport_h();
	}
	vpos = vpos_cur-CF->anc->p;
	if(vpos > (w-offset-1)) CF->anc->p += vpos-(w-offset-1);
}


void draw_numbers()
{
	char buf[6];
	Line *line;
	size_t firstl;
	size_t lastl;
	size_t x;
	size_t y;

	/* get first and last line number of current viewport */
	line = CF->first;
	for(firstl=1;line!=0&&line!=CF->anc->l;firstl++)line=line->next;
	for(lastl=firstl;line->next!=0&&lastl-firstl<h;lastl++)line=line->next;

	/* calculate space from last number */
	sprintf(buf, "%d", lastl);
	offset = strlen(buf);

	/* output */
	for(y=0; y<h ; y++){
		sprintf(buf, "%d", firstl+y);
		for(x=0; x<offset; x++){
			if(firstl+y <= lastl && offset-strlen(buf) <= x){
				tb_change_cell(x,y,buf[x-(offset-strlen(buf))],TB_BLACK,TB_WHITE);
			} else {
				tb_change_cell(x,y,' ',TB_BLACK,TB_WHITE);
			}
		tb_change_cell(offset,y,' ',TB_BLACK,TB_WHITE);
		}
	}
	offset++;
}

void draw_buffer()
{
	Line *line;

	char buf[6];
	uint32_t chr;
	size_t len;

	size_t firstl;
	size_t lastl;
	size_t x;
	size_t y;
	size_t bcnt;
	size_t tpos;
	size_t skip;
	size_t i;

	line = CF->anc->l;

	for(y = 0; line != 0 && y < h; y++){
		x = 0;
		bcnt = 0;

		tpos = TABSIZE;
		skip = CF->anc->p;

		while(bcnt < line->blen){

			/* ensure TABSIZE regeneration */
			if(tpos == 0) tpos = TABSIZE;

			/* extracts utf8 char making it available as uint32_t */
			len = tb_utf8_char_length(line->c[bcnt]);
			for(i=0; i<len; i++) buf[i]=line->c[bcnt+i];
			tb_utf8_char_to_unicode(&chr, buf);
			bcnt += len;

			/* viewport handling */
			if(skip > 0){
				if(chr == 9) {
					while(skip > 0 && tpos > 0){
						tpos--;
						skip--;
					}
					if(tpos==0) continue;
				} else {
					tpos--;
					skip--;
					continue;
				}
			}

			/* output */
			if(chr == 9){
				for(i = 0; i < tpos; i++){
					tb_change_cell(x+offset,y,' ',FG,BG);
					x++;
				}
				tpos = 0;
			} else {
				tb_change_cell(x+offset,y,chr,FG,BG);
				tpos--;
				x++;
			}
		}
		if(CF->cur->l == line) {
			tb_set_cursor(offset+misc_dispos(line,CF->cur->p)-CF->anc->p, y);
		}
		line = line->next;
	}
}

// TODO
void draw_cmd()
{
	size_t h;		// height of last line
	size_t os;		// offset
	size_t cp;		// cellpos
	size_t bp;		// bytepos
	size_t skp;		// skip
	size_t len;		// byte lenght of current char
	size_t bc;		// byte counter
	uint32_t chr;		// utf8 char
	char buf[6];		// tmp buffer to hold utf8 char

	// get height
	h = tb_height()-1;

	// initialize counter
	os = 0;
	bp = 0;

	// write full msg
	while(bp < strlen(CMD->msg) && os < tb_width()){
		len = tb_utf8_char_length(CMD->msg[bp]);

		// get all bytes of utf8 char
		for(bc = 0; bc < len; bc++) {
			buf[bc] = CMD->msg[bp+bc];
		}

		// convert to utf8 value
		tb_utf8_char_to_unicode(&chr, buf);
		tb_change_cell(os,h,chr,TB_BLACK,TB_WHITE);


		// raise byte counter
		bp += len;
		os++;
	}

	// write a single whitespace
	tb_change_cell(os,h,' ',TB_BLACK,TB_WHITE);
	os++;

	// adjust the viewport
	if((CMD->anc+(w-1)-os)<CMD->cur)CMD->anc +=CMD->cur-(CMD->anc+(w-1)-os);
	if(CMD->anc > CMD->cur)CMD->anc -=CMD->anc-CMD->cur;

	// initialize counter and skip variable
	cp = 0;
	bp = 0;
	skp = CMD->anc;

	// get first byte of possible utf8 char
	// extract write entire char to a single cell
	while(bp < CMD->blen && (cp+os) < tb_width()){
		len = tb_utf8_char_length(CMD->c[bp]);

		// get all bytes of utf8 char
		for(bc = 0; bc < len; bc++) {
			buf[bc] = CMD->c[bp+bc];
		}

		// convert to utf8 value
		tb_utf8_char_to_unicode(&chr, buf);

		// write into cell
		if (skp == 0) {
			tb_change_cell(cp+os,h,chr,TB_BLACK,TB_WHITE);
			cp++;
		} else {
			skp--;
		}

		// raise byte counter
		bp += len;
	}

	// fill rest of line with empty cells
	while((cp+os) < tb_width()){
		tb_change_cell(cp+os,h,' ',TB_BLACK,TB_WHITE);
		cp++;
	}

	// set cursor
	tb_set_cursor(os+CMD->cur-CMD->anc, h);


}

/*
 * writes an info text into the cmd area
 */
void draw_info(char *msg)
{
	size_t c;
	size_t h;

	h = tb_height()-1;

	for(c = 0; c < tb_width(); c++) {
		if(c < strlen(msg)){
			tb_change_cell(c,h,msg[c],TB_BLACK,TB_WHITE);
		} else {
			tb_change_cell(c,h,' ',TB_BLACK,TB_WHITE);
		}
	}

	tb_present();
}

void draw_all()
{
	h = tb_height();
	w = tb_width();

	tb_clear();
	draw_ensure_viewport_v();
	draw_numbers();
	draw_ensure_viewport_h();
	draw_buffer();
	if(CMD != 0) draw_cmd();
	tb_present();
}

