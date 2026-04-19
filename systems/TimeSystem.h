#pragma once

#include <chrono>

class TimeSystem {
public:
    enum class Speed {
        Low,
        Medium,
        High,
    };

    TimeSystem();

    void reset();
    bool shouldTick();
    void setSpeed(Speed speed);
    Speed speedForScore(int score) const;
    double lastDeltaSeconds() const;

private:
    std::chrono::steady_clock::time_point lastFrame;
    std::chrono::duration<double> accumulator;
    std::chrono::duration<double> tickInterval;
    double lastDelta;
    Speed currentSpeed;

    std::chrono::duration<double> intervalForSpeed(Speed speed) const;
};
