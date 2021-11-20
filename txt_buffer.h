#include "utils.h"
#define TAB_SIZE 4
#define LINE_NO_SIZE 5
#define MAX_PBUFF_SIZE 4096

typedef struct {
	uint width;
	uint height;
	uint col;
	uint line;
} screen_info;

typedef struct {
	uint lines;
	uint data_size;
	char data[1048576];
	char* path;
} txt_buffer;

typedef struct {
	uint lines;
	uint first_line_no;
	uint data_size;
	char data[MAX_PBUFF_SIZE];
} printable_buffer;

void buff_constr(txt_buffer* buff, const char* path);

void buff_free(txt_buffer* buff);

uint get_line_from_pos(txt_buffer* buff, uint pos);

uint get_pos_from_line(txt_buffer* buff, uint line);

void get_printable_buffer(printable_buffer* pbuff, txt_buffer* buff, screen_info scr_inf);