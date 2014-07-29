#include "cedit.h"

/*
 * creates a new empty file as current file - it will be located after the last
 * file - otherwise it will set itself as first file
 */
void file_new()
{
	File *file;
	file = newFile();

	if(CF == 0) {
		CF = file;
	} else {
		while(CF->next != 0) CF = CF->next;
		CF->next = file;
		file->prev = CF;
		CF = CF->next;
	}

	draw_all();
}

/*
 * loads a file into memory by calling file_new() and populating it with content
 */
void file_open(char *path)
{
	char ch;
	FILE *fp;
	File *file;
	Line *line;
	size_t len;

	if(path == 0) {
		dialog_openfile();
		return;
	}

	if(access(path, R_OK) != -1){
		fp = fopen(path, "r");
	} else {
		fp = fopen(path, "w");
	}
	if(fp == NULL){
		dialog_openfile_failure();
		return;
	}

	file_new();

	CF->path = malloc(strlen(path));
	strncpy(CF->path, path, strlen(path));
	CF->type = misc_filetype();

	line = CF->first;
	ch = fgetc(fp);
	while(ch != EOF){
		if (ch != 10) {
			len = tb_utf8_char_length(ch);
			line->blen += len;
			core_ensure_cap(line, line->blen);
			while(len > 0 && ch != EOF) {
				strncat(line->c, &ch, 1);
				ch = fgetc(fp);
				len--;
			}
			line->clen += 1;
		} else if((ch = fgetc(fp)) != EOF){
			line->next = newLine();
			line->next->prev = line;
			line = line->next;
		}
	}
	fclose(fp);
	draw_all();
	dialog_openfile_success();
}

/*
 * close the current file, freeing all of its contents and relinking remaining
 * files if neccessary
 */
void file_close()
{
	Line *line;
	File *f;

	f = CF;

	line = f->first;
	while(line != 0) {
		free(line->c);
		if(line->next != 0) {
			line = line->next;
			free(line->prev);
		} else {
			free(line);
			break;
		}
	}

	free(f->cur);
	free(f->anc);
	free(f->path);
	free(f->type);

	if(f->prev != 0) f->prev->next = f->next;
	if(f->next != 0) f->next->prev = f->prev;

	if(f->prev != 0){
		CF = f->prev;
	} else if(f->next != 0) {
		CF = f->next;
	} else {
		CF = 0;
	}

	free(f);
}

/*
 * saves current file to disc
 */
void file_save(char *path)
{
	FILE *fp;
	Line *line;
	size_t i;

	if(path != 0 && strlen(path) > 0){
		CF->path = malloc(strlen(path));
		strncpy(CF->path, path, strlen(path));
		CF->type = misc_filetype();
	}

	if(CF->path == 0) {
		dialog_savefile();
		return;
	}

	line = CF->first;

	fp = fopen(CF->path, "w+");
	if(fp == NULL){
		dialog_savefile_failure();
		return;
	}

	while(line != 0) {
		for(i = 0; i < line->blen; i++){
			fputc(line->c[i], fp);
		}
		fputc('\n', fp);
		line = line->next;
	}

	draw_all();
	fclose(fp);
	dialog_savefile_success();
}

/*
 * switch files to the right, left or to the n-th file
 */
void file_switch_right()
{
	if(CF->next != 0) {
		CF = CF->next;
		draw_all();
	}
}
void file_switch_left()
{
	if(CF->prev != 0) {
		CF = CF->prev;
		draw_all();
	}
}
void file_switch(size_t n){
	File *f;
	size_t c;

	f = CF;
	while(f->prev != 0) f = f->prev;

	for(c = 1; c < n ; c++){
		if(f->next == 0) break;
		f = f->next;
	}
	CF = f;

	draw_all();
}
