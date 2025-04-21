#include "player.h"
#include <SDL2/SDL_image.h>

Player create_player(SDL_Renderer* renderer) {
    Player p;
    SDL_Surface* surface = IMG_Load("assets/player.png");
    if (!surface) {
        SDL_Log("Failed to load player sprite: %s", IMG_GetError());
    }

    p.texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    p.rect.w = 32;
    p.rect.h = 32;
    p.rect.x = 320 - p.rect.w / 2;
    p.rect.y = 400;
    p.speed = 4;
    p.health = 3;
    

    return p;
}

void handle_player_input(Player* player, const Uint8* keystates) {
    if (keystates[SDL_SCANCODE_LEFT])
        player->rect.x -= player->speed;
    if (keystates[SDL_SCANCODE_RIGHT])
        player->rect.x += player->speed;
    if (keystates[SDL_SCANCODE_UP])
        player->rect.y -= player->speed;
    if (keystates[SDL_SCANCODE_DOWN])
        player->rect.y += player->speed;

    // Ekran dışına çıkmayı engelle
    if (player->rect.x < 0) player->rect.x = 0;
    if (player->rect.y < 0) player->rect.y = 0;
    if (player->rect.x > 640 - player->rect.w) player->rect.x = 640 - player->rect.w;
    if (player->rect.y > 480 - player->rect.h) player->rect.y = 480 - player->rect.h;
}

void draw_player(SDL_Renderer* renderer, Player* player) {
    SDL_RenderCopy(renderer, player->texture, NULL, &player->rect);
}

void destroy_player(Player* player) {
    SDL_DestroyTexture(player->texture);
}
