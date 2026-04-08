#pragma once

#include <string>
#include <deque>
#include "Snake.h"

struct GameState {
    std::deque<Point> snake;
    Point food;
    Direction direction = Direction::Right;
    int score = 0;
    int width = 0;
    int height = 0;
};

class SaveSystem {
public:
    bool saveGame(const GameState& gameState, const std::string& path = "savegame.txt") const;
    bool loadGame(GameState& gameState, const std::string& path = "savegame.txt") const;
};
