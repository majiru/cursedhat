#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "hat.h"

Chat all_chats[MAXWIN];

int curwin = 0;

int
main(int argc, char *argv[])
{
	WINDOW *chat_win;
	int startx, starty, width, height;
	int ch;
	int ch1;

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
		free(input);
		return -1;
	}

	initscr();			/* Start curses mode */
	cbreak();			/* No line buffer, we get every character as typed */
	keypad(stdscr, TRUE);		/* Allow special characters in*/
	noecho();

	/* Set origin for char window */
	origin = (Point){1, 1};
	/* Set size to be 3/4 size of the screen */
	chat = (Rectangle){origin, (Point){origin.x + (COLS/4*3), origin.y + (LINES/4*3) }};

	printw("Print F1 to exit");
	/* Send cursor to bottom where user will be typing */
	move(LINES - 1, inputcur);
	refresh();

	/* Init question windows*/
	for(int i = 0; i < MAXWIN; i++){
		all_chats[i] = newchat(&chat); //use constructor
		chat.min.x++;
		chat.min.y++;
	}

	/* Draw first window */
	drawwin(&all_chats[curwin]);
	chat_win = all_chats[curwin].win;

	while((ch = getch()) != KEY_F(1)){
		switch(ch){
		case '`':
			ch1 = getch();
			if(!isdigit(ch1))
				break;
			char buf[2] = "a";
			buf[0] = ch1;
			clearwin(&all_chats[curwin]);
			//Invalid input should redraw current window
			curwin = atoi(buf);
			drawwin(&all_chats[curwin]);
			chat_win = all_chats[curwin].win;
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

	endwin();			/* End curses mode */
	free(input);
	return 0;
}

