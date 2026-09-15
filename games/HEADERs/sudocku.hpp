#pragma once

#include <vector>
#include "game.hpp"
#include "types.hpp"

namespace emulator {

class sudocku : public game {
public:
    sudocku();
    ~sudocku() override = default;

    std::string getName() const override { return "Sudoku Puzzle"; }
    std::string getDescription() const override { return "Navigate 9x9 grid with arrows, press 1-9 to fill cells!"; }

    void init() override;
    void handleInput(InputKey key) override;
    void update(double deltaTime) override;
    void render(IRenderer& renderer) override;

    bool isGameOver() const override { return gameOver; }
    int getScore() const override { return score; }
    void reset() override;

private:
    int grid[9][9] = {{0}};
    bool initialCell[9][9] = {{false}};
    int cursorX = 0;
    int cursorY = 0;
    int score = 0;
    bool gameOver = false;
    bool isCompleted = false;

    int boardX = 32;
    int boardY = 2;

    void loadPuzzle();
    bool checkCompleted() const;
    bool isValidMove(int r, int c, int val) const;
};

using SudokuGame = sudocku;

} // namespace emulator
