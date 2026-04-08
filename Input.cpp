#include "Input.h"
#include <conio.h>
#include <cctype>

InputManager::InputManager()
    : quitRequested(false) {
}

bool InputManager::pollEvent(InputEvent& event) {
    if (!_kbhit()) {
        return false;
    }

    int code = _getch();
    event.any = true;

    if (code == 0 || code == 224) {
        code = _getch();
        event.direction = mapArrowKey(code);
        return true;
    }

    char ch = static_cast<char>(code);
    if (ch == 'q' || ch == 'Q') {
        quitRequested = true;
        event.quit = true;
    }

    event.key = ch;
    switch (std::tolower(ch)) {
        case 'w':
            event.direction = Direction::Up;
            break;
        case 's':
            event.direction = Direction::Down;
            break;
        case 'a':
            event.direction = Direction::Left;
            break;
        case 'd':
            event.direction = Direction::Right;
            break;
        default:
            break;
    }

    return true;
}

Direction InputManager::mapArrowKey(int code) const {
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

bool InputManager::shouldQuit() const {
    return quitRequested;
}
