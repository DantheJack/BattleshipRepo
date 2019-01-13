#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "coordonnees.h"
#include "boardtools.h"
#define tempssupp 200;

short printBoard(char** matrice, short nb_colonnes, short nb_lignes, short votreboard) //1 si c'est votre board, 0 sinon
{
    short i = 0;
    short j = 0;
    short k = 0;
    char contenu_case;
    system("cls");
    //color(14, 0);
    printf("\n         ------------- BATTLESHIP --------------\n\n");
    //color(15, 0);

    //Label des colonnes
    //color(11, 0);
    printf("     ");
    for (i=0; i<nb_colonnes; i++) {
        printf("   %c", i+65);
    }
    //color(15, 0);

    //1st ligne de la grille
    printf("\n      %c%c%c%c", 218, 196, 196, 196);
    for (i=0; i<nb_colonnes-1; i++) {
        printf("%c%c%c%c", 194, 196, 196, 196);
    }
    printf("%c", 191);

    //Contenu
    for (i=0; i<nb_lignes; i++) {

        //Label des lignes
        //color(11, 0);
        if(i<10) printf("\n    %d ", i);
        else printf("\n    %d", i);
        //color(15, 0);

        //1st ligne de separation verticale
        printf("%c ", 179);

        //Switch contenu de la case
        for (j=0; j<nb_colonnes; j++) {
            contenu_case = matrice[i][j];
            switch (contenu_case) {
            case 0 :
                printf(" "); //caractere representant l'eau
                break;
            case 'X' :
                printf("X"); //caractere representant un tir adverse dans l'eau
                break;
            case 'C' :
                if(votreboard) printf("C");
                else printf(" ");
                break;
            case 'c' :
                if(votreboard) printf("C");
                else printf("X");
                break;
            case 'B' :
                if(votreboard) printf("B");
                else printf(" ");
                break;
            case 'b' :
                if(votreboard) printf("B");
                else printf("X");
                break;
            case 'S' :
                if(votreboard) printf("S");
                else printf(" ");
                break;
            case 's' :
                if(votreboard) printf("S");
                else printf("X");
                break;
            case 'D' :
                if(votreboard) printf("D");
                else printf(" ");
                break;
            case 'd' :
                if(votreboard) printf("D");
                else printf("X");
                break;
            default :
                printf("j");
                break;
            }
            printf(" %c ", 179);
        }

        //Ligne de separation horizontale
        if(i != nb_lignes-1){
            printf("\n      %c%c%c%c", 195, 196, 196, 196); //premiere case de la ligne
            for (k=0; k<nb_colonnes-1; k++) {
                printf("%c%c%c%c", 197, 196, 196, 196); //reste de la ligne
            }
            printf("%c", 180);
        }
        else{ //Derniere ligne de la grille
            printf("\n      %c%c%c%c", 192, 196, 196, 196); //premiere case de la derniere ligne
            for (k=0; k<nb_colonnes-1; k++) {
                printf("%c%c%c%c", 193, 196, 196, 196); //reste de la derniere ligne
            }
            printf("%c", 217);
        }
    }
    //color(14, 0);
    printf("\n\n                  - (YOUR FLEET) -          \n\n");
    return 1;
}



short isPossibleMove(char deplacement, short** ship, short size_ship, short nb_colonnes, short nb_lignes){
    switch(deplacement){
        case 'z':
            if(ship[0][0] == 0){
                printf("c'est pas possible d'aller vers le haut");
                return 0;
            }
        break;
        case 'q':
            if(ship[0][1] == 0){
                printf("c'est pas possible d'aller a gauche");
                return 0;
            }
        break;
        case 'd':
            if(ship[size_ship-1][1] == nb_lignes-1){
                printf("c'est pas possible d'aller a droite");
                return 0;
            }
        break;
        case 's':
            if(ship[size_ship-1][0] == nb_colonnes-1){
                printf("c'est pas possible d'aller vers le bas");
                return 0;
            }
        break;

        default:
            break;
    }

    return 1;
}

