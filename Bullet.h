#ifndef BULLET_H
#define BULLET_H
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

class Bullet {
private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    SDL_Rect rect;
    static const int SPEED = 10;

public:
    Bullet(SDL_Renderer* rend, int startX, int startY);
    ~Bullet();
    Bullet(const Bullet&) = delete;
    Bullet& operator=(const Bullet&) = delete;
    Bullet(Bullet&& other) noexcept;
    Bullet& operator=(Bullet&& other) noexcept;
    void update();
    void render();
    int getX() const { return rect.x; }
    int getY() const { return rect.y; }
    int getW() const { return rect.w; }
    int getH() const { return rect.h; }
    SDL_Rect& getRect() { return rect; }
    const SDL_Rect& getRect() const { return rect; }
};

#endif
