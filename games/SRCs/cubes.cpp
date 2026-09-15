#include "cubes.hpp"
#include <algorithm>

namespace emulator {

cubes::cubes() : rng(std::random_device{}()) {
    init();
}

void cubes::init() {
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            board[r][c] = 0;
        }
    }

    score = 0;
    linesCleared = 0;
    gameOver = false;
    isPaused_ = false;
    dropTimer = 0.0;
    dropInterval = 0.45;

    nextPiece = getRandomPiece();
    currentPiece = getRandomPiece();
    piecePos = {BOARD_WIDTH / 2 - 2, 0};

    if (checkCollision(currentPiece, piecePos)) {
        gameOver = true;
    }
}

void cubes::reset() {
    init();
}

Tetromino cubes::getRandomPiece() {
    static const std::vector<Tetromino> templates = {
        // I
        {1, Color::BRIGHT_CYAN,   {{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
        // O
        {2, Color::BRIGHT_YELLOW, {{1, 0}, {2, 0}, {1, 1}, {2, 1}}},
        // T
        {3, Color::MAGENTA,       {{1, 0}, {0, 1}, {1, 1}, {2, 1}}},
        // S
        {4, Color::BRIGHT_GREEN,  {{1, 0}, {2, 0}, {0, 1}, {1, 1}}},
        // Z
        {5, Color::BRIGHT_RED,    {{0, 0}, {1, 0}, {1, 1}, {2, 1}}},
        // J
        {6, Color::BLUE,          {{0, 0}, {0, 1}, {1, 1}, {2, 1}}},
        // L
        {7, Color::WHITE,         {{2, 0}, {0, 1}, {1, 1}, {2, 1}}}
    };

    std::uniform_int_distribution<size_t> dist(0, templates.size() - 1);
    return templates[dist(rng)];
}

Tetromino cubes::rotatePiece(const Tetromino& piece) const {
    if (piece.id == 2) return piece; // O piece does not rotate

    Tetromino rotated = piece;
    for (auto& b : rotated.blocks) {
        // Rotate 90 deg clockwise around (1.5, 1.0)
        int oldX = b.x;
        int oldY = b.y;
        b.x = 2 - oldY;
        b.y = oldX;
    }
    return rotated;
}

bool cubes::checkCollision(const Tetromino& piece, const Point2D& pos) const {
    for (const auto& b : piece.blocks) {
        int x = pos.x + b.x;
        int y = pos.y + b.y;

        if (x < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT) {
            return true;
        }
        if (y >= 0 && board[y][x] != 0) {
            return true;
        }
    }
    return false;
}

void cubes::lockPiece() {
    for (const auto& b : currentPiece.blocks) {
        int x = piecePos.x + b.x;
        int y = piecePos.y + b.y;
        if (y >= 0 && y < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
            board[y][x] = static_cast<int>(currentPiece.color);
        }
    }

    clearLines();

    currentPiece = nextPiece;
    nextPiece = getRandomPiece();
    piecePos = {BOARD_WIDTH / 2 - 2, 0};

    if (checkCollision(currentPiece, piecePos)) {
        gameOver = true;
    }
}

void cubes::clearLines() {
    int clearedThisTick = 0;

    for (int r = BOARD_HEIGHT - 1; r >= 0; --r) {
        bool full = true;
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            if (board[r][c] == 0) {
                full = false;
                break;
            }
        }

        if (full) {
            clearedThisTick++;
            for (int rowAbove = r; rowAbove > 0; --rowAbove) {
                for (int c = 0; c < BOARD_WIDTH; ++c) {
                    board[rowAbove][c] = board[rowAbove - 1][c];
                }
            }
            for (int c = 0; c < BOARD_WIDTH; ++c) {
                board[0][c] = 0;
            }
            r++; // re-check row
        }
    }

    if (clearedThisTick > 0) {
        linesCleared += clearedThisTick;
        switch (clearedThisTick) {
            case 1: score += 100; break;
            case 2: score += 300; break;
            case 3: score += 500; break;
            case 4: score += 800; break;
            default: score += clearedThisTick * 200; break;
        }
        dropInterval = std::max(0.12, 0.45 - (linesCleared * 0.01));
    }
}

void cubes::handleInput(InputKey key) {
    if (gameOver || isPaused_) return;

    switch (key) {
        case InputKey::LEFT: {
            Point2D testPos = {piecePos.x - 1, piecePos.y};
            if (!checkCollision(currentPiece, testPos)) {
                piecePos = testPos;
            }
            break;
        }
        case InputKey::RIGHT: {
            Point2D testPos = {piecePos.x + 1, piecePos.y};
            if (!checkCollision(currentPiece, testPos)) {
                piecePos = testPos;
            }
            break;
        }
        case InputKey::UP: {
            Tetromino rotated = rotatePiece(currentPiece);
            if (!checkCollision(rotated, piecePos)) {
                currentPiece = rotated;
            } else {
                // Wall kick left / right
                Point2D kickLeft = {piecePos.x - 1, piecePos.y};
                Point2D kickRight = {piecePos.x + 1, piecePos.y};
                if (!checkCollision(rotated, kickLeft)) {
                    piecePos = kickLeft;
                    currentPiece = rotated;
                } else if (!checkCollision(rotated, kickRight)) {
                    piecePos = kickRight;
                    currentPiece = rotated;
                }
            }
            break;
        }
        case InputKey::DOWN: {
            Point2D testPos = {piecePos.x, piecePos.y + 1};
            if (!checkCollision(currentPiece, testPos)) {
                piecePos = testPos;
                score += 1;
            }
            break;
        }
        case InputKey::SPACE: {
            // Hard drop
            while (!checkCollision(currentPiece, {piecePos.x, piecePos.y + 1})) {
                piecePos.y++;
                score += 2;
            }
            lockPiece();
            break;
        }
        default:
            break;
    }
}

void cubes::update(double deltaTime) {
    if (gameOver || isPaused_) return;

    dropTimer += deltaTime;
    if (dropTimer >= dropInterval) {
        dropTimer = 0.0;
        Point2D testPos = {piecePos.x, piecePos.y + 1};
        if (!checkCollision(currentPiece, testPos)) {
            piecePos = testPos;
        } else {
            lockPiece();
        }
    }
}

void cubes::render(IRenderer& renderer) {
    int boardPixelWidth = BOARD_WIDTH * 2 + 2;
    int boardPixelHeight = BOARD_HEIGHT + 2;

    // Draw board border
    renderer.drawBox(boardX, boardY, boardPixelWidth, boardPixelHeight, '#', Color::BRIGHT_CYAN);

    // Draw locked blocks
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            int drawC = boardX + 1 + c * 2;
            int drawR = boardY + 1 + r;
            if (board[r][c] != 0) {
                renderer.drawString(drawC, drawR, "[]", static_cast<Color>(board[r][c]));
            } else {
                renderer.drawString(drawC, drawR, " .", Color::BLACK);
            }
        }
    }

    // Draw current falling piece
    for (const auto& b : currentPiece.blocks) {
        int c = piecePos.x + b.x;
        int r = piecePos.y + b.y;
        if (r >= 0 && r < BOARD_HEIGHT && c >= 0 && c < BOARD_WIDTH) {
            int drawC = boardX + 1 + c * 2;
            int drawR = boardY + 1 + r;
            renderer.drawString(drawC, drawR, "[]", currentPiece.color);
        }
    }

    // Draw HUD
    int hudX = boardX + boardPixelWidth + 4;
    renderer.drawBox(hudX, boardY, 26, 16, '#', Color::BRIGHT_CYAN);
    renderer.drawString(hudX + 2, boardY + 1, "CUBES / TETRIS", Color::BRIGHT_YELLOW);
    renderer.drawString(hudX + 2, boardY + 3, "Score: " + std::to_string(score), Color::WHITE);
    renderer.drawString(hudX + 2, boardY + 4, "Lines: " + std::to_string(linesCleared), Color::BRIGHT_GREEN);

    renderer.drawString(hudX + 2, boardY + 6, "Next Piece:", Color::WHITE);
    // Draw Next Piece preview
    for (const auto& b : nextPiece.blocks) {
        renderer.drawString(hudX + 6 + b.x * 2, boardY + 8 + b.y, "[]", nextPiece.color);
    }

    renderer.drawString(hudX + 2, boardY + 12, "Arrows: Move / Rotate", Color::DEFAULT);
    renderer.drawString(hudX + 2, boardY + 13, "Space: Hard Drop", Color::DEFAULT);
    renderer.drawString(hudX + 2, boardY + 14, "P: Pause | ESC: Menu", Color::DEFAULT);

    if (gameOver) {
        int midX = boardX + boardPixelWidth / 2 - 7;
        int midY = boardY + boardPixelHeight / 2;
        renderer.drawBox(midX - 2, midY - 1, 18, 4, '=', Color::BRIGHT_RED);
        renderer.drawString(midX + 1, midY, "GAME OVER!", Color::BRIGHT_RED);
        renderer.drawString(midX, midY + 1, "R: Retry | ESC", Color::WHITE);
    } else if (isPaused_) {
        int midX = boardX + boardPixelWidth / 2 - 5;
        int midY = boardY + boardPixelHeight / 2;
        renderer.drawString(midX, midY, "[ PAUSED ]", Color::BRIGHT_YELLOW);
    }
}

} // namespace emulator
