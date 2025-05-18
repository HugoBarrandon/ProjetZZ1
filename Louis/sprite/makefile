all: sprite clean

sprite: sprite.c 
	gcc sprite.c -o exec -Wall -Wextra -lSDL2 -lSDL2_image -lm
	./exec

clean:
	rm -rf *.o

mrproper: clean
	rm -rf sprite