#pragma once

#include <deque>
#include <algorithm>

enum class Direction { None, Up, Down, Left, Right };

struct Point {
    int x = 0;
    int y = 0;

    bool operator==(Point other) const {
        return x == other.x && y == other.y;
    }
};

class Snake {
public:
    Snake();
    explicit Snake(Point head, int initialLength = 4);

    void reset(Point head, int initialLength = 4);
    void setSegments(const std::deque<Point>& segments);
    void move(Direction dir);
    void grow();
    Point nextHead(Direction dir) const;
    bool collidesWith(Point point) const;
    bool collidesWithSelf() const;
    const std::deque<Point>& segments() const;
    Point head() const;

private:
    std::deque<Point> body;
    bool pendingGrow;
};
