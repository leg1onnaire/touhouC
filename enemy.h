#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define MAX_ENEMIES 20

typedef struct {
    SDL_Rect rect;
    bool active;
    SDL_Texture* texture; 
    Uint32 lastShotTime;
} Enemy;

extern Enemy enemies[MAX_ENEMIES];

void load_enemy_texture(SDL_Renderer* renderer);
void draw_enemies(SDL_Renderer* renderer);
void free_enemy_texture();

void init_enemies();
void spawn_enemy(int x, int y);
void update_enemies();

#endif
