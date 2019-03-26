#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "hat.h"

WINDOW*
create_newwin(Rectangle r)
{	WINDOW *win;
	int height = r.max.y - r.min.y;
	int width = r.max.x - r.min.x;

	win = newwin(height, width, r.min.y, r.min.x);
	box(win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(win);		/* Show that box		*/

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

void
moveorigin(Chat *c, Point p)
{
	c->r = (Rectangle){p, (Point){p.x + (c->r.max.x - c->r.min.x), p.y + (c->r.max.y - c->r.min.y)}};
	clearwin(c);
	drawwin(c);
}

Chat
newchat(Rectangle *r)
{
	Chat c;
	memset(&c, 0, sizeof(Chat));

	
	if(r == NULL)
		c.r = (Rectangle){(Point){1, 1}, (Point){1 + (COLS/4*3), 1 + (COLS/4*3)}};
	else
		c.r = *r;

	c.win = NULL;
	c.buf = malloc(COLS);
	c.bufsize = COLS;

	return c;
}

void
clearwin(Chat *c)
{
	destroy_win(c->win);
	c->win = NULL;
}

void
drawwin(Chat *c)
{
	c->win = create_newwin(c->r);
}
