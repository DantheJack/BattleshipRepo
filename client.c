
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
#define PORT 8080
#define COLONNES 4
#define LIGNES 4
#define NB_SHIPS 3
#define SIZE_MAN_O_WAR 5
#define SIZE_NAVIRE_DE_LIGNE 4
#define SIZE_FREGATE 3
#define SIZE_SLOOP 4

#include "printtools.h"
#include "boardtools.h"
#include "communication.h"
#include "client.h"
#include "server.h"

/**
*		Core of the client program
*		Displays the menu to choose game mode
**/
void startClient()
{
	short we_got_winner = 0;
	coordonnees_tir tir;
	short i, j, nb_boat_left = 0;
	short choice=0;
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
	}
	*/

	/***** ETAPE 0.1 : MENU ***********************************************/
	system("clear");
	printf("\n\n\n              Welcome to BattleShip !\n\n\n");
	_getch();
	while(leaveswitch1 == 0){
			choice = menuChoice2("PLAY AGAINST A FRIEND", "PLAY AGAINST THE CPU", 5, "");
			switch(choice){
					case 1:
							leaveswitch2 = 0;
							placing = 0;
							hostornot = 0;
							we_got_winner = 0;
							while(leaveswitch2 == 0){
									system("clear");
									hostornot = menuChoice2("HOST THE GAME", "JOIN A GAME", 12, "     Press Esc to exit to the previous menu.\n");
									int socket;
									int count = 0;
									char* buffer = (char*) malloc(1024 * sizeof(char));
									char** matrice_J1;
									char** matrice_J2;

									switch(hostornot){
											//Host game
											case 1:

													//Host server Connection: start the server
													printf("Share your address to your opponent: 127.0.0.1");
													pthread_t t_server;
										      pthread_create(&t_server, NULL, (void*) startServer, NULL);

													//Connect to server
										      socket = establishConnection("127.0.0.1");
										      printf("CLIENT1: is connected: %d\n", socket);

													//P1 (host) is now connected to server, indicate to P2 that he can connect
													//printf("release sem for client2 to connect\n");
													//sem_post(sem_connect);

													//read start msg
													printf("Waiting for an opponent...\n");
													read(socket, buffer, 1024);
													printf("%s\n", buffer);

													system("clear");
													printf("\n\n\n              The game is about to start...\n\n\n");
													_getch();
													matrice_J1 = (char**) creerMatrice(COLONNES, LIGNES);
													system("clear");
													placing = menuChoice2("PLACE MY SHIPS RANDOMLY", "I'LL PLACE MY SHIPS MYSELF", 2,"");
													switch(placing){
															case 1:
																	matrice_J1 = place_randomly(matrice_J1, COLONNES, LIGNES, NB_SHIPS);
																	printBoard(matrice_J1, COLONNES, LIGNES, 1);
																	_getch();
															break;
															case 2:
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 3);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 3);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 3);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 4);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 4);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 5);
															break;
															default:
															break;
													}

													//Init board and send it to server
										      displayMatrice(matrice_J1);
										      //Serialize Matrice
										      buffer = serializeMatrice(matrice_J1, LIGNES, COLONNES);
										    	//Send matrice

										    	send(socket, buffer, strlen(buffer), 0);
										      printf("CLIENT1: Matrice sent\n");

													//Reveive board from other player
										      //Retrieve serialized matrice from buffer
										      read(socket, buffer, 1024);
													//Deserialize matrice
													matrice_J2 = (char**) creerMatrice(COLONNES, LIGNES);

													for(i=0; i<LIGNES; i++) {
															for(j=0; j<COLONNES; j++, count++) {
																	if(buffer[count] != 'u')
																	{
																		matrice_J2[i][j] = buffer[count];
																	}
															}
													}

										      //deserializeMatrice(matrice_J2, buffer, LIGNES, COLONNES);
										      printf("CLIENT1 recieved this matrice from client 2:\n");
										      displayMatrice(matrice_J2);

													_getch();

													while(we_got_winner == 0){
															system("clear");

															//Detection de "J'AI PERDU"
															for(i=0; i<LIGNES; i++){
																	for(j=0; j<COLONNES; j++){
																			if(matrice_J1[i][j] == 'C' || matrice_J1[i][j] == 'S' || matrice_J1[i][j] == 'D' || matrice_J1[i][j] == 'B') nb_boat_left++;
																	}
															}
															if(nb_boat_left == 0)
															{
																	we_got_winner = 2;
																	break;
															}
															nb_boat_left = 0;
															//////////////////////////
															//Attente top depart du server
															printf("CLIENT1 : attente dE READ\n");
															read(socket, buffer, 1024);
															printf("CLIENT1 : read recu %s\n", buffer);
															printf("CLIENT1 : attente dE READ\n");
															read(socket, buffer, 1024);
															printf("CLIENT1 : read recu %s\n", buffer);
															if(buffer[0]=='a'){
																	printColorYellow();
																	printf("\n\n               IT'S YOUR TURN TO SHOOT !\n");
																	printColorWhite();
																	_getch();
																	tir = Aim(matrice_J2, COLONNES, LIGNES);

																	//******* TRANSMETTRE LE TIR A J2
																	buffer[0] = tir.a_la_ligne;
																	buffer[1] = tir.a_la_colonne;
																	send(socket, buffer, strlen(buffer), 0);

																	printBoard_af_tir(matrice_J2, COLONNES, LIGNES, tir, 1);
																	_getch();


															//Detection de "J'AI GAGNE"
															for(i=0; i<LIGNES; i++){
																	for(j=0; j<COLONNES; j++){
																			if(matrice_J2[i][j] == 'C' || matrice_J2[i][j] == 'S' || matrice_J2[i][j] == 'D' || matrice_J2[i][j] == 'B') nb_boat_left++;
																	}
															}
															if(nb_boat_left == 0)
															{
																	we_got_winner = 1;
																	break;
															}
															nb_boat_left = 0;
															//////////////////////////
															}
															else{
															system("clear");
															printColorYellow();
															printf("\n\n         TAKE COVER, THEY ARE SHOOTING AT YOU !\n");
															printColorWhite();
															_getch();

															read(socket, buffer, 1024);
																//******* RECEPTION DU TIR DE J2
																read(socket, buffer, 1024);
																tir.a_la_ligne = buffer[0];
																tir.a_la_colonne = buffer[1];

																printBoard_af_tir(matrice_J1, COLONNES, LIGNES, tir, 0);
																_getch();
															}
													}

													//Affichage du vainqueur
													if(we_got_winner == 1){
															system("clear");
															printColorRed();
															printf("\n\n   WE GOT A WINNER ! CONGRATULATIONS, YOU DEFEATED YOUR ENNEMY !");
													}
													else{
															system("clear");
															printColorGreen();
															printf("\n\n  YOU LOOSE. SORRY, ALL YOUR SHIPS HAVE BEEN DESTROYED. IT'S OVER.");
													}
													//////////////////////////
													_getch();
													libererMatrice((short**) matrice_J1, LIGNES);
													//libererMatrice((short**) matrice_J2, LIGNES);

													//******* EXTINCTION DU SERVEUR

													leaveswitch2 = 1;
													/*******/
											break;
											//Join game
											case 2:
													//Connect to server
													printf("Enter the ip address of your opponent to join his game\n");
													//récup input @IP

													//printf("CLIENT2: wait for client1 to connect to server\n");
													//sem_wait(sem_connect);

													socket = establishConnection("127.0.0.1");
													printf("CLIENT2: connected to server\n");

													//read start msg
													printf("Waiting for an opponent...\n");
													read(socket, buffer, 1024);
													printf("%s\n", buffer);

													system("clear");
													printf("\n\n\n              The game is about to start...\n\n\n");
													_getch();
													matrice_J2 = (char**) creerMatrice(COLONNES, LIGNES);
													system("clear");
													placing = menuChoice2("PLACE MY SHIPS RANDOMLY", "I'LL PLACE MY SHIPS MYSELF", 2,"");
													switch(placing){
															case 1:
																	matrice_J2 = place_randomly(matrice_J2, COLONNES, LIGNES, NB_SHIPS);
																	printBoard(matrice_J2, COLONNES, LIGNES, 1);
																	_getch();
															break;
															case 2:
																	matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 2);
																	matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 2);
																	matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 2);
																	matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 2);
																	matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 3);
																	matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 3);
																	matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 3);
																	matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 4);
																	matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 4);
																	matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 5);
															break;
															default:
															break;
													}

													//Init board and send it to server
													printf("CLIENT2 matrice:\n");
													displayMatrice(matrice_J2);

													//Serialize Matrice
													buffer = serializeMatrice(matrice_J2, LIGNES, COLONNES);

													//Send matrice
													send(socket, buffer, strlen(buffer), 0);
													printf("CLIENT2: Matrice sent\n");

													//Reveive board from other player
													//Retrieve serialized matrice from buffer
													read(socket, buffer, 1024);

													//Deserialize matrice
													matrice_J1 = (char**) creerMatrice(LIGNES, COLONNES);
													for(i=0; i<LIGNES; i++) {
															for(j=0; j<COLONNES; j++, count++) {
																	if(buffer[count] != 'u')
																	{
																		matrice_J1[i][j] = buffer[count];
																	}
															}
													}

													//deserializeMatrice(matrice_J1, buffer, LIGNES, COLONNES);
													printf("CLIENT2 recieved this matrice from client 1:\n");
													displayMatrice(matrice_J1);

													_getch();

													while(we_got_winner == 0){
															system("clear");

															//Detection de "J'AI PERDU"
															for(i=0; i<LIGNES; i++){
																	for(j=0; j<COLONNES; j++){
																			if(matrice_J2[i][j] == 'C' || matrice_J2[i][j] == 'S' || matrice_J2[i][j] == 'D' || matrice_J2[i][j] == 'B') nb_boat_left++;
																	}
															}
															if(nb_boat_left == 0)
															{
																	we_got_winner = 2;
																	break;
															}
															nb_boat_left = 0;
															//////////////////////////
															//Attente top depart du server
															printf("CLIENT2 : attente dE READ\n");
															read(socket, buffer, 1024);
																printf("CLIENT2 : read recu: %s\n", buffer);
																printf("CLIENT2 : attente dE READ\n");
																read(socket, buffer, 1024);
																	printf("CLIENT2 : read recu: %s\n", buffer);
															if(buffer[0]=='b'){

																	printColorYellow();
																	printf("\n\n               IT'S YOUR TURN TO SHOOT !\n");
																	printColorWhite();
																	_getch();
																	tir = Aim(matrice_J1, COLONNES, LIGNES);

																	//******* TRANSMETTRE LE TIR A J1
																	buffer[0] = tir.a_la_ligne;
																	buffer[1] = tir.a_la_colonne;
																	send(socket, buffer, strlen(buffer), 0);

																	printBoard_af_tir(matrice_J1, COLONNES, LIGNES, tir, 1);
																	_getch();

																	//Detection de "J'AI GAGNE"
																	for(i=0; i<LIGNES; i++){
																			for(j=0; j<COLONNES; j++){
																					if(matrice_J1[i][j] == 'C' || matrice_J1[i][j] == 'S' || matrice_J1[i][j] == 'D' || matrice_J1[i][j] == 'B') nb_boat_left++;
																			}
																	}
																	if(nb_boat_left == 0)
																	{
																			we_got_winner = 1;
																			break;
																	}
																	nb_boat_left = 0;
																	//////////////////////////
														}
															else{
																	system("clear");
																	printColorYellow();
																	printf("\n\n         TAKE COVER, THEY ARE SHOOTING AT YOU !\n");
																	printColorWhite();
																	_getch();

																	//******* RECEPTION DU TIR DE J1
																	read(socket, buffer, 1024);
																	tir.a_la_ligne = buffer[0];
																	tir.a_la_colonne = buffer[1];

																	printBoard_af_tir(matrice_J2, COLONNES, LIGNES, tir, 0);
																	_getch();
																}
													}

													//Affichage du vainqueur
													if(we_got_winner == 1){
															system("clear");
															printColorRed();
															printf("\n\n   WE GOT A WINNER ! CONGRATULATIONS, YOU DEFEATED YOUR ENNEMY !");
													}
													else{
															system("clear");
															printColorGreen();
															printf("\n\n  YOU LOOSE. SORRY, ALL YOUR SHIPS HAVE BEEN DESTROYED. IT'S OVER.");
													}
													//////////////////////////
													_getch();
													libererMatrice((short**) matrice_J2, LIGNES);
													//libererMatrice((short**) matrice_J2, LIGNES);

													//******* EXTINCTION DU SERVEUR

													leaveswitch2 = 1;
											/*******/
											break;
											case -1:
													leaveswitch2 = 1;
											break;
											default :
											break;
									}
							}
					break;

					case 2:
							leaveswitch3 = 0;
							difficulte = 0;
							leaveswitch4 = 0;
							while(leaveswitch3 == 0){
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
									if(leaveswitch3 == 0){
											system("clear");
											printf("\n\n\n              The game is about to start...\n\n\n");
											_getch();
											char** matrice_J1 = (char**)creerMatrice(COLONNES, LIGNES);
											system("clear");
											while(leaveswitch4==0){
													placing = menuChoice2("PLACE MY SHIPS RANDOMLY", "I'LL PLACE MY SHIPS MYSELF", 2,"");
													switch(placing){
															case 1:
																	matrice_J1 = place_randomly(matrice_J1, COLONNES, LIGNES, NB_SHIPS);
																	printBoard(matrice_J1, COLONNES, LIGNES, 1);
																	_getch();
																	leaveswitch4 = 1;
															break;
															case 2:
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 3);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 3);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 3);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 4);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 4);
																	matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 5);
																	leaveswitch4 = 1;
															break;
															default:
															break;
													}
											}
											char** matrice_J2 = (char**)creerMatrice(COLONNES, LIGNES);
											place_randomly(matrice_J2, COLONNES, LIGNES, NB_SHIPS);
											while(we_got_winner == 0){
													system("clear");
													printColorYellow();
													printf("\n\n               IT'S YOUR TURN TO SHOOT !\n");
													printColorWhite();
													_getch();
													tir = Aim(matrice_J2, COLONNES, LIGNES);
													printBoard_af_tir(matrice_J2, COLONNES, LIGNES, tir, 1);
													_getch();

													for(i=0; i<LIGNES; i++){
															for(j=0; j<COLONNES; j++){
																	if(matrice_J2[i][j] == 'C' || matrice_J2[i][j] == 'S' || matrice_J2[i][j] == 'D' || matrice_J2[i][j] == 'B') nb_boat_left++;
															}
													}
													if(nb_boat_left == 0)
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

													if(difficulte==1) tir = Aim_randomly(matrice_J1, COLONNES, LIGNES, 10);
													if(difficulte==2) tir = Aim_randomly(matrice_J1, COLONNES, LIGNES, 5);
													if(difficulte==3) tir = Aim_randomly(matrice_J1, COLONNES, LIGNES, 3);
													printBoard_af_tir(matrice_J1, COLONNES, LIGNES, tir, 0);
													_getch();

													for(i=0; i<LIGNES; i++){
															for(j=0; j<COLONNES; j++){
																	if(matrice_J1[i][j] == 'C' || matrice_J1[i][j] == 'S' || matrice_J1[i][j] == 'D' || matrice_J1[i][j] == 'B') nb_boat_left++;
															}
													}
													if(nb_boat_left == 0)
													{
															we_got_winner = 2;
															break;
													}
													nb_boat_left = 0;
											}
											if(we_got_winner == 1){
													system("clear");
													printColorRed();
													if(difficulte == 1)
													{
															printf("\n\n   WE GOT A WINNER ! CONGRATULATIONS, YOU DEFEATED THE CPU !");
															printColorWhite();
															printf("\n     But let's be honest... You were in easy mode, you chicken.");
													}
													if(difficulte == 2)
													{
															printf("\n\n            WE GOT A WINNER ! DAMN YOU'RE GOOD !");
															printColorWhite();
															printf("\n                       CONGRATULATIONS !");
													}
													if(difficulte == 3)
													{
															printf("\n\n             WE GOT A WINNER ! DAMN YOU'RE GOOD !");
															printColorWhite();
															printf("\n          Wow... I WASN'T BETTING ON YOU AT THIS POINT.");
															printf("\n    YOU PROVED ME WRONG. YOU ARE AMONG THE GODS OF BATTLESHIP !");
															printColorRed();
															printf("\n\n    Compliments from the developer team, you defeated our CPU.");
													}
											}
											else{
													system("clear");
													printColorGreen();
													if(difficulte == 1)
													{
															printf("\n\n                       WE GOT A WINNER !");
															printColorWhite();
															printf("\n       ... and it's NOT you ! Come on, that was the EASY mode !");
													}
													if(difficulte == 2)
													{
															printf("\n\n     YOU LOOSE. MAYBE YOU SHOULD TRY WITH A LOWER DIFFICULTY ?");
															printColorWhite();
															printf("\n              (we both know it will change nothing tho...)");
													}
													if(difficulte == 3)
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
											libererMatrice((short**) matrice_J1, LIGNES);
											libererMatrice((short**) matrice_J2, LIGNES);

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

	/*
		Display Menu
			switch
				- solo ==> startGame
				- host ==> startServer
				- join ==> establishConnection
	*/
		//int socket = establishConnection("127.0.0.1");

		//Send Matrice

		//ReceiveMatrice

		//Send coordonnees

		//ReveiceCoordonnes
}

/**
* 	Try to connect to the server until success.
*		Return connection socket
*/
int establishConnection(char* ip_address)
{
	printf("CLIENT started\n");

	struct sockaddr_in address;
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";
	char buffer[1024] = {0};
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

	/*
	send(sock , hello , strlen(hello) , 0 );
	printf("Client: Hello message sent\n");
	valread = read( sock , buffer, 1024);
	printf("%s\n",buffer );
	*/

	//Wait on semaphore to send to server
	//sem_t* sendRequest_sem = (sem_t*) param;
	//printf("CLIENT: wait sem before sending matrice\n");
	//sem_wait(sendRequest_sem);

	return sock;
}

/**
*		Send board game over socket connection
**/
