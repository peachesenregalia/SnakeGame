#pragma once

#include <string>
#include <vector>
#include "gameplay/Snake.h"
#include "gameplay/Food.h"
#include "core/StateMachine.h"

class RenderSystem {
public:
    RenderSystem(int width, int height);
    void initialize();
    void clear();
    void drawBorder();
    void drawSnake(const Snake& snake);
    void drawFood(const Food& food);
    void drawHUD(int score, GameState state);
    void present();

private:
    int width;
    int height;
    int bufferWidth;
    int bufferHeight;
    std::vector<std::string> buffer;

    void setPixel(Point position, char ch);
    void drawText(int x, int y, const std::string& text);
    void ensureConsoleCursor();

#ifdef _WIN32
    void* consoleHandle;
#endif
};
