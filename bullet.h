#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "enemy.h"


#define MAX_BULLETS 100

typedef struct {
    SDL_Rect rect;
    bool active;
} Bullet;

void init_bullets();
void fire_bullet(int x, int y);
void update_bullets();
void draw_bullets(SDL_Renderer* renderer);
void handle_bullet_enemy_collision(Enemy* enemies, int enemy_count, int* score);


#endif