char** placeShip(char** matrice, short nb_colonnes, short nb_lignes, short size_ship)
{
    short cpt_pblm = 0;
    short ship_done = 0;
    short i = 0;
    short j = 0;
    short k = 0;
    short m = 0;
    short n = 0;
    short o = 0;
    short p = 0;
    char touche;
    char contenu_case;
    char symbole_ship = '0';
    char symbole_ship_maj = '0';
    if(size_ship == 2){
        symbole_ship = 'd';
        symbole_ship_maj = 'D';
    }
    else if(size_ship == 3){
        symbole_ship = 's';
        symbole_ship_maj = 'S';
    }
    else if(size_ship == 4){
        symbole_ship = 'b';
        symbole_ship_maj = 'B';
    }
    else if(size_ship == 5){
        symbole_ship = 'c';
        symbole_ship_maj = 'C';
    }

    //Creation d'une sauvegarde du board
    char** matrice_saved = creerMatrice(nb_colonnes, nb_lignes);
    for (i=0; i<nb_colonnes; i++){
        for(j=0; j<nb_lignes; j++){
            matrice_saved[i][j]=matrice[i][j]; //memccpoy doesn't work for some reason...
        }
    }

    //Creation d'un bateau de taille 2, 3, 4 ou 5 (selon size_ship)
    short** ship = creerMatrice(2, size_ship);

    //initialisation placement bateau (coin sup. gauche verticalement)
    for (m=0; m<size_ship; m++){
        ship[m][0] = m; //remplit la colonne no 0 de ship avec les lignes sur lequel il se trouve dans matrice
    }

    //Creation d'une sauvegarde du ship (pour la rotation)
    char** ship_s = creerMatrice(2, size_ship);
    for (j=0; j<2; j++){
        for(i=0; i<size_ship; i++){
            ship_s[i][j]=ship[i][j];
        }
    }
    /**
    //Initialisation du temps d'affichage
    clock_t tempsaffmatrices = clock() + tempssupp;**/
    short switchmatrice = 1; // 1 pour matrice_saved, 0 pour matrice


    while(ship_done != 1){

        //Boucle remplacant matrice par sa version matrice_saved
        for (i=0; i<nb_colonnes; i++){
            for(j=0; j<nb_lignes; j++){
                matrice[i][j]=matrice_saved[i][j];
            }
        }

        //Boucle actualisant ship_s avec les nouvelles valeurs de ship (tjrs pour la rotation)
        for (j=0; j<2; j++){
            for(i=0; i<size_ship; i++){
                ship_s[i][j]=ship[i][j];
            }
        }

        //Boucle mettant le bateau dans matrice (actualisation du placement du bateau sur la grille)
        for (n=0; n<size_ship; n++)
        {
            matrice[ship[n][0]][ship[n][1]]=symbole_ship;
        }

        //Boucle d'affichage des deux matrices pour clignotement
        while (!kbhit()) {
            if(/**clock() >= tempsaffmatrices && **/switchmatrice == 1){
                system("cls");

                //color(14, 0);
                printf("\n       -------------- (YOUR FLEET) --------------\n\n");
                //color(15, 0);
                //Label des colonnes
                //color(11, 0);
                printf("     ");
                for (i=0; i<nb_colonnes; i++) {
                    printf("   %c", i+65);
                }
                //color(15, 0);

                //1st ligne de la grille
                printf("\n      %c%c%c%c", 218, 196, 196, 196);
                for (i=0; i<nb_colonnes-1; i++) {
                    printf("%c%c%c%c", 194, 196, 196, 196);
                }
                printf("%c", 191);

                //Contenu
                for (i=0; i<nb_lignes; i++) {

                    //Label des lignes
                    //color(11, 0);
                    if(i<10) printf("\n    %d ", i);
                    else printf("\n    %d", i);
                    //color(15, 0);

                    //1st ligne de separation verticale
                    printf("%c ", 179);

                    //Switch contenu de la case
                    for (j=0; j<nb_colonnes; j++) {

                        contenu_case = matrice[i][j];

                        if(contenu_case != matrice_saved[i][j] && matrice_saved[i][j] != 0) {}//color(12, 0);

                        switch (contenu_case) {
                            case 0 :
                                printf(" "); //caractere representant l'eau
                                break;
                            case 'X' :
                                printf("X"); //caractere representant un tir adverse dans l'eau
                            case 'C' :
                                printf("C");
                                break;
                            case 'c' :
                                printf("C");
                                break;
                            case 'B' :
                                printf("B");
                                break;
                            case 'b' :
                                printf("B");
                                break;
                            case 'S' :
                                printf("S");
                                break;
                            case 's' :
                                printf("S");
                                break;
                            case 'D' :
                                printf("D");
                                break;
                            case 'd' :
                                printf("D");
                                break;
                            default :
                                printf("u");
                                break;
                        }
                        //color(15, 0);
                        printf(" %c ", 179);
                    }

                    //Ligne de separation horizontale
                    if(i != nb_lignes-1){
                        printf("\n      %c%c%c%c", 195, 196, 196, 196); //premiere case de la ligne
                        for (k=0; k<nb_colonnes-1; k++) {
                            printf("%c%c%c%c", 197, 196, 196, 196); //reste de la ligne
                        }
                        printf("%c", 180);
                    }
                    else{ //Derniere ligne de la grille
                        printf("\n      %c%c%c%c", 192, 196, 196, 196); //premiere case de la derniere ligne
                        for (k=0; k<nb_colonnes-1; k++) {
                            printf("%c%c%c%c", 193, 196, 196, 196); //reste de la derniere ligne
                        }
                        printf("%c", 217);
                    }
                }
                //color(15, 0);
                printf("\n Press A or E to rotate, move your ship with Z, Q, S and D.");
                printf("\n                 Press Enter to confirm.\n");

                /**tempsaffmatrices = clock() + tempssupp;**/
                switchmatrice = 0;
            }

            /**if(clock() >= tempsaffmatrices && switchmatrice == 1){
                system("cls");
                printBoard(matrice_saved, nb_colonnes, nb_lignes, 1);
                tempsaffmatrices = clock() + tempssupp;
                switchmatrice = 0;
            }**/
        }
        touche = getch();
        switchmatrice = 1;
        switch(touche) {
            case 'z':
                if(isPossibleMove('z', ship, size_ship, nb_colonnes, nb_lignes)){
                    for(i=0; i<size_ship; i++){
                        ship[i][0]=ship[i][0]-1;
                    }
                }
            break;
            case 'q':
                if(isPossibleMove('q', ship, size_ship, nb_colonnes, nb_lignes)){
                    for(i=0; i<size_ship; i++){
                        ship[i][1]=ship[i][1]-1;
                    }
                }
            break;
            case 'd':
                if(isPossibleMove('d', ship, size_ship, nb_colonnes, nb_lignes)){
                    for(i=0; i<size_ship; i++){
                        ship[i][1]=ship[i][1]+1;
                    }
                }
            break;
            case 's':
                if(isPossibleMove('s', ship, size_ship, nb_colonnes, nb_lignes)){
                    for(i=0; i<size_ship; i++){
                        ship[i][0]=ship[i][0]+1;
                    }
                }
            break;
            case 'e': touche = 'a';
            case 'a':
                if(size_ship==2){
                    if(ship[1][0]!=ship[0][0]){ //vertical
                        if(ship[0][1]==nb_colonnes-1){
                            ship[0][1] = nb_colonnes-2;
                            ship[1][0] = ship[0][0];
                        }
                        else{
                            ship[1][0] = ship_s[1][0]-1;
                            ship[1][1] = ship_s[1][1]+1;

                        }
                    }
                    else{                       //horizontal
                        if(ship[1][0]==nb_lignes-1){
                            ship[0][0] = nb_lignes-2;
                            ship[0][1] = ship_s[1][1];
                        }
                        else{
                            ship[1][0] = ship_s[1][0]+1;
                            ship[1][1] = ship_s[1][1]-1;
                        }
                    }
                }
                if(size_ship==3){
                    if(ship[1][0]!=ship[0][0]){ //vertical
                        if(ship[1][1]==0){
                            ship[0][0] = ship_s[1][0];
                            ship[0][1] = 0;
                            ship[1][1] = 1;
                            ship[2][0] = ship_s[1][0];
                            ship[2][1] = 2;
                        }
                        else if(ship[1][1]==nb_colonnes-1){
                            ship[0][0] = ship_s[1][0];
                            ship[0][1] = nb_colonnes-3;
                            ship[1][1] = nb_colonnes-2;
                            ship[2][0] = ship_s[1][0];
                            ship[2][1] = nb_colonnes-1;
                        }
                        else{
                            ship[0][0] = ship_s[1][0];
                            ship[0][1] = ship_s[1][1]-1;
                            ship[2][0] = ship_s[1][0];
                            ship[2][1] = ship_s[1][1]+1;
                        }
                    }
                    else{                       //horizontal
                        if(ship[1][0]==0){
                            ship[0][1] = ship_s[1][1];
                            ship[1][0] = 1;
                            ship[2][0] = 2;
                            ship[2][1] = ship_s[1][1];
                        }
                        else if(ship[1][0]==nb_lignes-1){
                            ship[0][0] = nb_lignes-3;
                            ship[0][1] = ship_s[1][1];
                            ship[1][0] = nb_lignes-2;
                            ship[2][0] = nb_lignes-1;
                            ship[2][1] = ship_s[1][1];
                        }
                        else{
                            ship[0][0] = ship_s[1][0]-1;
                            ship[0][1] = ship_s[1][1];
                            ship[2][0] = ship_s[1][0]+1;
                            ship[2][1] = ship_s[1][1];
                        }
                    }
                }
                if(size_ship==4){
                    if(ship[1][0]!=ship[0][0]){ //vertical
                        if(ship[1][1]==0){
                            ship[0][0] = ship_s[1][0];
                            ship[0][1] = 0;
                            ship[1][1] = 1;
                            ship[2][0] = ship_s[1][0];
                            ship[2][1] = 2;
                            ship[3][0] = ship_s[1][0];
                            ship[3][1] = 3;
                        }
                        else if((ship[1][1]==nb_colonnes-1)||(ship[1][1]==nb_colonnes-2)){
                            ship[0][0] = ship_s[1][0];
                            ship[0][1] = nb_colonnes-4;
                            ship[1][1] = nb_colonnes-3;
                            ship[2][0] = ship_s[1][0];
                            ship[2][1] = nb_colonnes-2;
                            ship[3][0] = ship_s[1][0];
                            ship[3][1] = nb_colonnes-1;
                        }
                        else{
                            ship[0][0] = ship_s[1][0];
                            ship[0][1] = ship_s[1][1]-1;
                            ship[2][0] = ship_s[1][0];
                            ship[2][1] = ship_s[1][1]+1;
                            ship[3][0] = ship_s[1][0];
                            ship[3][1] = ship_s[1][1]+2;
                        }
                    }
                    else{                       //horizontal
                        if(ship[1][0]==0){
                            ship[0][1] = ship_s[1][1];
                            ship[1][0] = 1;
                            ship[2][0] = 2;
                            ship[2][1] = ship_s[1][1];
                            ship[3][0] = 3;
                            ship[3][1] = ship_s[1][1];
                        }
                        else if((ship[1][0]==nb_lignes-1)||(ship[1][0]==nb_lignes-2)){
                            ship[0][0] = nb_lignes-4;
                            ship[0][1] = ship_s[1][1];
                            ship[1][0] = nb_lignes-3;
                            ship[2][0] = nb_lignes-2;
                            ship[2][1] = ship_s[1][1];
                            ship[3][0] = nb_lignes-1;
                            ship[3][1] = ship_s[1][1];

                        }
                        else{
                            ship[0][0] = ship_s[1][0]-1;
                            ship[0][1] = ship_s[1][1];
                            ship[1][0] = ship_s[1][0];
                            ship[1][1] = ship_s[1][1];
                            ship[2][0] = ship_s[1][0]+1;
                            ship[2][1] = ship_s[1][1];
                            ship[3][0] = ship_s[1][0]+2;
                            ship[3][1] = ship_s[1][1];
                        }
                    }
                }
                if(size_ship==5){
                    if(ship[1][0]!=ship[0][0]){ //vertical
                        if((ship[1][1]==0)||(ship[1][1]==1)){
                            ship[0][0] = ship_s[2][0];
                            ship[0][1] = 0;
                            ship[1][0] = ship_s[2][0];
                            ship[1][1] = 1;
                            ship[2][1] = 2;
                            ship[3][0] = ship_s[2][0];
                            ship[3][1] = 3;
                            ship[4][0] = ship_s[2][0];
                            ship[4][1] = 4;
                        }
                        else if((ship[1][1]==nb_colonnes-1)||(ship[1][1]==nb_colonnes-2)){
                            ship[0][0] = ship_s[2][0];
                            ship[0][1] = nb_colonnes-5;
                            ship[1][0] = ship_s[2][0];
                            ship[1][1] = nb_colonnes-4;
                            ship[2][1] = nb_colonnes-3;
                            ship[3][0] = ship_s[2][0];
                            ship[3][1] = nb_colonnes-2;
                            ship[4][0] = ship_s[2][0];
                            ship[4][1] = nb_colonnes-1;
                        }
                        else{
                            ship[0][0] = ship_s[2][0];
                            ship[0][1] = ship_s[2][1]-2;
                            ship[1][0] = ship_s[2][0];
                            ship[1][1] = ship_s[2][1]-1;
                            ship[3][0] = ship_s[2][0];
                            ship[3][1] = ship_s[2][1]+1;
                            ship[4][0] = ship_s[2][0];
                            ship[4][1] = ship_s[2][1]+2;
                        }
                    }
                    else{                       //horizontal
                        if((ship[1][0]==0)||(ship[1][0]==1)){
                            ship[0][1] = ship_s[2][1];
                            ship[1][0] = 1;
                            ship[1][1] = ship_s[2][1];
                            ship[2][0] = 2;
                            ship[3][0] = 3;
                            ship[3][1] = ship_s[2][1];
                            ship[4][0] = 4;
                            ship[4][1] = ship_s[2][1];
                        }
                        else if((ship[1][0]==nb_lignes-1)||(ship[1][0]==nb_lignes-2)){
                            ship[0][0] = nb_lignes-5;
                            ship[0][1] = ship_s[2][1];
                            ship[1][0] = nb_lignes-4;
                            ship[1][1] = ship_s[2][1];
                            ship[2][0] = nb_lignes-3;
                            ship[3][0] = nb_lignes-2;
                            ship[3][1] = ship_s[2][1];
                            ship[4][0] = nb_lignes-1;
                            ship[4][1] = ship_s[2][1];
                        }
                        else{
                            ship[0][0] = ship_s[2][0]-2;
                            ship[0][1] = ship_s[2][1];
                            ship[1][0] = ship_s[2][0]-1;
                            ship[1][1] = ship_s[2][1];
                            ship[3][0] = ship_s[2][0]+1;
                            ship[3][1] = ship_s[2][1];
                            ship[4][0] = ship_s[2][0]+2;
                            ship[4][1] = ship_s[2][1];
                        }
                    }
                }
            break;
            case (short) 13 : //touche Entree
                for(o=0; o<size_ship; o++){
                    if(matrice_saved[ship[o][0]][ship[o][1]] != 0)
                    {
                        cpt_pblm++;
                    }
                }
                if(cpt_pblm == 0){
                    for(p=0; p<size_ship; p++){
                        matrice[ship[p][0]][ship[p][1]] = symbole_ship_maj;
                    }
                    ship_done = 1;
                }
                else{
                    printf("\nproblem !\n");
                }
                cpt_pblm = 0;
            break;
            default :
                printf("Key is : %d", (short)touche);
            break;
        }
    }
    return matrice;
}

