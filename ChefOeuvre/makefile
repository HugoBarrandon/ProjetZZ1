all: Exec clean

Exec : AfficherGrille.o Grille.o Markov.o Voiture.o Main.o
	gcc Grille.o Markov.o Voiture.o AfficherGrille.o Main.o -o exec -Wall -Wextra -lSDL2 -lSDL2_image -lSDL2_ttf -lm
	./exec

Grille.o : grille.c
	gcc -c grille.c  -o Grille.o -Wextra -Wall -lSDL2 -lm

Markov.o : markov.c
	gcc -c markov.c  -o Markov.o -Wextra -Wall

Voiture.o : voiture.c
	gcc -c voiture.c -o Voiture.o -Wextra -Wall -lSDL2 -lSDL2_ttf -lSDL2_image

AfficherGrille.o : afficherGrille.c
	gcc -c afficherGrille.c -o AfficherGrille.o -Wextra -Wall -lSDL2 -lSDL2_image

Main.o: main.c grille.h
	gcc -c main.c -o Main.o -Wall -Wextra -lSDL2 -lSDL2_image


clean:
	rm -rf *.o

mrproper: clean
	rm -rf Sprite
