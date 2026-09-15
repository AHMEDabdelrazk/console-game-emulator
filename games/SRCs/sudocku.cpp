#include "sudocku.hpp"

namespace emulator {

sudocku::sudocku() {
    init();
}

void sudocku::loadPuzzle() {
    // Classic starting board
    static const int defaultBoard[9][9] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            grid[r][c] = defaultBoard[r][c];
            initialCell[r][c] = (defaultBoard[r][c] != 0);
        }
    }
}

void sudocku::init() {
    loadPuzzle();
    cursorX = 0;
    cursorY = 0;
    score = 0;
    gameOver = false;
    isCompleted = false;
    isPaused_ = false;
}

void sudocku::reset() {
    init();
}

bool sudocku::isValidMove(int row, int col, int val) const {
    if (val == 0) return true;

    for (int c = 0; c < 9; ++c) {
        if (c != col && grid[row][c] == val) return false;
    }
    for (int r = 0; r < 9; ++r) {
        if (r != row && grid[r][col] == val) return false;
    }

    int boxStartRow = (row / 3) * 3;
    int boxStartCol = (col / 3) * 3;
    for (int r = boxStartRow; r < boxStartRow + 3; ++r) {
        for (int c = boxStartCol; c < boxStartCol + 3; ++c) {
            if ((r != row || c != col) && grid[r][c] == val) return false;
        }
    }
    return true;
}

bool sudocku::checkCompleted() const {
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (grid[r][c] == 0 || !isValidMove(r, c, grid[r][c])) {
                return false;
            }
        }
    }
    return true;
}

void sudocku::handleInput(InputKey key) {
    if (isCompleted || isPaused_) return;

    switch (key) {
        case InputKey::UP:
            if (cursorY > 0) cursorY--;
            break;
        case InputKey::DOWN:
            if (cursorY < 8) cursorY++;
            break;
        case InputKey::LEFT:
            if (cursorX > 0) cursorX--;
            break;
        case InputKey::RIGHT:
            if (cursorX < 8) cursorX++;
            break;
        case InputKey::KEY_1: if (!initialCell[cursorY][cursorX]) grid[cursorY][cursorX] = 1; break;
        case InputKey::KEY_2: if (!initialCell[cursorY][cursorX]) grid[cursorY][cursorX] = 2; break;
        case InputKey::KEY_3: if (!initialCell[cursorY][cursorX]) grid[cursorY][cursorX] = 3; break;
        case InputKey::KEY_4: if (!initialCell[cursorY][cursorX]) grid[cursorY][cursorX] = 4; break;
        case InputKey::SPACE: if (!initialCell[cursorY][cursorX]) grid[cursorY][cursorX] = 0; break; // erase
        default:
            break;
    }

    if (checkCompleted()) {
        isCompleted = true;
        gameOver = true;
        score = 1000;
    }
}

void sudocku::update(double deltaTime) {
    // Puzzle game: state changes on input
    (void)deltaTime;
}

void sudocku::render(IRenderer& renderer) {
    renderer.drawString(boardX, boardY - 1, "SUDOKU PUZZLE", Color::BRIGHT_YELLOW);

    // Draw grid border
    renderer.drawBox(boardX, boardY, 39, 19, '#', Color::BRIGHT_CYAN);

    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            int drawC = boardX + 2 + c * 4;
            int drawR = boardY + 1 + r * 2;

            bool isSelected = (r == cursorY && c == cursorX);
            int val = grid[r][c];
            std::string cellStr = (val == 0) ? " " : std::to_string(val);

            Color col = Color::WHITE;
            if (initialCell[r][c]) {
                col = Color::BRIGHT_CYAN;
            } else if (val != 0 && !isValidMove(r, c, val)) {
                col = Color::BRIGHT_RED;
            } else if (val != 0) {
                col = Color::BRIGHT_GREEN;
            }

            if (isSelected) {
                renderer.drawChar(drawC - 1, drawR, '[', Color::BRIGHT_YELLOW);
                renderer.drawString(drawC, drawR, cellStr, Color::BRIGHT_YELLOW);
                renderer.drawChar(drawC + 1, drawR, ']', Color::BRIGHT_YELLOW);
            } else {
                renderer.drawString(drawC, drawR, cellStr, col);
            }

            // Subgrid vertical line
            if (c % 3 == 2 && c < 8) {
                renderer.drawChar(drawC + 2, drawR, '|', Color::DEFAULT);
            }
        }

        // Subgrid horizontal line
        if (r % 3 == 2 && r < 8) {
            int drawR = boardY + 2 + r * 2;
            for (int c = boardX + 1; c < boardX + 38; ++c) {
                renderer.drawChar(c, drawR, '-', Color::DEFAULT);
            }
        }
    }

    // Sidebar instructions
    int hudX = boardX + 42;
    renderer.drawBox(hudX, boardY, 26, 10, '#', Color::BRIGHT_CYAN);
    renderer.drawString(hudX + 2, boardY + 1, "INSTRUCTIONS", Color::BRIGHT_YELLOW);
    renderer.drawString(hudX + 2, boardY + 3, "Arrows: Move cursor", Color::DEFAULT);
    renderer.drawString(hudX + 2, boardY + 4, "1-4: Set number", Color::DEFAULT);
    renderer.drawString(hudX + 2, boardY + 5, "Space: Clear cell", Color::DEFAULT);
    renderer.drawString(hudX + 2, boardY + 7, "P: Pause | ESC: Menu", Color::DEFAULT);

    if (isCompleted) {
        int midX = boardX + 10;
        int midY = boardY + 8;
        renderer.drawBox(midX, midY, 20, 3, '*', Color::BRIGHT_GREEN);
        renderer.drawString(midX + 4, midY + 1, "PUZZLE SOLVED!", Color::BRIGHT_GREEN);
    }
}

} // namespace emulator
