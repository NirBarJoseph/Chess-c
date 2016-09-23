CFLAGS = -lm -std=c99 -pedantic-errors -Wall -g
CC = gcc

all: Chessprog

clean:
	-rm GUI.o Util.o Minimax.o Logic.o Flow.o Main.o chessprog

Chessprog: Chess.h Logic.o Minimax.o Util.o Flow.o Main.o GUI.o
	gcc  -o chessprog GUI.o Main.o Flow.o Logic.o Minimax.o Util.o $(CFLAGS) `sdl-config --libs` 

Main.o: Chess.h GUI.c Flow.c Logic.c Main.c
	gcc  -c Main.c $(CFLAGS)

Flow.o: Chess.h Logic.c Flow.c
	gcc  -c Flow.c $(CFLAGS)

Logic.o: Chess.h Util.c Logic.c
	gcc  -c Logic.c $(CFLAGS)
	
Minimax.o: Chess.h Util.c Minimax.c
	gcc  -c Minimax.c $(CFLAGS)
	
Util.o: Chess.h Util.c
	gcc  -c Util.c $(CFLAGS)
	
GUI.o: Chess.h GUI.c
	gcc  -c  GUI.c $(CFLAGS) `sdl-config --cflags`
	