#include <stdio.h>
#include <stdlib.h>
#include "boardtools.h"
#include "coordonnees.h"
#define COLONNES 11
#define LIGNES 11
#define SIZE_MAN_O_WAR 5
#define SIZE_NAVIRE_DE_LIGNE 4
#define SIZE_FREGATE 3
#define SIZE_SLOOP 4



int main()
{
    short we_got_winner = 0;
    coordonnees_tir tir;
    short i, j, nb_boat_left = 0;


    /***** ETAPE 1 : CREATION DES BOARDS **********************************/

    char** matrice_J1 = (char**) creerMatrice(COLONNES, LIGNES);
    char** matrice_J2 = (char**) creerMatrice(COLONNES, LIGNES);

    /***** ETAPE 2 : DEMANDER A JOUEUR 1 DE PLACER SA FLOTTE **************/
    system("clear");    printf("\n\n      VEUILLEZ PASSEZ L'ORDINATEUR A JOUEUR 1\n");    getchar();
    /*matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 2);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 3);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 3);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 3);*/

    printf("going to place ship\n");
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 4);
    printf("shipPlaced\n");
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 4);
    matrice_J1 = placeShip(matrice_J1, COLONNES, LIGNES, 5);

    /***** ETAPE 3 : DEMANDER A JOUEUR 2 DE PLACER SA FLOTTE **************/
    system("clear");    printf("\n\n      VEUILLEZ PASSEZ L'ORDINATEUR A JOUEUR 2\n");    getchar();
    /*matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 2);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 2);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 2);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 2);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 3);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 3);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 3);*/
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 4);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 4);
    matrice_J2 = placeShip(matrice_J2, COLONNES, LIGNES, 5);

    /***** ETAPE 4 : DONNER LE BOARD DE JOUEUR 2 A JOUEUR 1 ***************/

    //ici, evidemment, on les a deja, puisqu'on est en local

    /***** ETAPE 5 : DONNER LE BOARD DE JOUEUR 1 A JOUEUR 2 **************/

    //ici, evidemment, on les a deja, puisqu'on est en local

    while(we_got_winner == 0){

    /***** ETAPE 6 : DEMANDER A JOUEUR 1 DE TIRER SUR JOUEUR 2 ************/
    system("clear");    printf("\n\n      VEUILLEZ PASSEZ L'ORDINATEUR A JOUEUR 1\n");    getchar();
    tir = Aim(matrice_J2, COLONNES, LIGNES);

    /***** ETAPE 7 : TRANSMETTRE LE TIR A JOUEUR 2 ************************/

    //fireOnCoordonnees(tir.a_la_ligne, tir.a_la_colonne, matrice_J2);
    //encore une fois, c'est pour de faux. On a les deux matrices en local
    //donc quand joueur 1 tire sur le board de joueur 2, on l'a deja marque.

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
    system("clear");    printf("\n\n      VEUILLEZ PASSEZ L'ORDINATEUR A JOUEUR 2\n");    getchar();

    /***** ETAPE 8 : DEMANDER A JOUEUR 2 DE TIRER SUR JOUEUR 1 ************/

    tir = Aim(matrice_J1, COLONNES, LIGNES);

    /***** ETAPE 9 : TRANSMETTRE LE TIR A JOUEUR 1 ************************/

    //fireOnCoordonnees(tir.a_la_ligne, tir.a_la_colonne, matrice_J1);
    //encore une fois, c'est pour de faux. On a les deux matrices en local
    //donc quand joueur 2 tire sur le board de joueur 1, on l'a deja marque.

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
    system("clear");    printf("\n\n      VEUILLEZ PASSEZ L'ORDINATEUR A JOUEUR 1\n");    getchar();

    }
    printf("\n\nWE GOT A WINNER ! CONGRATULATIONS, PLAYER %hd !", we_got_winner);
    getchar();

    libererMatrice(matrice_J1, LIGNES);
    libererMatrice(matrice_J2, LIGNES);

    /*
    char** my_matrice = (char**) creerMatrice(COLONNES, LIGNES);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 2);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 2);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 2);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 2);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 3);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 3);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 3);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 4);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 4);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 5);

    char** his_matrice = (char**) creerMatrice(COLONNES, LIGNES);


    char signe = 0;
    short i = 0;
    short j = 0;
    for(i = 0; i<COLONNES; i++){
        for(j = 0; j<LIGNES; j++){
            //printf("%c ", his_matrice[i][j]);
            his_matrice[i][j]=signe;
        }
    }

    his_matrice = place_randomly(his_matrice, COLONNES, LIGNES);


    printBoard(his_matrice, COLONNES, LIGNES, 1);

    fireOnCoordonnees(3, 4, his_matrice);
    fireOnCoordonnees(2, 5, his_matrice);
    fireOnCoordonnees(7, 8, his_matrice);
    fireOnCoordonnees(1, 1, his_matrice);
    fireOnCoordonnees(8, 8, his_matrice);
    fireOnCoordonnees(6, 1, his_matrice);
    fireOnCoordonnees(3, 8, his_matrice);
    fireOnCoordonnees(7, 1, his_matrice);
    fireOnCoordonnees(1, 5, his_matrice);
    fireOnCoordonnees(6, 6, his_matrice);

    Aim(his_matrice, COLONNES, LIGNES).a_la_ligne;
    Aim(his_matrice, COLONNES, LIGNES).a_la_ligne;
    Aim(his_matrice, COLONNES, LIGNES).a_la_ligne;

    printBoard(his_matrice, COLONNES, LIGNES, 0);

    getchar();


//    his_matrice = place_a_ship_here(his_matrice, COLONNES, LIGNES, SIZE_FREGATE, 3, 4, 'v');
//    his_matrice = place_a_ship_here(his_matrice, COLONNES, LIGNES, SIZE_MAN_O_WAR, 4, 2, 'h');
//    printBoard(his_matrice, COLONNES, LIGNES, 1);

    libererMatrice(my_matrice, LIGNES);
    libererMatrice(his_matrice, LIGNES);
    */

    return 0;

}




