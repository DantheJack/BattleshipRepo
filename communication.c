#include "communication.h"
#include <stdio.h>
#include <stdlib.h>

char* serializeMatrice(char** matrice, short row, short col)
{
	printf("serialize matrice\n");
	//create buffer obj
	char* buffer = (char *)calloc((row * col), sizeof(char *));
	if (matrice == NULL) {
			printf("-- Erreur lors de l'allocation de la memoire --\n");
			exit(0);
	}

	//Replace 0s
	int i, j;
	for(i=0; i<row; i++) {
			for(j=0; j<col; j++) {
					if(matrice[i][j] != 'C' && matrice[i][j] != 'S' && matrice[i][j] != 'D' && matrice[i][j] != 'B') matrice[i][j]='u';
			}
	}

	//init buffer
	int count = 0;
	for (i = 0; i<row; i++)
	{
		for (j = 0; j<col; j++, count++)
		{
			buffer[count] = matrice[i][j];
		}
	}

	//Display matrice before sending
	printf("display buffer before sending\n.");
	for (i = 0; i<row * col; i++)
	{
			printf("%c ", buffer[i]);
	}
	printf(".\n");

	return buffer;
}



void deserializeMatrice(char** matrice, char* buffer, short row, short col)
{
	printf("deserialize matrice\n");
	int i, j, count = 0;

	for (i = 0; i<row; i++)
	{
		for (j = 0; j<col; j++, count++)
		{
				matrice[i][j] = buffer[count];
		}
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
