// En üstte
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "enemy_bullet.h"

#define GAME_AREA_WIDTH 640
#define SCORE_PANEL_WIDTH 200
#define SCREEN_WIDTH (GAME_AREA_WIDTH + SCORE_PANEL_WIDTH)
#define SCREEN_HEIGHT 480

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;
int score = 0;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if (TTF_Init() == -1) return false;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return false;

    window = SDL_CreateWindow("Touhou Clone",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;

    font = TTF_OpenFont("assets/OpenSans-Regular.ttf", 16);
    if (!font) return false;

    Mix_Music* bgm = Mix_LoadMUS("assets/bgm.mp3");
    if (!bgm) return false;
    Mix_PlayMusic(bgm, -1);

    srand(time(NULL));
    return true;
}

void render_text(const char* text, int x, int y) {
    SDL_Color color = {255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void close_all() {
    TTF_CloseFont(font);
    Mix_HaltMusic();
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    free_enemy_texture();
}

int main(int argc, char* argv[]) {
    if (!init()) { printf("Init failed!\n"); return 1; }

    bool quit = false;
    SDL_Event e;

    Player player = create_player(renderer);
    player.health = 3;

    load_enemy_texture(renderer);
    init_bullets();
    init_enemy_bullets();
    init_enemies();

    Uint32 lastFire = 0, lastEnemySpawn = 0, lastSpiral = 0;
    float spiralAngle = 0;

    while (!quit) {
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT) quit = true;

        const Uint8* keys = SDL_GetKeyboardState(NULL);
        handle_player_input(&player, keys);

        Uint32 now = SDL_GetTicks();

        if (keys[SDL_SCANCODE_SPACE] && now > lastFire + 150) {
            fire_bullet(player.rect.x + player.rect.w / 2 - 2, player.rect.y);
            lastFire = now;
        }

        if (now > lastEnemySpawn + 2000) {
            spawn_enemy(rand() % (GAME_AREA_WIDTH - 32), -32);
            lastEnemySpawn = now;
        }

        // Spiral mermi → sadece oyuncu hayattaysa gönderilsin
        if (now > lastSpiral + 500 && player.health > 0) {
            spawn_spiral_bullets(320, 100, spiralAngle);
            spiralAngle += 10;
            lastSpiral = now;
        }

        update_bullets();
        update_enemy_bullets();
        update_enemies();

        handle_bullet_enemy_collision(enemies, MAX_ENEMIES, &score);
        handle_enemy_bullet_collision(&player.rect, &player.health);

        // Düşman çarpışması
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].active && SDL_HasIntersection(&player.rect, &enemies[i].rect)) {
                enemies[i].active = false;
                player.health--;
            }
        }

        if (player.health <= 0) {
            SDL_Log("GAME OVER!");
            quit = true;
        }

        SDL_SetRenderDrawColor(renderer, 10, 10, 30, 255);
        SDL_RenderClear(renderer);

        draw_player(renderer, &player);
        draw_bullets(renderer);
        draw_enemy_bullets(renderer);
        draw_enemies(renderer);

        // Sağ panel
        SDL_Rect panel = {GAME_AREA_WIDTH, 0, SCORE_PANEL_WIDTH, SCREEN_HEIGHT};
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderFillRect(renderer, &panel);
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
        SDL_RenderDrawLine(renderer, GAME_AREA_WIDTH, 0, GAME_AREA_WIDTH, SCREEN_HEIGHT);

        char buf[64];
        snprintf(buf, sizeof(buf), "Score: %06d", score);
        render_text(buf, GAME_AREA_WIDTH + 10, 20);
        render_text("Power: 1.00 / 4.00", GAME_AREA_WIDTH + 10, 50);
        render_text("Stage: 1", GAME_AREA_WIDTH + 10, 80);
        render_text("Lives:", GAME_AREA_WIDTH + 10, 110);

        for (int i = 0; i < player.health; i++) {
            SDL_Rect heart = {GAME_AREA_WIDTH + 70 + i * 20, 110, 16, 16};
            SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
            SDL_RenderFillRect(renderer, &heart);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    destroy_player(&player);
    close_all();
    return 0;
}
