#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>



void end_sdl(SDL_Window *window, SDL_Renderer *renderer,SDL_Texture * texture)
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
            end_sdl(window,renderer,texture);
            exit(1);
        }

    my_texture = SDL_CreateTextureFromSurface(renderer, my_image); // Chargement de l'image de la surface vers la texture
    SDL_FreeSurface(my_image);                                    // la SDL_Surface ne sert que comme element transitoire

    if (my_texture == NULL) 
        {
            printf("Echec de la transformation de la surface en texture\n");
            end_sdl(window,renderer,texture);
            exit(1);
        }

    return my_texture;
}


void play_with_texture_1(SDL_Texture * texture, SDL_Window * window, SDL_Renderer * renderer)
{
    SDL_Rect source = {0}, window_dimensions = {0}, destination = {0}; // zone de texture a recup, fenetre (on utilise que largeur et hauteur), depot de la zone_source dans le renderer

    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h); // Recuperation des dimensions de la fenetre

    SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);  // Recuperation des dimensions de l'image

    destination = window_dimensions;  // On fixe les dimensions de l'affichage a celles de la fenetre

    /* On veut afficher la texture de facon a ce que l'image occupe la totalite de la fenetre */

    SDL_RenderCopy(renderer, texture, &source, &destination);                 // Creation de l'element a afficher
}


void play_with_texture_2(SDL_Texture * texture, SDL_Window * window, SDL_Renderer * renderer)
{
    SDL_Rect source = {0}, window_dimensions = {0}, destination = {0}; // zone de texture a recup, fenetre (on utilise que largeur et hauteur), depot de la zone_source dans le renderer

    float zoom = 1.5;         // Facteur de zoom a appliquer

    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h); // Recuperation des dimensions de la fenetre

    SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);  // Recuperation des dimensions de l'image

        
    destination.w = source.w * zoom;     // La destination est un zoom de la source
    destination.h = source.h * zoom;     // La destination est un zoom de la source
    destination.x = (window_dimensions.w - destination.w)/2;  // La destination est au milieu de la largeur de la fenetre
    destination.y = (window_dimensions.h - destination.h)/2;  // La destination est au milieu de la hauteur de la fenetre

    SDL_RenderCopy(renderer, texture, &source, &destination);                 // Creation de l'element a afficher
    SDL_RenderPresent(renderer);                  // Affichage
    
    SDL_Delay(2000);                              // Pause en ms

    SDL_RenderClear(renderer);                    // Effacer la fenetre
}


void play_with_texture_3(SDL_Texture * texture, SDL_Window * window, SDL_Renderer * renderer)
{
    SDL_Rect source = {0}, window_dimensions = {0}, destination = {0}; // zone de texture a recup, fenetre (on utilise que largeur et hauteur), depot de la zone_source dans le renderer

    float zoom = 0.25;         // Facteur de zoom entre l'image source et l'image affichee
    float h = 0, l = 0;
    int nb_it = 200;          // Nombre d'images de l'animation

    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h); // Recuperation des dimensions de la fenetre

    SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);  // Recuperation des dimensions de l'image

    /* On decide de deplacer dans la fenetre cette image */

    destination.w = source.w * zoom;     // On applique le zoom sur la largeur
    destination.h = source.h * zoom;     // On applique le zoom sur la hauteur
    destination.x = (window_dimensions.w - destination.w)/2;  // On centre en largeur

    h = window_dimensions.h - destination.h;  // hauteur du deplacement a effectuer
    l = window_dimensions.w - destination.w;

    for (int i = 0; i < nb_it; ++i) 
    {
        destination.y = h * (1 - exp(-5.0 * i / nb_it) / 2 * (1 + cos(10.0 * i / nb_it * 2 * M_PI)));  // hauteur en fonction du numero d'image
        destination.x = l * (1 - exp(-5.0 * i / nb_it) / 2 * (1 + cos(10.0 * i / nb_it * 2 * M_PI)));  
        SDL_RenderClear(renderer);                    // Effacer l'image precedente

        SDL_SetTextureAlphaMod(texture,(1.0-1.0*i/nb_it)*255);      // L'opacite va passer de 255 a 0 au fil de l'animation
        SDL_RenderCopy(renderer, texture, &source, &destination);   // Preparation de l'affichage
        SDL_RenderPresent(renderer);                  // Affichage de la nouvelle image
        SDL_Delay(30);                                // Pause en ms
    }
    

    SDL_RenderClear(renderer);                    // Effacer la fenetre
}

