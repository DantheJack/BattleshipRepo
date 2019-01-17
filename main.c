#include <stdio.h>
#include <stdlib.h>
#include "boardtools.h"
#include "printtools.h"
#include "coordonnees.h"
#define COLONNES 10
#define LIGNES 10
#define NB_SHIPS 10
#define SIZE_MAN_O_WAR 5
#define SIZE_NAVIRE_DE_LIGNE 4
#define SIZE_FREGATE 3
#define SIZE_SLOOP 4


int main()
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
                    switch(hostornot){
                        case 1:
                            /*******/
                            /**
                            system("clear");
                            printf("\n\n\n              The game is about to start...\n\n\n");
                            _getch();
                            char** matrice_J1 = (char**)creerMatrice(COLONNES, LIGNES);
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

                                printColorYellow();
                                printf("\n\n               IT'S YOUR TURN TO SHOOT !\n");
                                printColorWhite();
                                _getch();
                                tir = Aim(matrice_J2, COLONNES, LIGNES);

                                //******* TRANSMETTRE LE TIR A J2

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

                                system("clear");
                                printColorYellow();
                                printf("\n\n         TAKE COVER, THEY ARE SHOOTING AT YOU !\n");
                                printColorWhite();
                                _getch();

                                //******* RECEPTION DU TIR DE J2

                                printBoard_af_tir(matrice_J1, COLONNES, LIGNES, tir, 0);
                                _getch();
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
                        case 2:
                            /*******/
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
    return 0;
}
