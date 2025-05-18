#include <SDL2/SDL.h>

#ifndef f_player
#define f_player

typedef struct player
{
    int speed;
    SDL_Texture * sprite;
    SDL_Point position;
    SDL_Point state;
    SDL_Point direction;
    int current_delay;
    int base_delay;


}player_t;

player_t * Player_Create(SDL_Texture * sprite, SDL_Point pos);
void Player_Update(player_t * player);
void Player_Draw(SDL_Renderer* renderer, player_t * player);
void Player_Destroy(player_t ** p);

#endif