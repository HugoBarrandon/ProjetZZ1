#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  
#include <SDL2/SDL_image.h>









///===sert a fficher le  fonds principale(entièreté de l'écran )====///
void play_with_texture_1(SDL_Texture *my_texture, SDL_Window *window,
                         SDL_Renderer *renderer) {
  SDL_Rect 
          source = {0},                         // Rectangle définissant la zone de la texture à récupérer
          window_dimensions = {0},              // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
          destination = {0};                    // Rectangle définissant où la zone_source doit être déposée dans le renderer

  SDL_GetWindowSize(
      window, &window_dimensions.w,
      &window_dimensions.h);                    // Récupération des dimensions de la fenêtre
  SDL_QueryTexture(my_texture, NULL, NULL,
                   &source.w, &source.h);       // Récupération des dimensions de l'image

  destination = window_dimensions;              // On fixe les dimensions de l'affichage à  celles de la fenêtre

  /* On veut afficher la texture de façon à ce que l'image occupe la totalité de la fenêtre */

  SDL_RenderCopy(renderer, my_texture,
                 &source,
                 &destination);                 // Création de l'élément à afficher
}


//=====gestion des paralaxes=====///
void play_with_texture_paralaxe(SDL_Texture *my_texture, SDL_Window *window,
                         SDL_Renderer *renderer) {
  SDL_Rect 
          source = {0},                         // Rectangle définissant la zone de la texture à récupérer
          window_dimensions = {0},              // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
          destination = {0};                    // Rectangle définissant où la zone_source doit être déposée dans le renderer

  SDL_GetWindowSize(
      window, &window_dimensions.w,
      &window_dimensions.h);                    // Récupération des dimensions de la fenêtre
  SDL_QueryTexture(my_texture, NULL, NULL,
                   &source.w, &source.h);       // Récupération des dimensions de l'image

  destination = window_dimensions;              // On fixe les dimensions de l'affichage à  celles de la fenêtre

  /* On veut afficher la texture de façon à ce que l'image occupe la totalité de la fenêtre */

  SDL_RenderCopy(renderer, my_texture,
                 &source,
                 &destination);                 // Création de l'élément à afficher
}





//comment faire boucler une image ? non nécéssaire pour l'instant



//==animation(remplacemnt des image)==//
void play_with_texture_4(SDL_Texture* my_texture,
                         SDL_Texture* bg_texture,
                         SDL_Window* window,
                         SDL_Renderer* renderer) {
       SDL_Rect 
             source = {0},                    // Rectangle définissant la zone totale de la planche
             window_dimensions = {0},         // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
             destination = {0},               // Rectangle définissant où la zone_source doit être déposée dans le renderer
             state = {0};                     // Rectangle de la vignette en cours dans la planche 

       SDL_GetWindowSize(window,              // Récupération des dimensions de la fenêtre
                         &window_dimensions.w,
                         &window_dimensions.h);
       SDL_QueryTexture(my_texture,           // Récupération des dimensions de l'image
                        NULL, NULL,
                        &source.w, &source.h);

       /* Mais pourquoi prendre la totalité de l'image, on peut n'en afficher qu'un morceau, et changer de morceau :-) */




  //===modif selon image(c'est mieux si c'est constant)

       

       int nb_images = 4;                     // Il y a 8 vignette dans la ligne de l'image qui nous intéresse
       float zoom = 4;                        // zoom, car ces images sont un peu petites
       int offset_x =source.w / nb_images,   // La largeur d'une vignette de l'image, marche car la planche est bien réglée
           offset_y = source.h / 4;           // La hauteur d'une vignette de l'image, marche car la planche est bien réglée


       state.x = 0 ;                          // La première vignette est en début de ligne
       state.y = 2*offset_y;                 // On s'intéresse à la 4ème ligne, le bonhomme qui court
       state.w =offset_x;                // Largeur de la vignette
       state.h = offset_y;                // Hauteur de la vignette

       destination.w = offset_x * zoom;       // Largeur du sprite à l'écran
       destination.h = offset_y * zoom;       // Hauteur du sprite à l'écran

       destination.y =                        // La course se fait en milieu d'écran (en vertical)
         (window_dimensions.h - destination.h)*0.72;



       int speed = 25;
       for (int x = 0; x < window_dimensions.w - destination.w; x += speed) {

         destination.x = x;                   // Position en x pour l'affichage du sprite
         state.x += offset_x;                 // On passe à la vignette suivante dans l'image
         state.x %= source.w;                 // La vignette qui suit celle de fin de ligne est
                                              // celle de début de ligne


         SDL_RenderClear(renderer);  // Effacer l'image précédente avant de dessiner la nouvelle

        //===arrière plan===//
        play_with_texture_1(bg_texture,window,renderer);

        // sprite 1er plan
         SDL_RenderCopy(renderer, my_texture, // Préparation de l'affichage
                        &state,
                        &destination);  
         

        //===affichage===//
         SDL_RenderPresent(renderer);         // Affichage
         SDL_Delay(90);                       // Pause en ms
       }
       SDL_RenderClear(renderer);             // Effacer la fenêtre avant de rendre la main
     }






