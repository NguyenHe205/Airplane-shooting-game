#include "Boss.h"
#include <SDL_image.h>

Boss::Boss(SDL_Renderer* rend, int x, int y, int h) : renderer(rend), health(h), shooting(true), burstCount(0), burstTimer(0), pauseTimer(0) {
    texture = IMG_LoadTexture(renderer, "boss-removebg-preview.png");
    if (!texture) {
        std::cout << "Failed to load boss texture: " << IMG_GetError() << std::endl;
        throw std::runtime_error("Failed to load boss texture");
    }
    rect.x = x;
    rect.y = y;
    rect.w = 160;
    rect.h = 160;
}

Boss::~Boss() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Boss::update(std::vector<BossBullet>& bossBullets) {
    rect.y += SPEED;

    if (shooting) {
        burstTimer++;
        if (burstTimer >= BURST_INTERVAL) {
            bossBullets.emplace_back(renderer, rect.x + rect.w/2 - 10, rect.y + rect.h);
            burstCount++;
            burstTimer = 0;
            if (burstCount >= BURST_COUNT) {
                shooting = false;
                burstCount = 0;
                pauseTimer = 0;
            }
        }
    } else {
        pauseTimer++;
        if (pauseTimer >= PAUSE_DURATION) {
            shooting = true;
            burstTimer = 0;
        }
    }
}

void Boss::render() {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void Boss::hit() {
    health--;
}

