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
