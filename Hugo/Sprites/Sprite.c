#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include "Player.h"

void update(SDL_Point * player_direction, SDL_Point * player_position, SDL_Point * player_state, int speed)
{
    int norme = sqrt((player_direction->x*player_direction->x) + (player_direction->y*player_direction->y));
    player_position->x += (player_direction->x * norme * speed);
    player_position->y += (player_direction->y * norme * speed);
    if(player_direction->x>0) player_state->y=3;
    if(player_direction->x<0) player_state->y=2;
    if(player_direction->y>0 && player_direction->x == 0) player_state->y=0;
    if(player_direction->y<0 && player_direction->x == 0) player_state->y=1;

    player_direction->x = 0;
    player_direction->y = 0;
}

void draw(SDL_Renderer* renderer, SDL_Window * window, player_t * player, SDL_Texture * bg)
{
    SDL_Rect 
          source = {0},                         // Rectangle définissant la zone de la texture à récupérer
          window_dimensions = {0},              // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
          destination = {0};                    // Rectangle définissant où la zone_source doit être déposée dans le renderer

  SDL_GetWindowSize(
      window, &window_dimensions.w,
      &window_dimensions.h);                    // Récupération des dimensions de la fenêtre
  SDL_QueryTexture(bg, NULL, NULL,
                   &source.w, &source.h);       // Récupération des dimensions de l'image

  destination = window_dimensions;              // On fixe les dimensions de l'affichage à  celles de la fenêtre

  /* On veut afficher la texture de façon à ce que l'image occupe la totalité de la fenêtre */

  SDL_RenderCopy(renderer, bg,
                 &source,
                 &destination);
    Player_Draw(renderer, player);
    
}

SDL_Texture* load_texture_from_image(char  *  file_image_name,SDL_Renderer *renderer ){
    SDL_Surface *my_image = NULL;           // Variable de passage
    SDL_Texture* my_texture = NULL;         // La texture

    my_image = SDL_LoadBMP(file_image_name);   // Chargement de l'image dans la surface
                                            // image=SDL_LoadBMP(file_image_name); fonction standard de la SDL, 
                                            // uniquement possible si l'image est au format bmp */

    my_texture = SDL_CreateTextureFromSurface(renderer, my_image); // Chargement de l'image de la surface vers la texture
    SDL_FreeSurface(my_image);                                     // la SDL_Surface ne sert que comme élément transitoire 

    return my_texture;
}

int main()
{
    player_t * player;

    SDL_bool program_on = SDL_TRUE;
    SDL_Event event;

    SDL_Texture *bg;
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_DisplayMode screen;

    SDL_Point player_position = {100,200};


    /* Initialisation de la SDL  + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }


    SDL_GetCurrentDisplayMode(0, &screen);
    window = SDL_CreateWindow("Premier dessin",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            screen.w ,
                            screen.h ,
                            SDL_WINDOW_OPENGL);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    bg = IMG_LoadTexture(renderer,"./Grass_Sample.png");


    player = Player_Create(IMG_LoadTexture(renderer,"./Player.png"),player_position);

    /*Boucle de jeu*/
    while(program_on)
    {
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    program_on = SDL_FALSE;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) 
                    {
                        case SDLK_z:
                            player->direction.y -= 1;
                            break; 
                        case SDLK_s:
                            player->direction.y += 1;
                            break;
                        case SDLK_q:
                            player->direction.x -= 1;
                            break;
                        case SDLK_d:
                            player->direction.x += 1;
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        
        
        SDL_RenderClear(renderer);
        Player_Update(player);
        draw(renderer, window,player,bg);

        SDL_RenderPresent(renderer);
    }



    Player_Destroy(&player);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(bg);
    IMG_Quit();
    SDL_Quit();

    return 0;
}