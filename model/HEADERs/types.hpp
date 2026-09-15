#pragma once

#include <cstdint>
#include <string>

namespace emulator {

enum class Direction {
    NONE = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class InputKey {
    NONE = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPACE,
    ENTER,
    ESC,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_P,
    KEY_R,
    KEY_Q
};

enum class Color {
    DEFAULT = 7,     // Standard terminal light gray
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 15,
    BRIGHT_GREEN = 10,
    BRIGHT_CYAN = 11,
    BRIGHT_RED = 12,
    BRIGHT_YELLOW = 14
};

enum class GameState {
    MENU,
    RUNNING,
    PAUSED,
    GAME_OVER,
    QUIT
};

struct Point2D {
    int x = 0; // column
    int y = 0; // row

    Point2D() = default;
    constexpr Point2D(int col, int row) : x(col), y(row) {}

    bool operator==(const Point2D& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point2D& other) const {
        return !(*this == other);
    }

    Point2D operator+(const Point2D& other) const {
        return {x + other.x, y + other.y};
    }

    Point2D operator-(const Point2D& other) const {
        return {x - other.x, y - other.y};
    }

    bool operator<(const Point2D& other) const {
        if (y != other.y) return y < other.y;
        return x < other.x;
    }
};

} // namespace emulator
