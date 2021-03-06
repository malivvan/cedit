/*
 * File:        core.c
 * Description: the core functionality for editing files
 */
void core_add_char(uint32_t ch);
void core_del_char();
void core_new_line();
void core_del_line();
void core_right();
void core_left();
void core_up();
void core_down();
void core_pageup();
void core_pagedown();
void core_change_line(Line *line);
void core_remove_line();
void core_ensure_cap(Line *line, size_t cap);

/*
 * File:        cmd.c
 * Description: handle dynamic input (like filenames)
 */
void cmd_new(char *msg, char *type, char input);
void cmd_del();
void cmd_exec();
void cmd_input_add(uint32_t ch);
void cmd_input_del();
void cmd_input_left();
void cmd_input_right();
void cmd_ensure_cap(size_t cap);

/*
 * File:        draw.c
 * Description: terminal output
 */
void draw_ensure_viewport_v();
void draw_ensure_viewport_h();
void draw_numbers();
void draw_buffer();
void draw_cmd();
void draw_info(char *msg);
void draw_all();

/*
 * File:        file.c
 * Description: basic file functions (open, close, save, ...)
 */
void file_new();
void file_open(char *path);
void file_close();
void file_save();
void file_switch_right();
void file_switch_left();
void file_switch(size_t n);

/*
 * File:        dialog.c
 * Description: dialog functions for easy translation
 */
void dialog_savefile();
void dialog_savefile_success();
void dialog_savefile_failure();
void dialog_openfile();
void dialog_openfile_success();
void dialog_openfile_failure();

/*
 * File:        syntax.c
 * Description: syntax coloring functions
 */
int  syntax_BC(Line *l, size_t bcnt, size_t len);
void syntax_BC_open();
int  syntax_ILC(Line *l, size_t bcnt, size_t len);
int  syntax_QM(Line *l, size_t bcnt, size_t len);
int  syntax_NUM(Line *l, size_t bcnt, size_t len);
int  syntax_WORD(Line *l, size_t bcnt, size_t len);
void syntax_reset();
void syntax_all(Line *line, size_t bcnt, size_t len);

/*
 * File:        shared.c
 * Description: function shared between other functions
 */
size_t match_line_buf(Line *l, size_t cur, char *buf);
size_t match_buf_buf(char *buf1, size_t cur1, char *buf2, size_t cur2, size_t len);
size_t utf8_bytepos(char *buf, size_t pos, size_t blen);
size_t dispos(Line *line, size_t pos);
char *getFiletype();
size_t isNumber(char c);
size_t isLetter(char c);
size_t isSpecial(char c);

/*
 * File:        selection.c
 * Description: handles line selection and functions like copy/paste etc.
 */
void selection_new();
void selection_del();
short selection_open();
void selection_paste();
void selection_copy();
void selection_free_clipboard();

/*
 * File:	types.c
 * Description:	structs and functions that create them
 */
Line *newLine();
Filepos *newFilepos();
File *newFile();
Cmd *newCmd();
