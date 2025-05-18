#ifndef f_carte
#define f_carte

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


enum type_salle {ATTAQUE, SOIN, DEFENSE, ENNEMI};   //les 4 types de salle que peut rencontrer le joueur dans le donjon

typedef struct salle        // structure lien horizontal et vertical arbre comme en SDD c'est a dire un seul fils et un seul frere pour chaque noeud
{
    enum type_salle type;      //la valeur est le type de salle
    struct salle * frere;          // pointeur vers frere
    struct salle * fils;            //puis fils
} salle_t;         


salle_t * Carte_creation_salle(int type);
void Carte_destruction_salles(salle_t ** tete);
void Carte_affichage(salle_t * tete);
int Carte_generer_salles_aleatoires(salle_t * pere);
int Carte_generer_salles(salle_t * pere);

/*on a fusionne nos pile.c .h avec carte.c carte.h*/
/*on a reutilise la gestion de pile qu on a code en TP de SDD*/

typedef salle_t * elt_type; 

typedef struct pile             
{
    int taille;
    int sommet;
    elt_type * base;
} pile_t;



pile_t * initPile(int taille);
int estVidePile(pile_t * p);
int estPleinePile(pile_t * p);
void libererPile(pile_t ** p);
void empilerPile(pile_t * p, elt_type val);
elt_type depilerPile(pile_t * p);
elt_type sommetPile(pile_t * p);


#endif