int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

// =========================INITIALISATION=================================================


  

    SDL_Window* window = NULL;             // on initialise le randerer et la fenetre au debut  
    SDL_Renderer* renderer = NULL;
    SDL_DisplayMode screen;                // pour récupérere la taille de l'écran 


    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        SDL_Log("Error : SDL initialisation - %s\n", 
                 SDL_GetError());                
        exit(EXIT_FAILURE);               // probleme d'initialisatin
    }

 
    SDL_GetCurrentDisplayMode(0, &screen); // On récupère la taille de l'écran
    printf("Résolution écran\n\tw : %d\n\th : %d\n",
         screen.w, screen.h);



//===création fenetre ===
    window = SDL_CreateWindow(
        "Mini animation pokémon",                    // codage en utf8, donc accents possibles
        0, 0,                                  // coin haut gauche en haut gauche de l'écran
        screen.w, screen.h,               // largeur et hauteur 
        SDL_WINDOW_OPENGL);                 // redimensionnable



    if (window == NULL) {
        SDL_Log("Error : SDL window 1 creation - %s\n", 
                SDL_GetError());                 // échec de la création de la fenêtre
        SDL_Quit();                              // On referme la SDL       
        exit(EXIT_FAILURE);
    }



//===Création du renderer ===
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) // problème de randerer
  { 
    SDL_DestroyWindow(window); 
    SDL_Log("Error : SDL randerer creation - %s\n", 
              SDL_GetError());                 // échec de la création du randerer
    SDL_Quit();                              // On referme la SDL       
    exit(EXIT_FAILURE);
      
  }


//=== création texture===
  SDL_Texture *texture; 
    texture = IMG_LoadTexture(renderer,"./planche2.png");
    if (texture == NULL) printf( "Echec du chargement de l'image dans la texture");

  SDL_Texture * bg_texture; 
  bg_texture = IMG_LoadTexture(renderer,"./1plan.png");
  if (texture == NULL) printf( "Echec du chargement de l'image dans la texture");


// d'autre fonds//

/*
SDL_Texture * bg1_texture; 
  bg1_texture = IMG_LoadTexture(renderer,"./1plan.xcf");
  if (texture == NULL) printf( "Echec du chargement de l'image dans la texture");

SDL_Texture * bg2_texture; 
  bg2_texture = IMG_LoadTexture(renderer,"./2plan.xcf");
  if (texture == NULL) printf( "Echec du chargement de l'image dans la texture");

SDL_Texture * bg3_texture; 
  bg3_texture = IMG_LoadTexture(renderer,"./3plan.scf");
  if (texture == NULL) printf( "Echec du chargement de l'image dans la texture");
*/

//============================le code qui va agir=========================//

  //drawf(bg_texture,window,renderer);

  play_with_texture_4(texture,bg_texture,window,renderer);
  
 




 





// =========================FIN=================================================// libération de tout les éléments
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(bg_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window); 
    IMG_Quit();
    SDL_Quit(); 
   
    

  return EXIT_SUCCESS;
}
