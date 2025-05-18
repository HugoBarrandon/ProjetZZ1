#include "grille.h"

int largeur_route = 20;

/*Génère une grille de la taille donnée*/
grille_t * Grille_Creation(int taille, int marge)
{
    int i,j;

    /*Creation de la grille*/
    grille_t * grille = (grille_t*)malloc(sizeof(grille_t));
    if(grille)
    {
        grille->taille = taille;
        grille->nb_ligne = taille+marge;
        grille->cases = (int**)malloc(sizeof(int*)*grille->nb_ligne);
        for(i=0;i<grille->nb_ligne;i++)
        {
            grille->cases[i] = (int*)malloc(sizeof(int)*taille);
            for(j=0; j<grille->taille; j++)
            {
                grille->cases[i][j] = 0;
            }
        }
    }

    return grille;
}

void Grille_CreationRoute(grille_t * grille)
{
    int i,j;
    int debut_route = (grille->taille-largeur_route)/2; //1ère colonne de la matrice qui contiendra la route (centré au début)
    int fin_route = (grille->taille+largeur_route)/2;
    for(i=0; i<grille->nb_ligne;i++)
    {
        for(j=debut_route;j<fin_route;j++)
        {
            grille->cases[i][j] = 1;
        }
    }
}

void Grille_Affichage(grille_t * grille)
{
    int i,j;
    for(i=0;i<grille->nb_ligne;i++)
    {
        for(j=0;j<grille->taille;j++)
        {
            printf("%d ",grille->cases[i][j]);
        }
        printf("\n");
    }
}

void Grille_Update(grille_t * grille, int variation)
{
    int marge = grille->nb_ligne - grille->taille;
    int future_cases[marge+1][grille->taille]; //matrice (Marge +1 x largeur) ne contenant que les futurs couches
    int i,j;

    /*Copie de la dernière couche de la grille (point de départ pour la variation)*/
    
    
    for(j=0;j<grille->taille;j++)
    {
        future_cases[marge][j]=grille->cases[0][j];
    }
    
    /*Generation des futures lignes*/
    switch (variation)
    {
        /*Tourne à gauche*/
        case -1:
            for(i=marge-1; i>=0;i--)
            {
                for(j=0;j<grille->taille;j++)
                {
                    future_cases[i][j]=future_cases[i+1][(j+1+grille->taille)%grille->taille];
                }
            }
            break;
        /*Tourne à droite*/
        case 1:
            for(i=marge-1; i>=0;i--)
            {
                for(j=0;j<grille->taille;j++)
                {
                    future_cases[i][j]=future_cases[i+1][(j-1+grille->taille)%grille->taille];
                }
            }
            break;
        /*Tout droit*/
        default:
            for(i=marge-1; i>=0;i--)
            {
                for(j=0;j<grille->taille;j++)
                {
                    future_cases[i][j]=future_cases[i+1][j];
                }
            }
            break;
    }

    /*Actualisation des cases*/
    /*Descente des valeurs actuel*/
    for(i=grille->nb_ligne-1;i>marge;i--)
    {
        for(j=0;j<grille->taille;j++)
        {
            grille->cases[i][j] = grille->cases[i-marge][j];
        }
    }

    /*Ajout valeur calculées*/
    for(i=0;i<marge+1;i++)
    {
        for(j=0;j<grille->taille;j++)
        {
            grille->cases[i][j] = future_cases[i][j];
        }
    }
}


void Grille_Destruction(grille_t ** grille)
{
    int i;
    for(i=0;i<(*grille)->nb_ligne;i++)
    {
        free((*grille)->cases[i]);
    }
    free((*grille)->cases);
    free(*grille);
    *grille=NULL;
}
/*
int estSurRoute(grille_t * grille, SDL_Rect voiture, int h)
{
    int ret = 1;
    int taille_carreau =  h/grille->taille;
    int i = voiture.x/taille_carreau;
    int j = h/2/taille_carreau;

    int i2 = (voiture.x + voiture.w)/taille_carreau;
    int j2 = (h/2 + voiture.h)/taille_carreau;

    if(grille->cases[j][i] == 0
        || grille->cases[j][i2] == 0
        || grille->cases[j2][i] == 0
        || grille->cases[j2][i2] ==0)
    {
        ret = 0;
    }
    return ret;
}
*/
int estSurRoute(grille_t * grille, SDL_Rect voiture, int h,int marge)
{
    int i,j;

    int ret = 1;

    int taille_carreau =  h/grille->taille;
    float centre_x = voiture.x + (float)voiture.w/2;
    float centre_y = voiture.y + (float)voiture.h/2;

    float dist = taille_carreau * 2;

    int i_x = centre_x/taille_carreau;
    int i_y = (centre_y + marge)/taille_carreau ;

    float centre_i;
    float centre_j;

    for(i= i_x-3; i<= i_x+3;i++)
    {
        for(j= i_y-3; j<= i_y+3;j++)
        {
            if(grille->cases[j][i] == 0)
            {
                centre_i = (i * taille_carreau) + ((float)taille_carreau * 0.5);
                centre_j = j * taille_carreau + (float)taille_carreau * 0.5;
                if(distance(centre_x, centre_y, centre_i, centre_j) < dist)
                {
                    ret = 0;
                }
            }
        }
    }

    return ret;
}

float distance(float x1, float y1, float x2, float y2)
{
    return sqrt((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2));
}