#include <stdlib.h>
#include "Game.h"

int main() {
    Game game;

    while(game.isWindowOpen()) {
        game.update();
        game.render();
    }

    return 0;
}