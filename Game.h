#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Background.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossBullet.h"
#include "Explosion.h"
#include "Buff.h"

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    int score;
    int highScore;
    bool spacePressed;
    bool inStartScreen;
    bool inGameOver;
    TTF_Font* font;
    SDL_Texture* scoreTexture;
    SDL_Rect scoreRect;
    SDL_Texture* highScoreTexture;
    SDL_Rect highScoreRect;
    SDL_Rect highScoreStartRect;
    SDL_Rect highScoreGameOverRect;
    SDL_Texture* startScreenTexture;
    SDL_Texture* gameOverTexture;
    SDL_Rect gameOverRect;
    SDL_Texture* finalScoreTexture;
    SDL_Rect finalScoreRect;
    SDL_Texture* heartTexture;
    SDL_Rect heartRects[3];
    std::vector<Explosion> explosions;
    int bossDefeatCount;
    std::vector<Buff> buffs;
    Boss* boss;
    static int bossHealth;
    std::vector<BossBullet> bossBullets;

public:
    Game();
    bool init();
    void cleanup();
    void run();
    void handleEvents(Player& player, std::vector<Bullet>& bullets);
    void handleStartScreenEvents();
    void handleGameOverEvents(Player& player, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies);
    void update(Player& player, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies);
    bool start();
    void render(Player& player, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, Background& bg);
    void renderStartScreen();
    void renderGameOverScreen();
    void renderHighScore();
    void updateScoreTexture();
    void updateHighScoreTexture();
    void updateGameOverTextures();
    void loadHighScore();
    void saveHighScore();
    SDL_Texture* loadTexture(const char* path);
};

#endif