char** fireOnCoordonnees(short ligne, short colonne, char** matrice)
{
    if(matrice[ligne][colonne]=='C')
    {
        matrice[ligne][colonne]='c';
    }
    else if(matrice[ligne][colonne]=='S')
    {
        matrice[ligne][colonne]='s';
    }
    else if(matrice[ligne][colonne]=='D')
    {
        matrice[ligne][colonne]='d';
    }
    else if(matrice[ligne][colonne]=='B')
    {
        matrice[ligne][colonne]='b';
    }
    else matrice[ligne][colonne]='X';
    return matrice;
}

coordonnees_tir Aim(char** matrice, short nb_colonnes, short nb_lignes){

    short votreboard = 0;
    short cpt_pblm = 0;
    short tir_done = 0;
    short i = 0;
    short j = 0;
    short k = 0;
    short o = 0;
    char touche;
    char contenu_case;
    short size_ship = 1;

    coordonnees_tir tir;

    //Creation d'une sauvegarde du board
    char** matrice_saved = creerMatrice(nb_colonnes, nb_lignes);
    for (i=0; i<nb_colonnes; i++){
        for(j=0; j<nb_lignes; j++){
            matrice_saved[i][j]=matrice[i][j];
        }
    }

    //Creation d'un curseur
    short** curseur = creerMatrice(2, 1);
    curseur[0][0] = 0;
    curseur[0][1] = 0;

    //Initialisation du temps d'affichage
    clock_t tempsaffmatrices = clock() + tempssupp;
    short switchmatrice = 1; // 1 pour matrice_saved, 0 pour matrice

    while(tir_done != 1){

        //Boucle remplacant matrice par sa version matrice_saved
        for (i=0; i<nb_colonnes; i++){
            for(j=0; j<nb_lignes; j++){
                matrice[i][j]=matrice_saved[i][j];
            }
        }

        //Boucle d'affichage des deux matrices pour clignotement
        while (!kbhit()) {
            if(/**clock() >= tempsaffmatrices && **/switchmatrice == 1){
                system("cls");

                //color(14, 0);
                printf("\n     -------------- (ENNEMY'S FLEET) --------------\n\n");
                //color(15, 0);
                //Label des colonnes
                //color(11, 0);
                printf("     ");
                for (i=0; i<nb_colonnes; i++) {
                    printf("   %c", i+65);
                }
                //color(15, 0);

                //1st ligne de la grille
                printf("\n      %c%c%c%c", 218, 196, 196, 196);
                for (i=0; i<nb_colonnes-1; i++) {
                    printf("%c%c%c%c", 194, 196, 196, 196);
                }
                printf("%c", 191);

                //Contenu
                for (i=0; i<nb_lignes; i++) {

                    //Label des lignes
                    //color(11, 0);
                    if(i<10) printf("\n    %d ", i);
                    else printf("\n    %d", i);
                    //color(15, 0);

                    //1st ligne de separation verticale
                    printf("%c", 179);
                    if (curseur[0][1]==0 && curseur[0][0]==i){
                        //color(14, 0);
                        printf("%c", 62);
                        //color(15, 0);
                    }
                    else printf(" ");

                    //Switch contenu de la case
                    for (j=0; j<nb_colonnes; j++) {

                        contenu_case = matrice[i][j];

                        switch (contenu_case) {
                        case 0 :
                            printf(" "); //caractere representant l'eau

                            break;
                        case 'X' :
                            printf("X"); //caractere representant un tir adverse dans l'eau
                            break;
                        case 'C' :
                            if(votreboard) printf("C");
                            else printf(" ");
                            break;
                        case 'c' :
                            if(votreboard) printf("C");
                            else printf("X");
                            break;
                        case 'B' :
                            if(votreboard) printf("B");
                            else printf(" ");
                            break;
                        case 'b' :
                            if(votreboard) printf("B");
                            else printf("X");
                            break;
                        case 'S' :
                            if(votreboard) printf("S");
                            else printf(" ");
                            break;
                        case 's' :
                            if(votreboard) printf("S");
                            else printf("X");
                            break;
                        case 'D' :
                            if(votreboard) printf("D");
                            else printf(" ");
                            break;
                        case 'd' :
                            if(votreboard) printf("D");
                            else printf("X");
                            break;
                        default :
                            printf("%c", 170);
                            break;
                        }
                        //color(15, 0);
                        if (curseur[0][0]==i && curseur[0][1]==j){
                            //color(14, 0);
                            printf("%c", 60);
                            //color(15, 0);
                            printf("%c ", 179);
                        }
                        else if (curseur[0][0]==i && curseur[0][1]==j+1){

                            printf(" %c", 179);
                            //color(14, 0);
                            printf("%c", 62);
                            //color(15, 0);
                        }
                        else printf(" %c ", 179);
                    }

                    //Ligne de separation horizontale
                    if(i != nb_lignes-1){
                        printf("\n      %c%c%c%c", 195, 196, 196, 196); //premiere case de la ligne
                        for (k=0; k<nb_colonnes-1; k++) {
                            printf("%c%c%c%c", 197, 196, 196, 196); //reste de la ligne
                        }
                        printf("%c", 180);
                    }
                    else{ //Derniere ligne de la grille
                        printf("\n      %c%c%c%c", 192, 196, 196, 196); //premiere case de la derniere ligne
                        for (k=0; k<nb_colonnes-1; k++) {
                            printf("%c%c%c%c", 193, 196, 196, 196); //reste de la derniere ligne
                        }
                        printf("%c", 217);
                    }
                }
                //color(15, 0);
                printf("\n         Move your reticule with Z, Q, S and D.");
                printf("\n                  Press Enter to confirm.");

                /**tempsaffmatrices = clock() + tempssupp;**/
                switchmatrice = 0;
            }

            /**if(clock() >= tempsaffmatrices && switchmatrice == 1){
                system("cls");
                printBoard(matrice_saved, nb_colonnes, nb_lignes, 1);
                tempsaffmatrices = clock() + tempssupp;
                switchmatrice = 0;
            }**/
        }
        touche = getch();
        switchmatrice = 1;
        switch(touche) {
            case 'z':
                if(isPossibleMove('z', curseur, size_ship, nb_colonnes, nb_lignes)){
                    for(i=0; i<size_ship; i++){
                        curseur[i][0]=curseur[i][0]-1;
                    }
                }
            break;
            case 'q':
                if(isPossibleMove('q', curseur, size_ship, nb_colonnes, nb_lignes)){
                    for(i=0; i<size_ship; i++){
                        curseur[i][1]=curseur[i][1]-1;
                    }
                }
            break;
            case 'd':
                if(isPossibleMove('d', curseur, size_ship, nb_colonnes, nb_lignes)){
                    for(i=0; i<size_ship; i++){
                        curseur[i][1]=curseur[i][1]+1;
                    }
                }
            break;
            case 's':
                if(isPossibleMove('s', curseur, size_ship, nb_colonnes, nb_lignes)){
                    for(i=0; i<size_ship; i++){
                        curseur[i][0]=curseur[i][0]+1;
                    }
                }
            break;

            case (short) 13 : //touche Entree
                for(o=0; o<size_ship; o++){
                    if((matrice_saved[curseur[o][0]][curseur[o][1]] == 'X')
                        ||(matrice_saved[curseur[o][0]][curseur[o][1]] == 'c')
                        ||(matrice_saved[curseur[o][0]][curseur[o][1]] == 'b')
                        ||(matrice_saved[curseur[o][0]][curseur[o][1]] == 's')
                        ||(matrice_saved[curseur[o][0]][curseur[o][1]] == 'd'))
                    {
                        cpt_pblm++;
                    }
                }
                if(cpt_pblm == 0){
                    fireOnCoordonnees(curseur[0][0], curseur[0][1], matrice);
                    tir.a_la_ligne = curseur[0][0];
                    tir.a_la_colonne = curseur[0][1];
                    tir_done = 1;
                }
                else{
                    printf("Vous avez deja tire ici, veuillez choisir une cible valide.");
                }
                cpt_pblm = 0;
            break;
            default :
                printf("Key is : %d", (short)touche);
            break;
        }
    }
    return tir;
}




