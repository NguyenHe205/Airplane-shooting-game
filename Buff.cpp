#include "Buff.h"
#include <SDL_image.h>
#include <iostream>

Buff::Buff(SDL_Renderer* rend, int x, int y, int type) : renderer(rend), type(type) {
    if (type == 1){
        texture = IMG_LoadTexture(renderer, "triple_shot.png");
    } else if (type == 2){
    texture = IMG_LoadTexture(renderer, "double_shot.png");
    }
    if (!texture) {
        std::cout << "Failed to load buff texture: " << IMG_GetError() << std::endl;
        throw std::runtime_error("Failed to load buff texture");
    }
    rect.x = x;
    rect.y = y;
    rect.w = 60;
    rect.h = 60;
}

Buff::~Buff() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Buff::update() {
    rect.y += SPEED;
}

void Buff::render() {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
bool Buff::isOutOfScreen(){
    return rect.y > 600;
}

