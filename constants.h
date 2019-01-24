#ifndef CONSTANT_H
#define CONSTANT_H


//Game config
#define PORT 8080
#define ROW 10
#define COL 10
#define NB_SHIPS 10
#define SIZE_MAN_O_WAR 5
#define SIZE_NAVIRE_DE_LIGNE 4
#define SIZE_FREGATE 3
#define SIZE_SLOOP 4

//Macros
#define DEBUG 1
#define debug(format, ...) if (DEBUG) fprintf (stdout, format, ##__VA_ARGS__)


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


#endif //CONSTANT_H
