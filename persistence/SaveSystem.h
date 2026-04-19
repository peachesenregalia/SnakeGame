#pragma once

#include <deque>
#include <string>
#include "gameplay/Snake.h"

struct SavedGameState {
    std::deque<Point> snake;
    Point food;
    Direction direction = Direction::Right;
    int score = 0;
    int width = 0;
    int height = 0;
};

class SaveSystem {
public:
    bool saveGame(const SavedGameState& gameState, const std::string& path = "savegame.txt") const;
    bool loadGame(SavedGameState& gameState, const std::string& path = "savegame.txt") const;
};
