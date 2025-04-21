#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define MAX_ENEMY_BULLETS 100

typedef struct {
    SDL_Rect rect;
    bool active;
    float vx, vy;  
} EnemyBullet;

void init_enemy_bullets();
void spawn_enemy_bullet(int x, int y);                     // Düz aşağı mermi
void spawn_boss_bullet(int x, int y, float dx, float dy);  // ⬅️➡️ Spiral için yönlü mermi
void spawn_spiral_bullets(int centerX, int centerY, float angleDeg); // Spiral paterni
void update_enemy_bullets();
void draw_enemy_bullets(SDL_Renderer* renderer);
void handle_enemy_bullet_collision(SDL_Rect* playerRect, int* playerHealth);

#endif
