#include "communication.h"
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

char* serializeMatrice(char** matrice, short row, short col)
{
	debug("serialize matrice\n");
	//create buffer obj
	char* buffer = (char*) calloc((row * col), sizeof(char*));
	if (matrice == NULL) {
		printf("-- Erreur lors de l'allocation de la memoire --\n");
		exit(0);
	}


	//Replace NUL char because they are not sent through socket
	int i, j;
	for(i = 0; i<row; i++) {
		for(j = 0; j<col; j++) {
			if(matrice[i][j] == 0) matrice[i][j] = '-';
		}
	}

	//init buffer
	int count = 0;
	for (i = 0; i<row; i++)
	{
		for (j = 0; j<col; j++, count++)
		{
			buffer[count] = (int) matrice[i][j];
		}
	}

	return buffer;
}



void deserializeMatrice(char*** matrice, char* buffer, short row, short col)
{
	debug("deserialize matrice\n");
	int i, j, count = 0;

	for (i = 0; i<row; i++)
	{
		for (j = 0; j<col; j++, count++)
		{
			//The matrice we are modifying here is initialized with NUL char
			//Do not read 0s that used to represent NUL char in the original matrice
			if (buffer[count] != '-') {
				(*matrice)[i][j] = buffer[count];
			}
		}
	}
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
			if (matrice[i][j] == 0)
			{
				printf(" ");
			}
			else
			{
				printf("%c", matrice[i][j]);
			}
		}
		printf("\n");
	}
}

/*
void ReceiveMatrice(char** matrice, int socket, char* buffer, short row, short col)
{
	//Retrieve serialized matrice from buffer
	read(socket, buffer, 1024);

	//Deserialize matrice
	deserializeMatrice(matrice, buffer, row, col);
}



void sendMatrice(char** matrice, int sock, short row, short col)
{
	//Serialize for sending through socket connection
	char* buffer = serializeMatrice(matrice, row, col)
	//sleep(5);

	//Send matrice
	send(sock, buffer, strlen(buffer), 0);

	printf("CLIENT: Matrice sent\n");
}
*/
