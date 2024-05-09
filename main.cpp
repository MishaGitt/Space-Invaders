#include "Game.h"

int main() {

    SetTraceLogLevel(LOG_ERROR);
    InitWindow(950, 950, "Space Invaders");

    Game game = Game();
    game.update();
    game.~Game();
    CloseWindow();
    return 0;
}