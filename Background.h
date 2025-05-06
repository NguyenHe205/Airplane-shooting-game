#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
class Background {
private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    SDL_Rect srcRect, destRect1, destRect2;
    float scrollSpeed;
    float yPos1, yPos2;

    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HIGHT = 600;

public:
    Background(SDL_Renderer* rend);
    ~Background();
    void update();
    void render();
};

#endif
