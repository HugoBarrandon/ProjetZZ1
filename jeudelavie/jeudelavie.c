#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


<<<<<<< HEAD
#define MAX 25
=======
#define MAX 35
>>>>>>> Oeuvre_Grille

const int survie[9] = {0,0,1,1,0,0,0,0,0};
const int naissance[9] = {0,0,0,1,0,0,0,0,0};



int max(int a, int b)
{
    return (a>b)? a:b;
}

int min(int a, int b)
{
    return (a<b)? a:b;
}

void initGrille(int grille[MAX][MAX])
{
    int i,j;
    for (i=0;i<MAX;i++)
    {
        for (j=0;j<MAX;j++)
        {
            grille[i][j] = 0;
        }
    }
}

void afficherGrille(int grille[MAX][MAX])
{
    int i,j;
    for (i=0;i<MAX;i++)
    {
        for (j=0;j<MAX;j++)
        {
            printf("%d ",grille[i][j]);
        }
        printf("\n");
    }
}

void dessin(SDL_Renderer * renderer, int grille[MAX][MAX],int h)
{
    SDL_Rect rectangle;
    
    int i,j;
    int carreau = h/MAX;

    for (i=0;i<MAX;i++)
    {
        for (j=0;j<MAX;j++)
        {
            

            rectangle.x = i * carreau;                                          
            rectangle.y = j * carreau;                                                  
            rectangle.w = carreau;                                                
            rectangle.h = carreau;

           

            if (grille[i][j] == 0)
            {
                SDL_SetRenderDrawColor(renderer,255, 255, 255,255);            
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0,255);   
            }
            SDL_RenderFillRect(renderer, &rectangle);
        }
 
    }
    
}

int nbvoisin(int grille[MAX][MAX], int x, int y)
{
    int nb = 0;
    int i,j;
    
    for (i=max(0,x-1);i<=min(MAX-1,x+1);i++)
    {
        for (j=max(0,y-1);j<=min(MAX-1,y+1);j++)
        {
            nb += grille[i][j];
        }
    }
    nb-= grille[x][y];

    return nb;
}

int nbvoisinTorique(int grille[MAX][MAX], int x, int y)
{
    int nb = 0;
    int i,j;
    
    for (i=x-1;i<=x+1;i++)
    {
        for (j=y-1;j<=y+1;j++)
        {
            nb += grille[(i+MAX)%MAX][(j+MAX)%MAX];
        }
    }
    nb-= grille[x][y];

    return nb;
}


void iter(int grille[MAX][MAX], SDL_bool tor)
{
    int grilletemp[MAX][MAX];
    int i,j;
    int diff=1;
    int nbvois = 0;

    for (i=0;i<MAX;i++)
    {
        for (j=0;j<MAX;j++)
        {
            if (tor)                // grille torique ou non
            {
                nbvois = nbvoisinTorique(grille, i, j);              
            } 
            else
            {
                nbvois = nbvoisin(grille, i, j);
            }   

            // maintenant on remplit le nv tableau

            if (grille[i][j] ==1)
            {
                grilletemp[i][j]=survie[nbvois];
            }
            else
            {
                grilletemp[i][j]=naissance[nbvois];
            }
        }
    }

  
    for (i=0;i<MAX;i++)
    {
        for (j=0;j<MAX;j++)
        {
            if(grille[i][j] != grilletemp[i][j])
            {
                diff=0;
                grille[i][j] = grilletemp[i][j];
            
            }


            
        }
    }

    if (diff==1)
    {
       printf("etat stable\n");
    }
}

void changerCarreau(int grille[MAX][MAX], int x, int y, int h)
{
    int carreau = h/MAX;
    int i,j;
    
    i = x/carreau;
    j = y/carreau;
    grille[i][j] = (grille[i][j] + 1)%2;
}

void save(int grille[MAX][MAX],char * f)
{
    FILE * fichier = fopen(f,"w");
    int i,j;
    
    if (fichier)
    {
        for (i=0;i<MAX;i++)
        {
            for (j=0;j<MAX;j++)
            {
                fprintf(fichier,"%d ",grille[i][j]);
            }
            fprintf(fichier,"\n");
        }
        
        
    fclose(fichier);
    }
}


