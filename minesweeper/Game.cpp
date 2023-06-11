#include "Game.h"
#include <chrono>

#ifdef DEBUG
#include <iostream>
#endif // DEBUG


Game::Game(uint16_t ROW, uint16_t COL, uint16_t Mine, uint16_t ButtonSize)
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
    init(game_row, game_col, game_mine);    // init 函数包含 Window::beginDraw();

    // 创建两个线程，分别负责事件处理和显示
    std::thread eventThread(&Game::event_handle, this);
    std::thread showThread(&Game::show, this);

    // 等待两个线程
    eventThread.join();
    showThread.join();
    
    Window::endDraw();
}

void Game::event_handle() {
    while (true)
    {
        ::getmessage(&m_msg, EX_MOUSE | EX_KEY);   // 获取消息

        switch (m_msg.message) {
        case WM_KEYDOWN:    // 键盘操作
            switch (m_msg.vkcode) {
            case 0x52: // R
                init(game_row, game_col, game_mine);
                break;
            case 0x51: // Q
                init(9, 9, 10);
                break;
            case 0x57: // W
                init(16, 16, 40);
                break;
            case 0x45: // E
                init(16, 30, 99);
                break;
            case VK_ESCAPE: {
                std::unique_lock<std::shared_mutex> lock(mutex_);
                quit_ = true;
                return;
            }
            default:
                continue;
            }
            break;
        case WM_LBUTTONDOWN:    // 鼠标操作
            if (lclick_forward())
                update_button();
            break;
        case WM_RBUTTONDOWN:
            if (rclick_forward())
                update_button();
            break;
        }
    }
}

void Game::show() {
    // 下一帧的期望时间
    auto expect_time = std::chrono::steady_clock::now();
    auto interval_ = std::chrono::milliseconds(1000 / FPS);

    while (true) {
        // 显示一帧
        {
            std::shared_lock<std::shared_mutex> lock(mutex_);
            if (quit_) break;

            Window::clear();
            show_button();
            Window::flushDraw();
        }
        // 更新下一帧的期望时间
        expect_time += interval_;
        // 睡眠等待下一帧
        auto time_now = std::chrono::steady_clock::now();
        if (time_now < expect_time)
            std::this_thread::sleep_for(expect_time - time_now);
        else if (expect_time + 2*interval_ < time_now )
            expect_time = time_now;
    }
}

bool Game::rclick_forward()
{
    if (finished || !started) [[unlikely]]
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
    if (emojiButton->isin(m_msg)) [[unlikely]] {
        init(game_row, game_col, game_mine);
        return false;
    }

    if (finished) [[unlikely]]
        return false;

    for (unsigned i = 0; i < btns.size(); i++)
    {
        if (btns[i]->isin(m_msg))
        {
            if (!started) [[unlikely]]
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


void Game::init(uint16_t ROW, uint16_t COL, uint16_t Mine)
{
    started = false;

    // 获取写锁
    std::unique_lock<std::shared_mutex> lock(mutex_);

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
    using enum Map::OpType;
    // 获取写锁
    std::unique_lock<std::shared_mutex> lock(mutex_);

    if (map->win()) {
        timer->stop();
        emojiButton->setImage(&icons[COOL]);
        finished = true;
    }

    while (!(map->empty_buffer())) {
        // opType: operationType 操作的类型，i: index 被操作元素的下标
        auto [opType, i] = map->front_buffer();

        switch (opType)
        {
        case UNMARK:
            btns[i]->setImage(false);
            labels[0]->setText(std::to_string(map->getmark()));
            break;
        case DOMARK:
            btns[i]->setImage(&icons[FLAG]);
            labels[0]->setText(std::to_string(map->getmark()));
            break;
        [[likely]] case BLANK:
            btns[i]->setBkClr(COLOR[0]);
            break;
        [[unlikely]] case TERM:
            timer->stop();
            emojiButton->setImage(&icons[CRY]);
            map->fail();
            finished = true;
            continue;
            break;
        [[unlikely]] case ONAMINE:
            btns[i]->setImage(&icons[MINERED]);
            btns[i]->setBkClr(RGB(254,0,0));
            break;
        [[unlikely]] case OTHERMINE:
            btns[i]->setImage(&icons[MINE]);
            btns[i]->setBkClr(COLOR[0]);
            break;
        [[unlikely]] case NOTAMINE:
            btns[i]->setImage(&icons[MINEREDX]);
            btns[i]->setBkClr(COLOR[0]);
            break;
        [[likely]] default:
            btns[i]->setBkClr(COLOR[0]);
            btns[i]->setText(std::to_string( static_cast<int>(opType) ));
            btns[i]->setTextClr(COLOR[opType]);
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
    ::loadimage(&icons[MINEREDX], "./icons/mine3.png", button_size * 3 / 4, button_size * 3 / 4);
    ::loadimage(&icons[FLAG], "./icons/flag.png", button_size * 3 / 4, button_size * 3 / 4);
}
