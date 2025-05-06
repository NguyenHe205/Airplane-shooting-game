#ifndef ENEMY_H
#define ENEMY_H
#include <SDL.h>
#include <iostream>
#include <SDL_ttf.h>

class Enemy {
private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    SDL_Rect rect;
    static const int SPEED = 1.5;
public:
    Enemy(SDL_Renderer* rend, int startX, int startY);
    ~Enemy();
    Enemy(const Enemy&) = delete;
    Enemy& operator=(const Enemy&) = delete;
    Enemy(Enemy&& other) noexcept;
    Enemy& operator=(Enemy&& other) noexcept;
    void update();
    void render();
    int getX() const { return rect.x; } // Thêm getter
    int getY() const { return rect.y; }
    int getW() const { return rect.w; }
    int getH() const { return rect.h; }
    SDL_Rect& getRect() { return rect; }
    const SDL_Rect& getRect() const { return rect; }
};

#endif
