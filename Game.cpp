#include "Game.h"
#include <chrono>
#include <random>
#include <sstream>
#include <string>
#include <thread>

Game::Game(int width_, int height_)
    : width(width_),
      height(height_),
      score(0),
      state(PlayState::Start),
      direction(Direction::Right),
      snake(Point{width_ / 2, height_ / 2}, 4),
      food(),
      renderer(width_, height_),
      input(),
      saveSystem(),
      quitRequested(false) {
    spawnFood();
    renderer.initialize();
}

void Game::run() {
    using clock = std::chrono::steady_clock;
    const auto frameDuration = std::chrono::milliseconds(120);
    auto lastTick = clock::now();

    while (!quitRequested) {
        InputEvent event;
        if (input.pollEvent(event)) {
            if (event.quit) {
                quitRequested = true;
            } else {
                switch (state) {
                    case PlayState::Start:
                        processStartInput(event);
                        break;
                    case PlayState::Running:
                        processRunningInput(event);
                        break;
                    case PlayState::Paused:
                        processPausedInput(event);
                        break;
                    case PlayState::GameOver:
                        processGameOverInput(event);
                        break;
                }
            }
        }

        if (state == PlayState::Running) {
            auto now = clock::now();
            if (now - lastTick >= frameDuration) {
                updateGame();
                lastTick = now;
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        drawFrame();
    }
}

void Game::reset() {
    score = 0;
    state = PlayState::Running;
    direction = Direction::Right;
    snake.reset(Point{width / 2, height / 2}, 4);
    spawnFood();
}

void Game::spawnFood() {
    std::vector<Point> available;
    available.reserve(width * height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Point check{x, y};
            if (!snake.collidesWith(check)) {
                available.push_back(check);
            }
        }
    }

    if (available.empty()) {
        food.position = {0, 0};
        return;
    }

    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, static_cast<int>(available.size()) - 1);
    food.position = available[dist(rng)];
}

void Game::drawFrame() {
    renderer.clear();
    renderer.drawBorder();
    renderer.drawSnake(snake);
    renderer.drawFood(food);
    renderer.drawHUD(score, state);
    renderer.present();
}

void Game::updateGame() {
    Point next = snake.nextHead(direction);
    bool willGrow = next == food.position;

    if (willGrow) {
        snake.grow();
    }

    snake.move(direction);
    Point head = snake.head();

    if (head.x < 0 || head.x >= width || head.y < 0 || head.y >= height || snake.collidesWithSelf()) {
        state = PlayState::GameOver;
        return;
    }

    if (willGrow) {
        score += 10;
        spawnFood();
    }
}

void Game::processStartInput(const InputEvent& event) {
    if (event.key == 'n' || event.key == 'N') {
        reset();
    } else if (event.key == 'l' || event.key == 'L') {
        if (loadSavedGame()) {
            state = PlayState::Running;
        }
    } else if (event.key == 'q' || event.key == 'Q') {
        quitRequested = true;
    }
}

void Game::processRunningInput(const InputEvent& event) {
    if (event.direction != Direction::None && !isOpposite(direction, event.direction)) {
        direction = event.direction;
    }

    if (event.key == 'p' || event.key == 'P') {
        state = PlayState::Paused;
    } else if (event.key == 's' || event.key == 'S') {
        saveCurrentGame();
    }
}

void Game::processPausedInput(const InputEvent& event) {
    if (event.key == 'p' || event.key == 'P') {
        state = PlayState::Running;
    } else if (event.key == 's' || event.key == 'S') {
        saveCurrentGame();
    }
}

void Game::processGameOverInput(const InputEvent& event) {
    if (event.key == 'r' || event.key == 'R') {
        reset();
    } else if (event.key == 'l' || event.key == 'L') {
        if (loadSavedGame()) {
            state = PlayState::Running;
        }
    } else if (event.key == 's' || event.key == 'S') {
        saveCurrentGame();
    }
}

void Game::saveCurrentGame() {
    GameState stateData;
    stateData.snake = snake.segments();
    stateData.food = food.position;
    stateData.direction = direction;
    stateData.score = score;
    stateData.width = width;
    stateData.height = height;
    saveSystem.saveGame(stateData);
}

bool Game::loadSavedGame() {
    GameState loaded;
    if (!saveSystem.loadGame(loaded) || loaded.snake.empty()) {
        return false;
    }

    width = loaded.width;
    height = loaded.height;
    score = loaded.score;
    direction = loaded.direction;
    snake.setSegments(loaded.snake);
    food.position = loaded.food;
    renderer = Renderer(width, height);
    renderer.initialize();
    return true;
}

bool Game::isOpposite(Direction a, Direction b) const {
    return (a == Direction::Up && b == Direction::Down) ||
           (a == Direction::Down && b == Direction::Up) ||
           (a == Direction::Left && b == Direction::Right) ||
           (a == Direction::Right && b == Direction::Left);
}
