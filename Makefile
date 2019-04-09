chat:
	gcc -Wall -Wextra -ggdb cerr.c chat.c main.c net.c -o chat -l ncurses

clean:
	rm -f chat
