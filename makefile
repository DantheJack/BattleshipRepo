SRCS = main.c server.c client.c boardtools.c printtools.c communication.c

LIBS = -pthread

all:  $(SRCS)
	gcc -o BattleShip $(SRCS) $(LIBS)
