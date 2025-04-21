// === enemy_bullet.c ===
#include "enemy_bullet.h"
#include <math.h>

#define BULLET_SPEED 4
#define BULLET_WIDTH 4
#define BULLET_HEIGHT 10
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static EnemyBullet bullets[MAX_ENEMY_BULLETS];

void init_enemy_bullets() {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        bullets[i].active = false;
    }
}

void spawn_enemy_bullet(int x, int y) {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].rect.x = x;
            bullets[i].rect.y = y;
            bullets[i].rect.w = BULLET_WIDTH;
            bullets[i].rect.h = BULLET_HEIGHT;
            bullets[i].vx = 0;
            bullets[i].vy = BULLET_SPEED;
            bullets[i].active = true;
            break;
        }
    }
}

void spawn_boss_bullet(int x, int y, float dx, float dy) {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].rect.x = x;
            bullets[i].rect.y = y;
            bullets[i].rect.w = BULLET_WIDTH;
            bullets[i].rect.h = BULLET_HEIGHT;
            bullets[i].vx = dx;
            bullets[i].vy = dy;
            bullets[i].active = true;
            break;
        }
    }
}

void spawn_spiral_bullets(int centerX, int centerY, float angleDeg) {
    for (int i = 0; i < 12; i++) {
        float angle = (angleDeg + i * 30) * (M_PI / 180.0f);
        float dx = cosf(angle) * 3;
        float dy = sinf(angle) * 3;
        spawn_boss_bullet(centerX, centerY, dx, dy);
    }
}

void update_enemy_bullets() {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].rect.x += bullets[i].vx;
            bullets[i].rect.y += bullets[i].vy;
            if (bullets[i].rect.y > 480 || bullets[i].rect.x < 0 || bullets[i].rect.x > 640)
                bullets[i].active = false;
        }
    }
}

void draw_enemy_bullets(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 200, 50, 255, 255);
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (bullets[i].active) {
            SDL_RenderFillRect(renderer, &bullets[i].rect);
        }
    }
}

void handle_enemy_bullet_collision(SDL_Rect* playerRect, int* playerHealth) {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (bullets[i].active) {
            if (SDL_HasIntersection(playerRect, &bullets[i].rect)) {
                bullets[i].active = false;
                (*playerHealth)--;
                if (*playerHealth <= 0) {
                    SDL_Log("PLAYER HIT BY ENEMY BULLET - GAME OVER");
                }
            }
        }
    }
}
