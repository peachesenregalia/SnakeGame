#pragma once

#include "Snake.h"

struct InputEvent {
    Direction direction = Direction::None;
    char key = 0;
    bool quit = false;
    bool any = false;
};

class InputManager {
public:
    InputManager();
    bool pollEvent(InputEvent& event);
    bool shouldQuit() const;

private:
    bool quitRequested;
    Direction mapArrowKey(int code) const;
};
