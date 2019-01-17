#include <stdlib.h>
#include <stdio.h>
#include "coordonnees.h"
#include "boardtools.h"
#include "printtools.h"
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>

//Linux Box-Drawing Characters
#define RB "\e(0\x6a\e(B" // RB Right Bottom corner
#define RT "\e(0\x6b\e(B" // RT Right Top corner
#define LT "\e(0\x6c\e(B" // LT Left Top cornet
#define LB "\e(0\x6d\e(B" // LB Left Bottom corner
#define MC "\e(0\x6e\e(B" // MC Midle Cross
#define HL "\e(0\x71\e(B" // HL Horizontal Line
#define LC "\e(0\x74\e(B" // LC Left Cross
#define RC "\e(0\x75\e(B" // RC Right Cross
#define BC "\e(0\x76\e(B" // BC Bottom Cross
#define TC "\e(0\x77\e(B" // TC Top Cross
#define VL "\e(0\x78\e(B" // VL Vertical Line

char _getch() {
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    //printf("%c\n",buf);
    return buf;
 }

short printBoard(char** matrice, short nb_colonnes, short nb_lignes, short votreboard) //1 si c'est votre board, 0 sinon
{
    short i = 0;
    short j = 0;
    short k = 0;
    char contenu_case;
    system("clear");
    printColorYellow();
    printf("\n        ------------- BATTLESHIP --------------\n\n");
    printColorWhite();

    //Label des colonnes
    printColorCyan();
    printf("     ");
    for (i=0; i<nb_colonnes; i++) {
        printf("   %c", i+65);
    }
    printColorWhite();

    //1st ligne de la grille
    printf("\n      %s%s%s%s", LT, HL, HL, HL);
    for (i=0; i<nb_colonnes-1; i++) {
        printf("%s%s%s%s", TC, HL, HL, HL);
    }
    printf("%s", RT);

    //Contenu
    for (i=0; i<nb_lignes; i++) {

        //Label des lignes
        printColorCyan();
        if(i<10) printf("\n    %d ", i);
        else printf("\n    %d", i);
        printColorWhite();

        //1st ligne de separation verticale
        printf("%s ", VL);

        //Switch contenu de la case
        for (j=0; j<nb_colonnes; j++) {
            contenu_case = matrice[i][j];
            switch (contenu_case) {
            case 0 :
                printf(" "); //caractere representant l'eau
                break;
            case 'X' :
                printColorCyan();
                printf("X");//caractere representant un tir dans l'eau
                printColorWhite();
                break;
            case 'C' :
                if(votreboard) { printColorWhite(); printf("C"); printColorWhite();}
                else printf(" ");
                break;
            case 'c' :
                if(votreboard) { printColorRed(); printf("C"); printColorWhite(); }
                else { printColorRed(); printf("X"); printColorWhite(); }
                break;
            case 'B' :
                if(votreboard) { printColorWhite(); printf("B"); printColorWhite();}
                else printf(" ");
                break;
            case 'b' :
                if(votreboard) { printColorRed(); printf("B"); printColorWhite(); }
                else { printColorRed(); printf("X"); printColorWhite(); }
                break;
            case 'S' :
                if(votreboard) { printColorWhite(); printf("S"); printColorWhite();}
                else printf(" ");
                break;
            case 's' :
                if(votreboard) { printColorRed(); printf("S"); printColorWhite(); }
                else { printColorRed(); printf("X"); printColorWhite(); }
                break;
            case 'D' :
                if(votreboard) { printColorWhite(); printf("D"); printColorWhite();}
                else printf(" ");
                break;
            case 'd' :
                if(votreboard) { printColorRed(); printf("D"); printColorWhite(); }
                else { printColorRed(); printf("X"); printColorWhite(); }
                break;
            default :
                printf("j");
                break;
            }
            printf(" %s ", VL);
        }

        //Ligne de separation horizontale
        if(i != nb_lignes-1){
            printf("\n      %s%s%s%s", LC, HL, HL, HL); //premiere case de la ligne
            for (k=0; k<nb_colonnes-1; k++) {
                printf("%s%s%s%s", MC, HL, HL, HL); //reste de la ligne
            }
            printf("%s", RC);
        }
        else{ //Derniere ligne de la grille
            printf("\n      %s%s%s%s", LB, HL, HL, HL); //premiere case de la derniere ligne
            for (k=0; k<nb_colonnes-1; k++) {
                printf("%s%s%s%s", BC, HL, HL, HL); //reste de la derniere ligne
            }
            printf("%s", RB);
        }
    }
    printColorYellow();
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
    char** matrice_saved = (char**)creerMatrice(nb_colonnes, nb_lignes);
    for (i=0; i<nb_colonnes; i++){
        for(j=0; j<nb_lignes; j++){
            matrice_saved[i][j]=matrice[i][j]; //memccpoy doesn't work for some reason...
        }
    }

    //Creation d'un bateau de taille 2, 3, 4 ou 5 (selon size_ship)
    short** ship = (short**)creerMatrice(2, size_ship);

    //initialisation placement bateau (coin sup. gauche verticalement)
    for (m=0; m<size_ship; m++){
        ship[m][0] = m; //remplit la colonne ne0 de ship avec les lignes sur lequel il se trouve dans matrice
    }

    //Creation d'une sauvegarde du ship (pour la rotation)
    short** ship_s = (short**)creerMatrice(2, size_ship);
    for (j=0; j<2; j++){
        for(i=0; i<size_ship; i++){
            ship_s[i][j]=ship[i][j];
        }
    }

    while(ship_done != 1){

        //Boucle remplaeant matrice par sa version matrice_saved
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

        system("clear");

        printColorYellow();
        printf("\n       -------------- (YOUR FLEET) --------------\n\n");
        printColorWhite();
        //Label des colonnes
        printColorCyan();
        printf("     ");
        for (i=0; i<nb_colonnes; i++) {
            printf("   %c", i+65);
        }
        printColorWhite();

        //1st ligne de la grille
        printf("\n      %s%s%s%s", LT, HL, HL, HL);
        for (i=0; i<nb_colonnes-1; i++) {
            printf("%s%s%s%s", TC, HL, HL, HL);
        }
        printf("%s", RT);

        //Contenu
        for (i=0; i<nb_lignes; i++) {

            //Label des lignes
            printColorCyan();
            if(i<10) printf("\n    %d ", i);
            else printf("\n    %d", i);
            printColorWhite();

            //1st ligne de separation verticale
            printf("%s ", VL);

            //Switch contenu de la case
            for (j=0; j<nb_colonnes; j++) {

                contenu_case = matrice[i][j];

                if(contenu_case != matrice_saved[i][j] && matrice_saved[i][j] != 0) printColorGreen();

                switch (contenu_case) {
                    case 0 :
                        printf(" "); //caractere representant l'eau
                        break;
                    case 'X' :
                        printColorCyan();
                        printf("X");//caractere representant un tir dans l'eau
                        printColorWhite();
                    case 'C' :
                    case 'c' :
                        printColorWhite();
                        printf("C");
                        printColorWhite();
                        break;
                    case 'B' :
                    case 'b' :
                        printColorWhite();
                        printf("B");
                        printColorWhite();
                        break;
                    case 'S' :
                    case 's' :
                        printColorWhite();
                        printf("S");
                        printColorWhite();
                        break;
                    case 'D' :
                    case 'd' :
                        printColorWhite();
                        printf("D");
                        printColorWhite();
                        break;
                    default :
                        printf("u");
                        break;
                }
                printColorWhite();
                printf(" %s ", VL);
            }

            //Ligne de separation horizontale
            if(i != nb_lignes-1){
                printf("\n      %s%s%s%s", LC, HL, HL, HL); //premiere case de la ligne
                for (k=0; k<nb_colonnes-1; k++) {
                    printf("%s%s%s%s", MC, HL, HL, HL); //reste de la ligne
                }
                printf("%s", RC);
            }
            else{ //Derniere ligne de la grille
                printf("\n      %s%s%s%s", LB, HL, HL, HL); //premiere case de la derniere ligne
                for (k=0; k<nb_colonnes-1; k++) {
                    printf("%s%s%s%s", BC, HL, HL, HL); //reste de la derniere ligne
                }
                printf("%s", RB);
            }
        }
        printColorWhite();
        printf("\n Press A or E to rotate, move your ship with Z, Q, S and D.");
        printf("\n                 Press Enter to confirm.\n");

        touche = _getch();
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
            case 'e':
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
            case (short) 10 : //touche Entree
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
                    printColorGreen();
                    printf("\nproblem !\n");
                    printColorWhite();
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
    char** matrice_saved = (char**)creerMatrice(nb_colonnes, nb_lignes);
    for (i=0; i<nb_colonnes; i++){
        for(j=0; j<nb_lignes; j++){
            matrice_saved[i][j]=matrice[i][j];
        }
    }

    //Creation d'un curseur
    short** curseur = (short**)creerMatrice(2, 1);
    curseur[0][0] = 0;
    curseur[0][1] = 0;

    while(tir_done != 1){

        //Boucle remplaeant matrice par sa version matrice_saved
        for (i=0; i<nb_colonnes; i++){
            for(j=0; j<nb_lignes; j++){
                matrice[i][j]=matrice_saved[i][j];
            }
        }
        system("clear");

        printColorYellow();
        printf("\n     -------------- (ENNEMY'S FLEET) --------------\n\n");
        printColorWhite();
        //Label des colonnes
        printColorCyan();
        printf("     ");
        for (i=0; i<nb_colonnes; i++) {
            printf("   %c", i+65);
        }
        printColorWhite();

        //1st ligne de la grille
        printf("\n      %s%s%s%s", LT, HL, HL, HL);
        for (i=0; i<nb_colonnes-1; i++) {
            printf("%s%s%s%s", TC, HL, HL, HL);
        }
        printf("%s", RT);

        //Contenu
        for (i=0; i<nb_lignes; i++) {

            //Label des lignes
            printColorCyan();
            if(i<10) printf("\n    %d ", i);
            else printf("\n    %d", i);
            printColorWhite();

            //1st ligne de separation verticale
            printf("%s", VL);
            if (curseur[0][1]==0 && curseur[0][0]==i){
                printColorYellow();
                printf("%c", 62);
                printColorWhite();
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
                    printColorCyan();
                    printf("X");//caractere representant un tir dans l'eau
                    printColorWhite();
                    break;
                case 'C' :
                    if(votreboard) { printColorWhite(); printf("C"); printColorWhite(); }
                    else printf(" ");
                    break;
                case 'c' :
                    if(votreboard) { printColorRed(); printf("C"); printColorWhite(); }
                    else { printColorRed(); printf("X"); printColorWhite(); }
                    break;
                case 'B' :
                    if(votreboard) { printColorWhite(); printf("B"); printColorWhite(); }
                    else printf(" ");
                    break;
                case 'b' :
                    if(votreboard) { printColorRed(); printf("B"); printColorWhite(); }
                    else { printColorRed(); printf("X"); printColorWhite(); }
                    break;
                case 'S' :
                    if(votreboard) { printColorWhite(); printf("S"); printColorWhite(); }
                    else printf(" ");
                    break;
                case 's' :
                    if(votreboard) { printColorRed(); printf("S"); printColorWhite(); }
                    else { printColorRed(); printf("X"); printColorWhite(); }
                    break;
                case 'D' :
                    if(votreboard) { printColorWhite(); printf("D"); printColorWhite(); }
                    else printf(" ");
                    break;
                case 'd' :
                    if(votreboard) { printColorRed(); printf("D"); printColorWhite(); }
                    else { printColorRed(); printf("X"); printColorWhite(); }
                    break;
                default :
                    break;
                }
                printColorWhite();
                if (curseur[0][0]==i && curseur[0][1]==j){
                    printColorYellow();
                    printf("%c", 60);
                    printColorWhite();
                    printf("%s ", VL);
                }
                else if (curseur[0][0]==i && curseur[0][1]==j+1){

                    printf(" %s", VL);
                    printColorYellow();
                    printf("%c", 62);
                    printColorWhite();
                }
                else printf(" %s ", VL);
            }

            //Ligne de separation horizontale
            if(i != nb_lignes-1){
                printf("\n      %s%s%s%s", LC, HL, HL, HL); //premiere case de la ligne
                for (k=0; k<nb_colonnes-1; k++) {
                    printf("%s%s%s%s", MC, HL, HL, HL); //reste de la ligne
                }
                printf("%s", RC);
            }
            else{ //Derniere ligne de la grille
                printf("\n      %s%s%s%s", LB, HL, HL, HL); //premiere case de la derniere ligne
                for (k=0; k<nb_colonnes-1; k++) {
                    printf("%s%s%s%s", BC, HL, HL, HL); //reste de la derniere ligne
                }
                printf("%s", RB);
            }
        }
        printColorWhite();
        printf("\n         Move your reticule with Z, Q, S and D.");
        printf("\n                  Press Enter to confirm.");

        touche = _getch();
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

            case (short) 10 : //touche Entree
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
                    //printf("Vous avez deja tire ici, veuillez choisir une cible valide.");
                }
                cpt_pblm = 0;
            break;
            default :
                //printf("Key is : %d", (short)touche);
            break;
        }
    }
    return tir;
}

