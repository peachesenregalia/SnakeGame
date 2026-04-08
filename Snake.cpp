#include "Snake.h"

Snake::Snake()
    : pendingGrow(false) {
    reset(Point{0, 0}, 4);
}

Snake::Snake(Point head, int initialLength)
    : pendingGrow(false) {
    reset(head, initialLength);
}

void Snake::reset(Point head, int initialLength) {
    body.clear();
    pendingGrow = false;
    for (int i = 0; i < initialLength; ++i) {
        body.push_back(Point{head.x - i, head.y});
    }
}

void Snake::move(Direction dir) {
    if (dir == Direction::None) {
        return;
    }

    body.push_front(nextHead(dir));
    if (pendingGrow) {
        pendingGrow = false;
    } else {
        body.pop_back();
    }
}

void Snake::grow() {
    pendingGrow = true;
}

Point Snake::nextHead(Direction dir) const {
    Point headPoint = head();
    switch (dir) {
        case Direction::Up:
            headPoint.y -= 1;
            break;
        case Direction::Down:
            headPoint.y += 1;
            break;
        case Direction::Left:
            headPoint.x -= 1;
            break;
        case Direction::Right:
            headPoint.x += 1;
            break;
        case Direction::None:
            break;
    }
    return headPoint;
}

bool Snake::collidesWith(Point point) const {
    return std::any_of(body.begin(), body.end(), [&](const Point& segment) {
        return segment == point;
    });
}

bool Snake::collidesWithSelf() const {
    if (body.size() < 4) {
        return false;
    }
    const Point headPoint = head();
    return std::any_of(std::next(body.begin()), body.end(), [&](const Point& segment) {
        return segment == headPoint;
    });
}

const std::deque<Point>& Snake::segments() const {
    return body;
}

Point Snake::head() const {
    return body.front();
}

void Snake::setSegments(const std::deque<Point>& segments) {
    body = segments;
    pendingGrow = false;
}
