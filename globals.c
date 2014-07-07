File *CF = 0;				// ptr to current file
Cmd *CMD = 0;				// ptr to current command line
size_t LINESIZE = 100;			// byte to be malloced
size_t TABSIZE = 8;			// default tab lenght
size_t LINEENDMARKER = 81;		// default end of line 0 == option disabled
uint16_t FG = TB_WHITE | TB_BOLD;
uint16_t BG = TB_BLACK;

#ifdef SYNTAX
uint16_t SYNTAX_QM   = TB_BLUE    | TB_BOLD;
uint16_t SYNTAX_NUM  = TB_MAGENTA | TB_BOLD;
uint16_t SYNTAX_TYPE = TB_CYAN    | TB_BOLD;
uint16_t SYNTAX_RES  = TB_YELLOW  | TB_BOLD;
#endif
