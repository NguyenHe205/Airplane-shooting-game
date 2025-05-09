#include "Player.h"
#include <SDL_image.h>
#include <iostream>
#include <vector>

Player::Player(SDL_Renderer* rend) : renderer(rend), lives(3), hasBuff(false), shootingMode(1), buffTimer(0) {
    texture = IMG_LoadTexture(renderer, "player.png");
    if (!texture) {
        std::cout << "Failed to load player texture: " << IMG_GetError() << std::endl;
        throw std::runtime_error("Failed to load player texture");
    }
    rect.w = 80;
    rect.h = 80;
    rect.x = 400 - rect.w / 2;
    rect.y = 600 - rect.h - 10;
}

Player::~Player() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Player::update(const Uint8* keyboard) {
    const int SPEED = 5;
    if (keyboard[SDL_SCANCODE_LEFT] && rect.x > 0) {
        rect.x -= SPEED;
    }
    if (keyboard[SDL_SCANCODE_RIGHT] && rect.x < 800 - rect.w) {
        rect.x += SPEED;
    }
    if (keyboard[SDL_SCANCODE_UP] && rect.y > 0) {
        rect.y -= SPEED;
    }
    if (keyboard[SDL_SCANCODE_DOWN] && rect.y < 600 - rect.h) {
        rect.y += SPEED;
    }
    updateBuffTimer();
}

void Player::render() {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void Player::shoot(std::vector<Bullet>& bullets) {
    if(shootingMode == 3){
            bullets.emplace_back(renderer, rect.x + rect.w/2 - 5, rect.y);
            bullets.emplace_back(renderer, rect.x + 10, rect.y);
            bullets.emplace_back(renderer, rect.x+rect.w - 10, rect.y);
    }else if (hasBuff) {
        bullets.emplace_back(renderer, rect.x + 10, rect.y);
        bullets.emplace_back(renderer, rect.x + rect.w - 10, rect.y);
    } else {
        bullets.emplace_back(renderer, rect.x + rect.w/2, rect.y);
    }
}

void Player::loseLife() {
    lives--;
}

void Player::resetLives() {
    lives = 3;
}
void Player::reset(){
     rect.x = 400 - rect.w/2;
     rect.y = 600 - rect.h - 10;
     lives = 3;
     hasBuff = false;
     shootingMode = 1;
     buffTimer = 0;
}

void Player::activateBuff(int buffType) {
    if(buffType == 1){
        shootingMode = 3;
        buffTimer = BUFF_DURATION;
    } else if (buffType == 2){
    hasBuff= true;
    }
}

void Player::updateBuffTimer(){
    if (buffTimer > 0){
        --buffTimer;
        if(buffTimer <= 0){
            shootingMode = 1;
        }
    }
}

