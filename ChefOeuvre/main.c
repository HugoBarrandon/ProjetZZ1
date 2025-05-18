#include <stdio.h>

#include "grille.h"
#include "afficherGrille.h"
#include "voiture.h"
#include "markov.h"


int main()
{
    char nomImage1[30] = "grass3.jpg";
    char nomImage2[30] = "goudron2.png";
    char nomImage3[30] = "explosion2.png";
    char nomImage4[30] = "voiture.png";

    int h;
    int taille_carreau = 20;
    int taille_matrice = 60;
    int marge_matrice=10;
    int marge_dessin;
    int distance =0;
    int etat = 0;
    int etat_explosion = 0;
    int temp=0;

    float millis;

    int voiture_speed = 6;


    clock_t debut;
    clock_t fin;

    int direction;

    SDL_Rect pos_voiture;

    SDL_Renderer * renderer = NULL;
    SDL_Window * window = NULL;
    SDL_Texture * texture1 = NULL;
    SDL_Texture * texture2 = NULL;
    SDL_Texture * texture3 = NULL;
    SDL_Texture * texture4 = NULL;
    
    SDL_bool program_on = SDL_TRUE, paused = SDL_TRUE;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    

    grille_t * grille = Grille_Creation(taille_matrice,marge_matrice);
    Grille_CreationRoute(grille);

    h = taille_carreau * grille->taille;

    window = SDL_CreateWindow("hello there",0,0,h,h,SDL_WINDOW_RESIZABLE);    
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    

    taille_carreau =  h/grille->taille;
    marge_dessin = (marge_matrice) * taille_carreau;



    texture1 = load_texture_from_image(nomImage1,window,renderer,texture1);
    texture2 = load_texture_from_image(nomImage2,window,renderer,texture2);
    texture3 = load_texture_from_image(nomImage3,window,renderer,texture3);
    texture4 = load_texture_from_image(nomImage4,window,renderer,texture4);

    pos_voiture.x = h/2;

    direction =0;

    //=========================================================================================================//

    while (program_on) 
    {                              // La boucle des evenements
        debut = clock();

        while(SDL_PollEvent(&event))                      // Tant que la file des evenements stockes n'est pas vide et qu'on n'a pas
        {         
            // termine le programme Defiler l'element en tete de file dans 'event'
            switch (event.type) 
            {                         // En fonction de la valeur du type de cet evenement
                case SDL_QUIT:                                // Un evenement simple, on a clique sur la x de la // fenetre
                    program_on = SDL_FALSE;                     // Il est temps d'arreter le programme
                    break;
                case SDL_KEYDOWN:                             // Le type de event est : une touche appuyee
                // comme la valeur du type est SDL_Keydown, dans la partie 'union' de
                // l'event, plusieurs champs deviennent pertinents   
                    switch (event.key.keysym.sym) 
                    {             // la touche appuyee est ...
                        case SDLK_SPACE:                            // ou 'SPC'
                            paused = !paused;                         // basculement pause/unpause
                            break;
                        case SDLK_q:
                            program_on = 0;    
                            break;
                        case SDLK_LEFT:
                            direction = 1;
                            break;
                        case SDLK_RIGHT: 
                            direction = -1;
                            break;

                        case SDLK_r :
                            grille = Grille_Creation(taille_matrice,marge_matrice);
                            Grille_CreationRoute(grille);
                            pos_voiture.x = h/2;
                            paused = SDL_TRUE;                            
                            break;
                        case SDLK_x:
                            etat_explosion = 1;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) 
                    {
                        case SDLK_LEFT:       
                            direction = 0;
                            break;
                        case SDLK_RIGHT:  
                            direction = 0;
                            break;
                    }
                    break;
                default:                                      // Les evenements qu'on n'a pas envisage
                    break;
            
            }
        

        }
        if(paused)
        {
            direction =0;
        }
        pos_voiture.x -= direction * voiture_speed;

        if ((!paused) && (etat_explosion == 0))
        {   if (temp%3==0) distance=distance+1;
            temp+=1;
            marge_dessin-= 8;
            if(marge_dessin < 0)
            {
                etat = markov(etat);
                Grille_Update(grille,etat);
                marge_dessin = (marge_matrice) * taille_carreau;
            }
        }
        if(pos_voiture.x<0) pos_voiture.x+=h;
        pos_voiture.x%=h;
        SDL_RenderClear(renderer);
        dessin(texture1, texture2, renderer, window, grille, h, marge_dessin);
        if (etat_explosion != 0)
        {
            
            gameover(renderer,window,h);
            explosion(texture3, window, renderer, etat_explosion, pos_voiture.x);
            etat_explosion+=1;
        }
        else
        {
            pos_voiture=voiture(texture4, window,renderer, pos_voiture.x, direction);
            
        }
       
        score(renderer, window,distance);
        score2(renderer, window);
        SDL_RenderPresent(renderer);
        
        if (estSurRoute(grille,pos_voiture,h,marge_dessin) == 0 && etat_explosion ==0 )
        {
            etat_explosion = 1;
        }
        
       
        
        fin = clock();
        millis = (fin - debut) * ((float)1000)/CLOCKS_PER_SEC;
        SDL_Delay(32 - millis);
        if (etat_explosion==20) 

        {
            SDL_RenderClear(renderer);
            grille = Grille_Creation(taille_matrice,marge_matrice);
            Grille_CreationRoute(grille);
            pos_voiture.x = h/2;
            paused = SDL_TRUE;  
            etat_explosion=0;
            distance=0;

        }  
    }

    

    //=========================================================================================================//
    

    Grille_Destruction(&grille);

    IMG_Quit();
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(texture3);
    SDL_DestroyTexture(texture4);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}