#ifndef BOARDTOOLS_H
#define BOARDTOOLS_H
#include "coordonnees.h"

char _getch(void);

short printBoard(char** matrice, short nb_colonnes, short nb_lignes, short votreboard);
short printBoard_af_tir(char** matrice, short nb_colonnes, short nb_lignes, coordonnees_tir dernier_tir, short votretir);

 void** creerMatrice(short nb_colonnes, short nb_lignes);
 void libererMatrice(short** matrice, short nb_lignes);

short isPossibleMove(char deplacement, short** ship,  short size_ship, short nb_colonnes, short nb_lignes);
char** placeShip(char** matrice, short nb_colonnes, short nb_lignes, short size_ship);

char** fireOnCoordonnees(short ligne, short colonne, char** matrice);
char** place_a_ship_here(char** matrice, short nb_colonnes, short nb_lignes, short size_ship, short col_prem_case, short ligne_prem_case, char orientation);
coordonnees_tir Aim(char** matrice, short nb_colonnes, short nb_lignes);


char** place_randomly(char** matrice, short nb_colonnes, short nb_lignes, short nb_ships);
coordonnees_tir Aim_randomly(char** matrice, short nb_colonnes, short nb_lignes, short difficulte);


short menuChoice2(char* choix1, char* choix2, short nb_space, char* message_quit);
short menuChoice3(char* choix1, char* choix2, char* choix3, short nb_space, char* message_quit);

short rand_a_b(short a, short b);

#endif // BOARDTOOLS_H
