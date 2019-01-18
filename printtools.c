#include "printtools.h"

//Linux color
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define ROW 10
#define COL 10

void printColorYellow(void)
{
  printf(KYEL);
}

void printColorCyan(void)
{
  printf(KCYN);
}

void printColorRed(void)
{
  printf(KRED);
}

void printColorWhite(void)
{
  printf(KWHT);
}

void printColorGreen(void)
{
  printf(KGRN);
}

void printColorMagenta(void)
{
  printf(KMAG);
}

void printColorBlue(void)
{
  printf(KBLU);
}


void displayMatrice(char** matrice)
{
	//Display matrice
	printf("display matrice\n");
	int i, j;
	for (i = 0; i<ROW; i++)
	{
		for (j = 0; j<COL; j++)
		{
			printf("%c ", matrice[i][j]);
		}
		printf("\n");
	}
}
