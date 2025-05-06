#include "Game.h"
#include <iostream>
int main(int argc, char* argv[]) {
    Game game;
    if(!game.init()){
        return 1;
    }
    if(!game.start()){
        return 1;
    }
    game.run();
    game.cleanup();
    return 0;
}
