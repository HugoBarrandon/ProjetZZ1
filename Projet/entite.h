#ifndef f_entite
#define f_entite

#include <stdlib.h>
#include <SDL2/SDL.h>

enum etat_entite {IDLE,WALK};

typedef struct entite
{
    int pv_max;
    int pv;
    int attaque_max;
    int attaque;
    int defense_max;
    int defense;
    int score;                      //Contient le score du joueur ou le score donner quand un monstre est battus

    enum etat_entite etat;          //Etat (IDLE = ne bouge pas - WALK = se déplace)
    int current_frame;              //La frame de l'animation courrante
    int delay_before_next_frame;    //Nombre de tour de boucle avant la prochaine frame
    int delay_between_frames;       //Nombre de tour de boucle entre chaque frame
    SDL_Point position;             //Position à l'ecran
    SDL_Point destination;          //Position de la salle vers laquelle on va marcher
    float speed;                    //Vitesse de déplacement

}entite_t;

entite_t * Entite_CreationJoueur(int pv_max, int atq_max, int def_max);
entite_t * Entite_CreationEnnemi(int etage);
int max(int a, int b);
void Entite_Liberation(entite_t ** e);


#endif