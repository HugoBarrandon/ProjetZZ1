all: serpents clean

serpents: serpent.c
	gcc serpent.c -o exec -Wall -Wextra -lSDL2 -lm
	./exec

clean:
	rm -rf *.o

mrproper: clean
	rm -rf serpents