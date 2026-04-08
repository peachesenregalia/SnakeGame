#pragma once

#include "Snake.h"
#include "Food.h"
#include "Renderer.h"
#include "Input.h"
#include "SaveSystem.h"

enum class PlayState { Start, Running, Paused, GameOver };

class Game {
public:
    Game(int width = 40, int height = 20);
    void run();

private:
    int width;
    int height;
    int score;
    PlayState state;
    Direction direction;
    Snake snake;
    Food food;
    Renderer renderer;
    InputManager input;
    SaveSystem saveSystem;
    bool quitRequested;

    void reset();
    void spawnFood();
    void drawFrame();
    void updateGame();
    void processStartInput(const InputEvent& event);
    void processRunningInput(const InputEvent& event);
    void processPausedInput(const InputEvent& event);
    void processGameOverInput(const InputEvent& event);
    void saveCurrentGame();
    bool loadSavedGame();
    bool isOpposite(Direction a, Direction b) const;
};
