all: x_fenetre clean

x_fenetre: x_fenetre.c
	gcc x_fenetre.c -o exec -Wall -Wextra -lSDL2
	./exec

clean:
	rm -rf *.o

mrproper: clean
	rm -rf x_fenetre