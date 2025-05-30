#include "Explosion.h"
#include <SDL_image.h>

Explosion::Explosion(SDL_Renderer* renderer, int x, int y)
    : renderer(renderer), texture(nullptr), currentFrame(0), totalFrames(4), frameWidth(72), frameHeight(88), frameDelay(8), frameCounter(0) {

    texture = IMG_LoadTexture(renderer, "explosion-removebg-preview.png");
    if (!texture) {
        SDL_Log("Failed to load explosion.png: %s", IMG_GetError());
    }


    srcRect = {0, 0, frameWidth, frameHeight};

    dstRect = {x - frameWidth / 2, y - frameHeight / 2, frameWidth, frameHeight};
}

Explosion::~Explosion() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Explosion::update() {
    frameCounter++;
    if (frameCounter >= frameDelay) {
        frameCounter = 0;
        currentFrame++;
        if (currentFrame < totalFrames) {

            srcRect.x = currentFrame * frameWidth;
        }
    }
}

void Explosion::render() {
    if (currentFrame < totalFrames && texture) {
        SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
    }
}

bool Explosion::isFinished() const {
    return currentFrame >= totalFrames;
}
