#pragma once

#include <vector>
#include <random>
#include "game.hpp"
#include "types.hpp"

namespace emulator {

struct ObstacleCar {
    int lane;   // 0, 1, 2, 3
    double y;   // vertical position (float for smooth movement)
    int speed;
    Color color;
};

class cars : public game {
public:
    cars();
    ~cars() override = default;

    std::string getName() const override { return "Highway Racer"; }
    std::string getDescription() const override { return "Dodge oncoming traffic, switch lanes, and survive the highway!"; }

    void init() override;
    void handleInput(InputKey key) override;
    void update(double deltaTime) override;
    void render(IRenderer& renderer) override;

    bool isGameOver() const override { return gameOver; }
    int getScore() const override { return score; }
    void reset() override;

private:
    static constexpr int NUM_LANES = 4;
    static constexpr int LANE_WIDTH = 8;
    static constexpr int ROAD_HEIGHT = 24;

    int roadX = 34;
    int roadY = 2;
    int roadWidth = NUM_LANES * LANE_WIDTH + 2;

    int playerLane = 1;
    int playerY = 20;

    std::vector<ObstacleCar> obstacles;
    double spawnTimer = 0.0;
    double spawnInterval = 1.4;

    double roadScroll = 0.0;
    double gameSpeed = 16.0; // road scroll speed

    int score = 0;
    double distanceTravelled = 0.0;
    bool gameOver = false;

    std::mt19937 rng;

    int getLaneCenterCol(int lane) const;
    void spawnObstacle();
    bool checkCollision(const ObstacleCar& obs) const;
};

using CarsGame = cars;

} // namespace emulator
