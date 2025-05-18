all: serpent clean


serpent: serpent.c
	gcc serpent.c -o exec -Wall -Wextra -lSDL2 -lm



clean:
	rm -rf *.o

mrproper:
	rm -rf serpent