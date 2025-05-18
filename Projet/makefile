CC=gcc
CFLAGS = -Wextra -Wall -g
LDFLAGS= -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
EXEC = exec

all: $(EXEC)
exec : Entite.o Carte.o Manager.o Affichage.o Apprentissage.o Main.o 
	$(CC) Entite.o Carte.o Manager.o Affichage.o Apprentissage.o Main.o -o exec $(CFLAGS) $(LDFLAGS)

Entite.o : entite.c entite.h
	$(CC) -c entite.c -o Entite.o $(CFLAGS)

Manager.o : manager.c manager.h carte.h affichage.h entite.h
	$(CC) -c manager.c  -o Manager.o $(CFLAGS)

Carte.o : carte.c carte.h
	$(CC) -c carte.c  -o Carte.o $(CFLAGS)

Affichage.o : affichage.c affichage.h carte.h entite.h
	$(CC) -c affichage.c -o Affichage.o $(CFLAGS)

Apprentissage.o : apprentissage.c apprentissage.h
	$(CC) -c apprentissage.c  -o Apprentissage.o $(CFLAGS)

Main.o: main.c manager.h apprentissage.h
	$(CC) -c main.c -o Main.o $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf Sprite
