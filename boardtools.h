#ifndef BOARDTOOLS_H
#define BOARDTOOLS_H
#include "coordonnees.h"

short isPossibleMove(char deplacement, short** ship,  short size_ship, short nb_colonnes, short nb_lignes);
char** placeShip(char** matrice, short nb_colonnes, short nb_lignes, short size_ship);
short printBoard(char** matrice, short nb_colonnes, short nb_lignes, short votreboard);
char** fireOnCoordonnees(short ligne, short colonne, char** matrice);
char** place_a_ship_here(char** matrice, short nb_colonnes, short nb_lignes, short size_ship, short col_prem_case, short ligne_prem_case, char orientation);
coordonnees_tir Aim(char** matrice, short nb_colonnes, short nb_lignes);

void** creerMatrice(short nb_colonnes, short nb_lignes);
char** place_randomly(char** matrice, short nb_colonnes, short nb_lignes);
void libererMatrice(short** matrice, short nb_lignes);

#endif // BOARDTOOLS_H
