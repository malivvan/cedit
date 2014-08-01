#include "cedit.h"

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

/*
 * checks if there is a selection open above anchor
 */
short selection_open()
{
	Line *line;

	line = CF->first;
	while(line != CF->anc->l){
		if(line == CF->cur->l) return 1;
		if(line == CF->sel) return 2;
		line = line->next;
	}
	return 0;
}

/*
 * paste clipboard to current file
 */
void selection_paste()
{
	size_t i;
	Line *line;
	Line *cline;
	Line *end;

	if(CLIP == 0) return;

	line = CF->cur->l;
	cline = CLIP;
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

/*
 * copy content from selection to clipboard, if clipboard not empty free it
 * first
 */
void selection_copy()
{
	size_t i;
	Line *line;
	Line *cline;
	short selstat;

	line = CF->first;

	/* if CLIP in use clear */
	if(CLIP != 0) selection_free_clipboard();

	/* 0=none 1=cur 2=sel */
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
			if(CLIP == 0) {
				CLIP = cline;
			} else {
				cline->prev = CLIP;
				CLIP->next = cline;
				CLIP = CLIP->next;
			}
		}

		if(selstat == 1 && line == CF->sel) break;
		if(selstat == 2 && line == CF->cur->l) break;
		line = line->next;
	}

	/* rewind CLIP  */
	while (CLIP->prev != 0) CLIP = CLIP->prev;

	/* delete selection */
	selection_del();
}

/*
 * frees current clipboard
 */
void selection_free_clipboard()
{
	while(CLIP != 0){
		free(CLIP->c);
		if(CLIP->next != 0){
			CLIP = CLIP->next;
			free(CLIP->prev);
		} else {
			free(CLIP);
			break;
		}
	}
	CLIP = 0;
}
