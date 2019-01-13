SRCS = boardtools.c main.c

# lncurses is the library for curses.h
LIBS = -lncurses

all: $(SRCS)
	gcc -o Battleship $(SRCS) $(LIBS) -I.