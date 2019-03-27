#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "hat.h"

Chat all_chats[MAXWIN];

int
main(int argc, char *argv[])
{
	char ch;
	char tmp;
	int sock;

	Point origin;
	Rectangle chat;
	Chat *cur;

	memset(all_chats, 0, sizeof(all_chats));

	sock = contoserver("127.0.0.1", 8000);
	if(sock < 0){
		perror("Could not connect to server");
		return -1;
	}
	/* Start curses mode */
	if (initscr() == NULL) {
		// TODO
	}

	/* No line buffer, we get every character as typed */
	if (cbreak() == ERR) {
		// TODO
	}

	/* Allow special characters in*/
	if (keypad(stdscr, TRUE) == ERR) {
		// TODO
	}

	if (noecho() == ERR) {
		// TODO
	}

	/* Set origin for char window */
	origin = (Point){1, 1};

	/* Set size to be 3/4 size of the screen */
	chat = (Rectangle){
		origin,
		(Point){origin.x + (COLS/4*3),
		origin.y + (LINES/4*3) }
	};

	(void)printw("Print F1 to exit");

	/* Send cursor to bottom where user will be typing */
	if (move(LINES - 1, 0) == ERR) {
		// TODO
	}

	if (refresh() == ERR) {
		// TODO
	}

	/* Init question windows*/
	for(int i = 0; i < MAXWIN; i++){
		all_chats[i] = newchat(&chat); //use constructor
		chat.min.x++;
		chat.min.y++;
	}

	/* Draw first window */
	cur = all_chats;
	drawwin(cur);

	while((ch = getch()) != KEY_F(1)){
		switch(ch){
		case '`':
			tmp = getch();
			if (tmp == ERR) {
				// TODO
			}

			if(!isdigit(tmp))
				break;

			char buf[2] = "a";
			buf[0] = tmp;
			tmp = atoi(buf);
			if(tmp >= MAXWIN)
				break;

			clearwin(cur);
			cur = &all_chats[atoi(buf)];
			drawwin(cur);
			break;
		default:
			/* Print what the user typed */
			(void)mvaddch(LINES - 1,cur->bufcur, ch);

			/* Make sure we can store the new character + null byte */
			if(cur->bufcur + 2 > cur->bufsize){
				cur->bufsize *= 2;
				cur->buf = realloc(cur->buf, cur->bufsize);

				if (cur->buf == NULL && cur->bufsize != 0) {
					// TODO error
				}
			}

			if (write(sock, &ch, 1) == -1) {
				// TODO error
			}

			/* store value and make it a proper string */
			cur->buf[cur->bufcur++] = ch;
			cur->buf[cur->bufcur] = '\0';

			/* Send new string to window */
			(void)mvwprintw(cur->win, 1, 1, cur->buf);

			if (wrefresh(cur->win) == ERR) {
				// TODO
			}
			break;
		}
	}

	/* End curses mode */
	if (endwin() == ERR) {
		// TODO error
	}

	for(int i = 0; i < MAXWIN; i++){
		free(all_chats[i].buf);
	}

	return 0;
}

