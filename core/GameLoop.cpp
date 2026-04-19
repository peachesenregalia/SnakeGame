#include "GameLoop.h"
#include <iostream>
#include <random>
#include <vector>

GameLoop::GameLoop(int width_, int height_)
    : width(width_),
      height(height_),
      score(0),
      stateMachine(),
      currentDirection(Direction::Right),
      snake(Point{width_ / 2, height_ / 2}, 4),
      food(),
      renderer(width_, height_),
      input(),
      timer(),
      saveSystem() {
    spawnFood();
    renderer.initialize();
    timer.reset();
}

void GameLoop::run() {
    while (!stateMachine.isExit()) {
        processInput();

        if (stateMachine.isRunning()) {
            if (timer.shouldTick()) {
                update();
            }
        }

        renderFrame();
    }
}

void GameLoop::resetGame() {
    score = 0;
    stateMachine.transition(GameState::Running);
    currentDirection = Direction::Right;
    snake.reset(Point{width / 2, height / 2}, 4);
    spawnFood();
    timer.setSpeed(timer.speedForScore(score));
    timer.reset();
}

void GameLoop::spawnFood() {
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

void GameLoop::processInput() {
    InputFrame frame = input.collectFrameInput(currentDirection);

    if (frame.quit) {
        stateMachine.transition(GameState::Exit);
        return;
    }

    if (stateMachine.isMenu()) {
        handleMenuInput(frame);
    } else if (stateMachine.isRunning()) {
        handleRunningInput(frame);
    } else if (stateMachine.isPaused()) {
        handlePausedInput(frame);
    } else if (stateMachine.isGameOver()) {
        handleGameOverInput(frame);
    }
}

void GameLoop::handleMenuInput(const InputFrame& frame) {
    if (frame.newGame) {
        resetGame();
    } else if (frame.load) {
        if (loadSavedGame()) {
            stateMachine.transition(GameState::Running);
            timer.setSpeed(timer.speedForScore(score));
            timer.reset();
        }
    }
}

void GameLoop::handleRunningInput(const InputFrame& frame) {
    applyMovement(frame.nextDirection);

    if (frame.pause) {
        stateMachine.transition(GameState::Paused);
    } else if (frame.save) {
        saveCurrentGame();
    } else if (frame.load) {
        if (loadSavedGame()) {
            stateMachine.transition(GameState::Running);
        }
    } else if (frame.restart) {
        resetGame();
    }
}

void GameLoop::handlePausedInput(const InputFrame& frame) {
    if (frame.pause) {
        stateMachine.transition(GameState::Running);
    } else if (frame.save) {
        saveCurrentGame();
    } else if (frame.load) {
        if (loadSavedGame()) {
            stateMachine.transition(GameState::Running);
        }
    }
}

void GameLoop::handleGameOverInput(const InputFrame& frame) {
    if (frame.restart) {
        resetGame();
    } else if (frame.newGame) {
        resetGame();
    } else if (frame.load) {
        if (loadSavedGame()) {
            stateMachine.transition(GameState::Running);
            timer.setSpeed(timer.speedForScore(score));
            timer.reset();
        }
    }
}

void GameLoop::applyMovement(Direction nextDirection) {
    if (nextDirection != currentDirection && nextDirection != Direction::None) {
        currentDirection = nextDirection;
    }
}

void GameLoop::update() {
    Point next = snake.nextHead(currentDirection);
    bool willGrow = next == food.position;

    if (willGrow) {
        snake.grow();
    }

    snake.move(currentDirection);
    Point head = snake.head();

    if (head.x < 0 || head.x >= width || head.y < 0 || head.y >= height || snake.collidesWithSelf()) {
        stateMachine.transition(GameState::GameOver);
        return;
    }

    if (willGrow) {
        score += 10;
        spawnFood();
        timer.setSpeed(timer.speedForScore(score));
    }
}

void GameLoop::renderFrame() {
    renderer.clear();
    renderer.drawBorder();
    renderer.drawSnake(snake);
    renderer.drawFood(food);
    renderer.drawHUD(score, stateMachine.current());
    renderer.present();
}

bool GameLoop::loadSavedGame() {
    SavedGameState loaded;
    if (!saveSystem.loadGame(loaded) || loaded.snake.empty()) {
        return false;
    }

    width = loaded.width;
    height = loaded.height;
    score = loaded.score;
    currentDirection = loaded.direction;
    snake.setSegments(loaded.snake);
    food.position = loaded.food;
    renderer = RenderSystem(width, height);
    renderer.initialize();
    return true;
}

void GameLoop::saveCurrentGame() const {
    SavedGameState stateData;
    stateData.snake = snake.segments();
    stateData.food = food.position;
    stateData.direction = currentDirection;
    stateData.score = score;
    stateData.width = width;
    stateData.height = height;
    saveSystem.saveGame(stateData);
}
