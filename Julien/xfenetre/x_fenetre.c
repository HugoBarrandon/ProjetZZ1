#include <SDL2/SDL.h>
#include <stdio.h>




/* création d'une fenetre:*/


int main(int argc,char ** argv)
{
    (void)argc;
    (void)argv;
    int i=0;
    int posx;
    int posy; 

    SDL_DisplayMode DM;
    int N=50;

                                    // on trace le x en créant des fenetre et en les suprimants tres rapidement le long de la diagoanale 
    for(i=0;i<N;i++)
    {
        SDL_Window                  //annonce des futures fenetres 
        *window_i=NULL,
        *window_j=NULL;

        if (SDL_Init(SDL_INIT_VIDEO) != 0) 
        {
            SDL_Log("Error : SDL initialisation - %s\n", 
                     SDL_GetError());                
            exit(EXIT_FAILURE);     // probleme d'initialisatin
        }

        SDL_GetCurrentDisplayMode(0, &DM  );
        //récupération de la taille de l'écran 






        /* Création d'une fenêtre à gauche: diagonale gauche*/
            window_i = SDL_CreateWindow(
        "Fenêtre à gauche",                      // codage en utf8, donc accents possibles
        (i*DM.w/N),(i*DM.h/N),                   // la fenetre parcours la diagonale
        100,10,                                     // largeur et hauteur
        SDL_WINDOW_RESIZABLE);                   // redimensionnable
        if (window_i == NULL) {
            SDL_Log("Error : SDL window 1 creation - %s\n", 
                  SDL_GetError());               // échec de la création de la fenêtre
            SDL_Quit();                          // On referme la SDL       
            exit(EXIT_FAILURE);
        }
    


        /* Création d'une fenêtre à droite: diagonale droite */
            window_j = SDL_CreateWindow(
        "Fenêtre à droite",                     // codage en utf8, donc accents possibles
        (DM.w-i*DM.w/N),(i*DM.h/N),             // la fenetre parcours la diagonale
        100,10,                                    // largeur et hauteur
        SDL_WINDOW_RESIZABLE);                  // redimensionnable
        if (window_j == NULL) {
            SDL_Log("Error : SDL window 1 creation - %s\n", 
                  SDL_GetError());              // échec de la création de la fenêtre
            SDL_Quit();                         // On referme la SDL       
            exit(EXIT_FAILURE);
        }

        SDL_Delay(70);    
        SDL_DestroyWindow(window_j);
        SDL_DestroyWindow(window_i);
        // on suprime la fenetre avant d'afficher la suivante

    }






    //on termine en faisant bouger une fenetre et en la redimensionant
    SDL_Window                  //annonce future fenetre
        *window_i=NULL;
       

        if (SDL_Init(SDL_INIT_VIDEO) != 0) 
        {
            SDL_Log("Error : SDL initialisation - %s\n", 
                     SDL_GetError());                
            exit(EXIT_FAILURE);     // probleme d'initialisatin
        }

        SDL_GetCurrentDisplayMode(0, &DM  );
        //récupération de la taille de l'écran




        window_i = SDL_CreateWindow(
        "Fenêtre à gauche",                      // codage en utf8, donc accents possibles
        DM.w/2,0,                            // la fenetre par du milieu en haut 
        100,70,                                  // largeur et hauteur
        SDL_WINDOW_RESIZABLE);                   // redimensionnable
        if (window_i == NULL) {
            SDL_Log("Error : SDL window 1 creation - %s\n", 
                  SDL_GetError());               // échec de la création de la fenêtre
            SDL_Quit();                          // On referme la SDL       
            exit(EXIT_FAILURE);
        }

        for(i=0;i<N*4;i++)
        {
             
            SDL_GetWindowPosition(window_i,&posx, &posy);          // on récupére la position de la fenetre
            SDL_SetWindowPosition(window_i, posx-5, 5*i);   //  on la décale vers le bas du pas
           
            
            if (i==40)
            {
                SDL_SetWindowSize(window_i,200,180);//on augmente la taille de la fenetre apres un tour 
            }
            if (i==90)
            {
                SDL_SetWindowSize(window_i,400,380);//on augmente la taille de la fenetre apres un tour 
            }
            if (i==130)
            {
                SDL_SetWindowSize(window_i,200,180);//on augmente la taille de la fenetre apres un tour 
            }

        }

      
        SDL_DestroyWindow(window_i);





    
    SDL_Quit();                                // la SDL

    return 0;
}


