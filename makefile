all: boardtools.c main.c
	gcc -o Battleship boardtools.c main.c -I.

