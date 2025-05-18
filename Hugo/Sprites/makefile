all: Exec

Exec : Player.o Sprite.o
	gcc -o exec Player.o Sprite.o -Wall -Wextra -lSDL2 -lSDL2_image -lm
	./exec

Player.o : Player.c
	gcc -o Player.o -c Player.c -Wextra -Wall -lSDL2 -lSDL2_image

Sprite.o: Sprite.c Player.h
	gcc -c Sprite.c -o Sprite.o -Wall -Wextra -lSDL2 -lSDL2_image -lm


clean:
	rm -rf *.o

mrproper: clean
	rm -rf Sprite
