#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#include "hat.h"

Chat all_chats[MAXWIN];

int curwin = 0;

int
main(int argc, char *argv[])
{	WINDOW *chat_win;
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
			moveorigin(&chat, origin);
			destroy_win(chat_win);
			chat_win = create_newwin(chat);
			mvwprintw(chat_win, 1, 1, input);
			wrefresh(chat_win);
			break;
		case KEY_RIGHT:
			origin.x++;
			moveorigin(&chat, origin);
			destroy_win(chat_win);
			chat_win = create_newwin(chat);
			mvwprintw(chat_win, 1, 1, input);
			wrefresh(chat_win);
			break;
		case KEY_UP:
			origin.y--;
			moveorigin(&chat, origin);
			destroy_win(chat_win);
			chat_win = create_newwin(chat);
			mvwprintw(chat_win, 1, 1, input);
			wrefresh(chat_win);
			break;
		case KEY_DOWN:
			origin.y++;
			moveorigin(&chat, origin);
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

void
moveorigin(Rectangle *r, Point p)
{
	*r = (Rectangle){p, (Point){p.x + (r->max.x - r->min.x), p.y + (r->max.y - r->min.y)}};
}

WINDOW*
create_newwin(Rectangle r)
{	WINDOW *win;
	int height = r.max.y - r.min.y;
	int width = r.max.x - r.min.x;

	win = newwin(height, width, r.min.y, r.min.x);
	box(win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(win);		/* Show that box 		*/

	return win;
}

void
destroy_win(WINDOW *win)
{	
	/* box(win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(win);
	delwin(win);
}