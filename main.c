#include <stdio.h>
#include <stdlib.h>
#include "printtools.h"
#include <time.h>
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

    char** my_matrice = creerMatrice(COLONNES, LIGNES);
    /*my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 2);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 2);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 2);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 2);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 3);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 3);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 3);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 4);
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 4);*/
    my_matrice = placeShip(my_matrice, COLONNES, LIGNES, 5);

    char** his_matrice = creerMatrice(COLONNES, LIGNES);


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




