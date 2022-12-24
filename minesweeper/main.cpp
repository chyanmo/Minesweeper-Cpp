#include "Game.h"
#include "Window.h"

int main() {

    const int ROW = 9, COL = 9, MINE = 72;

    const int ButtonSize = 30;

    // 窗口大小和行列数有关，其中宽度设置了最小值 240
    Window w( (ButtonSize * COL + 60 > 240) ? (ButtonSize * COL + 60) : 240,
               ButtonSize * ROW + 100,
               EX_SHOWCONSOLE);

    w.setWindowTitle("Minesweeper");

    Game g(ROW, COL, ButtonSize, MINE);

    g.run();

    return w.exec();
}