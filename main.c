#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <ncurses.h>
#include <stdlib.h>
#include "txt_buffer.h"
#include "utils.h"

enum MODE {MODE_COMMAND, MODE_INSERT} mode = MODE_COMMAND;

void print_pbuff(printable_buffer* pbuff, screen_info scr_inf)
{
	clear();
	move(scr_inf.height/2 - scr_inf.line, 0);
	uint current_line = pbuff->first_line_no;
	printw("%*d ", LINE_NO_SIZE, current_line);
	for(int i = 0; i < pbuff->data_size; i++){
		addch(pbuff->data[i]);
		if(pbuff->data[i] == '\n'){
			printw("%*d ", LINE_NO_SIZE, ++current_line);
		}
	}
	move(scr_inf.height/2, scr_inf.col + LINE_NO_SIZE + 1);
	refresh();
}

int main(int argc, char const *argv[])
{
	initscr();
	screen_info scr_inf = {getmaxx(stdscr), getmaxy(stdscr), 1, 3};
	txt_buffer buff; 
	buff_constr(&buff, "thing.txt");
	printable_buffer pbuff;
	get_printable_buffer(&pbuff, &buff, scr_inf);
	print_pbuff(&pbuff, scr_inf);
	while (true){
		switch (mode)
		{
		case MODE_COMMAND:
			int ch = getch();
			switch (ch)
			{
			case 'h':
				if (scr_inf.col != 0)
					scr_inf.col--;
				break;
			case 'j':
				if (scr_inf.line != 0)
					scr_inf.line--;
				break;
			case 'k':
				if (scr_inf.line < buff.lines)
					scr_inf.line++;
				break;
			case 'l':
				scr_inf.col++;
				break;
			default:
				break;
			}
			break;
		
		default:
			break;
		}
		get_printable_buffer(&pbuff, &buff, scr_inf);
		print_pbuff(&pbuff, scr_inf);
	}
	buff_free(&buff);
	endwin();
	return 0;
}