/** FONCTIONS A DEVELOPPER EN RELATION AVEC LE SERVEUR ET LA **/
/**            COMMUNICATION ENTRE LES JOUEURS               **/

/* request board d'un joueur */
// | Quand un joueur recoit cette requete, il va creer
// | sa matrice de board et la remplir (ship par ship)
// | puis, une fois le board operationnel, il va l'envoyer
// | au serveur.

/* transmission board d'un joueur */
// | Quand le serveur aura demande aux deux joueurs de creer
// | leurs boards respectifs et de les lui envoyer, il va les
// | transmettre a chaque joueur pour que tout le monde ait les
// | memes infos. Les joueurs devront donc enregistrer aussi la
// | matrice de l'advesaire.

/* request coordonnees d'un tir */
// | Quand un joueur recoit cette requete, il va definir
// | son prochain coup et envoyer les coordonnees de son
// | tir au serveur. Il va aussi modifier la matrice de
// | adversaire en consequence dans sa memoire.

/* transmission coordonnees du tir */
// | Le serveur, apres avoir demande un tir a J1, va
// | transmettre les coordonnees du tir a J2, afin que
// | celui-ci modifie son board en consequence.


/**                    INUTILE DE CODER CA                    **/

/* request nombre de cases Majuscule restante */
// | Le serveur demande au joueur sur qui on vient de
// | tirer combien de case Majuscule (donc de morceaux
// | de bateau intacts) il lui reste sur son board. Si
// | la reponse est zero, alors la partie prend fin.
// | ------------------------------------------------
// | Inutile, les deux joueurs peuvent le savoir direct
// | puisqu'ils ont toutes les donnees en main.
