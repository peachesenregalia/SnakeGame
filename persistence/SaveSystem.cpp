#include "SaveSystem.h"
#include <fstream>

bool SaveSystem::saveGame(const SavedGameState& gameState, const std::string& path) const {
    std::ofstream output(path);
    if (!output) {
        return false;
    }

    output << gameState.width << ' ' << gameState.height << ' ' << static_cast<int>(gameState.direction)
           << ' ' << gameState.score << ' ' << gameState.food.x << ' ' << gameState.food.y << '\n';
    output << gameState.snake.size() << '\n';
    for (const Point& segment : gameState.snake) {
        output << segment.x << ' ' << segment.y << '\n';
    }

    return output.good();
}

bool SaveSystem::loadGame(SavedGameState& gameState, const std::string& path) const {
    std::ifstream input(path);
    if (!input) {
        return false;
    }

    int directionValue = 0;
    size_t count = 0;
    if (!(input >> gameState.width >> gameState.height >> directionValue >> gameState.score >> gameState.food.x >> gameState.food.y)) {
        return false;
    }

    gameState.direction = static_cast<Direction>(directionValue);
    if (gameState.width <= 0 || gameState.height <= 0) {
        return false;
    }

    if (!(input >> count)) {
        return false;
    }

    gameState.snake.clear();
    for (size_t i = 0; i < count; ++i) {
        Point segment;
        if (!(input >> segment.x >> segment.y)) {
            return false;
        }
        gameState.snake.push_back(segment);
    }

    return !gameState.snake.empty();
}
