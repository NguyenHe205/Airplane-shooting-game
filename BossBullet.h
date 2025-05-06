#ifndef BOSS_BULLET_H
#define BOSS_BULLET_H
#include <iostream>
#include <SDL.h>

class BossBullet {
private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    SDL_Rect rect;
    static const int SPEED = 2;

public:
    BossBullet(SDL_Renderer* rend, int startX, int startY);
    ~BossBullet();
    BossBullet(const BossBullet&) = delete;
    BossBullet& operator=(const BossBullet&) = delete;
    BossBullet(BossBullet&& other) noexcept;
    BossBullet& operator=(BossBullet&& other) noexcept;
    void update();
    void render();
    SDL_Rect& getRect() { return rect; }
    const SDL_Rect& getRect() const { return rect; }
};

#endif
