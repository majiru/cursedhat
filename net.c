#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <ncurses.h>
#include "hat.h"

int
contoserver(char *name, int port)
{
	int fd;
	struct sockaddr_in sin;
	struct hostent *server = NULL;

	server = gethostbyname("localhost");
	if(server == NULL)
		return -1;

	fd = socket(AF_INET, SOCK_STREAM, 0);

	if (fd == -1) {
		// TODO
	}

	memset(&sin, 0, sizeof(struct sockaddr_in));
	memcpy(&sin.sin_addr,server->h_addr_list[0], server->h_length);

	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	int trueval = 1;
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &trueval, sizeof(int)) < 0){
		fprintf(stderr, "Failed to set socket options!\n");
		close(fd);
		return -1;
	}

	if(connect(fd, (struct sockaddr*)&sin, sizeof(sin)) < 0)
		return -1;

	return fd;
}

int
readmsg(int sock, char *buf, char delim)
{
	int n, count;
	n = count = 0;
	
	while( (n = read(sock, buf, 1)) > 0 && *(buf++) != delim)
		count += n;

	*(--buf) = '\0';
	return count;
}