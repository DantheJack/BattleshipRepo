#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

void startServer();
void setUpServer(int* server_fd, struct sockaddr_in* address, int* addrlen);
void waitForConnection(int* socket, int* server_fd, struct sockaddr_in* address, int* addrlen);
void startGame(int socket1, int socket2);

#endif // SERVER_H
