
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "printtools.h"
#include "boardtools.h"
#include "communication.h"
#include "client.h"
#include "server.h"
#include "constants.h"

/**
*		Core of the client program
*		Displays the menu to choose game mode
**/
void startClient()
{
	short i, j = 0;
	short choice= 0;
	short hostornot = 0;
	short difficulte = 0;
	short placing = 0;
	short leaveswitch1 = 0;
	short leaveswitch2 = 0;
	short leaveswitch3 = 0;
	short leaveswitch4 = 0;

	//Sem allows to always connect host first to server as P1
	/*
	sem_t* sem_connect = sem_open("sem_connect", O_CREAT, O_RDWR, 0);
	if (sem_unlink("sem_connect"))
	{
	printf("Couldn't unlink semaphore\n");
	}*/


	/***** ETAPE 0.1 : MENU ***********************************************/
	system("clear");
	printf("\n\n\n              Welcome to BattleShip !\n\n\n");
	_getch();
	while(leaveswitch1 == 0)
	{
		choice = menuChoice2("PLAY AGAINST A FRIEND", "PLAY AGAINST THE CPU", 5, "");
		switch(choice)
		{
			//Play against a friend choice
			case 1:
			leaveswitch2 = 0;
			hostornot = 0;
			while(leaveswitch2 == 0)
			{
				system("clear");
				hostornot = menuChoice2("HOST THE GAME", "JOIN A GAME", 12, "     Press Esc to exit to the previous menu.\n");
				int socket = 0;
				switch(hostornot)
				{
					//Host game
					case 1:
					//Host server Connection: start the server
					printf("Share your address to your opponent: 127.0.0.1");
					pthread_t t_server;
					pthread_create(&t_server, NULL, (void*) startServer, NULL);

					//Connect to server
					socket = establishConnection("127.0.0.1");
					printf("CLIENT1: is connected: %d\n", socket);

					//Start Playing
					playPvPGame(socket);

					//P1 (host) is now connected to server, indicate to P2 that he can connect
					//printf("release sem for client2 to connect\n");
					//sem_post(sem_connect);

					break;

					//Join game
					case 2:
					//Connect to server
					socket = establishConnection("127.0.0.1");
					printf("CLIENT1: is connected: %d\n", socket);

					//Start Playing
					playPvPGame(socket);
					break;
				}
			}
			break;

			//Play against the CPU choice
			case 2:
			leaveswitch3 = 0;
			difficulte = 0;
			leaveswitch4 = 0;
			while(leaveswitch3 == 0) {
				system("clear");
				difficulte = menuChoice3("EASY-PEASY", "KINDA HARDER", "YOU'LL NOT SURVIVE", 3, "     Press Esc to exit to the previous menu.\n");
				switch(difficulte){
					case 1:
					/*** nothing to do ***/
					break;
					case 2:
					/*** nothing to do ***/
					break;
					case 3:
					/*** nothing to do ***/
					break;
					case -1:
					leaveswitch3 = 1;
					break;
					default :
					break;
				}
				if (leaveswitch3 == 0)
				{
					system("clear");
					printf("\n\n\n              The game is about to start...\n\n\n");
					_getch();
					char** matrice_J1 = (char**) creerMatrice(COL, ROW);
					system("clear");
					while(leaveswitch4 == 0)
					{
						short placing = menuChoice2("PLACE MY SHIPS RANDOMLY", "I'LL PLACE MY SHIPS MYSELF", 2,"");
						switch(placing)
						{
							case 1:
							matrice_J1 = place_randomly(matrice_J1, COL, ROW, NB_SHIPS);
							printBoard(matrice_J1, COL, ROW, 1);
							_getch();
							leaveswitch4 = 1;
							break;
							case 2:
							matrice_J1 = placeShip(matrice_J1, COL, ROW, 2);
							matrice_J1 = placeShip(matrice_J1, COL, ROW, 2);
							matrice_J1 = placeShip(matrice_J1, COL, ROW, 2);
							matrice_J1 = placeShip(matrice_J1, COL, ROW, 2);
							matrice_J1 = placeShip(matrice_J1, COL, ROW, 3);
							matrice_J1 = placeShip(matrice_J1, COL, ROW, 3);
							matrice_J1 = placeShip(matrice_J1, COL, ROW, 3);
							matrice_J1 = placeShip(matrice_J1, COL, ROW, 4);
							matrice_J1 = placeShip(matrice_J1, COL, ROW, 4);
							matrice_J1 = placeShip(matrice_J1, COL, ROW, 5);
							leaveswitch4 = 1;
							break;
							default:
							break;
						}
					}
					char** matrice_J2 = (char**) creerMatrice(COL, ROW);
					place_randomly(matrice_J2, COL, ROW, NB_SHIPS);
					short we_got_winner = 0;
					short nb_boat_left = 0;
					coordonnees_tir tir;
					while(we_got_winner == 0)
					{
						system("clear");
						printColorYellow();
						printf("\n\n               IT'S YOUR TURN TO SHOOT !\n");
						printColorWhite();
						_getch();
						tir = Aim(matrice_J2, COL, ROW);
						printBoard_af_tir(matrice_J2, COL, ROW, tir, 1);
						_getch();

						for (i=0; i<ROW; i++) {
							for (j=0; j<COL; j++) {
								if (matrice_J2[i][j] == 'C' || matrice_J2[i][j] == 'S' || matrice_J2[i][j] == 'D' || matrice_J2[i][j] == 'B') nb_boat_left++;
							}
						}
						if (nb_boat_left == 0)
						{
							we_got_winner = 1;
							break;
						}
						nb_boat_left = 0;
						system("clear");
						printColorYellow();
						printf("\n\n         TAKE COVER, THEY ARE SHOOTING AT YOU !\n");
						printColorWhite();
						_getch();

						if (difficulte==1) tir = Aim_randomly(matrice_J1, COL, ROW, 10);
						if (difficulte==2) tir = Aim_randomly(matrice_J1, COL, ROW, 5);
						if (difficulte==3) tir = Aim_randomly(matrice_J1, COL, ROW, 3);
						printBoard_af_tir(matrice_J1, COL, ROW, tir, 0);
						_getch();

						for (i=0; i<ROW; i++) {
							for (j=0; j<COL; j++) {
								if (matrice_J1[i][j] == 'C' || matrice_J1[i][j] == 'S' || matrice_J1[i][j] == 'D' || matrice_J1[i][j] == 'B') nb_boat_left++;
							}
						}
						if (nb_boat_left == 0)
						{
							we_got_winner = 2;
							break;
						}
						nb_boat_left = 0;
					}
					if (we_got_winner == 1)
					{
						system("clear");
						printColorRed();
						if (difficulte == 1)
						{
							printf("\n\n   WE GOT A WINNER ! CONGRATULATIONS, YOU DEFEATED THE CPU !");
							printColorWhite();
							printf("\n     But let's be honest... You were in easy mode, you chicken.");
						}
						if (difficulte == 2)
						{
							printf("\n\n            WE GOT A WINNER ! DAMN YOU'RE GOOD !");
							printColorWhite();
							printf("\n                       CONGRATULATIONS !");
						}
						if (difficulte == 3)
						{
							printf("\n\n             WE GOT A WINNER ! DAMN YOU'RE GOOD !");
							printColorWhite();
							printf("\n          Wow... I WASN'T BETTING ON YOU AT THIS POINT.");
							printf("\n    YOU PROVED ME WRONG. YOU ARE AMONG THE GODS OF BATTLESHIP !");
							printColorRed();
							printf("\n\n    Compliments from the developer team, you defeated our CPU.");
						}
					}
					else
					{
						system("clear");
						printColorGreen();
						if (difficulte == 1)
						{
							printf("\n\n                       WE GOT A WINNER !");
							printColorWhite();
							printf("\n       ... and it's NOT you ! Come on, that was the EASY mode !");
						}
						if (difficulte == 2)
						{
							printf("\n\n     YOU LOOSE. MAYBE YOU SHOULD TRY WITH A LOWER DIFFICULTY ?");
							printColorWhite();
							printf("\n              (we both know it will change nothing tho...)");
						}
						if (difficulte == 3)
						{
							printf("\n\n                      AT LEAST YOU TRIED.");
							printColorWhite();
							printf("\n\n            Don't be sad... it's okay to being a loser.");
							printf("\n  That's how it is, some are above average, and others underneath...");
							printColorGreen();
							printf("\n\n  (yes. the computer is joking on you. that's your life now.)");
						}
					}

					_getch();
					libererMatrice((short**) matrice_J1, ROW);
					libererMatrice((short**) matrice_J2, ROW);

					we_got_winner = 0;
					difficulte = 0;
					leaveswitch3 = 1;
				}
			}

			break;
			case -1:
			printf("\n\n\nYou choose to quit. Goodbye !\n\n");
			leaveswitch1 = 1;
			break;

			default:
			break;
		}
	}

}

