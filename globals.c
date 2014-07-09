File *CF = 0;
Cmd *CMD = 0;

uint16_t FG = TB_WHITE | TB_BOLD;
uint16_t BG = TB_BLACK;

#ifdef SYNTAX
/* .c */
char *bc_c[3]     = {"/*", "*/", 0};
char *ilc_c[2]    = {"//", 0};
char *res_c[27]   = {"for", "break", "case", "continue", "default", "do",
	             "else", "entry", "for", "goto", "return", "sizeof",
	             "struct", "switch", "while", "auto", "extern", "register",
	             "static", "typedef", "union", "#include", "#define",
	             "#ifdef", "#endif", "if", 0};
char *type_c[11]  = {"signed", "unsigned","char","short", "int", "long",
	             "float", "double", "void", "size_t", 0};

/* .go */
char *bc_go[1]    = {0};
char *ilc_go[1]   = {0};
char *res_go[1]   = {0};
char *type_go[1]  = {0};
#endif
