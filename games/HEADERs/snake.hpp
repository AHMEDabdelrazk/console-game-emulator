#pragma once

#include <vector>
#include <random>
#include "game.hpp"
#include "types.hpp"

namespace emulator {

class snake : public game {
public:
    snake();
    ~snake() override = default;

    std::string getName() const override { return "Snake Classic"; }
    std::string getDescription() const override { return "Eat food, grow longer, avoid walls and your own tail!"; }

    void init() override;
    void handleInput(InputKey key) override;
    void update(double deltaTime) override;
    void render(IRenderer& renderer) override;

    bool isGameOver() const override { return gameOver; }
    int getScore() const override { return score; }
    void reset() override;

private:
    std::vector<Point2D> body;
    Direction currentDir = Direction::RIGHT;
    Direction nextDir = Direction::RIGHT;
    Point2D food{-1, -1};
    bool gameOver = false;
    int score = 0;

    int arenaX = 15;
    int arenaY = 2;
    int arenaWidth = 70;
    int arenaHeight = 22;

    double moveTimer = 0.0;
    double moveInterval = 0.12; // Base speed

    std::mt19937 rng;

    void spawnFood();
    bool isCellOccupied(const Point2D& pt) const;
};

using SnakeGame = snake;

} // namespace emulator