#include "BossBullet.h"
#include <SDL_image.h>

BossBullet::BossBullet(SDL_Renderer* rend, int startX, int startY) : renderer(rend) {
    texture = IMG_LoadTexture(renderer, "boss_bullet-removebg-preview.png");
    if (!texture) {
        std::cout << "Failed to load boss bullet texture: " << IMG_GetError() << std::endl;
        throw std::runtime_error("Failed to load boss bullet texture");
    }

    rect.x = startX;
    rect.y = startY;
    rect.w = 40;
    rect.h = 40;
}

BossBullet::~BossBullet() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

BossBullet::BossBullet(BossBullet&& other) noexcept
    : texture(other.texture), renderer(other.renderer), rect(other.rect) {
    other.texture = nullptr;
}

BossBullet& BossBullet::operator=(BossBullet&& other) noexcept {
    if (this != &other) {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
        texture = other.texture;
        renderer = other.renderer;
        rect = other.rect;
        other.texture = nullptr;
    }
    return *this;
}

void BossBullet::update() {
    rect.y += SPEED;
}

void BossBullet::render() {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
