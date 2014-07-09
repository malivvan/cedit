#define TABSIZE 8
#define LINEENDMARKER 81
#define LINESIZE 100

#ifdef SYNTAX
 #define SYNTAX_BC    TB_WHITE;
 #define SYNTAX_ILC   TB_WHITE;
 #define SYNTAX_RES   TB_YELLOW  | TB_BOLD;
 #define SYNTAX_TYPE  TB_CYAN    | TB_BOLD;
 #define SYNTAX_QM    TB_BLUE    | TB_BOLD;
 #define SYNTAX_NUM   TB_MAGENTA | TB_BOLD;
#endif
