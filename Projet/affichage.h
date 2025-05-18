#ifndef f_affich
#define f_affich


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>   
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "carte.h"
#include "entite.h"

/*Structure regroupant tous les éléments necessaire à l'affichage*/
typedef struct affichage_manager
{
    SDL_Window * window;
    SDL_Renderer * renderer;

    SDL_Texture * textures_salles;
    SDL_Texture * textures_joueur;
    SDL_Texture * texture_debut;
    SDL_Texture * background;

    TTF_Font * basic_font;
    
    //Pas très propre, il faudrait le nom en ressources_manager
    Mix_Music * musique_intro;
    Mix_Music * musique_principale;

} affichage_manager_t;


affichage_manager_t * Manager_CreationAffichage();
void Manager_DestroyAffichage(affichage_manager_t ** am);

void Dessiner_ecran_debut( affichage_manager_t * am) ;
void Dessiner_ecran_fin(affichage_manager_t * am, int salle_c,int salle_total, entite_t * joueur) ;

void Dessiner_salle(float zoo, affichage_manager_t * am ,SDL_Point coord, int type );
void Dessiner_fond( affichage_manager_t * am );

void Dessiner_Jeu( affichage_manager_t * am,salle_t* listsalle,int nb_salle);
void Dessiner_joueur(affichage_manager_t * am, entite_t * joueur);
void Ecrire_Numero_Salle(affichage_manager_t * am,int salle_c, int salle_total);
void Ecrire_Stats(affichage_manager_t * am,entite_t * joueur);

void Play_Intro(affichage_manager_t * am);
void Play_Switch_to_Principale(affichage_manager_t * am);
#endif