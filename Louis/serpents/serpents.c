 
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <SDL2/SDL.h>

void draw1(SDL_Renderer* renderer, int l, int h,SDL_DisplayMode DM)
{                                 
    SDL_Rect rectangle1;
    SDL_Rect rectangle2;
    SDL_Rect rectangle3;

    rectangle1.x = (DM.w-l)/2;                                          
    rectangle1.y = (DM.h-h)/2;                                                  
    rectangle1.w = l/3;                                                
    rectangle1.h = h;

    rectangle2.x = (DM.w-l)/2+l/3;                  
    rectangle2.y = (DM.h-h)/2;                                                  
    rectangle2.w = l/3;                                                
    rectangle2.h = h;

    rectangle3.x = (DM.w-l)/2+(l/3)*2;                                          
    rectangle3.y = (DM.h-h)/2;                                                  
    rectangle3.w = l/3;                                                
    rectangle3.h = h;


    SDL_SetRenderDrawColor(renderer,0, 0, 255,255);
    SDL_RenderFillRect(renderer, &rectangle1);

    SDL_SetRenderDrawColor(renderer,255, 255, 255,255);
    SDL_RenderFillRect(renderer, &rectangle2);

    SDL_SetRenderDrawColor(renderer,255, 0, 0,255);
    SDL_RenderFillRect(renderer, &rectangle3);                                             
}

void draw2(SDL_Renderer* renderer, int l, int h,SDL_DisplayMode DM, int posx, int posy, int * bord_droit, int * bord_bas)
{
    SDL_Rect rectangle1;
    SDL_Rect rectangle2;
    SDL_Rect rectangle3;
    
    if ((posx-l<-((DM.w/2)+100)) && *bord_droit)
    {
        *bord_droit = 0;
    }
    if ((posx+l>(DM.w/2)+100) && (*bord_droit == 0))
    {
        *bord_droit = 1;
    }
    if ((posy-h<-(DM.h/2)) && *bord_bas)
    {
        *bord_bas = 0;
    }
    if ((posy+h>DM.h/2) && *bord_bas ==0 )
    {
        *bord_bas = 1;
    }

    rectangle1.x = posx+(DM.w-l)/2;                                          
    rectangle1.y = posy+(DM.h-h)/2;                                                  
    rectangle1.w = l/3;                                                
    rectangle1.h = h;

    rectangle2.x = posx+(DM.w-l)/2+l/3;                  
    rectangle2.y = posy+(DM.h-h)/2;                                                  
    rectangle2.w = l/3;                                                
    rectangle2.h = h;

    rectangle3.x = posx+(DM.w-l)/2+(l/3)*2;                                          
    rectangle3.y = posy+(DM.h-h)/2;                                                  
    rectangle3.w = l/3;                                                
    rectangle3.h = h;

    SDL_SetRenderDrawColor(renderer,0, 0, 255,255);
    SDL_RenderFillRect(renderer, &rectangle1);

    SDL_SetRenderDrawColor(renderer,255, 255, 255,255);
    SDL_RenderFillRect(renderer, &rectangle2);

    SDL_SetRenderDrawColor(renderer,255, 0, 0,255);
    SDL_RenderFillRect(renderer, &rectangle3);
}

int main()
{
    SDL_Renderer * renderer = NULL;
    SDL_Window * window1 = NULL;

    SDL_DisplayMode DM;
    int largr, hauteur;
    int posx = 0, posy = 0;
    int direction = 1;
    int bord_droit = 0, bord_bas = 0;
 
    SDL_bool program_on = SDL_TRUE, paused = SDL_FALSE;                             
    SDL_Event event;



    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_GetCurrentDisplayMode(0, &DM);
    
    window1 = SDL_CreateWindow("hello there",0,0,DM.w,DM.h,SDL_WINDOW_RESIZABLE);
    
    renderer = SDL_CreateRenderer(window1,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    SDL_SetRenderDrawColor(renderer,0, 0, 0,0);
    SDL_RenderClear(renderer);

    SDL_Delay(1000);
    
    largr = DM.w;
    hauteur = DM.h;

    while (largr > 100 && hauteur > 80)
    {
        SDL_SetRenderDrawColor(renderer,0, 0, 0,0);
        SDL_RenderClear(renderer);
        draw1(renderer,largr,hauteur,DM);
        SDL_RenderPresent(renderer);
        largr-=7;
        hauteur-=4;
    }

                                    

    while (program_on) 
    {                              // La boucle des evenements
    while(SDL_PollEvent(&event))                      // Tant que la file des evenements stockes n'est pas vide et qu'on n'a pas
    {         
    // termine le programme Defiler l'element en tete de file dans 'event'
    switch (event.type) {                         // En fonction de la valeur du type de cet evenement
    case SDL_QUIT:                                // Un evenement simple, on a clique sur la x de la // fenetre
      program_on = SDL_FALSE;                     // Il est temps d'arreter le programme
      break;
    case SDL_KEYDOWN:                             // Le type de event est : une touche appuyee
    // comme la valeur du type est SDL_Keydown, dans la partie 'union' de
    // l'event, plusieurs champs deviennent pertinents   
        switch (event.key.keysym.sym) 
        {             // la touche appuyee est ...
            case SDLK_p:                                // 'p'
            case SDLK_SPACE:                            // ou 'SPC'
                paused = !paused;                         // basculement pause/unpause
                break;
            case SDLK_ESCAPE:                           // 'ESCAPE'  
            case SDLK_q:                                // ou 'q'
                program_on = 0;                           // 'escape' ou 'q', d'autres facons de quitter le programme                                     
                break;
            default:                                    // Une touche appuyee qu'on ne traite pas
                break;
        }
      break;
    case SDL_MOUSEBUTTONDOWN:                     // Click souris   
      if (SDL_GetMouseState(NULL, NULL) & 
          SDL_BUTTON(SDL_BUTTON_LEFT) ) {         // Si c'est un click gauche
        direction = 1;           // Fonction a executer lors d'un click gauche
      } else if (SDL_GetMouseState(NULL, NULL) & 
                 SDL_BUTTON(SDL_BUTTON_RIGHT) ) { // Si c'est un click droit
        direction = -1;                              // Fonction a executer lors d'un click droit
      }
      break;
        default:                                      // Les evenements qu'on n'a pas envisage
      break;
    }
  }
    if (!paused) 
    {                                  
        if (direction == 1)
        {
            if (bord_droit)
            {
                posx-=40;
            }
            else
            {
                posx+=40;
            }
        }
        else
        {
            if (bord_bas)
            {
                posy-=30;
            }
            else
            {
                posy+=30;
            }
        }
        SDL_SetRenderDrawColor(renderer,0, 0, 0,0);
        SDL_RenderClear(renderer);
        SDL_Delay(10);
        draw2(renderer,largr,hauteur,DM,posx,posy,&bord_droit,&bord_bas);
        SDL_Delay(10);
        SDL_RenderPresent(renderer);
    }

    }

        
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window1);

    SDL_Quit();

    return 0;
}