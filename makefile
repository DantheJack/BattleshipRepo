SRCS = boardtools.c main.c printtools.c

all: $(SRCS)
	gcc -o Battleship $(SRCS) -I.