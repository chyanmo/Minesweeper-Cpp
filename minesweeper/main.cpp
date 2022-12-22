#include "Game.h"
#include "Window.h"

int main() {
    Window w(330, 360, EX_SHOWCONSOLE);
    w.setWindowTitle("Minesweeper");
    Game g;
    g.run();

    return w.exec();

}