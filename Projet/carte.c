#include "carte.h"
#include "apprentissage.h"  // permet d'avoir aux constantes globales qui definit le nb d'etages et de salles par etage


/*on utilise tres peu cet arbre dans notre code general car on est passe sur un jeu statique pour faaciliter l apprentissage*/

/*le but originel etait de generer des salles aleatoirement avec Markov et donc les etages etant differents un arbre etait necessaire pour les retenir*/

salle_t * Carte_creation_salle(int type)        //creation d'une = une nouvelle salle
{
    salle_t * salle = (salle_t *)malloc(sizeof(salle_t));
    if(salle)
    {
        salle->type = type;
        salle->frere = NULL;
        salle->fils = NULL;          
    }

    return salle;
}

void Carte_destruction_salles(salle_t ** tete)      //parcours en profondeur de l arbre pour le liberer d'ou le besoin d'une pile 
{
    salle_t * cour = *tete;
    salle_t * temp;

    pile_t * pile = initPile(NBETG * NBETG);   // longueur * largeur = nb max de noeuds

    while (cour && !estVidePile(pile))
    {
        empilerPile(pile, cour->frere);
        temp = cour->fils;
        free(cour);
        cour = temp;
        while (cour == NULL)
        {
            cour = depilerPile(pile);
        }
    } 

    libererPile(&pile);
    *tete = NULL;
}

void Carte_affichage(salle_t * tete)          //fonction de test et debeugage
{
    salle_t * cour = tete;
    salle_t * temp = NULL;

    while (cour || temp)
    {
        temp = NULL;
        
        while(cour)
        {
            printf("%d ", cour->type);

            if (cour->fils)
            {
                temp = cour->fils;
            }
            cour = cour->frere;
        }   
        cour = temp;
        printf("\n");
    }

}

/*
fonction de generation aleatoire de salles. On a pas eu le temps de gerer cette fonctionnalite avec l'apprentissage

int Carte_generer_salles_aleatoires(salle_t * pere)
{
    int nb_salle;
    int i = 0;
    int t_salle;

    salle_t ** cours = &pere->fils;

    srand(time(NULL));

    nb_salle = rand()%4 + 2;  // entre 2 et 5 salles

    for (i=0;i<nb_salle;i++)
    {
        t_salle = Markov_type_future_salle(pere->type);
        *cours = Carte_creation_salle(t_salle);

        if(*cours)
        {
            cours = &((*cours)->frere);
        }
        else
        {
            return -1;
        }
    }
    return 0;
}
*/

int Carte_generer_salles(salle_t * pere)    // cette fonction genere les 4 salles d'un etage a partir d'un pere
{
    int i;
    salle_t ** cours = &pere->fils;         // on commence par generer le fils du pere
    for (i=0;i<NBSALLE;i++)             //i est le type de salle de 0 a 3
    {
        *cours = Carte_creation_salle(i);

        if(*cours)
        {
            cours = &((*cours)->frere);     // puis on genere les freres du fils comme une liste chainee
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

/*-----------fonction de gestion de la pile-------------*/

pile_t * initPile(int taille)
{
    pile_t * p = (pile_t *) malloc(sizeof(pile_t));

    if(p)
    {
        p->taille = taille;
        p->sommet = -1;
        p->base = (elt_type *) malloc(sizeof(elt_type)*taille);

        if (p->base == NULL)
        {
            free(p);
            p = NULL;
        }  
    } 

    return p;
} 

int estVidePile(pile_t * p)
{
    return p->sommet == -1;
} 

int estPleinePile(pile_t * p)
{
    return (p->sommet + 1 >= p->taille);
} 

void libererPile(pile_t ** p)
{
    free((*p)->base);
    free(*p);
    *p = NULL;
} 

void empilerPile(pile_t * p, elt_type val)
{
    p->sommet++;
    p->base[p->sommet] = val; 
} 

elt_type depilerPile(pile_t * p)
{
    elt_type ret;
    
    ret = p->base[p->sommet];
    p->sommet--;

    return ret;
} 

elt_type sommetPile(pile_t * p)
{
    return p->base[p->sommet]; 
} 
 