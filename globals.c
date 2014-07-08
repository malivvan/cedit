File *CF = 0;
Cmd *CMD = 0;

uint16_t FG = TB_WHITE | TB_BOLD;
uint16_t BG = TB_BLACK;

size_t LINESIZE = 100;
size_t TABSIZE = 8;
size_t LINEENDMARKER = 81;


#ifdef SYNTAX
/* colors */
uint16_t SYNTAX_BC   = TB_WHITE;
uint16_t SYNTAX_ILC  = TB_WHITE;
uint16_t SYNTAX_RES  = TB_YELLOW  | TB_BOLD;
uint16_t SYNTAX_TYPE = TB_CYAN    | TB_BOLD;
uint16_t SYNTAX_QM   = TB_BLUE    | TB_BOLD;
uint16_t SYNTAX_NUM  = TB_MAGENTA | TB_BOLD;

/* .c */
char *bc_c[3]     = {"/*", "*/", 0};
char *ilc_c[2]    = {"//", 0};
char *res_c[26]    = {"for", "break", "case", "continue", "default", "do",
	             "else", "entry", "for", "goto", "return", "sizeof",
	             "struct", "switch", "while", "auto", "extern", "register",
	             "static", "typedef", "union", "#include", "#define",
	             "#ifdef", "#endif", 0};
char *type_c[11]  = {"signed", "unsigned","char","short", "int", "long",
	             "float", "double", "void", "size_t", 0};

/* .go */
char *bc_go[1]    = {0};
char *ilc_go[1]   = {0};
char *res_go[1]   = {0};
char *type_go[1]  = {0};
#endif