/** uniquement pour le developpement **/
char** place_a_ship_here(char** matrice, short nb_colonnes, short nb_lignes, short size_ship, short col_prem_case, short ligne_prem_case, char orientation)
{
    short i = 0;
    short j = 0;
    short k = 0;
    char symbole_ship_maj = ' ';

    if(size_ship == 2){
        symbole_ship_maj = 'D';
    }
    else if(size_ship == 3){
        symbole_ship_maj = 'S';
    }
    else if(size_ship == 4){
        symbole_ship_maj = 'B';
    }
    else if(size_ship == 5){
        symbole_ship_maj = 'C';
    }

    //Creation d'un bateau de taille 2, 3, 4 ou 5 (selon size_ship)
    short** ship = creerMatrice(2, size_ship);

    //placement bateau
    if(orientation == 'v'){
        for (i=0; i<size_ship; i++){
            ship[i][0] = i + ligne_prem_case;
            ship[i][1] = col_prem_case;
        }
    }
    else{
        for (j=0; j<size_ship; j++){
            ship[j][0] = ligne_prem_case;
            ship[j][1] = j + col_prem_case;
        }
    }

    //placer ship dans matrice
    for(k=0; k<size_ship; k++){
        matrice[ship[k][0]][ship[k][1]] = symbole_ship_maj;
    }
    return matrice;
}

