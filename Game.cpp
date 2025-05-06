#include "Game.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <SDL_ttf.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int Game::bossHealth = 7;

Game::Game() : window(nullptr), renderer(nullptr), running(false), score(0), highScore(0), spacePressed(false), inStartScreen(true), inGameOver(false), font(nullptr), scoreTexture(nullptr), highScoreTexture(nullptr), startScreenTexture(nullptr), gameOverTexture(nullptr), finalScoreTexture(nullptr), heartTexture(nullptr), bossDefeatCount(0), boss(nullptr) {
    loadHighScore();
    explosions.clear();
    bossBullets.clear();
    buffs.clear();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL_Init thất bại: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("Airplane shooting",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "SDL_CreateWindow thất bại: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "SDL_CreateRenderer thất bại: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "IMG_Init thất bại: " << IMG_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cout << "TTF_Init thất bại: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        std::cout << "TTF_OpenFont thất bại: " << TTF_GetError() << std::endl;
        return false;
    }

    startScreenTexture = loadTexture("startscreen.png");
    if (!startScreenTexture) {
        std::cout << "Failed to load startscreen.png: " << IMG_GetError() << std::endl;
        return false;
    }

    heartTexture = loadTexture("heart-removebg-preview.png");
    if (!heartTexture) {
        std::cout << "Failed to load heart texture: " << IMG_GetError() << std::endl;
        return false;
    }


    for (int i = 0; i < 3; ++i) {
        heartRects[i].x = SCREEN_WIDTH - (i+1)*35;
        heartRects[i].y = 0;
        heartRects[i].w = 30;
        heartRects[i].h = 30;
    }

    srand(time(0));
    running = true;
    return true;
}

void Game::cleanup() {
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    if (highScoreTexture) SDL_DestroyTexture(highScoreTexture);
    if (startScreenTexture) SDL_DestroyTexture(startScreenTexture);
    if (gameOverTexture) SDL_DestroyTexture(gameOverTexture);
    if (finalScoreTexture) SDL_DestroyTexture(finalScoreTexture);
    if (heartTexture) SDL_DestroyTexture(heartTexture);
    if (font) TTF_CloseFont(font);
    if (boss) delete boss;

    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::run() {
    Background bg(renderer);
    Player player(renderer);
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    bullets.reserve(100);


    const float targetFrameTime = 1000.0f / 60.0f;

    while (running) {
        Uint32 currentTime = SDL_GetTicks();

        if (inStartScreen) {
            handleStartScreenEvents();
            render(player, bullets, enemies, bg);
        } else if (inGameOver) {
            handleGameOverEvents(player, bullets, enemies);
            render(player, bullets, enemies, bg);
        } else {
            handleEvents(player, bullets);
            bg.update();
            update(player, bullets, enemies);
            render(player, bullets, enemies, bg);
        }

        Uint32 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < targetFrameTime) {
            SDL_Delay(static_cast<Uint32>(targetFrameTime - frameTime));
        }
    }

    saveHighScore();
    std::cout << "Game Over! Score: " << score << std::endl;
}

void Game::handleEvents(Player& player, std::vector<Bullet>& bullets) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
    }

    const Uint8* keyboard = SDL_GetKeyboardState(NULL);
    player.update(keyboard);

    if (keyboard[SDL_SCANCODE_SPACE] && !spacePressed) {
        player.shoot(bullets);
        spacePressed = true;
    }
    if (!keyboard[SDL_SCANCODE_SPACE]) spacePressed = false;
}

void Game::handleStartScreenEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
            inStartScreen = false;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            SDL_Rect startButton = {SCREEN_WIDTH / 2 - 50, 300, 100, 50};
            if (x >= startButton.x && x <= startButton.x + startButton.w &&
                y >= startButton.y && y <= startButton.y + startButton.h) {
                inStartScreen = false;
            }
        }
    }
}

void Game::handleGameOverEvents(Player& player, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
            inGameOver = false;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            SDL_Rect restartButton = {SCREEN_WIDTH / 2 - 50, 300, 100, 50};
            if (x >= restartButton.x && x <= restartButton.x + restartButton.w &&
                y >= restartButton.y && y <= restartButton.y + restartButton.h) {
                std::cout << "Final Score: " << score << std::endl;
                saveHighScore();

                inGameOver = false;
                inStartScreen = true;
                score = 0;
                bossDefeatCount = 0;
                bullets.clear();
                enemies.clear();
                bossBullets.clear();
                buffs.clear();
                explosions.clear();
                if (boss) {
                    delete boss;
                    boss = nullptr;
                }
                player.reset();
                player.resetLives();

                loadHighScore();
                if (highScoreTexture) SDL_DestroyTexture(highScoreTexture);
                highScoreTexture = nullptr;
                renderHighScore();
            }
        }
    }
}

