#include "markov.h"


//int mart=3;
//int matmarkov[mart][mart]=[[50,25,25],[70,30,0],[70,0,30]];






//====pour avancer d'un état====//
int markov(int etat){
 
    int x;
    srand( time( NULL ) );
    int r=rand()%100;
    if (etat == 0) // on est tout droits
    {
        
        if (0<=r && r<31) x=0;
        if (31<=r && r<66) x=-1;
        if (66<=r && r<100) x=1;
    }
    if (etat == 1)    // on est  a  droite
    {
           
        if (0<=r && r<71) x=0;
        if (71<=r && r<100) x=1;
    }
    if(etat == -1)           // on est a aguche
    {
        if (0<=r && r<71) x=0;
        if (71<=r && r<100) x=-1;
    }


    return x;
}



// ====markov nfois====//
int* markovn(int etat, int nb)
{
    int tab[nb];
    for(int i=0;i<nb;i++)
    {
        etat=markov(etat);
        tab[i]=etat;
    }

    return tab;
}

