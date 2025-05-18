all: serpents clean

serpents: serpents.c
	gcc serpents.c -o exec -Wall -Wextra -lSDL2 -lm -g
	./exec

clean:
	rm -rf *.o

mrproper: clean
	rm -rf serpents
