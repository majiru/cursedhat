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
	int tmp;
	int ch;
	int sock;

	Point origin;
	Rectangle chat;
	Chat *cur;

	memset(all_chats, 0, sizeof(all_chats));

	sock = contoserver("127.0.0.1", 8000);
	if(sock < 0){
		cerr(1, "could not connect to server");
	}

	/* Start curses mode */
	if (initscr() == NULL) {
		cerrx(1, "main initscr");
	}

	/* No line buffer, we get every character as typed */
	if (cbreak() == ERR) {
		cerrx(1, "main cbreak");
	}

	/* Allow special characters in*/
	if (keypad(stdscr, TRUE) == ERR) {
		cerrx(1, "main keypad");
	}

	if (noecho() == ERR) {
		cerrx(1, "main noecho");
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
		cerrx(1, "main move");
	}

	if (refresh() == ERR) {
		cerrx(1, "main refresh");
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
			if ((tmp = getch()) == ERR) {
				cerrx(1, "main tmp getch");
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
		case '\t':
			tmp = readmsg(sock, cur->buf, ';');
			cur->bufcur = tmp;
			(void)mvwprintw(cur->win, 1, 1, cur->buf);
			(void)mvprintw(LINES - 1, 0, cur->buf);
			if (wrefresh(cur->win) == ERR) {
				cerrx(1, "main wrefresh cur->win (tab)");
			}
			break;
		default:
			/* Print what the user typed */
			(void)mvaddch(LINES - 1,cur->bufcur, ch);

			appendbuf(cur, ch);

			// TODO: Add support on the server to read
			//if (write(sock, &ch, 1) == -1) {
			//	cerr(1, "main write sock");
			//}

			/* Send new string to window */
			(void)mvwprintw(cur->win, 1, 1, cur->buf);

			if (wrefresh(cur->win) == ERR) {
				cerrx(1, "main wrefresh cur->win (default)");
			}
			break;
		}
	}

	/* End curses mode */
	if (endwin() == ERR) {
		cerrx(1, "main endwin");
	}

	for(int i = 0; i < MAXWIN; i++){
		free(all_chats[i].buf);
	}

	return 0;
}

