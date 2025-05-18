#include "manager.h"
#include "apprentissage.h"
#include <time.h>
#include <string.h>

/*Libère tout ce qui est alloué*/
int Destroy(game_manager_t ** gm)
{

    Manager_DestroyJeu(gm);
    Mix_CloseAudio();
    SDL_AudioQuit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}

/*Initialise tout ce qui est necessaire au jeu*/
int Init(game_manager_t ** gm)
{
    int code_ret = 0;

    /*Init SDL*/
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) 
    {
        SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if(TTF_Init())
    {
        SDL_Log("Error : TTF initialisation - %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0) //Initialisation de l'API Mixer
    {
        SDL_Log("Error : Music initialisation - %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    *gm = Manager_CreationJeu();
    
    if(*gm == NULL)
    {
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    return code_ret;
}
//======== boucle des commandes recues en mode joueur===///
void Handle_Input(game_manager_t * gm)
{
    SDL_Event event;
    /*On n'utilise pas la souris donc pour gagner du temps on retire tout ce qui est déplacement de souris car il peut y en avoir beaucoup et les traiter peut prendre du temps*/
    SDL_FlushEvent(SDL_MOUSEMOTION);

    while(SDL_PollEvent(&event))
    {
        switch (event.type) 
        {
            case SDL_QUIT:
                gm->program_on = SDL_FALSE;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) 
                {
                    case SDLK_SPACE:
                        if(gm->etat == DEBUT || gm->etat == FIN)
                        {
                            if(gm->etat == DEBUT)
                            {
                                Play_Switch_to_Principale(gm->affichage_m);
                            }
                            Manageur_ReInitGame(&gm);
                        }
                        break;
                    case SDLK_f:    //Pour tests
                        if(gm->etat == EN_JEU)
                        {
                            gm->etat = FIN;
                        }
                        break;
                    case SDLK_q:
                        gm->program_on = SDL_FALSE;
                        break;
                    case SDLK_ESCAPE:
                        gm->program_on = SDL_FALSE;
                        break;
                    case SDLK_KP_1:    //Pour tests
                        if(gm->etat == EN_JEU && gm->joueur->etat == IDLE && !gm->entrainement_IA)
                        {
                            gm->choix = 0;
                        }
                        break;
                    case SDLK_KP_2:    //Pour tests
                        if(gm->etat == EN_JEU && gm->joueur->etat == IDLE && !gm->entrainement_IA)
                        {
                            gm->choix = 1;
                        }
                        break;
                    case SDLK_KP_3:    //Pour tests
                        if(gm->etat == EN_JEU && gm->joueur->etat == IDLE && !gm->entrainement_IA)
                        {
                            gm->choix = 2;
                        }
                        break;
                    case SDLK_KP_4:    //Pour tests
                        if(gm->etat == EN_JEU && gm->joueur->etat == IDLE && !gm->entrainement_IA)
                        {
                            gm->choix = 3;
                        }
                        break;
                    case SDLK_KP_5:    //Pour tests
                        if(gm->etat == EN_JEU && gm->joueur->etat == IDLE && !gm->entrainement_IA)
                        {
                            gm->choix = 4;
                        }
                        break;
                }
                break;
            case SDL_KEYUP:
                break;
            default:                                      // Les evenements qu'on n'a pas envisage
                break; 
        }
    } 
}

void LectureParam(game_manager_t * gm, int argc, char ** argv, float * eps, int * nb_periode, int * choix_epsilon)
{
    int i=0;
    int seed = 0;
    while(i < argc)
    {
        if(strcmp(argv[i], "-a") == 0)
        {
            gm->animations = SDL_TRUE;
        }
        if(strcmp(argv[i], "-p") == 0)
        {
            gm->entrainement_IA = SDL_FALSE;
            gm->animations = SDL_TRUE;
        }
        if(strcmp(argv[i], "-s") == 0)
        {
            i++;
            seed = atoi(argv[i]);
        }
        if(strcmp(argv[i], "-eps") == 0)
        {
            i++;
            *eps = atof(argv[i]);
        }

        if(strcmp(argv[i], "-nbp") == 0)
        {
            i++;
            *nb_periode = atof(argv[i]);
        }
        if(strcmp(argv[i], "-deps") == 0)
        {
            i++;
            *choix_epsilon = atof(argv[i]);
        }
        i++;
    }
    /*Init random*/
    if(seed == 0)
    {
        seed = time(NULL);
        printf("Seed : %d\n",seed);
    }
    srand( seed );

}


int main(int argc, char ** argv)
{

    game_manager_t * gm; //Game Manager qui contient l'état du jeu

    clock_t debut;                      // pour que tous les tours dure la meme durée
    clock_t fin;
    float millis;
    int i = 0,j = 0;
    int a = 0;

    int choix_epsilon=1;                 //choix de l'agorithme de décrossance d'epsilon

    float eps = 1;
    char * nomfichier1= "save1.txt";
    char * nomfichier2="save2.txt";

    int ia_arret=1;
    int nb_periode = 1000000;
  

    float ** Qtableau1=Apprentissage_creer_tableQ(); // init la Q table de l'ia
    float ** Qtableau2=Apprentissage_creer_tableQ(); // init la Q table  2
  
    int parcours[2*NBETG];//tab des etas parcourus;
    for(j=0;j<(NBETG*2);j++) parcours[j]=-1 ; // on init le parcour a -1 pour pouvoir trouver la fin en cas de decès prématuré
    

    Init(&gm);
    Apprentissage_load(Qtableau1,"save1.txt");
    Apprentissage_load(Qtableau2,"save2.txt");


    Play_Intro(gm->affichage_m);
    LectureParam(gm, argc, argv, &eps, &nb_periode, &choix_epsilon);

    if(gm->entrainement_IA) //Lancement du jeu si IA s'entraine
    {
        gm->etat = EN_JEU;
        Play_Switch_to_Principale(gm->affichage_m);
    }
    
//=========================================================================================================//
    /*Boucle de jeu*/


    while (gm->program_on)
    {
        debut = clock();

        

        Handle_Input(gm);

        Manager_UpdateJoueur(gm);


        if(gm->joueur->etat == IDLE) //Attente fin animation si active
        {
            Manager_UpdateJeu(gm);

            //Cas IA
            if(gm->entrainement_IA)
            {
                if(gm->etat == EN_JEU) //cas jeu tourne
                {
                    //==on met l'IA ici================//
                    parcours[i] = (gm->nb_etage_courant*(PV*DEF*ATK)+ (gm->joueur->attaque-1)*(DEF*PV) + gm->joueur->defense*(PV) + gm->joueur->pv  );
                    
                    /*pour la ligne du dessus on remplit la liste parcours avec la position x dans la tableQ. les plus gros paragraphes de la table sont le numero de l'etage (de 0 a 9)
                    puis l'attaque (de 1 a 5, on retire 1 pour aller de 0 a 4 et ainsi ne pas sauter les lignes du tableau) puis la defense (de 0 a 3) et enfin les PV (de 0 a 5) */

                    a=rand()%100;  // choix selon le eps greedy
                    if (a<eps*100)
                    {
                        gm->choix = rand()%4; // exploration
                            
                    } 
                    else 
                    {
                        gm->choix=Apprentissage_choix(Qtableau1,Qtableau2,parcours[i]);  // exploitation
                    }

                    parcours[i+1] = gm->choix;
                    i=i+2;
                    /*Fin d'une époque*/
                    if(gm->etat == FIN)
                    {
                        
                    }
                }
                else //On relance la run
                {
                    //printf("%d : %d\t%f\n", ia_arret, gm->joueur->score,eps);

                    Apprentissage_Qupdate( Qtableau1,Qtableau2, gm->joueur->score, parcours); // on apprend rétrospectivement
                    ia_arret+=1;
                    if (ia_arret>nb_periode )
                    {

                        Apprentissage_save(Qtableau1,nomfichier1);
                        Apprentissage_save(Qtableau2,nomfichier2);
                        gm->program_on = SDL_FALSE;
                    }
                    else //On relance une époque
                    {
                        i=0;
                        for(j=0;j<(NBETG*2);j++)
                        {
                            parcours[j]=-1 ;
                        }
                        if (ia_arret%100==0)
                        {
                            printf("%d : %d\t%f\n", ia_arret, gm->joueur->score,eps);
                        }
                        Manageur_ReInitGame(&gm);
                        
                        switch (choix_epsilon)
                        {
                            case 1:
                                eps=apprentissage_eps1(eps);
                                break;
                            case 2:
                                eps=apprentissage_eps2(ia_arret);
                                break;
                            case 3:
                                eps=apprentissage_eps3(ia_arret,nb_periode);
                                break;
                            case 4:
                                eps=apprentissage_eps4(ia_arret);
                                break;
                            default:
                                break;
                        }

                        
                    }
                }
                
            }         
        }

        fin = clock();
        if(gm->animations)
        {
            Manager_AfficherJeu(gm);
                    
            millis = (fin - debut) * ((float)1000)/CLOCKS_PER_SEC;
            SDL_Delay(16 - millis); // 1sec/60 -> 16.6 ms
        }
    }
    
    //Apprentissage_AffichageQ(Qtableau);
    Apprentissage_liberertableQ(Qtableau1);
    Apprentissage_liberertableQ(Qtableau2);


    Destroy(&gm);// on libère et détruit ce qui ne sert plus 
   
    return 0;
}
