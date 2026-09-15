#include "snake.hpp"
#include <algorithm>

namespace emulator {

snake::snake() : rng(std::random_device{}()) {
    init();
}

void snake::init() {
    body.clear();
    int startX = arenaX + arenaWidth / 2;
    int startY = arenaY + arenaHeight / 2;
    body.push_back({startX - 2, startY});
    body.push_back({startX - 1, startY});
    body.push_back({startX, startY});

    currentDir = Direction::RIGHT;
    nextDir = Direction::RIGHT;
    gameOver = false;
    isPaused_ = false;
    score = 0;
    moveTimer = 0.0;
    moveInterval = 0.10;

    spawnFood();
}

void snake::reset() {
    init();
}

bool snake::isCellOccupied(const Point2D& pt) const {
    for (const auto& seg : body) {
        if (seg == pt) return true;
    }
    return false;
}

void snake::spawnFood() {
    std::uniform_int_distribution<int> distX(arenaX + 1, arenaX + arenaWidth - 2);
    std::uniform_int_distribution<int> distY(arenaY + 1, arenaY + arenaHeight - 2);

    int attempts = 0;
    do {
        food = {distX(rng), distY(rng)};
        attempts++;
    } while (isCellOccupied(food) && attempts < 200);
}

void snake::handleInput(InputKey key) {
    switch (key) {
        case InputKey::UP:
            if (currentDir != Direction::DOWN) nextDir = Direction::UP;
            break;
        case InputKey::DOWN:
            if (currentDir != Direction::UP) nextDir = Direction::DOWN;
            break;
        case InputKey::LEFT:
            if (currentDir != Direction::RIGHT) nextDir = Direction::LEFT;
            break;
        case InputKey::RIGHT:
            if (currentDir != Direction::LEFT) nextDir = Direction::RIGHT;
            break;
        default:
            break;
    }
}

void snake::update(double deltaTime) {
    if (gameOver || isPaused_) return;

    moveTimer += deltaTime;
    if (moveTimer < moveInterval) return;
    moveTimer = 0.0;

    currentDir = nextDir;
    Point2D head = body.back();

    switch (currentDir) {
        case Direction::UP:    head.y--; break;
        case Direction::DOWN:  head.y++; break;
        case Direction::LEFT:  head.x--; break;
        case Direction::RIGHT: head.x++; break;
        default: break;
    }

    // Boundary check
    if (head.x <= arenaX || head.x >= arenaX + arenaWidth - 1 ||
        head.y <= arenaY || head.y >= arenaY + arenaHeight - 1) {
        gameOver = true;
        return;
    }

    // Self collision
    for (size_t i = 0; i < body.size() - 1; ++i) {
        if (body[i] == head) {
            gameOver = true;
            return;
        }
    }

    // Move forward
    body.push_back(head);

    if (head == food) {
        score += 10;
        moveInterval = std::max(0.04, moveInterval * 0.97);
        spawnFood();
    } else {
        body.erase(body.begin());
    }
}

void snake::render(IRenderer& renderer) {
    // Draw arena border
    renderer.drawBox(arenaX, arenaY, arenaWidth, arenaHeight, '#', Color::BRIGHT_CYAN);

    // Draw HUD
    renderer.drawString(arenaX, arenaY - 1, "SCORE: " + std::to_string(score) + " pts", Color::BRIGHT_YELLOW);
    renderer.drawString(arenaX + 28, arenaY - 1, "Arrows/WASD: Move | P: Pause | ESC: Menu", Color::DEFAULT);

    // Draw food
    if (food.x > 0 && food.y > 0) {
        renderer.drawChar(food.x, food.y, '*', Color::BRIGHT_RED);
    }

    // Draw snake body
    for (size_t i = 0; i < body.size(); ++i) {
        if (i + 1 == body.size()) {
            renderer.drawChar(body[i].x, body[i].y, 'O', Color::BRIGHT_GREEN); // Head
        } else {
            renderer.drawChar(body[i].x, body[i].y, 'o', Color::GREEN);        // Body
        }
    }

    if (gameOver) {
        int midX = arenaX + arenaWidth / 2 - 10;
        int midY = arenaY + arenaHeight / 2;
        renderer.drawBox(midX - 2, midY - 1, 24, 4, '=', Color::BRIGHT_RED);
        renderer.drawString(midX + 2, midY, "GAME OVER!", Color::BRIGHT_RED);
        renderer.drawString(midX, midY + 1, "Press R: Retry | ESC", Color::WHITE);
    } else if (isPaused_) {
        int midX = arenaX + arenaWidth / 2 - 6;
        int midY = arenaY + arenaHeight / 2;
        renderer.drawString(midX, midY, "[ PAUSED ]", Color::BRIGHT_YELLOW);
    }
}

} // namespace emulator
