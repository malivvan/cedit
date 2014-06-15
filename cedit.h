/*
 * File:        core.c
 * Description: the core functionality for editing files
 * Reviewed:	08.06.2014
 */
void core_add_char(uint32_t ch);
void core_del_char();
void core_new_line();
void core_del_line();
void core_right();
void core_left();
void core_up();
void core_down();
void core_change_line(Line *line);			// MARKED FOR REWRITE
void core_ensure_cap(Line *line, size_t cap);

/*
 * File:        cmd.c
 * Description: handle dynamic input (like filenames)
 * Reviewed:	08.06.2014
 */
void cmd_new(char *msg, char *type);
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
 * Reviewed:    15.06.2014
 */
void draw_ensure_viewport_v();
void draw_ensure_viewport_h();
void draw_numbers();
void draw_buffer();
void draw_ensure_viewport_cmd();
void draw_cmd();				// MARKED FOR REVIEW
void draw_info(char *msg);			// MARKED FOR REVIEW
void draw_all();

/*
 * File:        file.c
 * Description: basic file functions (open, close, save, ...)
 * Reviewed:    15.06.2014
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
void dialog_savedfile();
void dialog_openfile();
void dialog_openedfile();

/*
 * File:        misc.c
 * Description: helper functions
 */
size_t misc_utf8_bytepos(char *c, size_t pos);
size_t misc_dispos(Line *line, size_t pos);