void printBattleShipMessage(char* chaine, char** matrice, short nb_colonnes, short nb_lignes){
    short i = 0;
    short j = 0;
    short k = 0;
    char contenu_case;
    printf("\n           ------------- BATTLESHIP --------------\n\n");

    //Label des colonnes
    //color(11, 0);
    printf("     ");
    for (i=0; i<nb_colonnes; i++) {
        printf("   %c", i+65);
    }
    //color(15, 0);

    //1st ligne de la grille
    printf("\n      %c%c%c%c", 218, 196, 196, 196);
    for (i=0; i<nb_colonnes-1; i++) {
        printf("%c%c%c%c", 194, 196, 196, 196);
    }
    printf("%c", 191);

    //Contenu
    for (i=0; i<nb_lignes; i++) {

        //Label des lignes
        //color(11, 0);
        if(i<10) printf("\n    %d ", i);
        else printf("\n    %d", i);
        //color(15, 0);

        //1st ligne de separation verticale
        printf("%c ", 179);

        //Switch contenu de la case
        for (j=0; j<nb_colonnes; j++) {

            printf("  %c ", 179);
        }

        //Ligne de separation horizontale
        if(i != nb_lignes-1){
            printf("\n      %c%c%c%c", 195, 196, 196, 196); //premiere case de la ligne
            for (k=0; k<nb_colonnes-1; k++) {
                printf("%c%c%c%c", 197, 196, 196, 196); //reste de la ligne
            }
            printf("%c", 180);
        }
        else{ //Derniere ligne de la grille
            printf("\n      %c%c%c%c", 192, 196, 196, 196); //premiere case de la derniere ligne
            for (k=0; k<nb_colonnes-1; k++) {
                printf("%c%c%c%c", 193, 196, 196, 196); //reste de la derniere ligne
            }
            printf("%c", 217);
        }
    }
    //color(14, 0);
    printf("\n\n                      - (YOUR FLEET) -          \n\n");
    getchar();
}


