#ifndef f_afficherGrille
#define f_afficherGrille

#include "grille.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

#define MAX 10

void dessin(SDL_Texture * texture1,SDL_Texture * texture2,SDL_Renderer * renderer, SDL_Window * window, grille_t * grille,int h, int marge);
void play_with_texture_2(SDL_Texture * texture, SDL_Window * window, SDL_Renderer * renderer, SDL_Rect rectangle);
SDL_Texture * load_texture_from_image(char  *  file_image_name, SDL_Window *window, SDL_Renderer *renderer,SDL_Texture * texture);
void end_sdl_affichage(SDL_Window *window, SDL_Renderer *renderer,SDL_Texture * texture);


#endif