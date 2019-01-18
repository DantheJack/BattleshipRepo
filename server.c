
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#include "boardtools.h"
#include "communication.h"
#include "server.h"

#define PORT 8080
#define ROW 10
#define COL 10

#define DEBUG 1
#define debug(format, ...) if (DEBUG) fprintf (stdout, format, ##__VA_ARGS__)

void startServer()
{
	//Server config
	int server_fd;
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	//Sockets for each client
	int socket1, socket2;

	setUpServer(&server_fd, &address, &addrlen);
	debug("SERVER STARTED\n");

	debug("SERVER: wait for connection 1\n");
	waitForConnection(&socket1, &server_fd, &address, &addrlen);

	debug("SERVER: wait for connection 2\n");
	waitForConnection(&socket2, &server_fd, &address, &addrlen);

	debug("SERVER: all connection accepted\n");

	startGame(socket1, socket2);
}

/**
*	Set up server socket,
*	listen for connection,
*	set up client sockets
**/
void setUpServer(int* server_fd, struct sockaddr_in* address, int* addrlen)
{
	int opt = 1;

	// Creating socket file descriptor
	if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		printf("socket failed");
		exit(EXIT_FAILURE);
	}

	// Setting socket options
	if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		printf("setsockopt");
		exit(EXIT_FAILURE);
	}
	address->sin_family = AF_INET;
	address->sin_addr.s_addr = INADDR_ANY;
	address->sin_port = htons( PORT );

	// Forcefully attaching socket to the port 8080
	if (bind(*server_fd, (struct sockaddr *) address,
								sizeof(*address))<0)
	{
		printf("bind failed");
		exit(EXIT_FAILURE);
	}

	debug("server listens\n");
	//Start listenin on port 8080 through socket
	if (listen(*server_fd, 2) < 0)
	{
		printf("listen failed");
		exit(EXIT_FAILURE);
	}
}

/**
*	Wait for client connection and assosiate it to socket
*/
void waitForConnection(int* socket, int* server_fd, struct sockaddr_in* address, int* addrlen)
{
	if ( (*socket = accept(*server_fd, (struct sockaddr *) address, (socklen_t *) addrlen)) < 0)
	{
		printf("Connection failed\n");
		exit(EXIT_FAILURE);
	}
}

void startGame(int socket1, int socket2)
{
	//Send start signal for both players
	char* startSignal = "Game can start !";
	debug("SERVER: sending start signal to player1\n");
	send(socket1, startSignal, strlen(startSignal), 0);
	debug("SERVER: sending start signal to player2\n");
	send(socket2, startSignal, strlen(startSignal), 0);


	//Wait to receive board of each client
	char* serializedMatrice_Client1 = (char*) malloc(1024 * sizeof(char));
	char* serializedMatrice_Client2 = (char*) malloc(1024 * sizeof(char));

	//Wait for Client1's board reception
	debug("SERVER: read buffer\n");
	read(socket1, serializedMatrice_Client1, 1024);
	debug("SERVER: received: .%s. from Client1\n", serializedMatrice_Client1);

	//Wait for Client2's board reception
	debug("SERVER: read buffer\n");
	read(socket2, serializedMatrice_Client2, 1024);
	debug("SERVER: received: .%s. from Client2\n", serializedMatrice_Client2);

	//Send back boards to the other player
	send(socket1, serializedMatrice_Client2, strlen(serializedMatrice_Client2), 0);
	send(socket2, serializedMatrice_Client1, strlen(serializedMatrice_Client1), 0);

	debug("SERVER: read coordinate\n");
	//The game can start
	char* buffer = (char*) malloc(1024 * sizeof(char));
	while (1)
	{
		//Tell client2 to shot
		buffer[0] = 'a';
		send(socket2, buffer, strlen(buffer), 0);
		send(socket2, buffer, strlen(buffer), 0);
		send(socket1, buffer, strlen(buffer), 0);
		send(socket1, buffer, strlen(buffer), 0);
		//Receive shot coordinates from Client2
		read(socket2, buffer, 1024);
		short x = buffer[0];
		short y = buffer[1];
		debug("SERVER: received (%d/%d) from P1\n", x, y);
		//Send shot coordinates to Client1
		send(socket1, buffer, strlen(buffer), 0);
		debug("SERVER: sent (%d/%d) to P2\n", x, y);

		//Tell client1 to shot
		buffer[0] = 'b';
		send(socket2, buffer, strlen(buffer), 0);
		send(socket1, buffer, strlen(buffer), 0);
		//Receive shot coordinates from client1
		read(socket1, buffer, 1024);
		x = buffer[0];
		y = buffer[1];
		debug("SERVER: received (%d/%d) from P2\n", x, y);
		//Send shot coordinates to client2
		send(socket2, buffer, strlen(buffer), 0);
		debug("SERVER: sent (%d/%d) to P1\n", x, y);
	}

}