char** place_randomly(char** matrice, short nb_colonnes, short nb_lignes){

    short size_ship = 5;
    short col_prem_case = 0;
    short ligne_prem_case = 0;
    char orientation = 'v';
    short cpt_pblm = 0;
    short i = 0;
    short j = 0;
    short o = 0;
    short k = 0;
    char symbole_ship_maj;
    short cpt = 1;
    srand((unsigned short)time(NULL)); // initialisation de rand



    //Creation d'un bateau de taille 5
    short** ship = creerMatrice(2, size_ship);

    while(cpt!=11){

        if(cpt == 1 || cpt == 2 || cpt == 3 || cpt == 4){
            size_ship = 2;
            symbole_ship_maj = 'D';
        }
        else if(cpt == 5 || cpt == 6 || cpt == 7){
            size_ship = 3;
            symbole_ship_maj = 'S';
        }
        else if(cpt == 8 || cpt == 9){
            size_ship = 4;
            symbole_ship_maj = 'B';
        }
        else if(cpt == 10){
            size_ship = 5;
            symbole_ship_maj = 'C';
        }

        orientation = (rand() & 1) ? 'v' : 'h';
        col_prem_case = (orientation == 'v') ? rand()%(nb_colonnes) : rand()%(nb_colonnes-size_ship) ; //rand entre 0 et nb_colonnes
        ligne_prem_case = (orientation == 'h') ? rand()%(nb_lignes) : rand()%(nb_lignes-size_ship) ;

        //placement bateau
        if(orientation == 'v'){
            for (i=0; i<size_ship; i++){
                ship[i][0] = i + ligne_prem_case;
                ship[i][1] = col_prem_case;
            }
        }
        else{
            for (j=0; j<size_ship; j++){
                ship[j][0] = ligne_prem_case;
                ship[j][1] = j + col_prem_case;
            }
        }
        //placer ship dans matrice
        for(o=0; o<size_ship; o++){
            if(matrice[ship[o][0]][ship[o][1]] != 0)
            {
                cpt_pblm++;
            }
        }
        if(cpt_pblm == 0){
            for(k=0; k<size_ship; k++){
                matrice[ship[k][0]][ship[k][1]] = symbole_ship_maj;
            }
            cpt++;
        }
        cpt_pblm = 0;
    }
    libererMatrice(ship, size_ship);
    return matrice;
}


void** creerMatrice(short nb_colonnes, short nb_lignes)
{
    short i=0;
    void** matrice = NULL;

    matrice = (void **)calloc(nb_lignes, sizeof(void *));
    if (matrice == NULL) {
        printf("-- Erreur lors de l'allocation de la memoire des colonnes --\n");
        exit(0);
    }

    for (i=0; i<nb_lignes; i++) {
        matrice[i] = (void *)calloc(nb_colonnes, sizeof(void));
        if (matrice[i] == NULL) {
            printf("-- Erreur lors de l'allocation de la memoire des lignes --\n");
        }
    }
    return matrice;
}

void libererMatrice(short** matrice, short nb_lignes) {

    short i = 0;

    //On libere le bloc memoire de chaque ligne (les tableaux au deuxieme niveau)
    for (i=0; i<nb_lignes; i++) {

        free(matrice[i]);
        matrice[i] = NULL;
    }

    //On libere le bloc memoire des pointeurs (premier tableau)
    free(matrice);
    matrice = NULL;
}
