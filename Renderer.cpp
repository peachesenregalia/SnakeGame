#include "Renderer.h"
#include "Game.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#endif

Renderer::Renderer(int width_, int height_)
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

void Renderer::initialize() {
    ensureConsoleCursor();
#ifdef _WIN32
    if (consoleHandle) {
        DWORD mode = 0;
        if (GetConsoleMode((HANDLE)consoleHandle, &mode)) {
            SetConsoleMode((HANDLE)consoleHandle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#endif
    std::cout << "\x1b[2J";
}

void Renderer::clear() {
    std::fill(buffer.begin(), buffer.end(), std::string(bufferWidth, ' '));
}

void Renderer::drawBorder() {
    for (int x = 0; x < bufferWidth; ++x) {
        setPixel({x, 0}, '#');
        setPixel({x, height + 1}, '#');
    }
    for (int y = 1; y <= height; ++y) {
        setPixel({0, y}, '#');
        setPixel({bufferWidth - 1, y}, '#');
    }
}

void Renderer::drawSnake(const Snake& snake) {
    const auto& segments = snake.segments();
    bool first = true;
    for (const auto& segment : segments) {
        char ch = first ? 'O' : 'o';
        setPixel({segment.x + 1, segment.y + 1}, ch);
        first = false;
    }
}

void Renderer::drawFood(const Food& food) {
    setPixel({food.position.x + 1, food.position.y + 1}, '@');
}

void Renderer::drawHUD(int score, PlayState state) {
    std::string status;
    switch (state) {
        case PlayState::Start:
            status = "N=New  L=Load  Q=Quit";
            break;
        case PlayState::Running:
            status = "P=Pause  S=Save  Q=Quit";
            break;
        case PlayState::Paused:
            status = "Paused - P=Resume  S=Save  Q=Quit";
            break;
        case PlayState::GameOver:
            status = "Game Over - R=Restart  L=Load  S=Save  Q=Quit";
            break;
    }

    std::ostringstream line1;
    line1 << "Score: " << score;
    drawText(1, height + 2, line1.str());
    drawText(1, height + 3, status);
}

void Renderer::present() {
    std::string frame;
    frame.reserve(static_cast<size_t>(bufferHeight) * (bufferWidth + 1));
    for (int y = 0; y < bufferHeight; ++y) {
        frame += buffer[y];
        frame += '\n';
    }

#ifdef _WIN32
    if (consoleHandle) {
        SetConsoleCursorPosition((HANDLE)consoleHandle, {0, 0});
    }
#endif
    std::cout << frame << std::flush;
}

void Renderer::setPixel(Point position, char ch) {
    if (position.x < 0 || position.x >= bufferWidth || position.y < 0 || position.y >= bufferHeight) {
        return;
    }
    buffer[position.y][position.x] = ch;
}

void Renderer::drawText(int x, int y, const std::string& text) {
    if (y < 0 || y >= bufferHeight) {
        return;
    }
    for (size_t i = 0; i < text.size() && x + static_cast<int>(i) < bufferWidth; ++i) {
        if (x + static_cast<int>(i) >= 0) {
            buffer[y][x + static_cast<int>(i)] = text[i];
        }
    }
}

void Renderer::ensureConsoleCursor() {
#ifdef _WIN32
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (consoleHandle && consoleHandle != INVALID_HANDLE_VALUE) {
        CONSOLE_CURSOR_INFO cursorInfo;
        cursorInfo.dwSize = 1;
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo((HANDLE)consoleHandle, &cursorInfo);
    }
#else
    std::cout << "\x1b[?25l";
#endif
}
