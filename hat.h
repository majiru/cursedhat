#define MAXWIN 20

typedef
struct Point {
	int x, y;
} Point;

typedef
struct Rectangle {
	Point min, max;
} Rectangle;

typedef
struct Chat {
	char *title;
	WINDOW *win;
	Rectangle r;
	char *buf;
	int	bufsize;
} Chat;

extern Chat all_chats[MAXWIN];
extern int curwin;

/* net.c */
int contoserver(char *name, int port);

/*chat.c */
WINDOW	*create_newwin(Rectangle);
void		destroy_win(WINDOW*);
void		moveorigin(Chat*, Point);
Chat		newchat(Rectangle *r);
void		clearwin(Chat*);
void		drawwin(Chat*);

