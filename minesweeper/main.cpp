#include "Game.h"
#include "Window.h"

int main() {

    const int ROW = 9, COL = 9, MINE = 10;

    const int ButtonSize = 30;

    Game g(ROW, COL, ButtonSize, MINE);

    g.run();

    return 0;
}