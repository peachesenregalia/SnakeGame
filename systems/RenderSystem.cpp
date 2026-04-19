#include "RenderSystem.h"
#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

RenderSystem::RenderSystem(int width_, int height_)
    : width(width_),
      height(height_),
      bufferWidth(width_ + 2),
      bufferHeight(height_ + 4),
      buffer(bufferHeight, std::string(bufferWidth, ' '))
#ifdef _WIN32
      , consoleHandle(nullptr)
#endif
{
}

void RenderSystem::initialize() {
#ifdef _WIN32
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
#endif
}

void RenderSystem::clear() {
    for (auto& row : buffer) {
        row.assign(bufferWidth, ' ');
    }
}

void RenderSystem::drawBorder() {
    for (int x = 0; x < bufferWidth; ++x) {
        buffer[0][x] = '#';
        buffer[height + 1][x] = '#';
    }
    for (int y = 0; y < height + 2; ++y) {
        buffer[y][0] = '#';
        buffer[y][width + 1] = '#';
    }
}

void RenderSystem::drawSnake(const Snake& snake) {
    bool first = true;
    for (const Point& segment : snake.segments()) {
        setPixel(segment, first ? 'O' : 'o');
        first = false;
    }
}

void RenderSystem::drawFood(const Food& food) {
    setPixel(food.position, '*');
}

void RenderSystem::drawHUD(int score, GameState state) {
    std::string status = "STATE: ";
    switch (state) {
        case GameState::Menu:
            status += "MENU";
            break;
        case GameState::Running:
            status += "RUNNING";
            break;
        case GameState::Paused:
            status += "PAUSED";
            break;
        case GameState::GameOver:
            status += "GAME_OVER";
            break;
        case GameState::Exit:
            status += "EXIT";
            break;
    }
    std::string scoreText = "SCORE: " + std::to_string(score);
    std::string controls = "[WASD/Arrows] Move  [P] Pause  [X] Save  [L] Load  [N] New  [Q] Quit";
    drawText(0, height + 2, status + "    " + scoreText);
    drawText(0, height + 3, controls);
}

void RenderSystem::present() {
    std::cout << "\x1b[H";
    for (const auto& row : buffer) {
        std::cout << row << '\n';
    }
    std::cout << std::flush;
}

void RenderSystem::setPixel(Point position, char ch) {
    int x = position.x + 1;
    int y = position.y + 1;
    if (x >= 1 && x <= width && y >= 1 && y <= height) {
        buffer[y][x] = ch;
    }
}

void RenderSystem::drawText(int x, int y, const std::string& text) {
    if (y < 0 || y >= bufferHeight) {
        return;
    }
    for (size_t i = 0; i < text.size() && x + static_cast<int>(i) < bufferWidth; ++i) {
        if (x + static_cast<int>(i) >= 0) {
            buffer[y][x + static_cast<int>(i)] = text[i];
        }
    }
}

void RenderSystem::ensureConsoleCursor() {
#ifdef _WIN32
    if (!consoleHandle) {
        consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
#endif
}
