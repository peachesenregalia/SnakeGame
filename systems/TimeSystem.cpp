#include "TimeSystem.h"

TimeSystem::TimeSystem()
    : lastFrame(std::chrono::steady_clock::now()),
      accumulator(0.0),
      tickInterval(intervalForSpeed(Speed::Medium)),
      lastDelta(0.0),
      currentSpeed(Speed::Medium) {
}

void TimeSystem::reset() {
    lastFrame = std::chrono::steady_clock::now();
    accumulator = std::chrono::duration<double>(0.0);
    tickInterval = intervalForSpeed(currentSpeed);
    lastDelta = 0.0;
}

bool TimeSystem::shouldTick() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> delta = now - lastFrame;
    lastFrame = now;
    lastDelta = delta.count();
    accumulator += delta;

    if (accumulator >= tickInterval) {
        accumulator -= tickInterval;
        return true;
    }
    return false;
}

void TimeSystem::setSpeed(Speed speed) {
    currentSpeed = speed;
    tickInterval = intervalForSpeed(speed);
}

TimeSystem::Speed TimeSystem::speedForScore(int score) const {
    if (score >= 100) {
        return Speed::High;
    }
    if (score >= 40) {
        return Speed::Medium;
    }
    return Speed::Low;
}

double TimeSystem::lastDeltaSeconds() const {
    return lastDelta;
}

std::chrono::duration<double> TimeSystem::intervalForSpeed(Speed speed) const {
    switch (speed) {
        case Speed::Low:
            return std::chrono::duration<double>(0.25);
        case Speed::Medium:
            return std::chrono::duration<double>(0.16);
        case Speed::High:
            return std::chrono::duration<double>(0.10);
    }
    return std::chrono::duration<double>(0.16);
}
