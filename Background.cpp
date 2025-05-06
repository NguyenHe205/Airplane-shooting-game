#include "Background.h"
#include <SDL_image.h>
#include <iostream>
Background::Background(SDL_Renderer* rend) : renderer(rend), yPos1(0.0f),yPos2(-600.0f), scrollSpeed(0.5f) {
    texture = IMG_LoadTexture(renderer, "background.png");
    if(!texture){
        std::cout << "Failed to load background texture: " << IMG_GetError() << std::endl;
    }
    srcRect = {0, 0, 800, 600};
    destRect1 = {0, 0, 800, 600};
    destRect2 = {0, -600, 800, 600};

}

Background::~Background() {
    if(texture){
    SDL_DestroyTexture(texture);
    texture = nullptr;
}
}

void Background::update() {
    yPos1 += scrollSpeed;
    yPos2 += scrollSpeed;

    if(yPos1 >= 600){
        yPos1 = -600;
    }
    if(yPos2 >= 600){
        yPos2 = -600;
    }
    destRect1.y = static_cast<int>(yPos1);
    destRect2.y = static_cast<int>(yPos2);

}

void Background::render() {
   if(texture){
    SDL_RenderCopy(renderer, texture, NULL , &destRect1);
    SDL_RenderCopy(renderer, texture, NULL , &destRect2);
   }
}
