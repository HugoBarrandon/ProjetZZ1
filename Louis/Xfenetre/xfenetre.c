#include <SDL2/SDL.h>
#include <stdio.h>

#define MAX 12

int main()  // cree des fenetres en diagonales en deplace une et les supprime
{
  int i = 0;
  int pasx, pasy;
  int posx = 0, posy = 0;
  SDL_Renderer * renderer; 

  SDL_DisplayMode DM;
  SDL_Window * windows[MAX];
    

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  
  SDL_GetCurrentDisplayMode(0, &DM); //recup dimension
  pasx = (DM.w/MAX)*2;
  pasy = (DM.h/MAX)*2;

  for (i=0;i<MAX/2;i++)                //premiere diagonale
  {
    windows[i] = SDL_CreateWindow("coucou",i*pasx,i*pasy,pasx,pasy,SDL_WINDOW_RESIZABLE);
  }

  for (i=0;i<MAX/2;i++)                //deuxieme diagonale
  {
    windows[i+MAX/2] = SDL_CreateWindow("coucou",DM.w-(i+1)*pasx,i*pasy,pasx,pasy,SDL_WINDOW_RESIZABLE);
  }


  SDL_Delay(3000);
  


  SDL_GetWindowPosition(windows[MAX-2],&posx,&posy);
  
  while((0<posx && posx<DM.w-pasx) && (0<posy && posy<DM.h-pasy)) //ne pas depasser les bords de l ecran
  {
    SDL_SetWindowPosition(windows[MAX-2], posx, posy);
    posx-=1;                                              //deplacement vers en haut a gauche
    posy-=3;
  }

  SDL_GetWindowPosition(windows[MAX-2],&posx,&posy);
  
  while((0<posx && posx<DM.w-pasx) && (0<posy && posy<DM.h-pasy))
  {
    SDL_SetWindowPosition(windows[MAX-2], posx, posy);
    posx+=10;                                        //deplacement vers en bas a droite      
    posy+=3;
  }

  
  SDL_Delay(2000);
  renderer =SDL_CreateRenderer(windows[0],-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_SetRenderDrawColor(renderer,50, 0, 0,255);
  SDL_RenderClear(renderer);

  SDL_GetWindowSize(windows[0], &pasx, &pasy);

  while((pasx < DM.w) && (pasy<DM.h))
  {
    SDL_SetWindowSize(windows[0], pasx, pasy);
    pasx+=20;
    pasy+=8;
  }

  SDL_Delay(2000);

  SDL_DestroyRenderer(renderer);

  for (i=0;i<MAX;i++)          //suppression
  {
    SDL_DestroyWindow(windows[i]);
  }


  SDL_Quit();

  return 0;
}


