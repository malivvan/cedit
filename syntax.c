#include "cedit.h"

static size_t delay = 0;
static size_t BC_lock = 0;
static size_t ILC_lock = 0;
static size_t QML_lock = 0;
static size_t QMS_lock = 0;

int syntax_isStartDelimiter(char chr)
{
	if(chr == ' ' || chr == '(' || chr == ';') return 1;
	return 0;
}

int syntax_isEndDelimiter(char chr)
{
	if(chr == ' ' || chr == ')') return 1;
	return 0;
}
int syntax_isQut(uint32_t chr)
{
	if(chr == 34 || chr == 39) return 1;
	return 0;
}

/*
 * decides if there is an inline comment or not
 */
void syntax_ILC(Line *l, size_t bcnt, size_t len)
{
	size_t i;
	size_t x;
	size_t isILC;
	char *ftype;

	/* do not do stuff if there is an ongoing blockcomment */
	if(BC_lock == 1) return;
	if(QML_lock == 1) return;
	if(QMS_lock == 1) return;

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

	/* do not do stuff if there is an ongoing inline comment */
	if(ILC_lock == 1) return;
	if(QML_lock == 1) return;
	if(QMS_lock == 1) return;

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

void syntax_NUM(Line *l, size_t bcnt, size_t len)
{
	size_t i;

	if(BC_lock == 1) return;
	if(ILC_lock == 1) return;
	if(QML_lock == 1) return;
	if(QMS_lock == 1) return;

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

/* quotation mark large (") */
void syntax_QML(Line *l, size_t bcnt, size_t len)
{
	size_t i;

	if(BC_lock == 1) return;
	if(ILC_lock == 1) return;

	if(l->c[bcnt] == 34){
		if(QML_lock == 1){
			delay = 1;
		} else {
			QML_lock = 1;
			FG = TB_BLUE | TB_BOLD;
		}
	}
}

/* quotation mark small (') */
void syntax_QMS(Line *l, size_t bcnt, size_t len)
{
	size_t i;

	if(BC_lock == 1) return;
	if(ILC_lock == 1) return;

	if(l->c[bcnt] == 39){
		if(QMS_lock == 1){
			delay = 1;
		} else {
			QMS_lock = 1;
			FG = TB_BLUE | TB_BOLD;
		}
	}
}

void syntax_WORD(Line *l, size_t bcnt, size_t len)
{

}


/*
 * resets the whole syntax system to the defaults
 */
void syntax_reset()
{
	BC_lock = 0;
	ILC_lock = 0;
	QML_lock = 0;
	QMS_lock = 0;
	FG = TB_WHITE | TB_BOLD;
	BG = TB_BLACK;
}

/*
 * called from the draw function once every char
 */
void syntax_all(Line *line, size_t bcnt, size_t len)
{
	if(delay == 1) syntax_reset();
	if(delay > 0)  delay--;
	if(delay > 0)  return;

	syntax_ILC(line, bcnt, len);
	syntax_BC(line, bcnt, len);
	syntax_NUM(line, bcnt, len);
	syntax_QML(line, bcnt, len);
	syntax_QMS(line, bcnt, len);
	syntax_WORD(line, bcnt, len);
}
