#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  



// dessin de rectangle
void draw(SDL_Renderer * renderer,int posx,int posy,int posh,int posw)
{
    SDL_Rect rectangle;


    SDL_SetRenderDrawColor(renderer,                                                
                         255,0, 255,                                  // couleur : rgb
                         255);      

    rectangle.x=posx;
    rectangle.y=posy;
    rectangle.w=posw;
    rectangle.h=posh;
    

    SDL_RenderFillRect(renderer, &rectangle);   // remplit le rectangle
}




//dessin d'arc de cercle à partir d'une liste de points
void draw_c(SDL_Renderer * renderer,int tab[])
{ int i=0;
  while(tab[i]!=0) 
  {
    
    
    for(float angle = 0; angle < M_PI/2; angle += M_PI / 4000) 
    {      
          SDL_SetRenderDrawColor(renderer,
                        (cos(angle * 2) + 1) * 255 / 2,          // quantité de Rouge      
                        (cos(angle * 5) + 1) * 255 / 2,          //          de vert 
                        (cos(angle) + 1) * 255 / 2,              //          de bleu
                        255);         
                        
                                                   // opacité = opaque                                    
          SDL_RenderDrawPoint(renderer,                                   
                          tab[i] + 100 * cos(angle),                     // coordonnée en x
                          tab[i+1] + 100 * sin(angle));                    //            en y   
    }
    i+=2;
  }

}










void init_bulle(int tab[],int nb)  // création d'un bulle qui est sur l'un des cotés supérieurs ou droit
{

  srand(clock()*100000000/CLOCKS_PER_SEC);
  int nbgen=rand()%2;   
  if (nbgen==1)
  {
      tab[nb]=rand()%500+1;
      tab[nb+1]=1;
    }
  else
  {
      tab[nb]=1;
      tab[nb+1]=rand()%500+1;
        
  }

}












int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

// =========================INIT=================================================


    int N=16;
    int i;
    //int posw=100;
    int tab[N];
    
    for(i=0;i<N;i++)
    {
      tab[i]=0;

    }



    int nb=0;
    tab[0]=1;
    tab[1]=1;

    SDL_Window* window = NULL;  // on initialise le randerer et la fenetre au debut  
    SDL_Renderer* renderer = NULL;
    SDL_DisplayMode screen; // pour récupérere la taille de l'écran 


    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        SDL_Log("Error : SDL initialisation - %s\n", 
                 SDL_GetError());                
        exit(EXIT_FAILURE);     // probleme d'initialisatin
    }

 




  SDL_GetCurrentDisplayMode(0, &screen); // On récupère la taille de l'écran
  printf("Résolution écran\n\tw : %d\n\th : %d\n",
         screen.w, screen.h);




  window = SDL_CreateWindow(
        "Fenêtre à gauche",                    // codage en utf8, donc accents possibles
        0, 0,                                  // coin haut gauche en haut gauche de l'écran
        screen.w, screen.h,               // largeur et hauteur 
        SDL_WINDOW_OPENGL);                 // redimensionnable



    if (window == NULL) {
        SDL_Log("Error : SDL window 1 creation - %s\n", 
                SDL_GetError());                 // échec de la création de la fenêtre
        SDL_Quit();                              // On referme la SDL       
        exit(EXIT_FAILURE);
    }




  // Création du renderer 
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) // problème de randerer
  { 
    SDL_DestroyWindow(window); 
    SDL_Log("Error : SDL randerer creation - %s\n", 
              SDL_GetError());                 // échec de la création du randerer
    SDL_Quit();                              // On referme la SDL       
    exit(EXIT_FAILURE);
      
  }

// =========================BOUCLE=================================================


SDL_bool program_on = SDL_TRUE;                         // Booléen pour dire que le programme doit continuer
 
SDL_Event event;                                  // Evènement à traiter

while (program_on) {                              // La boucle des évènements
  if(SDL_PollEvent(&event)) {                      // Tant que la file des évènements stockés n'est pas vide et qu'on n'a pas
                                                  // terminé le programme Défiler l'élément en tête de file dans 'event'
    switch (event.type) {                         // En fonction de la valeur du type de cet évènement
    case SDL_QUIT:                                // Un évènement simple, on a cliqué sur la x de la // fenêtre
      program_on = SDL_FALSE;                     // Il est temps d'arrêter le programme
      break;

    case SDL_KEYDOWN:                             // Le type de event est : une touche appuyée
                                                  // comme la valeur du type est SDL_Keydown, dans la partie 'union' de
                                                  // l'event, plusieurs champs deviennent pertinents   
        switch (event.key.keysym.sym) {             // la touche appuyée est ...
        case SDLK_q:                                // ou 'q'
            program_on = 0;                           // 'escape' ou 'q', d'autres façons de quitter le programme                                     
        break;
        default:                                    // Une touche appuyée qu'on ne traite pas
        break;
      }
    break;

    case SDL_MOUSEBUTTONDOWN:                     // Click souris   
      /*if (SDL_GetMouseState(NULL, NULL) & 
          SDL_BUTTON(SDL_BUTTON_LEFT) ) {         // Si c'est un click gauche
            posx=posx-10;           // Fonction à éxécuter lors d'un click gauche
      }*/
        if (SDL_GetMouseState(NULL, NULL) &  SDL_BUTTON(SDL_BUTTON_RIGHT) ) 
        { // Si c'est un click droit
            nb=nb+2;
            init_bulle(tab,nb);
                      // Fonction à éxécuter lors d'un click droit
        }

    



      break;
    default:                                      // Les évènements qu'on n'a pas envisagé
      break;
    }
  }
//============================le code qui va affecter constament les variable=========================




    SDL_SetRenderDrawColor(renderer,                                                
                         255,255,255,                                  // mode Red, Green, Blue (tous dans 0..255)
                         255);  

    SDL_RenderClear(renderer);


    int i=0;
    while(tab[i]!=0)
    {
      tab[i]=tab[i]+2;
      i+=1;
    }


    draw_c(renderer,tab); // appel de la fonction qui crée l'image  
    SDL_RenderPresent(renderer); // on affiche cette image
    SDL_Delay(10);                        // affichage


 }




// =========================FIN=================================================

    

  
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window); 
    SDL_Quit(); 

  return EXIT_SUCCESS;
}