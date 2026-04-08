#pragma once

#include <string>
#include <vector>
#include "Snake.h"
#include "Food.h"

enum class PlayState;

class Renderer {
public:
    Renderer(int width, int height);
    void initialize();
    void clear();
    void drawBorder();
    void drawSnake(const Snake& snake);
    void drawFood(const Food& food);
    void drawHUD(int score, PlayState state);
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
