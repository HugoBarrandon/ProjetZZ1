#include "afficherGrille.h"


void dessin(SDL_Texture * texture1,SDL_Texture * texture2,SDL_Renderer * renderer, SDL_Window * window, grille_t * grille,int h, int marge)
{
    SDL_Rect rectangle;
    int i,j;
    int carreau = h/grille->taille;

    for (i=0;i<grille->nb_ligne;i++)
    {
        for (j=0;j<grille->taille;j++)
        {
            rectangle.x = j * carreau;                                          
            rectangle.y = (i * carreau) - marge;                                                  
            rectangle.w = carreau;                                                
            rectangle.h = carreau;

            if (grille->cases[i][j] == 0)
            {
                play_with_texture_2(texture1, window, renderer, rectangle);
            }
            else
            {
                play_with_texture_2(texture2, window, renderer, rectangle);   
            }
        }
    }
}

void end_sdl_affichage(SDL_Window *window, SDL_Renderer *renderer,SDL_Texture * texture)
{
    IMG_Quit();
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

SDL_Texture * load_texture_from_image(char  *  file_image_name, SDL_Window *window, SDL_Renderer *renderer,SDL_Texture * texture)
{
    SDL_Surface * my_image = NULL;           // Variable de passage
    SDL_Texture * my_texture = NULL;         // La texture


    my_image = IMG_Load(file_image_name);   // Chargement de l'image dans la surface

    if (my_image == NULL)
        {
            printf("Chargement de l'image impossible\n");
            end_sdl_affichage(window,renderer,texture);
            exit(1);
        }

    my_texture = SDL_CreateTextureFromSurface(renderer, my_image); // Chargement de l'image de la surface vers la texture
    SDL_FreeSurface(my_image);                                    // la SDL_Surface ne sert que comme element transitoire

    if (my_texture == NULL) 
        {
            printf("Echec de la transformation de la surface en texture\n");
            end_sdl_affichage(window,renderer,texture);
            exit(1);
        }

    return my_texture;
}

void play_with_texture_2(SDL_Texture * texture, SDL_Window * window, SDL_Renderer * renderer, SDL_Rect rectangle)
{

    SDL_Rect source = {0}, window_dimensions = {0}, destination = {0}; // zone de texture a recup, fenetre (on utilise que largeur et hauteur), depot de la zone_source dans le renderer


    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h); // Recuperation des dimensions de la fenetre

    SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);  // Recuperation des dimensions de l'image

        
    destination.w = rectangle.w;
    destination.h = rectangle.h;
    destination.x = rectangle.x;
    destination.y = rectangle.y;

    SDL_RenderCopy(renderer, texture, &source, &destination);                 // Creation de l'element a afficher
}

/*
int main()
{
    int grille[MAX][MAX];
    int i,j;                        // a supprimer
    for (i=0;i<MAX;i++)
    {
        for (j=0;j<MAX;j++)
        {
            grille[i][j] = 0;
        }
    }

    for (i=4;i<6;i++)
    {
        for (j=0;j<MAX;j++)
        {
            grille[i][j] = 1;
        }
    }                               // jusque la
    

    int h;
    //int posx = 0, posy = 0;
    //int delay = 50;

    char nomImage1[30] = "grass.png";
    char nomImage2[30] = "goudron2.png";

    SDL_Renderer * renderer = NULL;
    SDL_Window * window = NULL;
    SDL_Texture * texture1 = NULL;
    SDL_Texture * texture2 = NULL;

    SDL_DisplayMode DM;
    
    SDL_bool program_on = SDL_TRUE, paused = SDL_TRUE;  

    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_GetCurrentDisplayMode(0, &DM);
    h = DM.h - 100;

    window = SDL_CreateWindow("hello there",0,0,h,h,SDL_WINDOW_RESIZABLE);    
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    //==============================================================================//    

    texture1 = load_texture_from_image(nomImage1,window,renderer,texture1);
    texture2 = load_texture_from_image(nomImage2,window,renderer,texture2);

    while (program_on) 
    {                              // La boucle des evenements
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
                        case SDLK_ESCAPE:                           // 'ESCAPE'  
                        case SDLK_q:                                // ou 'q'
                            program_on = 0;                           // 'escape' ou 'q', d'autres facons de quitter le programme                                     
                            break;
                        default:                                    // Une touche appuyee qu'on ne traite pas
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:                     // Click souris   

                    if ( SDL_BUTTON(SDL_BUTTON_LEFT)) 
                    {
                                  // Fonction a executer lors d'un click gauche
                    } 
                    break;
                
                default:                                      // Les evenements qu'on n'a pas envisage
                    break;
            }
        }

        if (!paused)
        {
            dessin(texture1, texture2, renderer, window, grille, h);
            SDL_RenderPresent(renderer);
        }

        dessin(texture1, texture2, renderer, window, grille, h);
        SDL_RenderPresent(renderer);

    }     
    
    //==============================================================================// 

    IMG_Quit();
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();
    return 0;

}*/