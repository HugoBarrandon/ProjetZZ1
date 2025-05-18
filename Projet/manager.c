#include "manager.h"



/*Créer un manageur avec toutes les initialisations necessaire au jeu (notamment un manager_affichage_t et un joueur)*/
game_manager_t * Manager_CreationJeu()
{
    game_manager_t * gm = (game_manager_t*)malloc(sizeof(game_manager_t));
    if(gm)
    {
        gm->program_on = SDL_TRUE;
        gm->carte =  Carte_creation_salle(1);
        gm->affichage_m = Manager_CreationAffichage();
        gm->etat = DEBUT;
        gm->joueur = Entite_CreationJoueur(5,5,3);
        gm->choix = -1;
        gm->salle_courante = gm->carte;
        gm->nb_etage_max = 10;
        gm->nb_etage_courant = 0;
        gm->entrainement_IA = SDL_TRUE;
        gm->animations = SDL_FALSE;

        if(gm->carte == NULL || gm->affichage_m == NULL || gm->joueur == NULL)
        {
            Manager_DestroyJeu(&gm);
        }
        else
        {
            Carte_generer_salles(gm->carte);
            Manager_UpdateJoueur(gm);
        }

    }

    return gm;
}

/*Libère les élements d'un game_manager_t s'ils sont alloués et libère le game manager*/
void Manager_DestroyJeu(game_manager_t ** gm)
{
    if((*gm)->carte)
    {
        Carte_destruction_salles(&((*gm)->carte));
    }
    if((*gm)->affichage_m)
    {
        Manager_DestroyAffichage(&((*gm)->affichage_m));
    }
    if((*gm)->joueur)
    {
        Entite_Liberation(&((*gm)->joueur));
    }

    free(*gm);
    *gm=NULL;
}


/*Permet de choisir qu'est ce qui doit afficher et dans quel ordre suivant l'etat du manager*/
void Manager_AfficherJeu(game_manager_t * gm)
{
    SDL_RenderClear(gm->affichage_m->renderer);
    switch (gm->etat)
    {
        case DEBUT:
            Dessiner_ecran_debut(gm->affichage_m);
            break;
        case EN_JEU:
            Dessiner_Jeu(gm->affichage_m, gm->salle_courante, get_nb_salles(gm->salle_courante));
            Ecrire_Numero_Salle(gm->affichage_m,gm->nb_etage_courant, gm->nb_etage_max);
            Dessiner_joueur(gm->affichage_m, gm->joueur);
            Ecrire_Stats(gm->affichage_m, gm->joueur);
            if(!Mix_PlayingMusic())
            {
                Play_Switch_to_Principale(gm->affichage_m);
            }
            break;
        case FIN:
            Dessiner_ecran_fin(gm->affichage_m,gm->nb_etage_courant, gm->nb_etage_max, gm->joueur);
            if(!Mix_PlayingMusic())
            {
                Play_Switch_to_Principale(gm->affichage_m);
            }
            break;
    }
    SDL_RenderPresent(gm->affichage_m->renderer);
}


/*Permet de calculer l'etat suivant du jeu*/
void Manager_UpdateJeu(game_manager_t * gm)
{
    //Si un choix est fait, on l'applique
    if(gm->choix != -1 && gm->choix <= get_nb_salles(gm->salle_courante))
    {
        //On arrive dans la nouvelle salle
        gm->salle_courante = get_fils_by_index(gm->salle_courante,gm->choix);
        //On crée les fils (on ne crée les fils que des salles explorées pour gagner du temps et de l'espace)
        Carte_generer_salles(gm->salle_courante);
        //On applique les effets de la nouvelle salle au joueur
        Manager_AppliquerChoix(gm);
        //On regarde si il est toujours en vie
        if(gm->joueur->pv <= 0)     //Si non alors on termine la partie en cours et on donne un score négatif basé sur le nombre d'étage restant
        {
            gm->joueur->pv = 0;
            gm->joueur->score = -(10-gm->nb_etage_courant);
            gm->etat = FIN;
        }
        else                         //Si oui alors on note qu'on a avancé à la salle suivante 
        {
            gm->nb_etage_courant++;
        }

        //On reset la variable pour montrer que le choix a été pris en compte
        gm->choix = -1;
        //Necessaire pour repositionner le joueur dans la salle de gauche
        //et être sur que tout soit pret pour le prochain choix (dans le cas de l'IA qui envois un choix à chaque tout de boucle)
        Manager_UpdateJoueur(gm);

    }
    //Test si on est arrivé à la fin
    if(gm->nb_etage_courant == gm->nb_etage_max)
    {
        gm->etat = FIN;
    }

}


