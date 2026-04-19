#include "InputSystem.h"
#include <conio.h>
#include <cctype>

InputSystem::InputSystem()
    : quitRequested(false) {
}

InputFrame InputSystem::collectFrameInput(Direction currentDirection) {
    InputFrame frameInput;
    frameInput.nextDirection = currentDirection;

    // Collect all pending key events for this frame.
    // Latest legal directional input overrides earlier inputs.
    // We deliberately do not use a queue because the game logic only needs
    // the final valid direction before the next update.
    // We also cannot take only the first input because a later valid key
    // press should override earlier stale input in the same frame.
    while (_kbhit()) {
        int code = _getch();
        if (code == 0 || code == 224) {
            code = _getch();
            Direction newDirection = mapArrowKey(code);
            if (newDirection != Direction::None && !isOpposite(newDirection, currentDirection)) {
                frameInput.nextDirection = newDirection;
            }
            continue;
        }

        char ch = static_cast<char>(code);
        char lower = static_cast<char>(std::tolower(ch));

        if (lower == 'q') {
            frameInput.quit = true;
            quitRequested = true;
            continue;
        }

        if (lower == 'p') {
            frameInput.pause = true;
            continue;
        }

        if (lower == 'x') {
            frameInput.save = true;
            continue;
        }

        if (lower == 'l') {
            frameInput.load = true;
            continue;
        }

        if (lower == 'r') {
            frameInput.restart = true;
            continue;
        }

        if (lower == 'n') {
            frameInput.newGame = true;
            continue;
        }

        switch (lower) {
            case 'w':
                if (!isOpposite(Direction::Up, currentDirection)) {
                    frameInput.nextDirection = Direction::Up;
                }
                break;
            case 's':
                if (!isOpposite(Direction::Down, currentDirection)) {
                    frameInput.nextDirection = Direction::Down;
                }
                break;
            case 'a':
                if (!isOpposite(Direction::Left, currentDirection)) {
                    frameInput.nextDirection = Direction::Left;
                }
                break;
            case 'd':
                if (!isOpposite(Direction::Right, currentDirection)) {
                    frameInput.nextDirection = Direction::Right;
                }
                break;
            default:
                // Ignore invalid keys and do not disturb the current direction.
                break;
        }
    }

    return frameInput;
}

bool InputSystem::isOpposite(Direction a, Direction b) {
    return (a == Direction::Up && b == Direction::Down) ||
           (a == Direction::Down && b == Direction::Up) ||
           (a == Direction::Left && b == Direction::Right) ||
           (a == Direction::Right && b == Direction::Left);
}

Direction InputSystem::mapArrowKey(int code) const {
    switch (code) {
        case 72:
            return Direction::Up;
        case 80:
            return Direction::Down;
        case 75:
            return Direction::Left;
        case 77:
            return Direction::Right;
        default:
            return Direction::None;
    }
}
