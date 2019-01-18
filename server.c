
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 8080
#define ROW 10
#define COL 10

#include "boardtools.h"
#include "communication.h"
#include "server.h"
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

void* startServer(void* param)
{
	printf("SERVER STARTED\n");

	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char *hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	printf("server listens\n");
	if (listen(server_fd, 2) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	//Struct with player sockets
	//p_sockets _p_sockets_p1; _p_sockets_p1.p = 1;
	//p_sockets _p_sockets_p2; _p_sockets_p1.p = 2;

	int socket1 = 0;
	int socket2 = 0;

	printf("SERVER: wait for connection 1\n");
	//Connect two clients
	if ((socket1 = accept(server_fd, (struct sockaddr *)&address,
					(socklen_t*)&addrlen))<0)
	{
		printf("accept P1 error");
		exit(EXIT_FAILURE);
	}

	printf("SERVER: wait for connection 2\n");
	if ((socket2 = accept(server_fd, (struct sockaddr *)&address,
					(socklen_t*)&addrlen))<0)
	{
		printf("accept P2 error");
		exit(EXIT_FAILURE);
	}

	printf("SERVER: all connection accepted\n");


	//Send start signal for both players
	char* startSignal = "Game can start !";
	printf("SERVER: sending start signal to player1\n");
	send(socket1, startSignal, strlen(startSignal), 0);
	printf("SERVER: sending start signal to player2\n");
	send(socket2, startSignal, strlen(startSignal), 0);


	//Wait to receive board of each client
	char* serializedMatrice_Client1 = (char*) malloc(1024 * sizeof(char));
	char* serializedMatrice_Client2 = (char*) malloc(1024 * sizeof(char));

	//Wait for Client1's board reception
	printf("SERVER: read buffer\n");
	read(socket1, serializedMatrice_Client1, 1024);
	printf("SERVER: received: .%s. from Client1\n", serializedMatrice_Client1);
	//deserializeMatrice(matrice, buffer+1, ROW, COL);

	//Wait for P2's board reception
	printf("SERVER: read buffer\n");
	read(socket2, serializedMatrice_Client2, 1024);
	printf("SERVER: received: .%s. from Client2\n", serializedMatrice_Client2);
	//deserializeMatrice(matrice, buffer+1, ROW, COL);

	//Send back boards to the other player
	send(socket1, serializedMatrice_Client2, strlen(serializedMatrice_Client2), 0);
	send(socket2, serializedMatrice_Client1, strlen(serializedMatrice_Client1), 0);

	printf("SERVER: read coordinate\n");
	//The game can start
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
		printf("SERVER: received (%d/%d) from P1\n", x, y);
		//Send shot coordinates to Client1
		send(socket1, buffer, strlen(buffer), 0);
		printf("SERVER: sent (%d/%d) to P2\n", x, y);

		//Tell client1 to shot
		buffer[0] = 'b';
		send(socket2, buffer, strlen(buffer), 0);
		send(socket1, buffer, strlen(buffer), 0);
		//Receive shot coordinates from client1
		read(socket1, buffer, 1024);
		x = buffer[0];
		y = buffer[1];
		printf("SERVER: received (%d/%d) from P2\n", x, y);
		//Send shot coordinates to client2
		send(socket2, buffer, strlen(buffer), 0);
		printf("SERVER: sent (%d/%d) to P1\n", x, y);
	}



	//sem_t* sendRequest_sem = (sem_t*) param;
	//sem_post(sendRequest_sem);

	//Listen to client 2
	/*
	printf("SERVER: read buffer\n");
	read( socket2 , buffer, 1024);
	printf("SERVER: received: .%s.\n", buffer);
	processRequest(buffer);
	*/


}

int processRequest(char* buffer)
{
	char id = buffer[0];
	printf("request id is: %c\n", id);

	switch (id) {
		case '1':; //BoardIsReady
			printf("BoardIsReady Request\n");
			char** matrice = (char**) creerMatrice(ROW, COL);
			deserializeMatrice(matrice, buffer+1, ROW, COL);
			displayMatrice(matrice);
			return 1;
		break;
		case '2':; //sendCoordinate
			printf("Coordinate Request\n");
		break;
		case '3':; //win
			printf("Win Request\n");
		break;
	}

	return 0;
}

void displayMatrice(char** matrice)
{
	//Display matrice
	printf("display matrice\n");
	int i, j;
	for (i = 0; i<ROW; i++)
	{
		for (j = 0; j<COL; j++)
		{
			printf("%c ", matrice[i][j]);
		}
		printf("\n");
	}
}
