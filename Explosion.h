#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <SDL.h>

class Explosion {
public:
    Explosion(SDL_Renderer* renderer, int x, int y);
    ~Explosion();

    void update();
    void render();
    bool isFinished() const;

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
    int currentFrame;
    int totalFrames;
    int frameWidth;
    int frameHeight;
    int frameDelay;
    int frameCounter;
};

#endif
