#include "affichage.h"

/*Allocation et chargement des textures dans la structure*/
affichage_manager_t * Manager_CreationAffichage()
{

    SDL_DisplayMode screen;
    affichage_manager_t * am = (affichage_manager_t *)malloc(sizeof(affichage_manager_t));

    if(am)
    {
        SDL_GetCurrentDisplayMode(0, &screen);
        am->window = SDL_CreateWindow("Hello there",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                1536 ,
                                824,
                                SDL_WINDOW_OPENGL);

        am->renderer = SDL_CreateRenderer(am->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        am->texture_debut = IMG_LoadTexture(am->renderer,"./images/ecrandebut.png");
        am->textures_joueur = IMG_LoadTexture(am->renderer,"./images/heros.png");
        am->textures_salles = IMG_LoadTexture(am->renderer,"./images/tile.pnj");
        am->textures_joueur = IMG_LoadTexture(am->renderer,"./images/heros.png");
        am->background = IMG_LoadTexture(am->renderer,"./images/Openbook.png");

        am->basic_font = TTF_OpenFont("./polices/Pacifico.ttf", 50);

        am->musique_intro = Mix_LoadMUS("./musiques/intro.mp3");
        am->musique_principale = Mix_LoadMUS("./musiques/principale.mp3");

        if( am->renderer == NULL ||
            am->window == NULL ||
            am->texture_debut == NULL ||
            am->textures_joueur == NULL ||
            am->textures_salles == NULL ||
            am->background == NULL ||
            am->basic_font == NULL ||
            am->musique_intro == NULL)
        {
            printf("Echec chargement textures ou polices ou musique\n");
            Manager_DestroyAffichage(&am);
        }
    }

    return am;
}

/*Libération de tous les elements de la structure s'ils existent et la structure*/
void Manager_DestroyAffichage(affichage_manager_t ** am)
{
    if((*am)->texture_debut)
    {
        SDL_DestroyTexture((*am)->texture_debut);
    }
    if((*am)->textures_joueur)
    {
        SDL_DestroyTexture((*am)->textures_joueur);
    }
    if((*am)->textures_salles)
    {
        SDL_DestroyTexture((*am)->textures_salles);
    }
    if ((*am)->background)
    {
          SDL_DestroyTexture((*am)->background);
    }
    if ((*am)->background)
    {
          TTF_CloseFont((*am)->basic_font);
    }

    if ((*am)->musique_intro)
    {
        Mix_FreeMusic((*am)->musique_intro);
    }
    if ((*am)->musique_principale)
    {
        Mix_FreeMusic((*am)->musique_principale);
    }

    if ((*am)->renderer)
    {
        SDL_DestroyRenderer((*am)->renderer);
        (*am)->renderer = NULL;
    }
    if ((*am)->window)
    {
        SDL_DestroyWindow((*am)->window);
        (*am)->window= NULL;
    }

    free(*am);
    *am=NULL;

}



/*Affichage la texture de début de jeu*/
void Dessiner_ecran_debut( affichage_manager_t * am) 
{
     SDL_Texture * my_texture = am->texture_debut;
     SDL_Rect 
          source = {0},                         // Rectangle définissant la zone de la texture à récupérer
          window_dimensions = {0},              // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
          destination = {0};                    // Rectangle définissant où la zone_source doit être déposée dans le renderer

     SDL_GetWindowSize(
      am->window, &window_dimensions.w,
      &window_dimensions.h);                    // Récupération des dimensions de la fenêtre
     SDL_QueryTexture(my_texture, NULL, NULL,&source.w, &source.h);       // Récupération des dimensions de l'image

     destination.x = window_dimensions.w/6;
     destination.y = 0;
     destination.w = window_dimensions.w*2/3;
     destination.h = window_dimensions.h;

     SDL_RenderCopy(am->renderer, my_texture,&source,&destination);

}


/*Affichage la texture de fin de jeu et ecrit les stats*/
void Dessiner_ecran_fin(affichage_manager_t * am, int salle_c,int salle_total, entite_t * joueur) 
{

    Dessiner_fond(am);

//Affichage Stats   
    int gris = 0;
    char texte[100];
    sprintf(texte, "Salle : %d/%d\nPV : %d/%d\nAtq : %d\nDef : %d", salle_c, salle_total, joueur->pv, joueur->pv_max, joueur->attaque, joueur->defense);


    SDL_Rect pos = {50,30, 0, 0};
    SDL_Rect window_size = {0};

    SDL_Color color = {gris, gris, gris, 255};
    SDL_Texture* text_texture = NULL; 
    SDL_Surface* text_surface = TTF_RenderText_Blended_Wrapped(am->basic_font, texte, color,300);
    if(text_surface)
    {
        text_texture = SDL_CreateTextureFromSurface(am->renderer, text_surface);
        if(text_texture)
        {
            SDL_QueryTexture(text_texture, NULL, NULL, &pos.w, &pos.h);
            SDL_GetWindowSize(am->window,&window_size.w, &window_size.h);
            pos.x = (window_size.w - pos.w)/4;
            pos.y = (window_size.h - pos.h)/3;
            SDL_RenderCopy(am->renderer, text_texture, NULL, &pos);
            SDL_DestroyTexture(text_texture); 
        }
        SDL_FreeSurface(text_surface);
    }

//Affichage Score
    sprintf(texte, "Score :\n%d", joueur->score);
    text_texture = NULL; 
    text_surface = TTF_RenderText_Blended_Wrapped(am->basic_font, texte, color,200);
    if(text_surface)
    {
        text_texture = SDL_CreateTextureFromSurface(am->renderer, text_surface);
        if(text_texture)
        {
            SDL_QueryTexture(text_texture, NULL, NULL, &pos.w, &pos.h);
            SDL_GetWindowSize(am->window,&window_size.w, &window_size.h);
            pos.x = (window_size.w - pos.w)*3/4;
            pos.y = (window_size.h - pos.h)/3;
            SDL_RenderCopy(am->renderer, text_texture, NULL, &pos);
            SDL_DestroyTexture(text_texture); 
        }
        SDL_FreeSurface(text_surface);
    }
}


/*Dessine une salle en particulier*/
/*Input:
    zoo : le zoom (permet d'avoir deux tailles differentes pour la salle de gauche et celles de droites)
    coord : coordonnées où doit etre affiché la salle
    type : le type de la salle pour savoir quel sprite afficher
*/
void Dessiner_salle(float zoom, affichage_manager_t * am ,SDL_Point coord, int type ) {
     SDL_Rect 
           source = {0},                      // Rectangle définissant la zone de la texture à récupérer
           window_dimensions = {0},           // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
           destination = {0};                 // Rectangle définissant où la zone_source doit être déposée dans le renderer
        SDL_Rect state = {0};

     SDL_GetWindowSize(am->window, &window_dimensions.w,&window_dimensions.h); // Récupération des dimensions de la fenêtre
     SDL_QueryTexture(am->textures_salles, NULL, NULL,&source.w, &source.h);  // Récupération des dimensions de l'image

     int offset_y = source.h / 4;
    
    state.x = 0;
    state.y = type * offset_y;
    state.w = source.w;
    state.h = offset_y;

     destination.w = state.w * zoom;         // La destination est un zoom de la source
     destination.h = state.h * zoom;         // La destination est un zoom de la source
     destination.x =coord.x;                  // La destination :  x et y
     destination.y =coord.y;                   // La destination

     SDL_RenderCopy(am->renderer, am->textures_salles,&state,&destination);            
    
}



/*Dessine le livre*/
void Dessiner_fond( affichage_manager_t * am )
{
    SDL_Rect 
          source = {0},                         // Rectangle définissant la zone de la texture à récupérer
          window_dimensions = {0},              // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
          destination = {0};                    // Rectangle définissant où la zone_source doit être déposée dans le renderer

  SDL_GetWindowSize(
      am->window, &window_dimensions.w,
      &window_dimensions.h);                    // Récupération des dimensions de la fenêtre
  SDL_QueryTexture(am->background, NULL, NULL,
                   &source.w, &source.h);       // Récupération des dimensions de l'image

  destination = window_dimensions;              // On fixe les dimensions de l'affichage à  celles de la fenêtre

  /* On veut afficher la texture de façon à ce que l'image occupe la totalité de la fenêtre */

  SDL_RenderCopy(am->renderer, am->background,
                 &source,
                 &destination);
}



/*Dessine l'ensemble des sprites necessaire quand le jeu est dans l'état "EN_JEU"*/
void Dessiner_Jeu( affichage_manager_t * am,salle_t* listsalle,int nb_salle)
{
    float zoom_g=0.6;
    float zoom_d=0.20;

    SDL_RenderClear(am->renderer);
    SDL_Rect source = {0};
    SDL_Point centre_gauche = {0};

    SDL_Point centre_courant = {0};

    salle_t* courant=listsalle;
    SDL_Point point;

    int h;
    int w;
    float marge_y = (1/(float)nb_salle);
    SDL_GetWindowSize(am->window, &w,&h);

//Dessine le livre
    Dessiner_fond(am);

//Dessine les salles de droites
    SDL_QueryTexture(am->textures_salles, NULL, NULL,
                   &source.w, &source.h);
    //Calcule du centre de la salle de gauche pour tracer les lignes
    point.x=(w*0.15);
    point.y=(h/2)-(source.h/8)*zoom_g;
    courant=courant->fils;

    centre_gauche.x = point.x + (source.w/2)*zoom_g;
    centre_gauche.y = point.y + (source.h/8)*zoom_g;

    for(int i=0;i<nb_salle;i++)
    {
    //Calcule des coordonées de la salle courante de droite
        point.x=(float)w*0.7;
        point.y=(float)h*marge_y*i + h*marge_y/8;

        centre_courant.x = point.x + (source.w*zoom_d)/2;
        centre_courant.y = point.y + (source.h/4*zoom_d)/2;

    //Dessine les traits pour "relier" la salle de gauche à celles de droites
        SDL_SetRenderDrawColor(am->renderer, 0, 0, 0, 255);                   
        SDL_RenderDrawLine(am->renderer,centre_gauche.x,centre_gauche.y,centre_courant.x,centre_courant.y);
    //Dessine la salle de droite par dessus le trait et pas l'inverse
        Dessiner_salle(zoom_d,am,point,courant->type);
        courant=courant->frere;
    }
//Dessine la salle de gauche (par dessus les traits)
    point.x=(w*0.15);
    point.y=(h/2)-(source.h/8)*zoom_g;
    Dessiner_salle(zoom_g,am,point, listsalle->type);
}



/*Ecrit le numero de la salle courrante pour savoir où on en est*/
void Ecrire_Numero_Salle(affichage_manager_t * am,int salle_c, int salle_total)
{
    int gris = 0;
    char nb_salle[20];
    sprintf(nb_salle, "Salle : %d/%d", salle_c, salle_total);

    SDL_Color color = {gris, gris, gris, 255};
    SDL_Texture* text_texture = NULL; 
    SDL_Surface* text_surface = TTF_RenderText_Blended(am->basic_font, nb_salle, color);
    if(text_surface)
    {
        text_texture = SDL_CreateTextureFromSurface(am->renderer, text_surface);
        if(text_texture)
        {
            SDL_Rect pos = {350,730, 0, 0};
            SDL_QueryTexture(text_texture, NULL, NULL, &pos.w, &pos.h);
            SDL_RenderCopy(am->renderer, text_texture, NULL, &pos);
            SDL_DestroyTexture(text_texture); 
        }
        SDL_FreeSurface(text_surface);
    }
}



/*Dessine le sprite du joueur en fonction de sa position, son etat et sa frame (contenu dans la structure entite_t)*/
void Dessiner_joueur(affichage_manager_t * am, entite_t * joueur)
{
    int ligne = 0;
    
    SDL_Rect 
             source = {0},                    // Rectangle définissant la zone totale de la planche
             destination = {0},               // Rectangle définissant où la zone_source doit être déposée dans le renderer
             state = {0};
    
    SDL_QueryTexture(am->textures_joueur,
                        NULL, NULL,
                        &source.w, &source.h);

    int nb_images_par_ligne = 7;
    int nb_ligne = 5;
    float zoom = 2;
    int offset_x = source.w / nb_images_par_ligne,
           offset_y = source.h / nb_ligne;

    //Actualise la frame, on change de frame tous les "delay_between_frames" tour de boucle
    joueur->delay_before_next_frame--;
    if(joueur->delay_before_next_frame<=0)
    {
        joueur->current_frame++;
        joueur->delay_before_next_frame = joueur->delay_between_frames;
    }

    //On choisi la ligne à afficher en fonction de l'état
    switch(joueur->etat)
    {
        case IDLE:
            joueur->current_frame%=4; //Idle est animé sur 4 frames
            ligne = 0; //L'animation IDLE est sur la 1ère ligne de l'image
            break;
        case WALK:
            joueur->current_frame%=14; //Walk est animé sur 14 frames
            ligne = 3 + joueur->current_frame/7; //Walk est sur 2 lignes (la 4eme et la 5eme)
            joueur->delay_before_next_frame--; //On donne un effet de vitesse en passant le sprites plus rapidement
            break;
    }
    
    state.x = (joueur->current_frame%7) * offset_x;
    state.y = ligne * offset_y;
    state.w = offset_x;
    state.h = offset_y;

    destination.w = offset_x * zoom;
    destination.h = offset_y * zoom;

    destination.x = joueur->position.x;
    destination.y = joueur->position.y;

    SDL_RenderCopy(am->renderer, am->textures_joueur,
                        &state,
                        &destination);
}



/*Ecrit les stats du joueur pour savoir où on en est*/
void Ecrire_Stats(affichage_manager_t * am,entite_t * joueur)
{
    int gris = 0;
    char stats[40];
    sprintf(stats, "PV : %d/%d\tAtq : %d\tDef : %d", joueur->pv, joueur->pv_max, joueur->attaque, joueur->defense);

    SDL_Color color = {gris, gris, gris, 255};
    SDL_Texture* text_texture = NULL; 
    SDL_Surface* text_surface = TTF_RenderText_Blended(am->basic_font, stats, color);
    if(text_surface)
    {
        text_texture = SDL_CreateTextureFromSurface(am->renderer, text_surface);
        if(text_texture)
        {
            SDL_Rect pos = {250,30, 0, 0};
            SDL_QueryTexture(text_texture, NULL, NULL, &pos.w, &pos.h);
            SDL_RenderCopy(am->renderer, text_texture, NULL, &pos);
            SDL_DestroyTexture(text_texture); 
        }
        SDL_FreeSurface(text_surface);
    }
}

void Play_Intro(affichage_manager_t * am)
{
    Mix_PlayMusic(am->musique_intro, 0);
}

void Play_Switch_to_Principale(affichage_manager_t * am)
{
    if(Mix_PlayingMusic())
    {
        Mix_HaltMusic();
    }
    Mix_PlayMusic(am->musique_principale, 0);
}
