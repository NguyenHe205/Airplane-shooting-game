#ifndef BOSS_H
#define BOSS_H

#include <SDL.h>
#include <iostream>
#include <vector>
#include "BossBullet.h"

class Boss{
private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    SDL_Rect rect;
    int health;
    int shootTimer;
    static const int SPEED = 0.75;
    static const int BURST_INTERVAL = 10 ;
    static const int BURST_COUNT = 1;
    static const int PAUSE_DURATION = 120;
    bool shooting;
    int burstCount;
    int burstTimer;
    int pauseTimer;
public:
    Boss(SDL_Renderer* rend, int startX, int startY, int initialHealth);
    ~Boss();
    Boss(const Boss&) = delete;
    Boss& operator = (const Boss&) = delete;
    Boss(Boss&& other) noexcept;
    Boss& operator = (Boss&& other) noexcept;
    void update(std::vector<BossBullet>& bossBullets);
    void render();
    void hit();
    bool isDestroyed() const {return health <= 0;}
    SDL_Rect& getRect() {return rect;}
    const SDL_Rect& getRect() const {return rect; }
};
#endif // BOSS_H