void Game::update(Player& player, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies) {
    if (!boss && (rand() % 2000 == 0)) {
        int bossWidth = 120;
        int maxX = SCREEN_WIDTH - bossWidth;
        int spawnX = rand() % (maxX + 1);
        boss = new Boss(renderer, spawnX, 0, bossHealth);
    }

    if (rand() % 60 == 0) {
        int enemyWidth = 80;
        int maxX = SCREEN_WIDTH - enemyWidth;
        int spawnX = rand() % (maxX + 1);
        enemies.emplace_back(renderer, spawnX, 0);
    }

    for (auto it = bullets.begin(); it != bullets.end();) {
        it->update();
        if (it->getY() < -it->getH()) {
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = bossBullets.begin(); it != bossBullets.end();) {
        it->update();
        if (it->getRect().y > SCREEN_HEIGHT) {
            it = bossBullets.erase(it);
        } else {
            ++it;
        }
    }
    for( auto it = buffs.begin(); it != buffs.end();){
        it -> update();
        if( it -> isOutOfScreen()){
            it = buffs.erase(it);

        } else if (SDL_HasIntersection(&it-> getRect(), &player.getRect())){
            player.activateBuff();
            it = buffs.erase(it);
        } else {
            ++it;
        }
    }

    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
        enemyIt->update();
        bool enemyRemoved = false;

        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end() && !enemyRemoved;) {
            if (SDL_HasIntersection(&enemyIt->getRect(), &bulletIt->getRect())) {
                score += 10;
                explosions.emplace_back(renderer, enemyIt->getX() + enemyIt->getW()/2, enemyIt->getY()+enemyIt->getH()/2);
                enemyIt = enemies.erase(enemyIt);
                bulletIt = bullets.erase(bulletIt);
                enemyRemoved = true;
                break;
            } else {
                ++bulletIt;
            }
        }

        if (!enemyRemoved && SDL_HasIntersection(&enemyIt->getRect(), &player.getRect())) {
            player.loseLife();
            explosions.emplace_back(renderer, player.getRect().x + player.getRect().w/2, player.getRect().y + player.getRect().h/2);
            enemyIt = enemies.erase(enemyIt);
            if (player.getLives() <= 0) {
                inGameOver = true;
                break;
            }
        } else if (!enemyRemoved && enemyIt->getY() > SCREEN_HEIGHT) {
            enemyIt = enemies.erase(enemyIt);
        } else if (!enemyRemoved) {
            ++enemyIt;
        }
    }

    if (boss) {
        boss->update(bossBullets);
        bool bossHit = false;

        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end() && !bossHit;) {
            if (SDL_HasIntersection(&boss->getRect(), &bulletIt->getRect())) {
                boss->hit();
                bulletIt = bullets.erase(bulletIt);
                if (boss->isDestroyed()) {
                    score += 300;
                    bossDefeatCount++;
                    if( bossDefeatCount == 2){
                        int buffWidth = 30;
                        int maxX = SCREEN_WIDTH - buffWidth;
                        int spawnX = rand() % (maxX + 1);
                        buffs.emplace_back(renderer, spawnX , 0);
                    }
                    explosions.emplace_back(renderer, boss->getRect().x + boss->getRect().w/2, boss->getRect().y + boss->getRect().h/2);
                    delete boss;
                    boss = nullptr;
                    bossHealth += 3;
                }
                bossHit = true;
            } else {
                ++bulletIt;
            }
        }

        if (!bossHit && boss && SDL_HasIntersection(&boss->getRect(), &player.getRect())) {
            player.loseLife();
            if (player.getLives() <= 0) {
                inGameOver = true;
            }
        }

        if (!bossHit && boss && boss->getRect().y > SCREEN_HEIGHT) {
            delete boss;
            boss = nullptr;
        }
    }

    for (auto bulletIt = bossBullets.begin(); bulletIt != bossBullets.end();) {
        if (SDL_HasIntersection(&bulletIt->getRect(), &player.getRect())) {
            player.loseLife();
            bulletIt = bossBullets.erase(bulletIt);
            if (player.getLives() <= 0) {
                inGameOver = true;
                break;
            }
        } else {
            ++bulletIt;
        }
    }

    for (auto it = explosions.begin(); it != explosions.end();) {
        it->update();
        if (it->isFinished()) {
            it = explosions.erase(it);
        } else {
            ++it;
        }
    }
}

bool Game::start() {
    if (!inStartScreen) return true;

    renderStartScreen();
    handleStartScreenEvents();
    return true;
}

void Game::render(Player& player, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, Background& bg) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (inStartScreen) {
        renderStartScreen();
    } else if (inGameOver) {
        renderGameOverScreen();
    } else {
        bg.render();
        player.render();
        for (auto& bullet : bullets) bullet.render();
        for (auto& enemy : enemies) enemy.render();
        if (boss) boss->render();
        for (auto& bullet : bossBullets) bullet.render();
        for (auto& buff : buffs) buff.render();
        for (auto& explosion : explosions) explosion.render();

        updateScoreTexture();
        updateHighScoreTexture();


        SDL_RenderCopy(renderer, highScoreTexture, nullptr, &highScoreRect);
        SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);


        int lives = player.getLives();
        for (int i = 0; i < lives && i < 3; ++i) {
            SDL_RenderCopy(renderer, heartTexture, nullptr, &heartRects[i]);
        }
    }

    SDL_RenderPresent(renderer);
}

