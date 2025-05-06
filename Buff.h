#ifndef BUFF_H
#define BUFF_H

#include <SDL.h>

class Buff {
private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect rect;
    static const int SPEED = 2;
public:
    Buff(SDL_Renderer* rend, int x, int y);
    ~Buff();
    void update();
    void render();
    SDL_Rect& getRect() { return rect; }
    bool isOutOfScreen() const { return rect.y > 600; }
};

#endif
