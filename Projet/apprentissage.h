#ifndef f_apprentissage
#define f_apprentissage

#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "entite.h"

#define ATK 5
#define DEF 4
#define PV 6  // 5 PV max
#define NBETG 10
#define NBSALLE 4
#define alpha 0.5  // a voir ???????
#define gamma 0.9 // a voir ???? DOUILLE 


/*typedef struct perception
{
    int etage;
    entite_t joueur;
    int choix[4];

} perception_t;

*/

float ** Apprentissage_creer_tableQ();

int Apprentissage_taille(int liste[2 * NBETG]);

void Apprentissage_Qupdate(float** Qtableau1, float** Qtableau2, int r,int * parcours);

int Apprentissage_choix(float** Qtableau1, float** Qtableau2, int posx);

int Apprentissage_choix2(float** Qtableau2, int posx);




void Apprentissage_liberertableQ(float ** tableau);

void Apprentissage_AffichageQ(float ** Qtableau);

void Apprentissage_load(float** Qtableau ,char * f);


void Apprentissage_save(float ** Qtableau,char * f);


float apprentissage_eps1(float eps);

float apprentissage_eps2(float nbiter);

float apprentissage_eps3(float nbiter,int nb_periode);

float apprentissage_eps4(float nbiter);




#endif