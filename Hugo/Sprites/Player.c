#include "Player.h"

player_t * Player_Create(SDL_Texture * sprite, SDL_Point pos)
{
    player_t * p = (player_t *)malloc(sizeof(player_t));
    if(p)
    {
        p->speed = 10;
        p->sprite = sprite;
        p->position.x = pos.x;
        p->position.y = pos.y;
        p->state.x = 0;
        p->state.y = 0;
        p->direction.x = 0;
        p->direction.y = 0;
        p->base_delay = 8;
        p->current_delay = 8;
    }
    return p;
}

void Player_Update(player_t * player)
{
    int norme = sqrt((player->direction.x*player->direction.x) + (player->direction.y*player->direction.y));
    player->position.x += (player->direction.x * norme * player->speed);
    player->position.y += (player->direction.y * norme * player->speed);
    if(player->direction.x>0) player->state.y=3;
    if(player->direction.x<0) player->state.y=2;
    if(player->direction.y>0 && player->direction.x == 0) player->state.y=0;
    if(player->direction.y<0 && player->direction.x == 0) player->state.y=1;

    if(player->direction.x != 0 || player->direction.y !=0)
    {
        player->current_delay -= 1;
    }

    player->direction.x = 0;
    player->direction.y = 0;
}

void Player_Draw(SDL_Renderer* renderer, player_t * player)
{
    SDL_Rect 
             source = {0},                    // Rectangle définissant la zone totale de la planche
             destination = {0},               // Rectangle définissant où la zone_source doit être déposée dans le renderer
             state = {0};
    
    SDL_QueryTexture(player->sprite,
                        NULL, NULL,
                        &source.w, &source.h);

    int nb_images = 4;
    float zoom = 0.25;
    int offset_x = source.w / nb_images,
           offset_y = source.h / 4;
    
    state.x = player->state.x * offset_x;
    state.y = player->state.y * offset_y;
    state.w = offset_x;
    state.h = offset_y;

    destination.w = offset_x * zoom;
    destination.h = offset_y * zoom;

    destination.x = player->position.x;
    destination.y = player->position.y;

    SDL_RenderCopy(renderer, player->sprite,
                        &state,
                        &destination);
    if(player->current_delay<=0)
    {
        player->state.x += 1;
        player->state.x %= 4;
        player->current_delay = player->base_delay;
    }
}

void Player_Destroy(player_t ** p)
{
    free(*p);
    *p=NULL;
}