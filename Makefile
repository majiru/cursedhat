chat:
	gcc -Wall -Wextra -ggdb cerr.c chat.c main.c net.c -o chat -l ncurses

client-go:
	go build client.go

server-rs:
	rustc server.rs

clean:
	rm -f chat
