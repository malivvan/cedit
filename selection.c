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
	
}

void selection_copy()
{
	size_t i;
	Line *line;
	Line *cline;
	short selstat;
	

	line = CF->first;

	/* -1=done 0=none 1=cur 2=sel */
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
				clipboard->next = cline;
				clipboard = clipboard->next;
			}		
		}
		
		if(selstat == 1 && line == CF->sel) selstat = -1;
		if(selstat == 2 && line == CF->cur->l) selstat = -1;
		line = line->next;
	}

	/* rewind clipboard  */
	while (clipboard->prev != 0) clipboard = clipboard->prev;
}
