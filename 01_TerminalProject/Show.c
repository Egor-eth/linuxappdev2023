#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

enum {
	BUFSIZE = 1024,
	CONTENTS_ALLOC_STEP = 8
};

unsigned wrapped_readline(char *dst, unsigned size, FILE *f, bool *has_newline) {
	if(fgets(dst, size, f)) {
		int len = strlen(dst);
		*has_newline = dst[len - 1] == '\n';
		return len;
	} else {
		return 0;
	}
}

unsigned umin(unsigned a, unsigned b) {
	return a < b ? a : b;
}

int main(int argc, char **argv)
{
	char *filename;
	if(argc == 2) {
		filename = argv[1]; 
	} else return 1;


	FILE *f = fopen(filename, "r");
	if(f == NULL) {
		perror("Error opening file");
		return 2;
	}
	//fseek(f, 0L, SEEK_END);
	//const long file_size = ftell(f);
	//fseek(f, 0L, SEEK_SET);

	unsigned contents_size = CONTENTS_ALLOC_STEP;
	char **contents = malloc(contents_size * sizeof(char *));
	unsigned lines_count = 0;
	bool not_eof = true;
	while(not_eof) {
		fprintf(stderr, "Reading line %i\n", lines_count);
		if(lines_count >= contents_size) {
			contents = realloc(contents, (contents_size += CONTENTS_ALLOC_STEP) * sizeof(char *));
		}
		fprintf(stderr, "Mark");
		bool has_newline = false;
		unsigned len = 0;

		unsigned line_length = 0u;
		unsigned line_size = BUFSIZE;
		contents[lines_count] = malloc(BUFSIZE);
		contents[lines_count][0] = '\0';
		do {
			char buf[BUFSIZE];
			fprintf(stderr, "Mark\n");
			len = wrapped_readline(buf, BUFSIZE, f, &has_newline);
			if(feof(f)) not_eof = false;
			if(line_length + len >= line_size) {
				contents[lines_count] = realloc(contents[lines_count], line_size += (BUFSIZE * (len / BUFSIZE + 1)));
			}
			strcat(contents[lines_count], buf);
			line_length += len;
		} while(len && !has_newline);
		if(line_length > 0) lines_count += 1;

		if(len == 0 || !has_newline) break;
	}

	fprintf(stderr, "Printing file. \n");

	if(!initscr()) {
		fprintf(stderr, "Ncurses error.\n");
		return 1;
	}

	nodelay(stdscr, true);
	curs_set(0);
	mvprintw(0, 0, "File: %s", filename);

	WINDOW *win = newwin(LINES - 2, COLS - 2, 1, 1);
	


	refresh();
	unsigned cursor = 0u;
	const unsigned max_lines = LINES - 4;
	while(true) {
		mvprintw(0, 0, "File: %s, Cursor: %u", filename, cursor);
		werase(win);

		unsigned lines_available = umin(max_lines, lines_count - cursor);

		for(unsigned i = 0; i < lines_available; ++i) {
			mvwprintw(win, 1 + i, 1, "%s", contents[cursor + i]);
		}
		box(win, 0, 0);
		wrefresh(win);

		int ch = getch();
		if(ch == 27) { //ESC of ALT
			ch = getch();
			if(ch == -1) { //ESC
				break;
			}
		} else if(ch == ' ' && cursor < lines_count) {
			cursor += 1;
		}
	};
	delwin(win);
	endwin();

	//Freeing memory
	for(unsigned i = 0; i < lines_count; ++i) {
		free(contents[i]); 
	}
	free(contents);


	return 0;
}
