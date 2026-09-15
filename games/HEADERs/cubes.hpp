#pragma once

#include <vector>
#include <random>
#include "game.hpp"
#include "types.hpp"

namespace emulator {

struct Tetromino {
    int id;
    Color color;
    std::vector<Point2D> blocks; // 4 blocks relative to piece origin
};

class cubes : public game {
public:
    cubes();
    ~cubes() override = default;

    std::string getName() const override { return "Cubes (Falling Blocks)"; }
    std::string getDescription() const override { return "Rotate and stack falling tetromino cubes to clear lines!"; }

    void init() override;
    void handleInput(InputKey key) override;
    void update(double deltaTime) override;
    void render(IRenderer& renderer) override;

    bool isGameOver() const override { return gameOver; }
    int getScore() const override { return score; }
    void reset() override;

private:
    static constexpr int BOARD_WIDTH = 10;
    static constexpr int BOARD_HEIGHT = 20;

    int boardX = 36;
    int boardY = 2;

    int board[BOARD_HEIGHT][BOARD_WIDTH] = {{0}}; // 0 = empty, >0 = color
    Tetromino currentPiece;
    Point2D piecePos{0, 0};

    Tetromino nextPiece;
    double dropTimer = 0.0;
    double dropInterval = 0.45;

    int score = 0;
    int linesCleared = 0;
    bool gameOver = false;

    std::mt19937 rng;

    Tetromino getRandomPiece();
    bool checkCollision(const Tetromino& piece, const Point2D& pos) const;
    void lockPiece();
    void clearLines();
    Tetromino rotatePiece(const Tetromino& piece) const;
};

using CubesGame = cubes;

} // namespace emulator