coordonnees_tir Aim_randomly(char** matrice, short nb_colonnes, short nb_lignes, short difficulte){
    short o = 0;
    short i = 0;
    short j = 0;
    short cpt_pblm = 0;
    short tir_done = 0;
    short sens_de_tir = 0;
    coordonnees_tir tir;
    //Creation d'un curseur
    short curseur[2];
    curseur[0] = 0;
    curseur[1] = 0;
    srand((unsigned short)time(NULL)); // initialisation de rand
    short bullseye = rand_a_b(0, difficulte);
    if(bullseye == 0){
        while (tir_done != 1){
            curseur[0] = rand()%nb_lignes;
            curseur[1] = rand()%nb_colonnes;
            if(   (matrice[curseur[0]][curseur[1]] == 'C')
                ||(matrice[curseur[0]][curseur[1]] == 'B')
                ||(matrice[curseur[0]][curseur[1]] == 'S')
                ||(matrice[curseur[0]][curseur[1]] == 'D'))
            {
                fireOnCoordonnees(curseur[0], curseur[1], matrice);
                tir.a_la_ligne = curseur[0];
                tir.a_la_colonne = curseur[1];
                tir_done = 1;
            }
            else{
                //printf("Vous avez deja tire ici, veuillez choisir une cible valide.");
            }
        }
        return tir;
    }
    else{
        while (tir_done != 1){
            curseur[0] = rand()%nb_lignes;
            curseur[1] = rand()%nb_colonnes;
            if(   (matrice[curseur[0]][curseur[1]] != 'X')
                ||(matrice[curseur[0]][curseur[1]] != 'c')
                ||(matrice[curseur[0]][curseur[1]] != 'b')
                ||(matrice[curseur[0]][curseur[1]] != 's')
                ||(matrice[curseur[0]][curseur[1]] != 'd'))
            {
                fireOnCoordonnees(curseur[0], curseur[1], matrice);
                tir.a_la_ligne = curseur[0];
                tir.a_la_colonne = curseur[1];
                tir_done = 1;
            }
            else{
                //printf("Vous avez deja tire ici, veuillez choisir une cible valide.");
            }
        }
    }
    return tir;
}

