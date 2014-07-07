#include "cedit.h"

static size_t delay = 0;
static size_t BC_lock = 0;
static size_t ILC_lock = 0;


/*
 * decides if there is an inline comment or not
 */
void syntax_ILC(Line *l, size_t bcnt, size_t len)
{
	size_t i;
	size_t x;
	size_t isILC;
	char *ftype;

	/* reset @ every new line because this is an inline comment */
	if(bcnt == 0) syntax_reset();

	/* syntax highlighting depending on filetype */
	char **ilc = 0;
	char *ilc_c[2]   = {"//", 0};
	char *ilc_go[3]  = {"aa", "bbb", 0};
	ftype = misc_filetype();
	if(ftype == 0){
		return;
	} else {
		if(strcmp(ftype, "c" ) == 0)        ilc = ilc_c;
		if(strcmp(ftype, "go") == 0)        ilc = ilc_go;
		free(ftype);
	}
	if(ilc == 0) return;

	/* check every inline comment delimiter */
	for(x = 0; ilc[x] != 0; x++){
		isILC = 1;
		if(bcnt + strlen(ilc[x]) <= l->blen){
			for(i = 0; i < strlen(ilc[x]); i++){
				if(l->c[bcnt+i] != ilc[x][i]){
					isILC = 0;
					break;
				}
				if(i == strlen(ilc[x])-1 && isILC){
					FG = TB_WHITE;
					ILC_lock = 1;
					return;
				}
			}
		}
	}
}

/*
 * decides if there is an block comment or not
 */
void syntax_BC(Line *l, size_t bcnt, size_t len)
{
	size_t x;
	size_t i;
	size_t isBC;
	char *ftype;

	/* syntax highlighting depending on filetype */
	char **bc = 0;
	char *bc_c[3]   =      {"/*", "*/", 0};
	char *bc_go[3]  =      {"aa", "bbb", 0};
	ftype = misc_filetype();
	if(ftype == 0){
		return;
	} else {
		if(strcmp(ftype, "c" ) == 0)        bc = bc_c;
		if(strcmp(ftype, "go") == 0)        bc = bc_go;
		free(ftype);
	}
	if(bc == 0) return;

	/* check every block comment delimiter */
	for(x = 0; bc[x] != 0; x++){
		isBC = 1;
		/* closing braces */
		if(x % 2 == 1){
			if(bcnt + strlen(bc[x]) <= l->blen){
				for(i = 0; i < strlen(bc[x]); i++){
					if(l->c[bcnt+i] != bc[x][i]){
						isBC = 0;
						break;
					}
					if(i == strlen(bc[x])-1 && isBC){
						delay = strlen(bc[x]);
						return;
					}
				}
			}
		/* opening braces */
		} else {
			if(bcnt + strlen(bc[x]) <= l->blen){
				for(i = 0; i < strlen(bc[x]); i++){
					if(l->c[bcnt+i] != bc[x][i]){
						isBC = 0;
						break;
					}
					if(i == strlen(bc[x])-1 && isBC){
						FG = TB_WHITE;
						BC_lock = 1;
						return;
					}
				}
			}
		}
	}
}




/*
 * if quotation is detected count the utf8 chars until the next quotation mark
 * of the same kind and set the calculated value as delay
 */
void syntax_QM(Line *l, size_t bcnt, size_t len)
{
	char c;
	size_t i;
	size_t d;

	if(l->c[bcnt] != 39 && l->c[bcnt] != 34) return;

	c = l->c[bcnt];
	d = 1;

	for(i = 1; bcnt+i <= l->blen; i += tb_utf8_char_length(l->c[bcnt+i])){
		d++;
		if(l->c[bcnt+i] == c){
			FG = TB_BLUE | TB_BOLD;
			delay = d;
		}
	}
}

/*
 * hightlights every number that is not enclosed by invalid chars
 */
