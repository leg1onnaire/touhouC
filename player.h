#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Texture* texture;
    SDL_Rect rect;
    int speed;
    int health;
} Player;

Player create_player(SDL_Renderer* renderer);
void handle_player_input(Player* player, const Uint8* keystates);
void draw_player(SDL_Renderer* renderer, Player* player);
void destroy_player(Player* player);

#endif
