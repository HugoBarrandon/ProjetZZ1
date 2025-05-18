#include "apprentissage.h"

//==========initialisation de la Qtable, avec uniquement des 0, tous les chemin sont bons au début===========//
float ** Apprentissage_creer_tableQ()
{
    float ** tableau = (float **)malloc((ATK * DEF * PV * NBETG) * sizeof(float *) );
    int i,j; 

    if (tableau)
    {
        for (i=0;i<(ATK * DEF * PV * NBETG);i++)
        {
            tableau[i] = (float*)malloc(NBSALLE * sizeof(float));
            
            if (tableau[i])
            {
                for (j=0;j<NBSALLE;j++)
                {
                    tableau[i][j] = 0.0;
                    
                }
            }
            else 
            {   //=== en cas d'erreur on l'indique===//
                printf("erreur allocation ligne\n");
                for (j=i;j>-1;j--)
                {
                    free(tableau[j]);
                }
                free(tableau);
                break;
            }
            
        }
    }
    else 
    {
        printf("erreur allocation colonne\n");
    }
    return tableau;
}



//====libere la Q table une fois l'apprentissage terminé====// 
void Apprentissage_liberertableQ(float ** tableau)
{
    int  i = 0;
    for (i=0;i<(ATK * DEF * PV * NBETG);i++)
    {
        free(tableau[i]);

    }
    free(tableau);
}



// calcule la taille de la liste donné: on a mis des -1 partout dedans avant remplissage,on cherche donc ce -1 pour avoir la taille
int Apprentissage_taille(int liste[2 * NBETG])
{
    int i=0;
    while(i < 2 * NBETG && liste[i] != -1)
    {
        i+=1;
    }

    return i;
}



//================ parcour rétroactif(parcour droite gauche)===================//
void Apprentissage_Qupdate(float ** Qtableau1,float ** Qtableau2, int r,int * parcours)
{
    int i;
    int taille = Apprentissage_taille(parcours);
    int a;



    //====on gère le cas de la fin de la liste=====//


    a=rand()%2;
    if (a==0)
    {
        Qtableau1[parcours[taille-2]][parcours[taille-1]] += alpha * (r - Qtableau1[parcours[taille-2]][parcours[taille-1]]);
    }
    else
    {
        Qtableau2[parcours[taille-2]][parcours[taille-1]] += alpha * (r - Qtableau2[parcours[taille-2]][parcours[taille-1]]);
    }


    for(i=taille-3;i>0;i-=2)
    {  // on prend en compte les états suivant atteignables ainsi que leur "utilité" pour determiner la nv valeur de Q I,J

        a=rand()%2;
        if (a)
        {
            Qtableau1[parcours[i-1]][parcours[i]]+= alpha*(gamma*Qtableau2[parcours[i+1]][Apprentissage_choix2(Qtableau2, parcours[i+1])] - Qtableau1[parcours[i-1]][parcours[i]]);
        }
        else
        {
            Qtableau2[parcours[i-1]][parcours[i]]+= alpha*(gamma*Qtableau1[parcours[i+1]][Apprentissage_choix2(Qtableau1, parcours[i+1])] - Qtableau2[parcours[i-1]][parcours[i]]);
        }
                                                                   
    
    }
    /*recompence null sauf etat final: on utilise le score comme recompense*/
}




// ==== sert a déterminer en cas d'exploitation la valeur a prendre; ici simple, c'est le max(qui depend déja des états suivants par rétroaction)===////
int Apprentissage_choix(float ** Qtableau1,float** Qtableau2, int posx)        //frauduleux?
{
    int a = 0;
    int i = 1;

    for (i=1;i<NBSALLE;i++)
    {
        if ((Qtableau1[posx][a]+Qtableau2[posx][a]) < (Qtableau1[posx][a]+Qtableau2[posx][i])) a = i;
    }
    
    return(a);
}







int Apprentissage_choix2(float** Qtableau2, int posx)
{
    int a = rand()%NBSALLE;
    int i = 1;

    for (i=0;i<NBSALLE;i++)
    {
        if (Qtableau2[posx][a] < Qtableau2[posx][i]) a = i;
    }
    
    return(a);
}







//========permet tout simplement d'afficher la Q table si désiré une fois l'apprentissage terminé========//
void Apprentissage_AffichageQ(float ** Qtableau)
{
    int i,j;
    for (i=0;i<(ATK * DEF * PV * NBETG);i++)
    {
        for (j=0;j<NBSALLE;j++)
        {
            printf("%f ",Qtableau[i][j]);
                    // on va voir tkt if (i%PV == 0) tableau[i][j] = -1;
        }
        printf("\n");
    }
}






//============permet d'enregistrer la Q table apres apprentissage, afin de ne pas repartir de 0 si désiré=======//
void Apprentissage_save(float ** Qtableau,char * f)
{
    FILE * fichier = fopen(f,"w");
    int i,j;
    
    if (fichier)
    {
        for (i=0;i<(PV*ATK*NBETG*DEF);i++)
        {
            for (j=0;j<NBSALLE;j++)
            {
                fprintf(fichier,"%f ",Qtableau[i][j]);
            }
            fprintf(fichier,"\n");
        }
        
        
    fclose(fichier);
    }
}
// ============permet de cherger la Q table precedement enregistré  ==============//
void Apprentissage_load(float** Qtableau ,char * f)
{
    FILE * fichier = fopen(f,"r");
    int i,j;
    
    if (fichier)
    {
        for (i=0;i<(PV*ATK*NBETG*DEF);i++)
        {
            for (j=0;j<NBSALLE;j++)
            {
                fscanf(fichier, "%f" ,&Qtableau[i][j] );
            }
         
        }
        
        
    fclose(fichier);
    }
}



// ici on a codé différentes méthode de décroissance d'epsilon 




float apprentissage_eps1(float eps)// décroissance géometrique au bout de 100 000 ittération 30% d'incertitude
{
    return eps*0.99999;
}

float apprentissage_eps2(float nbiter)// décroissance 
{
    int a=10000;
    return(a/(nbiter+a));
}


float apprentissage_eps3(float nbiter,int nb_periode)// décroissance 
{    int a=10000;
    int b=100000;
    float c=0.00000000000000000000000000000001;
    float d= 10*logf( b + nbiter/c);
    float w;
    if (nbiter<nb_periode/2) w=a/(a + d) ;
    else
    {
        if (nbiter<(7/8)*nb_periode)  w = (-2.347*nbiter/nb_periode)+2.0935  ;
        else w=-0.24*nbiter/nb_periode + 0.25;
    }
    return w;
}

float apprentissage_eps4(float nbiter)
{
    return 1 - exp(nbiter - 10000);
}
