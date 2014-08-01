#include "cedit.h"

Line *clipboard = 0;

/*
 * creates a new selection on current cursor position
 */
void selection_new()
{
	CF->sel = CF->cur->l;
	draw_all();
}

/*
 * delete current selection
 */
void selection_del()
{
	CF->sel = 0;
	draw_all();
}

void selection_paste()
{
	size_t i;
	Line *line;
	Line *cline;
	Line *end;
	
	if(clipboard == 0) return;

	line = CF->cur->l;
	cline = clipboard;
	end = CF->cur->l->next;
	
	while(cline != 0) {

		/* create newline and ensure linkage  */
		line->next = newLine();
		line->next->prev = line;
		line = line->next;	

		/* ensure cap of line */
		if(cline->mlen > line->mlen){
			line->c = realloc(line->c, cline->mlen);
			line->mlen = cline->mlen;
		} 
		
		/* copy over content and counters */
		for(i = 0; i <= cline->blen; i++) line->c[i] = cline->c[i];
		line->blen = cline->blen;
		line->clen = cline->clen;

		cline = cline->next;
	}

	/* links lines */
	line->next = end;
	if(end != 0) end->prev = line;
		
	draw_all();
}

void selection_copy()
{
	size_t i;
	Line *line;
	Line *cline;
	short selstat;
	
	line = CF->first;

	/* if clipboard in use clear */
	if(clipboard != 0) selection_free_clipboard();

	/* 0=none 1=cur 2=sel 3=delay */
	selstat = 0;

	while(line != 0){
		if(selstat == 0 && line == CF->cur->l) selstat = 1;
		if(selstat == 0 && line == CF->sel) selstat = 2;

		/* copy here */
		if(selstat > 0) {
			cline = newLine();

			/* ensure cap of cline */
			if(cline->mlen < line->mlen){
				cline->c = realloc(cline->c, line->mlen);
				cline->mlen = line->mlen;
			}
			
			/* copy over content and counters */
			for(i =0; i <= line->blen; i++) cline->c[i] = line->c[i];
			cline->blen = line->blen;
			cline->clen = line->clen;
			
			/* set to correct position */
			if(clipboard == 0) {
				clipboard = cline;
			} else {
				cline->prev = clipboard;
				clipboard->next = cline;
				clipboard = clipboard->next;
			}

			/* if delay was enabled stop copying */
			if(selstat == 3) break;
		}
		
		if(selstat == 1 && line == CF->sel) selstat = 3;
		if(selstat == 2 && line == CF->cur->l) selstat = 3;
		line = line->next;
	}

	/* rewind clipboard  */
	while (clipboard->prev != 0) clipboard = clipboard->prev;
}

void selection_free_clipboard()
{
	while(clipboard != 0){
		free(clipboard->c);
		if(clipboard->next != 0){
			clipboard = clipboard->next;
			free(clipboard->prev);	
		} else {
			free(clipboard);
			break;
		}
	}
	clipboard = 0;	
}
