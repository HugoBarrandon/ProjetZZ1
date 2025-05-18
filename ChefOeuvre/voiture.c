#include "voiture.h"
#include <string.h>

void end_sdl(int code, char* message,SDL_Window *window, SDL_Renderer * renderer)
{
    printf("%s\n",message);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_Quit();  
    exit(code);
}


//==animation explosion==//
void explosion(SDL_Texture* my_texture,SDL_Window* window,SDL_Renderer* renderer, int etat,int posx) 
{

       
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

       
    int nb_lignes = 4;
    int nb_colonnes = 5;                     // Il y a 8 vignette dans la ligne de l'image qui nous intéresse
    float zoom = 1;                        // zoom, car ces images sont un peu petites
    int offset_x = source.w / nb_colonnes,   // La largeur d'une vignette de l'image, marche car la planche est bien réglée
           offset_y = source.h / nb_lignes;           // La hauteur d'une vignette de l'image, marche car la planche est bien réglée

    state.w = offset_x;                    // Largeur de la vignette
    state.h = offset_y;                    // Hauteur de la vignette

    destination.w = offset_x * zoom;       // Largeur du sprite à l'écran
    destination.h = offset_y * zoom;       // Hauteur du sprite à l'écran



    //==position explosition==//: milieu
    destination.y = (window_dimensions.h - destination.h)/2; // pour l'instant explosion au milieu
    destination.x = posx; 

    state.x=((etat-1)%nb_colonnes)*offset_x;
    state.y=((etat-1)/nb_colonnes)*offset_y;

        

    SDL_RenderCopy(renderer, my_texture,&state,&destination);  
    SDL_Delay(100);
       

         
}


SDL_Rect voiture(SDL_Texture* my_texture,SDL_Window* window,SDL_Renderer* renderer,int posx,int direction) 
{
    double angle = -45*direction;
     SDL_Rect 
           source = {0},                      // Rectangle définissant la zone de la texture à récupérer
           window_dimensions = {0},           // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur 
           destination = {0};                 // Rectangle définissant où la zone_source doit être déposée dans le renderer

     SDL_GetWindowSize(
         window, &window_dimensions.w,
         &window_dimensions.h);               // Récupération des dimensions de la fenêtre
     SDL_QueryTexture(my_texture, NULL, NULL,
                      &source.w, &source.h);  // Récupération des dimensions de l'image

     float zoom = 0.18;                        // Facteur de zoom à appliquer    
     destination.w = source.w * zoom;         // La destination est un zoom de la source
     destination.h = source.h * zoom;         // La destination est un zoom de la source
     destination.x = posx;   // La destination est au milieu de la largeur de la fenêtre
     destination.y =
         (window_dimensions.h - destination.h) / 2;  // La destination est au milieu de la hauteur de la fenêtre

     SDL_RenderCopyEx(renderer, my_texture,     // Préparation de l'affichage  
                    &source,
                    &destination,
                    angle,
                    NULL,
                    SDL_FLIP_NONE);


    return destination;



}











void score(SDL_Renderer *renderer, SDL_Window * window,int scoreint){

    if (TTF_Init() < 0) end_sdl(0, "Couldn't initialize SDL TTF", window, renderer);

    TTF_Font* font = NULL;                                               // la variable 'police de caractère'
    font = TTF_OpenFont("./Pacifico.ttf", 100);                     // La police à charger, la taille désirée


    if (font == NULL) end_sdl(0, "Can't load font", window, renderer);

    TTF_SetFontStyle(font, TTF_STYLE_ITALIC | TTF_STYLE_BOLD);           // en italique, gras

    SDL_Color color = {0, 0, 0, 255};                                  // la couleur du texte
    SDL_Surface* text_surface = NULL;                                    // la surface  (uniquement transitoire)

    char score[10];
    sprintf(score, "%d", scoreint);
    text_surface = TTF_RenderText_Blended(font, score, color); // création du texte dans la surface 


    text_surface = TTF_RenderText_Blended(font, score, color); // création du texte dans la surface 
    if (text_surface == NULL) end_sdl(0, "Can't create text surface", window, renderer);




    SDL_Texture* text_texture = NULL;                                    // la texture qui contient le texte
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface); // transfert de la surface à la texture
    if (text_texture == NULL) end_sdl(0, "Can't create texture from surface", window, renderer);
    SDL_FreeSurface(text_surface);                                       // la texture ne sert plus à rien

    SDL_Rect pos = {50,50, 0, 0};                                         // rectangle où le texte va être prositionné
    SDL_QueryTexture(text_texture, NULL, NULL, &pos.w, &pos.h);          // récupération de la taille (w, h) du texte 
    SDL_RenderCopy(renderer, text_texture, NULL, &pos);                  // Ecriture du texte dans le renderer   
    SDL_DestroyTexture(text_texture);                                    // On n'a plus besoin de la texture avec le texte

}








