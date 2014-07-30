#include "cedit.h"

/*
 * creates a new selection on current cursor position
 */
void selection_new()
{
	CF->sel = newFilepos();
	CF->sel->l = CF->cur->l;
	CF->sel->p = CF->sel->p;
	draw_all();
}

/*
 * delete current selection
 */
void selection_del()
{
	free(CF->sel);
	CF->sel = 0;
	draw_all();
}
