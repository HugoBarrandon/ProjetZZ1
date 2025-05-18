#ifndef f_manager
#define f_manager

#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "carte.h"
#include "affichage.h"
#include "entite.h"

enum etat_jeu {DEBUT, EN_JEU, FIN};

typedef struct game_manager
{
    SDL_bool program_on;

    salle_t * carte;
    affichage_manager_t * affichage_m;
    entite_t * joueur;

    SDL_bool entrainement_IA;
    SDL_bool animations;

    enum etat_jeu etat;

    int choix;
    salle_t * salle_courante;

    int nb_etage_max;
    int nb_etage_courant;


}game_manager_t;

game_manager_t * Manager_CreationJeu();
void Manager_DestroyJeu(game_manager_t ** gm);
void Manager_AfficherJeu(game_manager_t * gm);
void Manager_UpdateJeu(game_manager_t * gm);
int get_nb_salles(salle_t * pere);
salle_t * get_fils_by_index(salle_t * pere, int index);
void Manageur_ReInitGame(game_manager_t ** gm);
void Manager_AppliquerChoix(game_manager_t * gm);
void Manager_Combat(game_manager_t * gm);

void Manager_UpdateJoueur(game_manager_t * gm);

int min(int a, int b);

#endif