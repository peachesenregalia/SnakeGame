#include "SaveSystem.h"
#include <fstream>
#include <sstream>

bool SaveSystem::saveGame(const GameState& gameState, const std::string& path) const {
    std::ofstream file(path);
    if (!file) {
        return false;
    }

    file << "score=" << gameState.score << "\n";
    file << "dir=" << static_cast<int>(gameState.direction) << "\n";
    file << "width=" << gameState.width << "\n";
    file << "height=" << gameState.height << "\n";
    file << "food=" << gameState.food.x << "," << gameState.food.y << "\n";
    file << "snake=";

    for (size_t i = 0; i < gameState.snake.size(); ++i) {
        const Point& point = gameState.snake[i];
        file << point.x << "," << point.y;
        if (i + 1 < gameState.snake.size()) {
            file << ";";
        }
    }
    file << "\n";
    return true;
}

bool SaveSystem::loadGame(GameState& gameState, const std::string& path) const {
    std::ifstream file(path);
    if (!file) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        const size_t pos = line.find('=');
        if (pos == std::string::npos) {
            continue;
        }

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        if (key == "score") {
            gameState.score = std::stoi(value);
        } else if (key == "dir") {
            gameState.direction = static_cast<Direction>(std::stoi(value));
        } else if (key == "width") {
            gameState.width = std::stoi(value);
        } else if (key == "height") {
            gameState.height = std::stoi(value);
        } else if (key == "food") {
            std::istringstream stream(value);
            std::string part;
            if (std::getline(stream, part, ',')) {
                gameState.food.x = std::stoi(part);
            }
            if (std::getline(stream, part, ',')) {
                gameState.food.y = std::stoi(part);
            }
        } else if (key == "snake") {
            gameState.snake.clear();
            std::istringstream stream(value);
            std::string token;
            while (std::getline(stream, token, ';')) {
                std::istringstream pointStream(token);
                std::string coord;
                Point point{0, 0};
                if (std::getline(pointStream, coord, ',')) {
                    point.x = std::stoi(coord);
                }
                if (std::getline(pointStream, coord, ',')) {
                    point.y = std::stoi(coord);
                }
                gameState.snake.push_back(point);
            }
        }
    }

    return !gameState.snake.empty();
}