void syntax_NUM(Line *l, size_t bcnt, size_t len)
{
	size_t i;

	if(isNumber(l->c[bcnt])){
		/* check front */
		if(bcnt != 0){
			if(!((32  <= l->c[bcnt-1] && l->c[bcnt-1] <= 47 )||
			     (58  <= l->c[bcnt-1] && l->c[bcnt-1] <= 64 )||
	                     (91  <= l->c[bcnt-1] && l->c[bcnt-1] <= 96 )||
	                     (123 <= l->c[bcnt-1] && l->c[bcnt-1] <= 126)||
	                     (l->c[bcnt-1] == 95))) return;
	        }

		/* break if a non-number was found */
		for(i = 1; bcnt+i <= l->blen; i++){
			if(!(isNumber(l->c[bcnt+i]))) break;
		}

		/* check back */
		if(bcnt+i < l->blen){
			if(!((32  <= l->c[bcnt+i] && l->c[bcnt+i] <= 64 )||
			     (91  <= l->c[bcnt+i] && l->c[bcnt+i] <= 96 )||
                             (123 <= l->c[bcnt+i] && l->c[bcnt+i] <= 126)||
                             (l->c[bcnt+i] == 95))) return;
                }

		/* set FG and a delay */
		FG = TB_MAGENTA | TB_BOLD;
		delay = i;
	}
}

/*
 * parses every word, decides if it is a type or a reserved word and
 * hightlights it
 */
void syntax_WORD(Line *l, size_t bcnt, size_t len)
{
	size_t i;
	size_t x;
	size_t isType;
	char *ftype;

	/* type will hold datatypes, res(erved) will hold reserved words and
	   word will point to one of them */
	char **word;
	char **type = 0;
	char **res = 0;

	/* .c */
	char *res_c[2]     =   {"break", 0};
	char *type_c[11]   =   {"signed", "unsigned","char","short", "int"
		                "long", "float", "double", "void", "size_t", 0};

	/* .go */
	char *res_go[2]    =   {"break", 0};
	char *type_go[3]   =   {"aa", "bbb", 0};

	/* decide which words to highlight depending on filetype */
	ftype = misc_filetype();
	if(ftype == 0){
		return;
	} else {
		if(strcmp(ftype, "c" ) == 0) { type = type_c; res = res_c;     }
		if(strcmp(ftype, "go") == 0) { type = type_go; res = res_go;   }
		free(ftype);
	}
	if(type == 0 || res == 0) return;

	/* detect words and highlight them if they match */
	for(x = 0; type[x] != 0; x++){
		isType = 1;
		if(bcnt + strlen(type[x]) <= l->blen){
			for(i = 0; i < strlen(type[x]); i++){
				if(l->c[bcnt+i] != type[x][i]){
					isType = 0;
					break;
				}
				if(i == strlen(type[x])-1 && isType){
					if(bcnt!=0 &&
					   !isSpecial(l->c[bcnt-1]))  return;
					if(bcnt+i+1 < l->blen &&
					   !isSpecial(l->c[bcnt+i+1]))return;

					FG = TB_CYAN | TB_BOLD;
					delay = i+1;
					return;
				}
			}
		}
	}
}

/*
 * resets the whole syntax system to the defaults
 */
void syntax_reset()
{
	BC_lock = 0;
	ILC_lock = 0;
	FG = TB_WHITE | TB_BOLD;
	BG = TB_BLACK;
}

/*
 * called from the draw function once every char
 */
void syntax_all(Line *line, size_t bcnt, size_t len)
{
	if(delay == 1) syntax_reset();
	if(delay > 0) delay--;
	if(delay > 0) return;

	/* comments: block and inline comments preclude each other */
	if(!BC_lock) syntax_ILC(line, bcnt, len);
	if(!ILC_lock) syntax_BC(line, bcnt, len);

	/* numbers, quotations and words: only if there is no ongoing comment */
	if(BC_lock || ILC_lock) return;
	syntax_QM(line, bcnt, len);
	syntax_NUM(line, bcnt, len);
	syntax_WORD(line, bcnt, len);
}
