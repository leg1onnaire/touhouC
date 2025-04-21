#include "bullet.h"
#include "enemy.h"

#define BULLET_SPEED 8
#define BULLET_WIDTH 4
#define BULLET_HEIGHT 10

static Bullet bullets[MAX_BULLETS];

void init_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++)
        bullets[i].active = false;
}

void fire_bullet(int x, int y) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].rect.x = x;
            bullets[i].rect.y = y;
            bullets[i].rect.w = BULLET_WIDTH;
            bullets[i].rect.h = BULLET_HEIGHT;
            bullets[i].active = true;
            break;
        }
    }
}

void update_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].rect.y -= BULLET_SPEED;
            if (bullets[i].rect.y + bullets[i].rect.h < 0)
                bullets[i].active = false;
        }
    }
}

void draw_bullets(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (int i = 0; i < MAX_BULLETS; i++)
        if (bullets[i].active)
            SDL_RenderFillRect(renderer, &bullets[i].rect);
}

void handle_bullet_enemy_collision(Enemy* enemies, int enemy_count, int* score) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;
        for (int j = 0; j < enemy_count; j++) {
            if (!enemies[j].active) continue;

            if (SDL_HasIntersection(&bullets[i].rect, &enemies[j].rect)) {
                bullets[i].active = false;
                enemies[j].active = false;
                (*score) += 100;
                break;
            }
        }
    }
}
