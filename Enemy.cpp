#include "Enemy.h"
#include <SDL_image.h>
#include <iostream>

Enemy::Enemy(SDL_Renderer* rend, int startX, int startY) : renderer(rend), texture(nullptr) {
    texture = IMG_LoadTexture(renderer, "enemy-removebg.png");
    if (!texture) {
        std::cout << "Failed to load enemy texture: " << IMG_GetError() << std::endl;
    }

    rect.x = startX;
    rect.y = startY;
    rect.w = 80;
    rect.h = 80;
}

Enemy::~Enemy() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

Enemy::Enemy(Enemy&& other) noexcept
    : texture(other.texture), renderer(other.renderer), rect(other.rect) {
    other.texture = nullptr;
}

Enemy& Enemy::operator=(Enemy&& other) noexcept {
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

void Enemy::update() {
    rect.y += SPEED;
}

void Enemy::render() {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
