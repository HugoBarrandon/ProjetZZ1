#include "entite.h"

/*Renvois une entité avec les stats d'un joueur*/
entite_t * Entite_CreationJoueur(int pv_max, int atq_max, int def_max)
{
    entite_t * player = (entite_t*)malloc(sizeof(entite_t));
    if(player)
    {
        player->pv_max = pv_max;
        player->pv = player->pv_max;
        player->attaque_max = atq_max;
        player->attaque = 1;
        player->defense_max = def_max;
        player->defense = 0;
        player->score = 0;

        player->etat = IDLE;
        player->delay_between_frames = 10;
        player->delay_before_next_frame = player->delay_between_frames;
        player->current_frame = 0;
        player->speed = 15;

        player->position.x = 0;
        player->position.y = 0;


        player->destination.x = player->position.x;
        player->destination.y = player->position.y;
    }

    return player;
}


/*Renvois une entité avec les stats d'un enemis fixe*/
entite_t * Entite_CreationEnnemi(int etage)
{
    /*Stats des ennemis par étage format PV/Atq/Def*/
    int stats_ennemis[20][3] = {
        {2,1,0}, //1
        {2,1,0}, //2
        {2,1,1}, //3
        {2,1,1}, //4
        {3,1,1}, //5
        {3,2,2}, //6
        {4,2,2}, //7
        {4,2,3}, //8
        {4,2,3}, //9

        {4,3,5}, //10 // éééééénorme douille

        {5,2,3}, //11
        {5,2,3}, //12
        {5,2,3}, //13
        {5,2,3}, //14
        {5,3,3}, //15
        {6,3,3}, //16
        {6,3,3}, //17
        {6,3,3}, //18
        {6,4,3}, //19
        {5,4,5}, //20
    };
    entite_t * ennemi = (entite_t*)malloc(sizeof(entite_t));
    if(ennemi)
    {  
        ennemi->pv_max = stats_ennemis[etage][0];
        ennemi->pv = ennemi->pv_max;
        ennemi->attaque = stats_ennemis[etage][1];
        ennemi->defense = stats_ennemis[etage][2];
        ennemi->score = etage;
    }
    return ennemi;
}

/*Retourne le maximum de deux entier*/
int max(int a, int b)
{
    return (a<b) ? b : a;
}

/*Libère une entité*/
void Entite_Liberation(entite_t ** e)
{
    free(*e);
    *e=NULL;
}