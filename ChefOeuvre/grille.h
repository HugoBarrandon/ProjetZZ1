#ifndef f_grille
#define f_grille

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>

typedef struct grille
{
    int taille;
    int nb_ligne;
    int ** cases;
}grille_t;

grille_t * Grille_Creation(int taille, int marge);
void Grille_CreationRoute(grille_t * grille);
void Grille_Update(grille_t * grille, int variation);
void Grille_Affichage(grille_t * grille);
void Grille_Destruction(grille_t ** grille);
int estSurRoute(grille_t * grille, SDL_Rect voiture, int h, int marge);
float distance(float x1, float y1, float x2, float y2);

#endif