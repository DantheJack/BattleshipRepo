#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void color(short t,short f)
{
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, f*16+t);
}

void printCOM(char* s)
{
    color(14, 0);
    printf(s);
    color(15, 0);
}

void printRES(char* s)
{
    color(10, 0);
    printf(s);
    color(15, 0);
}

void printERROR(char* s)
{
    color(12, 0);
    printf(s);
    color(15, 0);
}

void printNUM(short s, short codecouleur)
{
    color(codecouleur, 0);
    printf("%d", s);
    color(15, 0);
}

void coolprint(char* ligne, short nbiterations, short vitesseaffichagechar)
{
    //printf("\n%s  taille : %d", ligne, strlen(ligne));
   //static char const ligne[sizeof tab];
   //strncpy(ligne, tab, sizeof tab);

   /* a ajuster selon le systeme */
   short tempsadditionnel = vitesseaffichagechar;

   short length = strlen(ligne); //length = taille ligne - 1

   //printf("\nici : %d\n", length);
   clock_t duedate = clock() + tempsadditionnel; //clock_t duedate : le temps que la clock doit dépasser avant de lancer une nouvelle itération
   short mode = 0;
   short cptlength = 0;
   printf("\n");
   short i = 0;

   while(i < 5){
      if (clock() >= duedate){                  //clock  parcouru le temps tempsadditionnel
         duedate = clock() + tempsadditionnel;  //on lui rajoute tempsadditionnel
         if ( !mode){
            if (cptlength < length){            //tant qu'il reste des morceaux à ecrire
               printf("%c", ligne[cptlength]);
               cptlength++;
            }
            else{
               printf("\r\t\t");                //revenir au debut de la ligne
               mode = 1;                        //on est en mode effaceur
               cptlength = 0;
            }
         }
         else{
            if (cptlength < length){            //tant qu'il reste des morceaux à effacer
               printf(" ");
               cptlength++;
            }
            else{
               printf("\r");
               mode = 0;                        //on est en mode ecriture
               cptlength = 0;
               i++;
            }
         }
         fflush(stdout);
      }
   }
}


/**
0 : Noir
1 : Bleu foncé
2 : Vert foncé
3 : Turquoise
4 : Rouge foncé
5 : Violet
6 : Vert caca d'oie
7 : Gris clair
8 : Gris foncé
9 : Bleu fluo
10 : Vert fluo
11 : Turquoise
12 : Rouge fluo
13 : Violet 2
14 : Jaune
15 : Blanc
**/
