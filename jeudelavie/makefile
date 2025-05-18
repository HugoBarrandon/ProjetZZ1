all: jeudelavie clear

jeudelavie: jeudelavie.c 
	gcc jeudelavie.c -o exec -Wall -Wextra -lSDL2 
	./exec -t


clear:
	rm -rf *.o

mrproper: clean
	rm -rf jeudelavie

