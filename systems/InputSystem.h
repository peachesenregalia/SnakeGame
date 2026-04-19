#pragma once

#include "gameplay/Snake.h"

struct InputFrame {
    Direction nextDirection = Direction::None;
    bool quit = false;
    bool pause = false;
    bool save = false;
    bool load = false;
    bool restart = false;
    bool newGame = false;
};

class InputSystem {
public:
    InputSystem();
    InputFrame collectFrameInput(Direction currentDirection);

    static bool isOpposite(Direction a, Direction b);

private:
    bool quitRequested;
    Direction mapArrowKey(int code) const;
};