void score2(SDL_Renderer *renderer, SDL_Window * window){

    if (TTF_Init() < 0) end_sdl(0, "Couldn't initialize SDL TTF", window, renderer);

    TTF_Font* font = NULL;                                               // la variable 'police de caractère'
    font = TTF_OpenFont("./game_over.ttf",200);                     // La police à charger, la taille désirée


    if (font == NULL) end_sdl(0, "Can't load font", window, renderer);

    //TTF_SetFontStyle(font, TTF_STYLE_ITALIC | TTF_STYLE_BOLD);           // en italique, gras

    SDL_Color color = {0, 0, 0, 255};                                  // la couleur du texte
    SDL_Surface* text_surface = NULL;                                    // la surface  (uniquement transitoire)
    text_surface = TTF_RenderText_Blended(font, "Score :", color); // création du texte dans la surface 
    if (text_surface == NULL) end_sdl(0, "Can't create text surface", window, renderer);
    
    SDL_Texture* text_texture = NULL;                                    // la texture qui contient le texte
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface); // transfert de la surface à la texture
    if (text_texture == NULL) end_sdl(0, "Can't create texture from surface", window, renderer);
    SDL_FreeSurface(text_surface);                                       // la texture ne sert plus à rien

    SDL_Rect pos = {0,0, 0, 0};                                         // rectangle où le texte va être prositionné
    SDL_QueryTexture(text_texture, NULL, NULL, &pos.w, &pos.h);          // récupération de la taille (w, h) du texte 
    SDL_RenderCopy(renderer, text_texture, NULL, &pos);                  // Ecriture du texte dans le renderer   
    SDL_DestroyTexture(text_texture);                                    // On n'a plus besoin de la texture avec le texte
     
}

























void gameover(SDL_Renderer *renderer, SDL_Window * window, int h){

    if (TTF_Init() < 0) end_sdl(0, "Couldn't initialize SDL TTF", window, renderer);

    TTF_Font* font = NULL;                                               // la variable 'police de caractère'
    font = TTF_OpenFont("./Pacifico.ttf",200);                     // La police à charger, la taille désirée


    if (font == NULL) end_sdl(0, "Can't load font", window, renderer);

    //TTF_SetFontStyle(font, TTF_STYLE_ITALIC | TTF_STYLE_BOLD);           // en italique, gras

    SDL_Color color = {0, 0, 0, 255};                                  // la couleur du texte
    SDL_Surface* text_surface = NULL;                                    // la surface  (uniquement transitoire)
    text_surface = TTF_RenderText_Blended(font, "Game over", color); // création du texte dans la surface 
    if (text_surface == NULL) end_sdl(0, "Can't create text surface", window, renderer);
    char score[10];
    
    SDL_Texture* text_texture = NULL;                                    // la texture qui contient le texte
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface); // transfert de la surface à la texture
    if (text_texture == NULL) end_sdl(0, "Can't create texture from surface", window, renderer);
    SDL_FreeSurface(text_surface);                                       // la texture ne sert plus à rien

    SDL_Rect pos = {h/2-195,h/2-200, 0, 0};                                         // rectangle où le texte va être prositionné
    SDL_QueryTexture(text_texture, NULL, NULL, &pos.w, &pos.h);          // récupération de la taille (w, h) du texte 
    SDL_RenderCopy(renderer, text_texture, NULL, &pos);                  // Ecriture du texte dans le renderer   
    SDL_DestroyTexture(text_texture);                                    // On n'a plus besoin de la texture avec le texte
     
}


