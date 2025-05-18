#include <SDL/SDL.h>
#include <stdio.h>


/*verif instalation sdl*/ 

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  SDL_version nb;
  SDL_VERSION(&nb);

  printf("Version de la SDL : %d.%d.%d\n", nb.major, nb.minor, nb.patch);
  return 0;
}
