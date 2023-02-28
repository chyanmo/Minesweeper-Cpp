#include "Game.h"
#include <thread>
#include <chrono>

#ifdef DEBUG
#include <iostream>
#endif // DEBUG


Game::Game(unsigned ROW, unsigned COL, unsigned Mine, unsigned ButtonSize)
    : game_row(ROW), game_col(COL), game_mine(Mine), button_size(ButtonSize) {}

Game::~Game()
{
    clear_button();
    if (window) {
        Window::endDraw();
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
    init(game_row, game_col, game_mine);    // Window::beginDraw();

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
                else if (m_msg.vkcode == VK_ESCAPE)
                    return;
                break;
            case WM_LBUTTONDOWN:            // 鼠标操作
                if (lclick_forward())
                    update_button();
                break;
            case WM_RBUTTONDOWN:
                if (rclick_forward())
                    update_button();
                break;
            default:
                break;
            }
        }
        else
            std::this_thread::sleep_for(std::chrono::milliseconds(4));


        Window::flushDraw();
    }
    Window::endDraw();
}


bool Game::rclick_forward()
{
    if (finished || !started)
        return false;

    for (unsigned i = 0; i < btns.size(); i++) 
    {
        if(btns[i]->isin(m_msg))
        {
            map->right_click(i / game_col, i % game_col);
            return true;
        }
    }

    return false;
}

bool Game::lclick_forward()
{
    if (emojiButton->isin(m_msg)) {
        init(game_row, game_col, game_mine);
        return false;
    }

    if (finished)
        return false;

    for (unsigned i = 0; i < btns.size(); i++)
    {
        if (btns[i]->isin(m_msg))
        {
            if (!started)
            {
                map->init(i / game_col, i % game_col);
                started = true;
                timer->start();
            }
            map->left_click(i / game_col, i % game_col);
            return true;
        }
    }

    return false;
}


void Game::init(unsigned ROW, unsigned COL, unsigned Mine)
{
    started = false;

    if (ROW == game_row && COL == game_col && Mine == game_mine && inited)
        ;
    else {
        game_row = ROW;
        game_col = COL;
        game_mine = Mine;

        if (window) {
            Window::endDraw();
            delete window;
            window = nullptr;
        }

        window = new Window(button_size * max(COL, 6) + 60 , button_size * (ROW + 2) + 60, EX_SHOWCONSOLE || EX_NOCLOSE);

        window->setWindowTitle("Minesweeper");

        ::setbkcolor(RGB(157, 211, 168));

        ::settextstyle(button_size, 0, "Consolas", 0, 0, FW_HEAVY, 0, 0, 0);

        Window::beginDraw();
        Window::clear();
    }
    
    clear_button();
    init_button();

    if (map) {
        delete map;
        map = nullptr;
    }

    map = new Map(game_row, game_col, game_mine);

    inited = true;
    finished = false;
}

void Game::update_button()
{
    if (map->win()) {
        timer->stop();
        emojiButton->setImage(&icons[COOL]);
        finished = true;
    }

    while (!(map->empty_buffer())) {
        auto opt = map->front_buffer();

        switch (opt.first)
        {
        case Map::UNMARK:
            btns[opt.second]->setImage(false);
            labels[0]->setText(std::to_string(map->getmark()));
            break;
        case Map::DOMARK:
            btns[opt.second]->setImage(&icons[FLAG]);
            labels[0]->setText(std::to_string(map->getmark()));
            break;
        case Map::BLANK:
            btns[opt.second]->setBkClr(COLOR[0]);
            break;
        case Map::TERM:
            timer->stop();
            emojiButton->setImage(&icons[CRY]);
            map->fail();
            finished = true;
            continue;
            break;
        case Map::ONAMINE:
            btns[opt.second]->setImage(&icons[MINERED]);
            btns[opt.second]->setBkClr(RGB(254,0,0));
            break;
        case Map::OTHERMINE:
            btns[opt.second]->setImage(&icons[MINE]);
            btns[opt.second]->setBkClr(COLOR[0]);
            break;
        case Map::NOTAMINE:
            btns[opt.second]->setImage(&icons[NOTAMINE]);
            btns[opt.second]->setBkClr(COLOR[0]);
            break;
        default:
            btns[opt.second]->setBkClr(COLOR[0]);
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

    emojiButton->show();
    timer->show();

    for (auto lab : labels)
        lab->show();
}

void Game::init_button()
{
    int xspace = (Window::width() - button_size * max(game_col, 6)) / 2;
    int yspace = (Window::height() - button_size * (game_row + 2)) / 2;

    labels.push_back(new Label(xspace, yspace, 1.5 * button_size, 1.1 * button_size, WHITE, std::to_string(game_mine), RED));
    timer =  new TimeBox(Window::width() - xspace - 1.5 * button_size, yspace, 1.5 * button_size, 1.1 * button_size);

    xspace = (Window::width() - 1.1 * button_size) / 2;
    emojiButton = new Button(xspace, yspace, 1.1 * button_size, 1.1 * button_size);

    xspace = (Window::width() - button_size * game_col) / 2;
    yspace += 2 * button_size;

    btns.resize(game_row * game_col);
    for (int i = 0; auto & btn : btns) {
        int bx = xspace + button_size * (i % game_col);
        int by = yspace + button_size * (i / game_col);
        btn = new Button(bx, by, button_size, button_size);
        i++;
    }

    load_icons();

    emojiButton->setImage(&icons[SMILE]);
}

void Game::clear_button()
{
    for (auto btn : btns)
        delete btn;
    
    btns.clear();
    
    delete emojiButton;
    emojiButton = nullptr;

    delete timer;
    timer = nullptr;

    for (auto lab : labels)
        delete lab;

    labels.clear();
}

void Game::load_icons()
{
    ::loadimage(&icons[SMILE], "./icons/smile.png", emojiButton->width() * 3 / 4, emojiButton->height() * 3 / 4);
    ::loadimage(&icons[COOL], "./icons/cool.png", emojiButton->width() * 3 / 4, emojiButton->height() * 3 / 4);
    ::loadimage(&icons[CRY], "./icons/cry.png", emojiButton->width() * 3 / 4, emojiButton->height() * 3 / 4);
    ::loadimage(&icons[MINE], "./icons/mine2.png", button_size * 3 / 4, button_size * 3 / 4);
    ::loadimage(&icons[MINERED], "./icons/mine1.png", button_size * 3 / 4, button_size * 3 / 4);
    ::loadimage(&icons[NOTAMINE], "./icons/notamine.png", button_size * 3 / 4, button_size * 3 / 4);
    ::loadimage(&icons[FLAG], "./icons/flag.png", button_size * 3 / 4, button_size * 3 / 4);
}