/*Permet de calculer l'etat suivant du joueur*/
void Manager_UpdateJoueur(game_manager_t * gm)
{
    SDL_Rect surface_case = {0};
    SDL_Rect surface_joueur = {0};
    SDL_Point point = {0};
    float velocity_x = 0.0;
    float velocity_y = 0.0;

    float norme;

    int h;
    int w;
    float marge_y = ((float)1/4);

    //On obtient les tailles pour définir les positions
    SDL_GetWindowSize(gm->affichage_m->window, &w,&h);

    SDL_QueryTexture(gm->affichage_m->textures_salles, NULL, NULL,
                   &surface_case.w, &surface_case.h);
    SDL_QueryTexture(gm->affichage_m->textures_joueur, NULL, NULL,
                   &surface_joueur.w, &surface_joueur.h);

    //Si un choix est fait on joue l'animation de déplacement vers la bonne salle
    if(gm->choix != -1 && gm->animations)
    {
        gm->joueur->etat = WALK;
        /*Destination*/
        point.x=(float)w*0.7;
        point.y=(float)h*marge_y*(gm->choix);

        gm->joueur->destination.x = point.x - (surface_case.w*0.05);
        gm->joueur->destination.y = point.y;
    }

    //Si on est en train de se déplacer on met à jour la position
    if(gm->joueur->etat == WALK)
    {
        velocity_x = gm->joueur->destination.x - gm->joueur->position.x;
        velocity_y = gm->joueur->destination.y - gm->joueur->position.y;
        norme = sqrt(velocity_x*velocity_x + velocity_y*velocity_y);
        if(norme > 10)
        {
            velocity_x = (velocity_x/norme) * gm->joueur->speed;
            velocity_y = (velocity_y/norme) * gm->joueur->speed;
        }
        gm->joueur->position.x += velocity_x;
        gm->joueur->position.y += velocity_y;
        if(velocity_x == 0.0 && velocity_y == 0.0)
        {
            gm->joueur->etat = IDLE;


            gm->joueur->destination.x = gm->joueur->position.x;
            gm->joueur->destination.y = gm->joueur->position.y;
        }
    }
    //Sinon on s'assure que le joueur soit au centre de la salle de gauche
    else
    {
        point.x=(w*0.15) + surface_case.w/8;
        point.y=(h/2) - 50;

        gm->joueur->position.x = point.x ;
        gm->joueur->position.y = point.y - surface_joueur.h/6;
    }
}


/*Permet de reset les variables pour relancer une run quand on a atteind le 10eme étage*/
void Manageur_ReInitGame(game_manager_t ** gm)
{

    /*Reset etage*/
    (*gm)->nb_etage_courant = 0;

    /*Reset joueur*/
    Entite_Liberation(&((*gm)->joueur));
    (*gm)->joueur = Entite_CreationJoueur(5,5,3);

    /*Reset carte*/
    Carte_destruction_salles(&((*gm)->carte));
    (*gm)->carte = Carte_creation_salle(1);
    if((*gm)->carte)
    {
        (*gm)->salle_courante = (*gm)->carte;
        Carte_generer_salles((*gm)->carte);
    }


    /*On passe en jeu*/
    (*gm)->etat = EN_JEU;

    if((*gm)->carte == NULL || (*gm)->joueur == NULL)
    {
        Manager_DestroyJeu(gm);
    }

}

/*Retourne le nombre de fils direct de la salle passé en entrée*/
int get_nb_salles(salle_t * pere)
{
    int nb=0;

    salle_t * cour = pere->fils;
    while(cour != NULL)
    {
        cour = cour->frere;
        nb++;
    }

    return nb;
}


/*Retourne un pointeur sur le fils numero index du père (liste chainée de fils donc pas d'accès direct)*/
salle_t * get_fils_by_index(salle_t * pere, int index)
{
    int index_cour = 0;
    salle_t * cour = pere->fils;
    while(index_cour != index && cour!= NULL)
    {
        index_cour++;
        cour = cour->frere;
    }
    if(index_cour != index) //index plus grand que nombre de fils
    {
        cour = NULL;
    }
    return cour;
}


/*Applique l'effet de la salle dans laquelle on arrive*/
void Manager_AppliquerChoix(game_manager_t * gm)
{
    switch (gm->salle_courante->type)
    {
        case ENNEMI:
            Manager_Combat(gm);
            break;
        case SOIN:
            gm->joueur->pv = min(gm->joueur->pv+2, gm->joueur->pv_max);
            break;
        case ATTAQUE:
            gm->joueur->attaque = min(gm->joueur->attaque+1, gm->joueur->attaque_max);
            break;
        case DEFENSE:
            gm->joueur->defense = min(gm->joueur->defense+1, gm->joueur->defense_max);
            break;
        default:
            printf("Qu'est ce que tu as foutu avec le type des cases ??\n");
            break;
    }
}

/*Retourne le minimum de deux entier*/
int min(int a, int b)
{
    return (a<b) ? a : b;
}

/*Simule un combat simple entre le joueur et l'ennemi de l'etage*/
/*Le combat est simple : le joueur tape, si l'ennemis a survécu il tape en retour et ainsi jusqu'à que l'un des deux à 0 pv ou moins*/
void Manager_Combat(game_manager_t * gm)
{
    entite_t * ennemi = Entite_CreationEnnemi(gm->nb_etage_courant);
    while(gm->joueur->pv >0 && ennemi->pv >0)
    {
        ennemi->pv -= max(1, gm->joueur->attaque - ennemi->defense ); //Au moins 1 de dégat
        if(ennemi->pv > 0)
        {
            gm->joueur->pv -= max(1, ennemi->attaque - gm->joueur->defense ); //Au moins 1 de dégat
        }
    }
    if(ennemi->pv <= 0)
    {
        gm->joueur->score += ennemi->score;
        Entite_Liberation(&ennemi);
    }
}