void Game::renderStartScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 225);
    SDL_RenderClear(renderer);

    SDL_Rect startScreenRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, startScreenTexture, nullptr, &startScreenRect);

    renderHighScore();

}

void Game::renderGameOverScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    updateGameOverTextures();
    SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
    SDL_RenderCopy(renderer, finalScoreTexture, nullptr, &finalScoreRect);
    SDL_RenderCopy(renderer, highScoreTexture, nullptr, &highScoreGameOverRect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    const int buttonWidth = 100;
    const int buttonHeight = 50;
    SDL_Rect restartButton = {SCREEN_WIDTH / 2 - buttonWidth / 2, 300, buttonWidth, buttonHeight};
    SDL_RenderFillRect(renderer, &restartButton);

    SDL_Surface* restartSurface = TTF_RenderText_Solid(font, "Restart", {0, 0, 0, 255});
    SDL_Texture* restartTexture = SDL_CreateTextureFromSurface(renderer, restartSurface);
    SDL_Rect restartTextRect = {0, 0, 0, 0};
    SDL_QueryTexture(restartTexture, nullptr, nullptr, &restartTextRect.w, &restartTextRect.h);
    restartTextRect.x = SCREEN_WIDTH / 2 - restartTextRect.w / 2;
    restartTextRect.y = 310;
    SDL_RenderCopy(renderer, restartTexture, nullptr, &restartTextRect);
    SDL_FreeSurface(restartSurface);
    SDL_DestroyTexture(restartTexture);
}

void Game::renderHighScore() {
    updateHighScoreTexture();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect frameRect = {highScoreStartRect.x, highScoreStartRect.y, highScoreStartRect.w + 10, highScoreStartRect.h + 10};
    SDL_RenderFillRect(renderer, &frameRect);
    SDL_RenderCopy(renderer, highScoreTexture, nullptr, &highScoreStartRect);
}

void Game::updateScoreTexture() {
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), {255, 255, 255, 255}); // Sửa: Màu trắng
    scoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_QueryTexture(scoreTexture, nullptr, nullptr, &scoreRect.w, &scoreRect.h);
    scoreRect.x = 0;
    scoreRect.y = highScoreRect.h; // Dưới High Score
}

void Game::updateHighScoreTexture() {
    if (highScoreTexture) SDL_DestroyTexture(highScoreTexture);
    std::string highScoreText = "High Score: " + std::to_string(highScore);
    SDL_Surface* surface = TTF_RenderText_Solid(font, highScoreText.c_str(), {255, 255, 255, 255}); // Sửa: Màu trắng
    highScoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_QueryTexture(highScoreTexture, nullptr, nullptr, &highScoreRect.w, &highScoreRect.h);
    highScoreRect.x = 0;
    highScoreRect.y = 0; // Góc trên trái
}

void Game::updateGameOverTextures() {
    if (gameOverTexture) SDL_DestroyTexture(gameOverTexture);
    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, "Game Over", {255, 255, 255, 255});
    gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_QueryTexture(gameOverTexture, nullptr, nullptr, &gameOverRect.w, &gameOverRect.h);
    gameOverRect.x = SCREEN_WIDTH / 2 - gameOverRect.w / 2;
    gameOverRect.y = 150;
    SDL_FreeSurface(gameOverSurface);

    if (finalScoreTexture) SDL_DestroyTexture(finalScoreTexture);
    std::string finalScoreText = "Your Score: " + std::to_string(score);
    SDL_Surface* finalScoreSurface = TTF_RenderText_Solid(font, finalScoreText.c_str(), {255, 255, 255, 255});
    finalScoreTexture = SDL_CreateTextureFromSurface(renderer, finalScoreSurface);
    SDL_QueryTexture(finalScoreTexture, nullptr, nullptr, &finalScoreRect.w, &finalScoreRect.h);
    finalScoreRect.x = SCREEN_WIDTH / 2 - finalScoreRect.w / 2;
    finalScoreRect.y = 200;
    SDL_FreeSurface(finalScoreSurface);

    if (highScoreTexture) SDL_DestroyTexture(highScoreTexture);
    std::string highScoreText = "High Score: " + std::to_string(highScore);
    SDL_Surface* highScoreSurface = TTF_RenderText_Solid(font, highScoreText.c_str(), {255, 255, 255, 255});
    highScoreTexture = SDL_CreateTextureFromSurface(renderer, highScoreSurface);
    SDL_QueryTexture(highScoreTexture, nullptr, nullptr, &highScoreGameOverRect.w, &highScoreGameOverRect.h);
    highScoreGameOverRect.x = SCREEN_WIDTH / 2 - highScoreGameOverRect.w / 2;
    highScoreGameOverRect.y = 250;
    SDL_FreeSurface(highScoreSurface);
}

void Game::loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    } else {
        highScore = 0;
    }
}

void Game::saveHighScore() {
    if (score > highScore) {
        highScore = score;
        std::ofstream file("highscore.txt");
        if (file.is_open()) {
            file << highScore;
            file.close();
        }
    }
}

SDL_Texture* Game::loadTexture(const char* path) {
    return IMG_LoadTexture(renderer, path);
}
