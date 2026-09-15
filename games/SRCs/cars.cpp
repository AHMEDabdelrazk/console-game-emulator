#include "cars.hpp"
#include <cmath>
#include <algorithm>

namespace emulator {

cars::cars() : rng(std::random_device{}()) {
    init();
}

void cars::init() {
    obstacles.clear();
    playerLane = 1;
    playerY = roadY + ROAD_HEIGHT - 3;
    score = 0;
    distanceTravelled = 0.0;
    gameOver = false;
    isPaused_ = false;
    spawnTimer = 0.0;
    spawnInterval = 1.3;
    roadScroll = 0.0;
    gameSpeed = 16.0;
}

void cars::reset() {
    init();
}

int cars::getLaneCenterCol(int lane) const {
    return roadX + 1 + lane * LANE_WIDTH + (LANE_WIDTH / 2);
}

void cars::spawnObstacle() {
    std::uniform_int_distribution<int> distLane(0, NUM_LANES - 1);
    int lane = distLane(rng);

    // Make sure we don't spawn directly on top of another car
    for (const auto& obs : obstacles) {
        if (obs.lane == lane && obs.y < 4.0) {
            return;
        }
    }

    ObstacleCar obs;
    obs.lane = lane;
    obs.y = static_cast<double>(roadY + 1);
    obs.speed = 8;
    obs.color = Color::BRIGHT_RED;
    obstacles.push_back(obs);
}

void cars::handleInput(InputKey key) {
    if (gameOver || isPaused_) return;

    switch (key) {
        case InputKey::LEFT:
            if (playerLane > 0) playerLane--;
            break;
        case InputKey::RIGHT:
            if (playerLane < NUM_LANES - 1) playerLane++;
            break;
        case InputKey::UP:
            gameSpeed = std::min(30.0, gameSpeed + 2.0);
            break;
        case InputKey::DOWN:
            gameSpeed = std::max(10.0, gameSpeed - 2.0);
            break;
        default:
            break;
    }
}

bool cars::checkCollision(const ObstacleCar& obs) const {
    if (obs.lane == playerLane) {
        double diff = std::abs(obs.y - static_cast<double>(playerY));
        if (diff < 1.8) {
            return true;
        }
    }
    return false;
}

void cars::update(double deltaTime) {
    if (gameOver || isPaused_) return;

    roadScroll += gameSpeed * deltaTime;
    distanceTravelled += gameSpeed * deltaTime;
    score = static_cast<int>(distanceTravelled / 5.0);

    // Difficulty ramp
    spawnInterval = std::max(0.6, 1.4 - (score * 0.003));

    // Spawn obstacles
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.0;
        spawnObstacle();
    }

    // Move obstacles downward
    for (auto it = obstacles.begin(); it != obstacles.end();) {
        it->y += (gameSpeed * 0.7) * deltaTime;

        if (checkCollision(*it)) {
            gameOver = true;
            return;
        }

        // Clean up passed cars
        if (it->y > roadY + ROAD_HEIGHT) {
            it = obstacles.erase(it);
            score += 5; // bonus for overtaking
        } else {
            ++it;
        }
    }
}

void cars::render(IRenderer& renderer) {
    // Road boundaries
    for (int r = roadY; r < roadY + ROAD_HEIGHT; ++r) {
        renderer.drawString(roadX, r, "||", Color::BRIGHT_YELLOW);
        renderer.drawString(roadX + roadWidth - 2, r, "||", Color::BRIGHT_YELLOW);

        // Lane markings
        int scrollOffset = static_cast<int>(roadScroll) % 4;
        bool isDashed = ((r + scrollOffset) % 4) < 2;

        for (int l = 1; l < NUM_LANES; ++l) {
            int divCol = roadX + 1 + l * LANE_WIDTH;
            if (isDashed) {
                renderer.drawChar(divCol, r, '|', Color::DEFAULT);
            }
        }
    }

    // Draw Obstacles
    for (const auto& obs : obstacles) {
        int col = getLaneCenterCol(obs.lane) - 1;
        int row = static_cast<int>(obs.y);
        if (row >= roadY + 1 && row + 1 < roadY + ROAD_HEIGHT) {
            renderer.drawString(col, row, "!X!", obs.color);
            renderer.drawString(col, row + 1, "o-o", obs.color);
        }
    }

    // Draw Player Car
    int pCol = getLaneCenterCol(playerLane) - 1;
    renderer.drawString(pCol, playerY, "o=o", Color::BRIGHT_CYAN);
    renderer.drawString(pCol, playerY + 1, "[#]", Color::BRIGHT_CYAN);

    // Draw HUD Sidebar
    int hudX = roadX + roadWidth + 4;
    renderer.drawBox(hudX, roadY, 26, 12, '#', Color::BRIGHT_CYAN);
    renderer.drawString(hudX + 2, roadY + 1, "HIGHWAY RACER", Color::BRIGHT_YELLOW);
    renderer.drawString(hudX + 2, roadY + 3, "Score: " + std::to_string(score) + " m", Color::WHITE);
    renderer.drawString(hudX + 2, roadY + 5, "Speed: " + std::to_string(static_cast<int>(gameSpeed * 5)) + " km/h", Color::BRIGHT_GREEN);
    renderer.drawString(hudX + 2, roadY + 7, "A/D: Switch Lanes", Color::DEFAULT);
    renderer.drawString(hudX + 2, roadY + 8, "W/S: Speed / Brake", Color::DEFAULT);
    renderer.drawString(hudX + 2, roadY + 9, "P: Pause | ESC: Menu", Color::DEFAULT);

    if (gameOver) {
        int midX = roadX + roadWidth / 2 - 8;
        int midY = roadY + ROAD_HEIGHT / 2;
        renderer.drawBox(midX - 2, midY - 1, 20, 4, '=', Color::BRIGHT_RED);
        renderer.drawString(midX + 2, midY, "CRASHED!", Color::BRIGHT_RED);
        renderer.drawString(midX, midY + 1, "R: Retry | ESC", Color::WHITE);
    } else if (isPaused_) {
        int midX = roadX + roadWidth / 2 - 5;
        int midY = roadY + ROAD_HEIGHT / 2;
        renderer.drawString(midX, midY, "[ PAUSED ]", Color::BRIGHT_YELLOW);
    }
}

} // namespace emulator
