#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>   




void play_with_texture_1(SDL_Texture *my_texture, SDL_Window *window,SDL_Renderer *renderer);

void explosion(SDL_Texture* my_texture,SDL_Window* window,SDL_Renderer* renderer, int etat,int posx);

SDL_Rect voiture(SDL_Texture* my_texture,SDL_Window* window,SDL_Renderer* renderer,int posx,int direction);


void gameover(SDL_Renderer *renderer, SDL_Window * window,int h);

void score(SDL_Renderer *randerer, SDL_Window * window,int scoreint);

void end_sdl(int code, char* message,SDL_Window *window, SDL_Renderer * renderer);

void score2(SDL_Renderer *renderer, SDL_Window * window);