#pragma once

#include <string>

enum class GameState {
    Menu,
    Running,
    Paused,
    GameOver,
    Exit,
};

class StateMachine {
public:
    StateMachine();

    GameState current() const;
    bool transition(GameState next);
    bool isRunning() const;
    bool isPaused() const;
    bool isMenu() const;
    bool isGameOver() const;
    bool isExit() const;
    std::string name() const;

private:
    GameState state;
    bool canTransition(GameState next) const;
};
