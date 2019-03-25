#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "hat.h"

Chat all_chats[MAXWIN];

int curwin = 0;

int
main(int argc, char *argv[])
{
	WINDOW *chat_win;
	int startx, starty, width, height;
	int ch;
	Point origin;
	Rectangle chat;
	char *input;
	int sock;

	/* Init buffer for user input */
	int inputsize = 50;
	int inputcur = 0;
	input = malloc(sizeof(char) * inputsize);

	memset(all_chats, 0, sizeof(all_chats));

	sock = contoserver("127.0.0.1", 8000);
	if(sock < 0){
		perror("Could not connect to server");
		return -1;
	}

	initscr();			/* Start curses mode  */
	cbreak();			/* No line buffer, we get every character as typed */
	keypad(stdscr, TRUE);		/* Allow special characters in*/

	/* Set origin for char window */
	origin = (Point){1, 1};
	/* Set size to be 3/4 size of the screen */
	chat = (Rectangle){origin, (Point){origin.x + (COLS/4*3), origin.y + (LINES/4*3) }};

	printw("Print F1 to exit");
	/* Send cursor to bottom where user will be typing */
	move(LINES - 1, inputcur);
	refresh();

	/* Draw first window */
	chat_win = create_newwin(chat);

	while((ch = getch()) != KEY_F(1)){
		switch(ch){
		case KEY_LEFT:
			/* Resize window with new cordinates, print what is in the buffer */
			origin.x--;
			//moveorigin(&chat, origin);
			destroy_win(chat_win);
			chat_win = create_newwin(chat);
			mvwprintw(chat_win, 1, 1, input);
			wrefresh(chat_win);
			break;
		case KEY_RIGHT:
			origin.x++;
			//moveorigin(&chat, origin);
			destroy_win(chat_win);
			chat_win = create_newwin(chat);
			mvwprintw(chat_win, 1, 1, input);
			wrefresh(chat_win);
			break;
		case KEY_UP:
			origin.y--;
			//moveorigin(&chat, origin);
			destroy_win(chat_win);
			chat_win = create_newwin(chat);
			mvwprintw(chat_win, 1, 1, input);
			wrefresh(chat_win);
			break;
		case KEY_DOWN:
			origin.y++;
			//moveorigin(&chat, origin);
			destroy_win(chat_win);
			chat_win = create_newwin(chat);
			mvwprintw(chat_win, 1, 1, input);
			wrefresh(chat_win);
			break;
		default:
			/* Print what the user typed */
			mvaddch(LINES - 1,inputcur, ch);

			/* Make sure we can store the new character + null byte */
			if(inputcur + 2 > inputsize){
				inputsize = inputsize * 2;
				input = realloc(input, inputsize);
			}

			write(sock, &ch, 1);

			/* store value and make it a proper string */
			input[inputcur++] = ch;
			input[inputcur] = '\0';

			/* Send new string to window */
			mvwprintw(chat_win, 1, 1, input);
			wrefresh(chat_win);
			break;
		}
	}
		
	endwin();			/* End curses mode		  */
	return 0;
}