/**
* 	Try to connect to the server until success.
*		Return connection socket
*/
int establishConnection(char* ip_address)
{
	printf("CLIENT started\n");

	struct sockaddr_in address;
	int sock = 0;
	struct sockaddr_in serv_addr;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		exit(-1);
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, ip_address, &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		exit(-1);
	}

	while(1)
	{
		if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		{
			printf("\nConnection Failed, attempting another connection \n");
			sleep(1);
		}
		else
		{
			printf("\nConnection to server successfull\n");
			break;
		}
	}

	return sock;
}

void playPvPGame(int socket)
{
	short placing = 0;
	short we_got_winner = 0;
	short nb_boat_left = 0;
	coordonnees_tir tir;
	short leaveswitch2 = 0;

	char* buffer = (char*) malloc(ROW * COL * sizeof(char));
	char** matrice_J1;
	char** matrice_J2;

	//read start msg
	printf("Waiting for an opponent...\n");
	read(socket, buffer, ROW * COL);
	printf("%s\n", buffer);

	system("clear");
	printf("\n\n\n              The game is about to start...\n\n\n");
	_getch();
	matrice_J1 = (char**) creerMatrice(COL, ROW);
	system("clear");
	placing = menuChoice2("PLACE MY SHIPS RANDOMLY", "I'LL PLACE MY SHIPS MYSELF", 2,"");
	switch(placing)
	{
		case 1:
		matrice_J1 = place_randomly(matrice_J1, COL, ROW, NB_SHIPS);
		printBoard(matrice_J1, COL, ROW, 1);
		_getch();
		break;
		case 2:
		matrice_J1 = placeShip(matrice_J1, COL, ROW, 2);
		matrice_J1 = placeShip(matrice_J1, COL, ROW, 2);
		matrice_J1 = placeShip(matrice_J1, COL, ROW, 2);
		matrice_J1 = placeShip(matrice_J1, COL, ROW, 2);
		matrice_J1 = placeShip(matrice_J1, COL, ROW, 3);
		matrice_J1 = placeShip(matrice_J1, COL, ROW, 3);
		matrice_J1 = placeShip(matrice_J1, COL, ROW, 3);
		matrice_J1 = placeShip(matrice_J1, COL, ROW, 4);
		matrice_J1 = placeShip(matrice_J1, COL, ROW, 4);
		matrice_J1 = placeShip(matrice_J1, COL, ROW, 5);
		break;
		default:
		break;
	}

	//Init board and send it to server
	if (DEBUG) displayMatrice(matrice_J1);
	//Serialize Matrice
	int retour = 0;
	buffer = serializeMatrice(matrice_J1, ROW, COL);

	debug("CLIENT: strlen(buffer): %ld\n", strlen(buffer));
	debug("CLIENT: buffer before sending matrice: %s\n", buffer);
	//Send matrice
	retour = send(socket, buffer, strlen(buffer), 0);
	debug("CLIENT1: send() returned %d\n", retour);
	debug("CLIENT1: Matrice sent\n\n");

	//Reveive board from other player
	//Retrieve serialized matrice from buffer
	retour = read(socket, buffer, ROW * COL);
	debug("CLIENT: read %d bytes from buffer\n", retour);
	debug("CLIENT: received buffer: %s from server\n", buffer);
	//Deserialize matrice
	matrice_J2 = (char**) creerMatrice(ROW, COL);
	deserializeMatrice(&matrice_J2, buffer, ROW, COL);

	debug("CLIENT1 recieved this matrice from client 2:\n");
	if (DEBUG) displayMatrice(matrice_J2);

	//Tell to server that client is ready to start
	//send(socket, buffer, strlen(buffer), 0);

	_getch();

	while(we_got_winner == 0) {
		//system("clear");

		//Detection de "J'AI PERDU"
		int i, j;
		for (i=0; i<ROW; i++) {
			for (j=0; j<COL; j++) {
				if (matrice_J1[i][j] == 'C' || matrice_J1[i][j] == 'S' || matrice_J1[i][j] == 'D' || matrice_J1[i][j] == 'B') nb_boat_left++;
			}
		}
		if (nb_boat_left == 0)
		{
			we_got_winner = 2;
			break;
		}
		nb_boat_left = 0;
		//////////////////////////
		//Serveur indique si le client doit attendre ou tirer 'a' = tirer
		debug("CLIENT : game is about to start, waiting for server\n");
		read(socket, buffer, 2);
		debug("CLIENT : read recu : %s\n", buffer);

		if (buffer[0] == 'a') //Your turn to shoot
		{
			printColorYellow();
			printf("\n\n               IT'S YOUR TURN TO SHOOT !\n");
			printColorWhite();
			_getch();
			tir = Aim(matrice_J2, COL, ROW);

			//******* TRANSMETTRE LE TIR A J2
			buffer[0] = tir.a_la_ligne;
			buffer[1] = tir.a_la_colonne;
			debug("CLIENT: is going to send coordinates %d/%d\n", buffer[0], buffer[1]);
			retour = send(socket, buffer, strlen(buffer), 0);
			debug("CLIENT: sent %d bytes\n", retour);

			printBoard_af_tir(matrice_J2, COL, ROW, tir, 1);
			_getch();


			//Detection de "J'AI GAGNE"
			for (i=0; i<ROW; i++) {
				for (j=0; j<COL; j++) {
					if (matrice_J2[i][j] == 'C' || matrice_J2[i][j] == 'S' || matrice_J2[i][j] == 'D' || matrice_J2[i][j] == 'B') nb_boat_left++;
				}
			}
			if (nb_boat_left == 0)
			{
				we_got_winner = 1;
				break;
			}
			nb_boat_left = 0;
			//////////////////////////
		}
		else //Enemy turn to shoot
		{
			//system("clear");
			printColorYellow();
			printf("\n\n         TAKE COVER, THEY ARE SHOOTING AT YOU !\n");
			printColorWhite();
			_getch();

			read(socket, buffer, ROW * COL);
			//******* RECEPTION DU TIR DE J2
			read(socket, buffer, ROW * COL);
			tir.a_la_ligne = buffer[0];
			tir.a_la_colonne = buffer[1];

			printBoard_af_tir(matrice_J1, COL, ROW, tir, 0);
			_getch();
		}
	}

	//Affichage du vainqueur
	if (we_got_winner == 1) {
		system("clear");
		printColorRed();
		printf("\n\n   WE GOT A WINNER ! CONGRATULATIONS, YOU DEFEATED YOUR ENNEMY !");
	}
	else
	{
		system("clear");
		printColorGreen();
		printf("\n\n  YOU LOOSE. SORRY, ALL YOUR SHIPS HAVE BEEN DESTROYED. IT'S OVER.");
	}
	//////////////////////////
	_getch();
	libererMatrice((short**) matrice_J1, ROW);
	//libererMatrice((short**) matrice_J2, ROW);

	//******* EXTINCTION DU SERVEUR

	leaveswitch2 = 1;
	/*******/
}

/**
*		Send board game over socket connection
**/
