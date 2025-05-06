#include "Bullet.h"
#include <SDL_image.h>
#include <iostream>

const int Bullet::SPEED;
Bullet::Bullet(SDL_Renderer* rend, int startX, int startY) : renderer(rend) {
    texture = IMG_LoadTexture(renderer, "bullet-removebg-preview.png");
    if(!texture){
        std::cout << "Failed to load bullet texture:" << IMG_GetError << std :: endl;
    }


rect .x = startX;
rect .y = startY;
rect .w = 30;
rect .h = 30;
}

Bullet::~Bullet() {
    if(texture){
    SDL_DestroyTexture(texture);
 }
}
Bullet::Bullet(Bullet&& other) noexcept
    : texture(other.texture), renderer(other.renderer), rect(other.rect) {
    other.texture = nullptr;
}

Bullet& Bullet::operator=(Bullet&& other) noexcept {
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

void Bullet::update() {
    rect .y -= SPEED;
}

void Bullet::render() {

    SDL_RenderCopy(renderer, texture, NULL, &rect);
}
