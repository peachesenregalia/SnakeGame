#pragma once

#include "core/StateMachine.h"
#include "systems/InputSystem.h"
#include "systems/RenderSystem.h"
#include "systems/TimeSystem.h"
#include "gameplay/Snake.h"
#include "gameplay/Food.h"
#include "persistence/SaveSystem.h"

class GameLoop {
public:
    GameLoop(int width = 40, int height = 20);
    void run();

private:
    int width;
    int height;
    int score;
    StateMachine stateMachine;
    Direction currentDirection;
    Snake snake;
    Food food;
    RenderSystem renderer;
    InputSystem input;
    TimeSystem timer;
    SaveSystem saveSystem;

    void resetGame();
    void spawnFood();
    void processInput();
    void update();
    void renderFrame();
    void handleMenuInput(const InputFrame& frame);
    void handleRunningInput(const InputFrame& frame);
    void handlePausedInput(const InputFrame& frame);
    void handleGameOverInput(const InputFrame& frame);
    void applyMovement(Direction nextDirection);
    bool loadSavedGame();
    void saveCurrentGame() const;
};
