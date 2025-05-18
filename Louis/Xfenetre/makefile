all: xfenetre clean

xfenetre: xfenetre.c
	gcc xfenetre.c -o exec -Wall -Wextra -lSDL2
	./exec

clean:
	rm -rf *.o

mrproper: clean
	rm -rf xfenetre