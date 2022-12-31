#include "Game.h"
#include <thread>
#include <chrono>

#ifdef DEBUG
#include <iostream>
#endif // DEBUG


Game::Game(int ROW, int COL, int ButtonSize, int Mine)
    : game_row(ROW), game_col(COL), button_size(ButtonSize), game_mine(Mine) {}

Game::~Game()
{
    clear_button();
    if (window) {
        Window::endDraw;
        delete window;
        window = nullptr;
    }
    if (map) {
        delete map;
        map = nullptr;
    }
}

void Game::run()
{
    init(game_row, game_col, game_mine);

    // init 中已经包含 Window::beginDraw();


    while (true)
    {
        Window::clear();

        show_button();

        if (Window::hasMsg()) {
            m_msg = Window::getMsg();   // 获取消息

            switch (m_msg.message)
            {
            case WM_KEYDOWN:    // 键盘操作
                if (m_msg.vkcode == 0x52)       // R
                    init(game_row, game_col, game_mine);
                else if (m_msg.vkcode == 0x51)  // Q
                    init(9, 9, 10);
                else if (m_msg.vkcode == 0x57)  // W
                    init(16, 16, 40);
                else if (m_msg.vkcode == 0x45)  // E
                    init(16, 30, 99);
                break;
            default:            // 鼠标操作
                if (click_forward())
                    update_button();

                break;
            }
        }
        else
            std::this_thread::sleep_for(std::chrono::milliseconds(4));


        Window::flushDraw();
    }
    Window::endDraw();
}


bool Game::click_forward()
{
    unsigned int i = 0;
    unsigned short key = 0;

    for (; i < btns.size(); i++) {

        btns[i]->getmsg2(m_msg);  // 传递消息

        key = btns[i]->is_clicked();

        if (key == 1) {
            // std::cout << "L";
            if (!started) {
                map->init(i / game_col, i % game_col);
                started = true;
            }
            map->left_click(i / game_col, i % game_col);
            return true;
        }
        else if (key == 2 && started) {
            // std::cout << "R";
            map->right_click(i / game_col, i % game_col);
            return true;
        }

    }
    return false;
}

void Game::init(int ROW, int COL, int Mine)
{
    started = false;

    if (ROW == game_row && COL == game_col && Mine == game_mine && inited)
        ;
    else {
        game_row = ROW;
        game_col = COL;
        game_mine = Mine;

        if (window) {
            Window::endDraw;
            delete window;
            window = nullptr;
        }

        window = new Window(button_size * max(COL, 6) + 60 , button_size * (ROW + 2) + 60, EX_SHOWCONSOLE);

        window->setWindowTitle("Minesweeper");

        Window::beginDraw();
        Window::clear();

        ::setbkcolor(RGB(157, 211, 168));
    }
    
    clear_button();
    init_button();

    if (map) {
        delete map;
        map = nullptr;
    }

    map = new Map(game_row, game_col, game_mine);

    inited = true;
}

void Game::update_button()
{
    while (!(map->empty_buffer())) {
        auto opt = map->front_buffer();

        switch (opt.first)
        {
        case -2:
            btns[opt.second]->setText();
            break;
        case -1:
            btns[opt.second]->setText("M");
            break;
        case 0:
            btns[opt.second]->setBkClr(RGB(244, 240, 230));
            break;
        case 9:
            btns[opt.second]->setText("X");
            btns[opt.second]->setBkClr(RED);
            break;
        default:
            btns[opt.second]->setBkClr(RGB(244, 240, 230));
            btns[opt.second]->setText(std::to_string(opt.first));
            btns[opt.second]->setTextClr(COLOR[opt.first]);
            break;
        }


        map->pop_buffer();
    }
}

void Game::show_button()
{
    for (auto btn : btns)
        btn->show();
    for (auto menu : menus)
        menu->show();
    for (auto lab : labels)
        lab->show();
}

void Game::init_button()
{
    int xspace = (Window::width() - button_size * max(game_col, 6)) / 2;
    int yspace = (Window::height() - button_size * (game_row + 2)) / 2;

    labels.push_back(new Label(xspace, yspace, 1.5 * button_size, 1.1 * button_size, WHITE, std::to_string(game_mine), RED));
    labels.push_back(new Label(Window::width() - xspace - 1.5 * button_size, yspace, 1.5 * button_size, 1.1 * button_size, WHITE, "0", RED));

    xspace = (Window::width() - 1.1 * button_size) / 2;
    menus.push_back(new Button(xspace, yspace, 1.1 * button_size, 1.1 * button_size));

    xspace = (Window::width() - button_size * game_col) / 2;
    yspace += 2 * button_size;

    btns.resize(game_row * game_col);
    for (int i = 0; auto & btn : btns) {
        int bx = xspace + button_size * (i % game_col);
        int by = yspace + button_size * (i / game_col);
        btn = new Button(bx, by, button_size, button_size);
        i++;
    }
}

void Game::clear_button()
{
    for (auto btn : btns)
        delete btn;
    
    btns.clear();
    
    for (auto menu : menus)
        delete menu;

    menus.clear();

    for (auto lab : labels)
        delete lab;

    labels.clear();
}
