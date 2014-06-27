#include "cedit.h"

static size_t h;
static size_t w;
static size_t offset;

/*
 * ensures vertical viewport
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

/*
 * ensures horizontal viewport
 */
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

/*
 * draws line numbers
 */
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

/*
 * draw buffer
 */
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
					if(LINEENDMARKER != 0 && x+CF->anc->p == LINEENDMARKER){
						tb_change_cell(x+offset,y,' ',FG,TB_RED);
					} else {
						tb_change_cell(x+offset,y,' ',FG,BG);
					}
					x++;
				}
				tpos = 0;
			} else {
				if(LINEENDMARKER != 0 && x+CF->anc->p == LINEENDMARKER){
					tb_change_cell(x+offset,y,chr,FG,TB_RED);
				} else {
					tb_change_cell(x+offset,y,chr,FG,BG);
				}
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

/*
 * draws interactive command line
 */
void draw_cmd()
{
	char buf[6];
	uint32_t chr;
	size_t len;

	size_t i;
	size_t pos;
	size_t skip;
	size_t bcnt;

	/* write msg and a whitespace */
	pos = 0;
	bcnt = 0;
	while(bcnt < strlen(CMD->msg) && pos < w){
		len = tb_utf8_char_length(CMD->msg[bcnt]);
		for(i=0; i<len; i++) buf[i]=CMD->msg[bcnt+i];
		tb_utf8_char_to_unicode(&chr, buf);
		bcnt += len;

		tb_change_cell(pos,h-1,chr,TB_BLACK,TB_WHITE);
		pos++;
	}
	tb_change_cell(pos,h-1,' ',TB_BLACK,TB_WHITE);
	pos++;

	/* set cursor */
	tb_set_cursor(pos+CMD->cur-CMD->anc, h-1);

	/* adjust viewport*/
	if((CMD->anc+(w-1)-pos)<CMD->cur)CMD->anc +=CMD->cur-(CMD->anc+(w-1)-pos);
	if(CMD->anc > CMD->cur)CMD->anc -=CMD->anc-CMD->cur;

	/* output */
	bcnt = 0;
	skip = CMD->anc;
	while(bcnt < CMD->blen && pos < w){
		len = tb_utf8_char_length(CMD->c[bcnt]);
		for(i=0; i<len; i++) buf[i]=CMD->c[bcnt+i];
		tb_utf8_char_to_unicode(&chr, buf);
		bcnt += len;

		if (skip == 0) {
			tb_change_cell(pos,h-1,chr,TB_BLACK,TB_WHITE);
			pos++;
		} else {
			skip--;	
		}
	}
	for(i = pos; i < w; i++) tb_change_cell(i,h-1,' ',TB_BLACK,TB_WHITE);
}

/*
 * writes an info text into the cmd area
 */
void draw_info(char *msg)
{
	char buf[6];
	uint32_t chr;
	size_t len;

	size_t i;
	size_t pos;
	size_t bcnt;

	bcnt = 0;
	pos = 0;
	i=0;

	while(bcnt < strlen(msg)){
		len = tb_utf8_char_length(msg[bcnt]);
		for(i=0; i<len; i++) buf[i]=msg[bcnt+i];
		tb_utf8_char_to_unicode(&chr, buf);
		bcnt += len;

		tb_change_cell(pos,h-1,chr,TB_BLACK,TB_WHITE);
		pos++;
	}
	for(i = pos; i < w; i++) tb_change_cell(i,h-1,' ',TB_BLACK,TB_WHITE);

	tb_present();
}

/*
 * draws everything
 */
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
