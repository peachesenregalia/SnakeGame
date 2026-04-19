#include "StateMachine.h"

StateMachine::StateMachine()
    : state(GameState::Menu) {
}

GameState StateMachine::current() const {
    return state;
}

bool StateMachine::transition(GameState next) {
    if (!canTransition(next)) {
        return false;
    }
    state = next;
    return true;
}

bool StateMachine::isRunning() const {
    return state == GameState::Running;
}

bool StateMachine::isPaused() const {
    return state == GameState::Paused;
}

bool StateMachine::isMenu() const {
    return state == GameState::Menu;
}

bool StateMachine::isGameOver() const {
    return state == GameState::GameOver;
}

bool StateMachine::isExit() const {
    return state == GameState::Exit;
}

std::string StateMachine::name() const {
    switch (state) {
        case GameState::Menu:
            return "MENU";
        case GameState::Running:
            return "RUNNING";
        case GameState::Paused:
            return "PAUSED";
        case GameState::GameOver:
            return "GAME_OVER";
        case GameState::Exit:
            return "EXIT";
    }
    return "UNKNOWN";
}

bool StateMachine::canTransition(GameState next) const {
    if (state == next) {
        return true;
    }

    switch (state) {
        case GameState::Menu:
            return next == GameState::Running || next == GameState::Exit;
        case GameState::Running:
            return next == GameState::Paused || next == GameState::GameOver || next == GameState::Exit;
        case GameState::Paused:
            return next == GameState::Running || next == GameState::Exit;
        case GameState::GameOver:
            return next == GameState::Menu || next == GameState::Running || next == GameState::Exit;
        case GameState::Exit:
            return false;
    }
    return false;
}
