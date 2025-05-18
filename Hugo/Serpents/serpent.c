#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define Radius 100

/*Fonction qui dessine le cercle sur le rendu*/

void draw(SDL_Renderer* renderer, SDL_Point centre) {
  int i;

  for(i=Radius;i<= Radius; i+=2)
  {
    for (float angle = 0; angle < 2 * M_PI; angle += M_PI / 4000) {
      SDL_SetRenderDrawColor(renderer,0, 0, 255, 255); 

      SDL_RenderDrawPoint(renderer,                                   
                        centre.x + i * cos(angle),                     // coordonnée en x
                        centre.y + i * sin(angle));                    //            en y   
    }
  }
    
}

/*Fonction qui met à jour la position du cercle*/
/*centre : centre du cercle
direction: direction dans laquelle avance le cercle
velocity : vitesse du deplacement
bordure : point en bas à droite de l'ecran*/
void update(SDL_Point* centre, SDL_Point* direction, int velocity, SDL_Point bordure)
{
  centre->x += direction->x * velocity;
  centre->y += direction->y * velocity;

  if(centre->x <Radius || centre->x>bordure.x - Radius)
  {
    direction->x=-direction->x;
  }
  if(centre->y <Radius || centre->y>bordure.y - Radius)
  {
    direction->y=-direction->y;
  }
}

/*Dessine un cercle qui rebondi sur les bords*/
int main()
{
  SDL_Window* window;
  SDL_Renderer* renderer = NULL;

  SDL_Point centre_cercle = {200,200};
  SDL_Point direction = {-1,1};
  SDL_Point bordure;

  SDL_bool program_on = SDL_TRUE;
  SDL_Event event; 

  int velocity = 10;

  SDL_DisplayMode screen;

  /* Initialisation de la SDL  + gestion de l'échec possible */
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
  }

  /*Creation de la fenetre et du rendu*/
  SDL_GetCurrentDisplayMode(0, &screen);
  bordure.x = screen.w;
  bordure.y = screen.h;
  window = SDL_CreateWindow("Premier dessin",
                          SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED,
                          screen.w ,
                          screen.h ,
                          SDL_WINDOW_OPENGL);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  /*Boucle de jeu*/
  while(program_on)
  {
    while(SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:                                // Un évènement simple, on a cliqué sur la x de la // fenêtre
          program_on = SDL_FALSE;                     // Il est temps d'arrêter le programme
          break;
      }
    }
    update(&centre_cercle,&direction,velocity,bordure);
    draw(renderer,centre_cercle);
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
  }

  /*Liberation*/
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}