void load(int grille[MAX][MAX],char * f)
{
    FILE * fichier = fopen(f,"r");
    int i,j;
    
    if (fichier)
    {
        for (i=0;i<MAX;i++)
        {
            for (j=0;j<MAX;j++)
            {
                fscanf(fichier, "%d" ,&grille[i][j] );


            }
         
        }
        
        
    fclose(fichier);
    }
}

int main(int argc, char ** argv)
{
    int grille[MAX][MAX];
    int h;
    int posx = 0, posy = 0;
    int delay = 100;
    
    char f[20] = "sauvegarde.txt";
    
    clock_t debut;
    clock_t fin;

    float millis;


    SDL_Renderer * renderer = NULL;
    SDL_Window * window = NULL;

    SDL_DisplayMode DM;
    
    
    SDL_bool program_on = SDL_TRUE, paused = SDL_TRUE, tor = SDL_FALSE;                             
    SDL_Event event;

    if (argc > 1)                          //cas grille torique
    {
        if (strcmp(argv[1],"-t") == 0) tor = SDL_TRUE;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_GetCurrentDisplayMode(0, &DM);
    h = DM.h - 100;
    window = SDL_CreateWindow("hello there",0,0,h,h,SDL_WINDOW_RESIZABLE);
    
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    initGrille(grille);
    dessin(renderer, grille, h);
    SDL_RenderPresent(renderer);
            dessin(renderer, grille, h);
            SDL_RenderPresent(renderer);

    while (program_on) 
    {                              // La boucle des evenements
        debut = clock();


        while(SDL_PollEvent(&event))                      // Tant que la file des evenements stockes n'est pas vide et qu'on n'a pas
        {         
            // termine le programme Defiler l'element en tete de file dans 'event'
            switch (event.type) 
            {                         // En fonction de la valeur du type de cet evenement
                case SDL_QUIT:                                // Un evenement simple, on a clique sur la x de la // fenetre
                    program_on = SDL_FALSE;                     // Il est temps d'arreter le programme
                    break;
                case SDL_KEYDOWN:                             // Le type de event est : une touche appuyee
                // comme la valeur du type est SDL_Keydown, dans la partie 'union' de
                // l'event, plusieurs champs deviennent pertinents   
                    switch (event.key.keysym.sym) 
                    {             // la touche appuyee est ...
                        case SDLK_SPACE:                            // ou 'SPC' 2 touche car peek capture l'espace
                            paused = !paused;                         // basculement pause/unpause
                            break;
                        case SDLK_a:
                            printf("test");
                            paused = !paused;                         // basculement pause/unpause
                            break;
                        case SDLK_s : save(grille, f);
                            break; // save

                        case SDLK_l : load(grille,f ) ;      //on reload de la marice
                            break;
                        case SDLK_RIGHT:
                            delay = delay-50;
                            break;

                        case SDLK_LEFT:
                            delay = delay+2;
                            break;


                        case SDLK_q:                                // ou 'q'
                            program_on = 0;                           // 'escape' ou 'q', d'autres facons de quitter le programme    
                            break;
                        case SDLK_r :
                            initGrille(grille);
                            paused = SDL_TRUE;                            
                            break;
                        default:
                            break;

                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:  
                    if ( SDL_BUTTON(SDL_BUTTON_LEFT)) 
                    {
                        SDL_GetMouseState(&posx, &posy);
                        changerCarreau(grille,posx,posy,h);           // Fonction a executer lors d'un click gauche
                    } 
                    break;
                default:                                      // Les evenements qu'on n'a pas envisage
                    break;
            
            }

        }
        if (!paused)
        {
            if(delay > 0)
            {
                delay-=2;
            }
            else{
                iter(grille,tor);
                delay =25;
            }
        }
        
        dessin(renderer, grille, h);
        SDL_RenderPresent(renderer);
        
        fin = clock();
        millis = (fin - debut) * ((float)1000)/CLOCKS_PER_SEC;
        SDL_Delay(16 - millis);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();
    return 0;
    
}

