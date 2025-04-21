// === enemy.c (updated) ===
#include "enemy.h"
#include <SDL2/SDL_image.h>

#define ENEMY_WIDTH 32
#define ENEMY_HEIGHT 32
#define ENEMY_SPEED 2

SDL_Texture* enemyTexture = NULL;
Enemy enemies[MAX_ENEMIES];

void init_enemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = false;
        enemies[i].lastShotTime = 0;
    }
}

void spawn_enemy(int x, int y) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            enemies[i].rect.x = x;
            enemies[i].rect.y = y;
            enemies[i].rect.w = ENEMY_WIDTH;
            enemies[i].rect.h = ENEMY_HEIGHT;
            enemies[i].active = true;
            enemies[i].lastShotTime = 0;
            break;
        }
    }
}

void load_enemy_texture(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("assets/enemy.png");
    if (!surface) {
        SDL_Log("Enemy sprite load failed: %s", IMG_GetError());
        return;
    }
    enemyTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!enemyTexture) {
        SDL_Log("Enemy texture create failed: %s", SDL_GetError());
    }
}

void draw_enemies(SDL_Renderer* renderer) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            if (enemyTexture)
                SDL_RenderCopy(renderer, enemyTexture, NULL, &enemies[i].rect);
            else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &enemies[i].rect);
            }
        }
    }
}


void free_enemy_texture() {
    if (enemyTexture) {
        SDL_DestroyTexture(enemyTexture);
        enemyTexture = NULL;
    }
}
