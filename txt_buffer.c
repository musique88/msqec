#include "txt_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

void buff_constr(txt_buffer* buff, const char* path)
{
	FILE* fp = fopen(path, "r");
	if(fp == NULL)
		fprintf(stderr, "Unable to read file");
	fseek(fp, 0l, SEEK_END);
	buff->data_size = ftell(fp);
	buff->path = malloc(sizeof(char) * strlen(path));
	fseek(fp, 0l, SEEK_SET);
	buff->lines = 0;
	char c;
	for(int i = 0; i < buff->data_size; i++){
		fscanf(fp, "%c", &c);
		buff->data[i] = c;
		if(c == '\n')
			buff->lines++;
	}
	strcpy(buff->path, path);
	fclose(fp);
}

void buff_free(txt_buffer* buff)
{
	free(buff->path);
}

uint get_line_from_pos(txt_buffer* buff, uint pos)
{
	assert(pos < buff->data_size);
	uint current_col = 0;
	bool reset_next = false;
	for(int i = 0; i < pos; i++){
		current_col++;
		if(reset_next)
			current_col = 0;
		if(buff->data[i] == '\n')
			reset_next = true;
	}
	return current_col;
}

uint get_pos_from_line(txt_buffer* buff, uint line)
{
	uint current_line = 0;
	uint i = 0;
	for(; current_line < line; i++)
		if(buff->data[i] == '\n')
			current_line++;
	return i;
}

void get_printable_buffer(printable_buffer* pbuff, txt_buffer* buff, screen_info scr_inf)
{
	memset(pbuff->data, '\0', MAX_PBUFF_SIZE);
	uint line_in_file = scr_inf.line < (scr_inf.height / 2) ? 0 : scr_inf.line - (scr_inf.height / 2);
	uint pos_in_file = get_pos_from_line(buff, line_in_file);
	pbuff->lines = 0;
	pbuff->data_size = 0;
	uint size_in_buff = 0;
	uint lines_limit = scr_inf.line < (scr_inf.height / 2) ? scr_inf.height - (scr_inf.height/2 - scr_inf.line) : scr_inf.height;
	for(int i = pos_in_file; i < buff->data_size && pbuff->lines < lines_limit; i++){
		if(buff->data[i] == '\n')
			pbuff->lines++;
		if(buff->data[i] == '\t')
			pbuff->data_size += TAB_SIZE;
		else
			pbuff->data_size++;
		size_in_buff++;
	}
	uint pos = 0;
	char* buffer_to_write = &buff->data[pos_in_file];
	for(int i = 0; i < size_in_buff; i++){
		if (buffer_to_write[i] == '\t'){ //TODO deal adequately with tabs using sprintf
			for(int j = 0; j < TAB_SIZE; j++)
				pbuff->data[pos+j] = ' ';
			pos += TAB_SIZE;
		}
		else {
			pbuff->data[pos] = buffer_to_write[i];
			pos++;
		}
	}
	if(pbuff->data[pbuff->data_size-1] == '\n')
		pbuff->data[pbuff->data_size-1] = ' ';
	pbuff->first_line_no = line_in_file;
}

