#include <SDL2/SDL.h>
#include <stdio.h>

#define MAX 10

/*Fonction qui créé N fenetres en x et les déplace dans les coins*/

int main() {

  int i=0, screen_width,screen_height;
  float j =0;
  SDL_Window* windows[MAX];
  SDL_DisplayMode DM;
  int pas_x;
  int pas_y;
  int pos_x,pos_y;
  int marge;
  float scale = 0.2;
  int window_w, window_h;

  /* Initialisation de la SDL  + gestion de l'échec possible */
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  /*Recupération de la taille de l'écran*/
  SDL_GetCurrentDisplayMode(0, &DM);

  screen_width = DM.w;
  screen_height = DM.h;


  /*définition ecart entre fenetre*/
  pas_x = (screen_width-20)/(MAX/2);
  pas_y = (screen_height-20)/(MAX/2);


  /*Creation des fenetres*/
  for(i=0;i<MAX/2;i++)
  {
    /*première diagonale*/
    windows[i] = SDL_CreateWindow(
                  "",
                  i*pas_x, i*pas_y,                                  
                  200, 200,                             
                  SDL_WINDOW_RESIZABLE);
    
    /*2nde diagonale*/
    windows[i+MAX/2] = SDL_CreateWindow(
                  "",
                  screen_width-200-(i*pas_x), i*pas_y,                                  
                  200, 200,                             
                  SDL_WINDOW_RESIZABLE);
    
  }

  marge=0;
  /*déplacement des fenetres*/
  while(marge<(screen_width-200))
  {
    for(i=0;i<MAX/2;i++)
    {
      /*1ère diag va en bas à droite*/
      SDL_GetWindowPosition(windows[i],&pos_x,&pos_y);
      SDL_SetWindowPosition(windows[i],pos_x+pas_x*scale, pos_y+pas_y*scale);

      /*2nd diag va en haut à gauche*/
      SDL_GetWindowPosition(windows[i+MAX/2],&pos_x,&pos_y);
      SDL_SetWindowPosition(windows[i+MAX/2],pos_x-pas_x*scale, pos_y-pas_y);
    }
    marge+=pas_x*scale;
  }



  /* fermeture des fenetres */
  for(i=1;i<MAX;i++)
  {
    SDL_DestroyWindow(windows[i]);
  }


  SDL_SetWindowPosition(windows[0],100,100);

  /*On redimmensionne la seule fenetre encor ouverte*/
  SDL_GetWindowSize(windows[0], &window_w, &window_h);
  for(j=1; j<5;j= j +0.01)
  {
    SDL_SetWindowSize(windows[0], window_w * j, window_h *j);
  }

  SDL_Quit();

  return 0;
}