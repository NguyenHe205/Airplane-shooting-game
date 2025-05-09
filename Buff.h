#ifndef BUFF_H
#define BUFF_H

#include <SDL.h>

class Buff {
private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect rect;
    static const int SPEED = 2;
    int type;;
public:
    Buff(SDL_Renderer* rend, int x, int y, int type);
    ~Buff();
    void update();
    void render();
    bool isOutOfScreen();
    SDL_Rect& getRect() { return rect; }
    int getType() const {return type;}
};

#endif
