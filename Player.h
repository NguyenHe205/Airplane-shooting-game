#ifndef PLAYER_H
#define PLAYER_H
#include<iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include "Bullet.h"
class Player {
private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    SDL_Rect rect;
    int lives;
    const int SPEED = 5;
    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 600;
    bool hasBuff;
    int shootingMode;
    int buffTimer;
    const int BUFF_DURATION = 1800;

public:
    Player(SDL_Renderer* rend);
    ~Player();
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&& other) noexcept;
    Player& operator=(Player&& other) noexcept;
    void update(const Uint8* keyboard);
    void render();
    void shoot(std::vector<Bullet>& bullets);
    void loseLife();
    void resetLives();
    void reset();
    void activateBuff(int buffType);
    void updateBuffTimer();
    int getX() const { return rect.x; }
    int getY() const { return rect.y; }
    int getW() const { return rect.w; }
    int getH() const { return rect.h; }
    int getLives() const {return lives;}
    SDL_Rect& getRect() { return rect; }
    const SDL_Rect& getRect() const { return rect; }
    bool isBuffActive() const {return shootingMode == 3 && buffTimer > 0;}
};

#endif