void play_with_texture_4(SDL_Texture * texture, SDL_Window * window, SDL_Renderer * renderer)
{
    SDL_Rect source = {0}, window_dimensions = {0}, destination = {0}; // zone de texture a recup, fenetre (on utilise que largeur et hauteur), depot de la zone_source dans le renderer
    SDL_Rect state = {0};       // Rectangle de la vignette en cours dans la planche 
    float zoom = 2;         // zoom, car ces images sont un peu petites
    int nb_images = 3;
    int nb_lignes = 4;       
    int offset_x, offset_y;
    int speed = 20;

    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h); // Recuperation des dimensions de la fenetre

    SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);  // Recuperation des dimensions de l'image

    /* Mais pourquoi prendre la totalit de l'image? on peut n'en afficher qu'un morceau, et changer de morceau :-) */

    offset_x = source.w / nb_images;   // La largeur d'une vignette de l'image, marche car la planche est bien reglee
    offset_y = source.h / nb_lignes;

    state.x = 0 ;                          // La premiere vignette est en debut de ligne
    state.y = 0 * offset_y;                
    state.w = offset_x;                    // Largeur de la vignette
    state.h = offset_y;                    // Hauteur de la vignette
    
    destination.w = offset_x * zoom;       // Largeur du sprite a l'ecran
    destination.h = offset_y * zoom;       // Hauteur du sprite a l'ecran
    destination.y = (window_dimensions.h - destination.h)/2; // La course se fait en milieu d'ecran (en vertical)

    for (int x = 0; x < window_dimensions.w - destination.w; x += speed) 
    {
        destination.x = x;                   // Position en x pour l'affichage du sprite
        state.x += offset_x;                 // On passe a la vignette suivante dans l'image
        state.x %= source.w;                 // La vignette qui suit celle de fin de ligne est celle de debut de ligne
                                             
        SDL_RenderClear(renderer);           // Effacer l'image precedente avant de dessiner la nouvelle
        SDL_RenderCopy(renderer, texture, &state, &destination);  // Preparation de l'affichage
        SDL_RenderPresent(renderer);         // Affichage
        SDL_Delay(80);                       // Pause en ms
    }

    

    SDL_RenderClear(renderer);                    // Effacer la fenetre
}

void play_with_texture_5(SDL_Texture * texture,SDL_Texture * arr_texture,SDL_Window * window, SDL_Renderer * renderer)
{
    SDL_Rect source = {0}, window_dimensions = {0}, destination = {0}; // zone de texture a recup, fenetre (on utilise que largeur et hauteur), depot de la zone_source dans le renderer
    SDL_Rect state = {0};       // Rectangle de la vignette en cours dans la planche 
    float zoom = 2;         // zoom, car ces images sont un peu petites
    int nb_images = 3;
    int nb_lignes = 4;       
    int offset_x, offset_y;
    int speed = 20;

    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h); // Recuperation des dimensions de la fenetre

    SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);  // Recuperation des dimensions de l'image

    /* Mais pourquoi prendre la totalit de l'image? on peut n'en afficher qu'un morceau, et changer de morceau :-) */

    offset_x = source.w / nb_images;   // La largeur d'une vignette de l'image, marche car la planche est bien reglee
    offset_y = source.h / nb_lignes;

    state.x = 0 ;                          // La premiere vignette est en debut de ligne
    state.y = 0 * offset_y;                
    state.w = offset_x;                    // Largeur de la vignette
    state.h = offset_y;                    // Hauteur de la vignette
    
    destination.w = offset_x * zoom;       // Largeur du sprite a l'ecran
    destination.h = offset_y * zoom;       // Hauteur du sprite a l'ecran
    destination.y = (window_dimensions.h - destination.h)/2 + 360; // La course se fait en milieu d'ecran (en vertical)

    for (int x = 0; x < window_dimensions.w - destination.w; x += speed) 
    {
        destination.x = x;                   // Position en x pour l'affichage du sprite
        state.x += offset_x;                 // On passe a la vignette suivante dans l'image
        state.x %= source.w;                 // La vignette qui suit celle de fin de ligne est celle de debut de ligne
                                             
        SDL_RenderClear(renderer);           // Effacer l'image precedente avant de dessiner la nouvelle
        play_with_texture_1(arr_texture,window,renderer);
        SDL_RenderCopy(renderer, texture, &state, &destination);  // Preparation de l'affichage
        SDL_RenderPresent(renderer);         // Affichage
        SDL_Delay(80);                       // Pause en ms
    }

    

    SDL_RenderClear(renderer);                    // Effacer la fenetre
}


int main()
{
    char nomImage[30] = "tablespritedetoure.png";
    char nomImage2[30] = "imagedefond.xcf";
    SDL_Renderer * renderer = NULL;
    SDL_Window * window = NULL;
    SDL_Texture * texture = NULL;
    SDL_Texture * arr_texture = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("hello there",0,0,1800,1000,SDL_WINDOW_RESIZABLE);   
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    //==============================================================================//

    texture = load_texture_from_image(nomImage,window,renderer,texture);
    arr_texture =  load_texture_from_image(nomImage2,window,renderer,arr_texture);

    SDL_Delay(1000);

    play_with_texture_5(texture, arr_texture, window, renderer);

    SDL_Delay(1000);

    //==============================================================================//

    IMG_Quit();
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(arr_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}