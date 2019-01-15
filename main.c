#include <stdio.h>
#include <stdlib.h>
#include "printtools.h"
#include "boardtools.h"
#include "coordonnees.h"
#include <windows.h>
#define COLONNES 11
#define LIGNES 11
#define SIZE_MAN_O_WAR 5
#define SIZE_NAVIRE_DE_LIGNE 4
#define SIZE_FREGATE 3
#define SIZE_SLOOP 4



int main()
{
    system("mode con lines=70 cols=70");
    short we_got_winner = 0;
    coordonnees_tir tir;
    short i, j, nb_boat_left = 0;


    /***** ETAPE 1 : CREATION DES BOARDS **********************************/

    char** matrice_J1 = (char**)creerMatrice(COLONNES, LIGNES);
    char** matrice_J2 = (char**)creerMatrice(COLONNES, LIGNES);

    /***** ETAPE 2 : DEMANDER A JOUEUR 1 DE PLACER SA FLOTTE **************/
    system("cls");    printCOM("\n\n      VEUILLEZ PASSEZ L'ORDINATEUR A JOUEUR 1\n");    getchar();
    /*matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 3);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 3);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 3);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 4);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 4);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 5);*/
    place_randomly(matrice_J1, COLONNES, LIGNES);

    /***** ETAPE 3 : DEMANDER A JOUEUR 2 DE PLACER SA FLOTTE **************/
    system("cls");    printCOM("\n\n      VEUILLEZ PASSEZ L'ORDINATEUR A JOUEUR 2\n");    getchar();
    /*matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 2);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 2);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 2);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 2);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 3);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 3);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 3);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 4);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 4);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 5);*/
    place_randomly(matrice_J2, COLONNES, LIGNES);

    /***** ETAPE 4 : DONNER LE BOARD DE JOUEUR 2 A JOUEUR 1 ***************/

    //ici, évidemment, on les a dejà, puisqu'on est en local
    printBoard(matrice_J1, COLONNES, LIGNES, 1);
    getch();

    /***** ETAPE 5 : DONNER LE BOARD DE JOUEUR 1 A JOUEUR 2 **************/

    //ici, évidemment, on les a dejà, puisqu'on est en local

    while(we_got_winner == 0){

        /***** ETAPE 6 : DEMANDER A JOUEUR 1 DE TIRER SUR JOUEUR 2 ************/
        system("cls");    printCOM("\n\n      VEUILLEZ PASSEZ L'ORDINATEUR A JOUEUR 1\n");    getchar();
        tir = Aim(matrice_J2, COLONNES, LIGNES);
        printBoard_af_tir(matrice_J2, COLONNES, LIGNES, tir, 1);
        getch();

        /***** ETAPE 7 : TRANSMETTRE LE TIR A JOUEUR 2 ************************/

        //fireOnCoordonnees(tir.a_la_ligne, tir.a_la_colonne, matrice_J2);
        //encore une fois, c'est pour de faux. On a les deux matrices en local
        //donc quand joueur 1 tire sur le board de joueur 2, on l'a déja marqué.

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
        system("cls");    printCOM("\n\n      VEUILLEZ PASSEZ L'ORDINATEUR A JOUEUR 2\n");    getchar();

        /***** ETAPE 8 : DEMANDER A JOUEUR 2 DE TIRER SUR JOUEUR 1 ************/

        //tir = Aim(matrice_J1, COLONNES, LIGNES);
        tir = Aim_randomly(matrice_J1, COLONNES, LIGNES);
        printBoard_af_tir(matrice_J1, COLONNES, LIGNES, tir, 0);
        getch();

        /***** ETAPE 9 : TRANSMETTRE LE TIR A JOUEUR 1 ************************/

        //fireOnCoordonnees(tir.a_la_ligne, tir.a_la_colonne, matrice_J1);
        //encore une fois, c'est pour de faux. On a les deux matrices en local
        //donc quand joueur 2 tire sur le board de joueur 1, on l'a déja marqué.

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
    printf("\n\nWE GOT A WINNER ! CONGRATULATIONS, PLAYER %hd !", we_got_winner);
    getchar();

    libererMatrice(matrice_J1, LIGNES);
    libererMatrice(matrice_J2, LIGNES);

    return 0;

}




/** FONCTIONS A DEVELOPPER EN RELATION AVEC LE SERVEUR ET LA **/
/**            COMMUNICATION ENTRE LES JOUEURS               **/

/* request board d'un joueur */
// | Quand un joueur reçoit cette requete, il va créer
// | sa matrice de board et la remplir (ship par ship)
// | puis, une fois le board operationnel, il va l'envoyer
// | au serveur.

/* transmission board d'un joueur */
// | Quand le serveur aura demande aux deux joueurs de créer
// | leurs boards respectifs et de les lui envoyer, il va les
// | transmettre à chaque joueur pour que tout le monde ait les
// | mêmes infos. Les joueurs devront donc enregistrer aussi la
// | matrice de l'advesaire.

/* request coordonnées d'un tir */
// | Quand un joueur reçoit cette requete, il va définir
// | son prochain coup et envoyer les coordonnées de son
// | tir au serveur. Il va aussi modifier la matrice de
// | adversaire en conséquence dans sa mémoire.

/* transmission coordonnées du tir */
// | Le serveur, après avoir demandé un tir à J1, va
// | transmettre les coordonnées du tir à J2, afin que
// | celui-ci modifie son board en conséquence.


/**                    INUTILE DE CODER CA                    **/

/* request nombre de cases Majuscule restante */
// | Le serveur demande au joueur sur qui on vient de
// | tirer combien de case Majuscule (donc de morceaux
// | de bateau intacts) il lui reste sur son board. Si
// | la réponse est zéro, alors la partie prend fin.
// | ------------------------------------------------
// | Inutile, les deux joueurs peuvent le savoir direct
// | puisqu'ils ont toutes les données en main.