char** place_randomly(char** matrice, short nb_colonnes, short nb_lignes, short nb_ships){ //par defaut : 10

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
    short** ship = (short**)creerMatrice(2, size_ship);

    nb_ships++;

    while(cpt!=nb_ships){

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

short printBoard_af_tir(char** matrice, short nb_colonnes, short nb_lignes, coordonnees_tir dernier_tir, short votretir) //1 si c'est vous qui venez de tirer, 0 sinon
{
    short i = 0;
    short j = 0;
    short k = 0;
    char contenu_case;
    system("clear");
    printColorYellow();
    if(!votretir) printf("\n     -------- (OUR FLEET IS UNDER ATTACK !) -------\n\n");
    else          printf("\n     -------------- (ENNEMY'S FLEET) --------------\n\n");
    printColorWhite();

    //Label des colonnes
    printColorCyan();
    printf("     ");
    for (i=0; i<nb_colonnes; i++) {
        printf("   %c", i+65);
    }
    printColorWhite();

    //1st ligne de la grille
    printf("\n      %s%s%s%s", LT, HL, HL, HL);
    for (i=0; i<nb_colonnes-1; i++) {
        printf("%s%s%s%s", TC, HL, HL, HL);
    }
    printf("%s", RT);

    //Contenu
    for (i=0; i<nb_lignes; i++) {

        //Label des lignes
        printColorCyan();
        if(i<10) printf("\n    %d ", i);
        else printf("\n    %d", i);
        printColorWhite();

        //1st ligne de separation verticale
        printf("%s", VL);
        if (dernier_tir.a_la_colonne==0 && dernier_tir.a_la_ligne==i){
                printColorMagenta();
                printf("%c", 62);
                printColorWhite();
            }
            else printf(" ");

        //Switch contenu de la case
        for (j=0; j<nb_colonnes; j++){
            contenu_case = matrice[i][j];
            if (i==dernier_tir.a_la_ligne && j==dernier_tir.a_la_colonne){

            }
            switch (contenu_case){
            case 0 :
                printf(" "); //caractere representant l'eau
                break;
            case 'X' :
                printColorCyan();
                printf("X");//caractere representant un tir dans l'eau
                printColorWhite();
                break;
            case 'C' :
                if(votretir) printf(" ");
                else { printColorWhite(); printf("C"); printColorWhite(); }
                break;
            case 'c' :
                printColorGreen();
                if(!votretir){ printf("C"); }
                else printf("X");
                printColorWhite();
                break;
            case 'B' :
                if(votretir) printf(" ");
                else { printColorWhite(); printf("B"); printColorWhite(); }
                break;
            case 'b' :
                printColorGreen();
                if(!votretir){ printf("B"); }
                else printf("X");
                printColorWhite();
                break;
            case 'S' :
                if(votretir) printf(" ");
                else { printColorWhite(); printf("S"); printColorWhite(); }
                break;
            case 's' :
                printColorGreen();
                if(!votretir){ printf("S"); }
                else printf("X");
                printColorWhite();
                break;
            case 'D' :
                if(votretir) printf(" ");
                else { printColorWhite(); printf("D"); printColorWhite(); }
                break;
            case 'd' :
                printColorGreen();
                if(!votretir){ printf("D"); }
                else printf("X");
                printColorWhite();
                break;
            default :
                printf("j");
                break;
            }
            if (dernier_tir.a_la_ligne==i && dernier_tir.a_la_colonne==j){
                printColorMagenta();
                printf("%c", 60);
                printColorWhite();
                printf("%s ", VL);
            }
            else if (dernier_tir.a_la_ligne==i && dernier_tir.a_la_colonne==j+1){

                printf(" %s", VL);
                printColorMagenta();
                printf("%c", 62);
                printColorWhite();
            }
            else printf(" %s ", VL);
        }

        //Ligne de separation horizontale
        if(i != nb_lignes-1){
            printf("\n      %s%s%s%s", LC, HL, HL, HL); //premiere case de la ligne
            for (k=0; k<nb_colonnes-1; k++) {
                printf("%s%s%s%s", MC, HL, HL, HL); //reste de la ligne
            }
            printf("%s", RC);
        }
        else{ //Derniere ligne de la grille
            printf("\n      %s%s%s%s", LB, HL, HL, HL); //premiere case de la derniere ligne
            for (k=0; k<nb_colonnes-1; k++) {
                printf("%s%s%s%s", BC, HL, HL, HL); //reste de la derniere ligne
            }
            printf("%s", RB);
        }
    }
    printColorYellow();
    //printf("\n\n                  - (YOUR FLEET) -          \n\n");
    return 1;
}


void** creerMatrice(short nb_colonnes, short nb_lignes)
{
    short i=0;
    void** matrice = NULL;

    matrice = (void **)calloc(nb_lignes, sizeof(void *));
    if (matrice == NULL) {
        printColorRed();
        printf("-- Erreur lors de l'allocation de la memoire des colonnes --\n");
        printColorWhite();
        exit(0);
    }

    for (i=0; i<nb_lignes; i++) {
        matrice[i] = (void *)calloc(nb_colonnes, sizeof(void));
        if (matrice[i] == NULL) {
            printColorRed();
            printf("-- Erreur lors de l'allocation de la memoire des lignes --\n");
            printColorWhite();
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



short menuChoice2(char* choix1, char* choix2, short nb_space, char* message_quit)
{
    short size1 = strlen(choix1);
    short size2 = strlen(choix2);
    short curseur = 1;
    short i = 0;
    short valid_enter = 0;
    char touche;

    while(valid_enter==0){
        system("clear");
        if(curseur==1){
            //1st ligne
            printf("\n");
            for(i = 0; i<nb_space; i++){ printf(" "); }
            printColorYellow(); //contour en jaune
            printf(" %s%s", LT, HL);
            for(i = 0; i<size1+1; i++){
                printf("%s", HL);
            }
            printf("%s", RT);

            //2me ligne
            printf("\n ");
            for(i = 0; i<nb_space; i++){ printf(" "); }
            printf("%s ", VL);
            printColorWhite();  //texte en blanc
            printf("%s", choix1);
            printColorYellow();  //contour en jaune
            printf(" %s     ", VL);
            printColorWhite();  //texte en blanc
            printf("%s", choix2);

            //3me ligne
            printf("\n");
            for(i = 0; i<nb_space; i++){ printf(" "); }
            printColorYellow();  //contour en jaune
            printf(" %s%s", LB, HL);
            for(i = 0; i<size1+1; i++){
                printf("%s", HL);
            }
            printf("%s\n", RB);
        }
        else if(curseur==2){
            //1st ligne
            printf("\n");
            for(i = 0; i<nb_space; i++){ printf(" "); }
            printColorYellow(); //contour en jaune
            printf("   ");
            for(i = 0; i<size1+1; i++){
                printf(" ");
            }
            printf(" ");

            printf("   %s%s", LT, HL);
            for(i = 0; i<size2+1; i++){
                printf("%s", HL);
            }
            printf("%s", RT);

            //2me ligne
            printf("\n ");
            for(i = 0; i<nb_space; i++){ printf(" "); }
            printf("  ");
            printColorWhite();  //texte en blanc
            printf("%s", choix1);
            printColorYellow();  //contour en jaune
            printf("     %s ", VL);
            printColorWhite();  //texte en blanc
            printf("%s", choix2);
            printColorYellow();  //contour en jaune
            printf(" %s", VL);

            //3me ligne
            printf("\n");
            for(i = 0; i<nb_space; i++){ printf(" "); }
            printColorYellow();  //contour en jaune
            printf("   ");
            for(i = 0; i<size1+1; i++){
                printf(" ");
            }
            printf(" ");

            printf("   %s%s", LB, HL);
            for(i = 0; i<size2+1; i++){
                printf("%s", HL);
            }
            printf("%s\n", RB);
        }
        printColorWhite();
        printf("      Please, select your choice. Press Enter to confirm.\n");
        printf("     %s", message_quit);
        touche = _getch();
        switch(touche) {
        case 'q':
            if(curseur != 1){
                curseur--;
            }
        break;
        case 'd':
            if(curseur != 2){
                curseur++;
            }
        break;
        case (short) 10: //touche Entree
            valid_enter = curseur;
        break;
        case 27:
            valid_enter = -1;
        break;
        default:
        break;
        }
    }
    return valid_enter;
}


short menuChoice3(char* choix1, char* choix2, char* choix3, short nb_space, char* message_quit)
{
    short size1 = strlen(choix1);
    short size2 = strlen(choix2);
    short size3 = strlen(choix3);
    short curseur = 1;
    short i = 0;
    short valid_enter = 0;
    char touche;

    while(valid_enter==0){
        system("clear");
        if(curseur==1){
            //1st ligne
            printf("\n");
            for(i = 0; i<nb_space; i++){ printf(" "); }
            printColorYellow(); //contour en jaune
            printf(" %s%s", LT, HL);
            for(i = 0; i<size1+1; i++){
                printf("%s", HL);
            }
            printf("%s", RT);

            //2me ligne
            printf("\n ");
            for(i = 0; i<nb_space; i++){ printf(" "); }
            printf("%s ", VL);
            printColorWhite();  //texte en blanc
            printf("%s", choix1);
            printColorYellow();  //contour en jaune
            printf(" %s     ", VL);
            printColorWhite();  //texte en blanc
            printf("%s", choix2);
            printf("       ");
            printColorWhite();  //texte en blanc
            printf("%s", choix3);


            //3me ligne
            printf("\n");
            for(i = 0; i<nb_space; i++){ printf(" "); }
            printColorYellow();  //contour en jaune
            printf(" %s%s", LB, HL);
            for(i = 0; i<size1+1; i++){
                printf("%s", HL);
            }
            printf("%s\n", RB);
        }
        else if(curseur==2){
            //1st ligne
            printf("\n");
            for(i = 0; i<nb_space; i++){ printf(" "); }
            printColorYellow(); //contour en jaune
            printf("   ");
            for(i = 0; i<size1+1; i++){
                printf(" ");
            }
            printf(" ");

            printf("   %s%s", LT, HL);
            for(i = 0; i<size2+1; i++){
                printf("%s", HL);
            }
            printf("%s", RT);

            //2me ligne
            printf("\n ");
            for(i = 0; i<nb_space; i++){ printf(" "); }
            printf("  ");
            printColorWhite();  //texte en blanc
            printf("%s", choix1);
            printColorYellow();  //contour en jaune
            printf("     %s ", VL);
            printColorWhite();  //texte en blanc
            printf("%s", choix2);
            printColorYellow();  //contour en jaune
            printf(" %s", VL);
            printColorWhite();  //texte en blanc
            printf("     ");
            printf("%s", choix3);


            //3me ligne
            printf("\n");
            for(i = 0; i<nb_space; i++){ printf(" "); }
            printColorYellow();  //contour en jaune
            printf("   ");
            for(i = 0; i<size1+1; i++){
                printf(" ");
            }
            printf(" ");

            printf("   %s%s", LB, HL);
            for(i = 0; i<size2+1; i++){
                printf("%s", HL);
            }
            printf("%s\n", RB);
        }
        else if(curseur==3){
            //1st ligne
            printf("\n");
            for(i = 0; i<nb_space; i++){ printf(" "); }
            printColorYellow(); //contour en jaune
            printf("   ");
            for(i = 0; i<size1+1; i++){
                printf(" ");
            }
            printf(" ");
            printf("   ");
            for(i = 0; i<size2+1; i++){
                printf(" ");
            }
            printf("   ");
            printf("   %s%s", LT, HL);
            for(i = 0; i<size3+1; i++){
                printf("%s", HL);
            }
            printf("%s", RT);

            //2me ligne
            printf("\n ");
            for(i = 0; i<nb_space; i++){ printf(" "); }
            printf("  ");
            printColorWhite();  //texte en blanc
            printf("%s", choix1);
            printf("  ");
            printf("     ");
            printColorWhite();  //texte en blanc
            printf("%s", choix2);
            printf("    ");
            printColorYellow();  //contour en jaune
            printf(" %s ", VL);
            printColorWhite();  //texte en blanc
            printf("%s", choix3);
            printColorYellow();  //contour en jaune
            printf(" %s", VL);


            //3me ligne
            printf("\n");
            for(i = 0; i<nb_space; i++){ printf(" "); }
            printColorYellow();  //contour en jaune
            printf("   ");
            for(i = 0; i<size1+1; i++){
                printf(" ");
            }
            printf(" ");
            printf("   ");
            for(i = 0; i<size2+1; i++){
                printf(" ");
            }
            printf("   ");
            printf("   %s%s", LB, HL);
            for(i = 0; i<size3+1; i++){
                printf("%s", HL);
            }
            printf("%s\n", RB);
        }
        printColorWhite();
        printf("    Please, select your difficulty. Press Enter to confirm.\n");
        printf("     %s", message_quit);
        touche = _getch();
        switch(touche) {
        case 'q':
            if(curseur != 1){
                curseur--;
            }
        break;
        case 'd':
            if(curseur != 3){
                curseur++;
            }
        break;
        case (short) 10: //touche Entree
            valid_enter = curseur;
        break;
        case 27:
            valid_enter = -1;
        break;
        default:
        break;
        }
    }
    return valid_enter;
}

// On suppose a<b
short rand_a_b(short a, short b){
    return (short) rand()%(b-a) +a;
}
