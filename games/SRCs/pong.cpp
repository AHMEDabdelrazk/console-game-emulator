#include "pong.hpp"
#include <cmath>
#include <algorithm>

namespace emulator {

pong::pong() {
    init();
}

void pong::init() {
    playerY = courtY + COURT_HEIGHT / 2.0 - PADDLE_HEIGHT / 2.0;
    aiY = courtY + COURT_HEIGHT / 2.0 - PADDLE_HEIGHT / 2.0;
    playerScore = 0;
    aiScore = 0;
    gameOver = false;
    isPaused_ = false;
    winnerText.clear();
    resetBall(true);
}

void pong::reset() {
    init();
}

void pong::resetBall(bool serveToPlayer) {
    ballX = courtX + COURT_WIDTH / 2.0;
    ballY = courtY + COURT_HEIGHT / 2.0;
    ballVx = serveToPlayer ? -22.0 : 22.0;
    ballVy = 8.0;
}

void pong::handleInput(InputKey key) {
    if (gameOver || isPaused_) return;

    double paddleSpeed = 2.0;
    switch (key) {
        case InputKey::UP:
            playerY = std::max(static_cast<double>(courtY + 1), playerY - paddleSpeed);
            break;
        case InputKey::DOWN:
            playerY = std::min(static_cast<double>(courtY + COURT_HEIGHT - 1 - PADDLE_HEIGHT), playerY + paddleSpeed);
            break;
        default:
            break;
    }
}

void pong::update(double deltaTime) {
    if (gameOver || isPaused_) return;

    // AI movement logic
    double aiCenter = aiY + PADDLE_HEIGHT / 2.0;
    double aiSpeed = 16.0 * deltaTime;
    if (ballVx > 0) { // Ball moving toward AI
        if (ballY < aiCenter - 0.5) {
            aiY = std::max(static_cast<double>(courtY + 1), aiY - aiSpeed);
        } else if (ballY > aiCenter + 0.5) {
            aiY = std::min(static_cast<double>(courtY + COURT_HEIGHT - 1 - PADDLE_HEIGHT), aiY + aiSpeed);
        }
    }

    // Ball movement
    ballX += ballVx * deltaTime;
    ballY += ballVy * deltaTime;

    // Top / Bottom wall bounce
    if (ballY <= courtY + 1) {
        ballY = courtY + 1;
        ballVy = -ballVy;
    } else if (ballY >= courtY + COURT_HEIGHT - 2) {
        ballY = courtY + COURT_HEIGHT - 2;
        ballVy = -ballVy;
    }

    // Player paddle hit
    double playerPaddleX = courtX + 2;
    if (ballX <= playerPaddleX + 1 && ballX >= playerPaddleX - 1) {
        if (ballY >= playerY - 0.5 && ballY <= playerY + PADDLE_HEIGHT + 0.5) {
            ballVx = std::min(40.0, std::abs(ballVx) * 1.06);
            double offset = (ballY - (playerY + PADDLE_HEIGHT / 2.0)) / (PADDLE_HEIGHT / 2.0);
            ballVy = offset * 18.0;
            ballX = playerPaddleX + 1.2;
        }
    }

    // AI paddle hit
    double aiPaddleX = courtX + COURT_WIDTH - 3;
    if (ballX >= aiPaddleX - 1 && ballX <= aiPaddleX + 1) {
        if (ballY >= aiY - 0.5 && ballY <= aiY + PADDLE_HEIGHT + 0.5) {
            ballVx = -std::min(40.0, std::abs(ballVx) * 1.06);
            double offset = (ballY - (aiY + PADDLE_HEIGHT / 2.0)) / (PADDLE_HEIGHT / 2.0);
            ballVy = offset * 18.0;
            ballX = aiPaddleX - 1.2;
        }
    }

    // Point scoring
    if (ballX < courtX) {
        aiScore++;
        if (aiScore >= WINNING_SCORE) {
            gameOver = true;
            winnerText = "CPU WON!";
        } else {
            resetBall(true);
        }
    } else if (ballX > courtX + COURT_WIDTH) {
        playerScore++;
        if (playerScore >= WINNING_SCORE) {
            gameOver = true;
            winnerText = "YOU WON!";
        } else {
            resetBall(false);
        }
    }
}

void pong::render(IRenderer& renderer) {
    // Draw court border
    renderer.drawBox(courtX, courtY, COURT_WIDTH, COURT_HEIGHT, '#', Color::BRIGHT_CYAN);

    // Center net
    int midCol = courtX + COURT_WIDTH / 2;
    for (int r = courtY + 1; r < courtY + COURT_HEIGHT - 1; r += 2) {
        renderer.drawChar(midCol, r, '|', Color::DEFAULT);
    }

    // Score board
    std::string scoreStr = "PLAYER  " + std::to_string(playerScore) + "  :  " + std::to_string(aiScore) + "  CPU";
    renderer.drawString(midCol - static_cast<int>(scoreStr.length()) / 2, courtY - 1, scoreStr, Color::BRIGHT_YELLOW);

    // Draw player paddle
    int pCol = courtX + 2;
    for (int r = 0; r < PADDLE_HEIGHT; ++r) {
        renderer.drawChar(pCol, static_cast<int>(playerY) + r, '|', Color::BRIGHT_GREEN);
    }

    // Draw AI paddle
    int aiCol = courtX + COURT_WIDTH - 3;
    for (int r = 0; r < PADDLE_HEIGHT; ++r) {
        renderer.drawChar(aiCol, static_cast<int>(aiY) + r, '|', Color::BRIGHT_RED);
    }

    // Draw ball
    renderer.drawChar(static_cast<int>(ballX), static_cast<int>(ballY), 'O', Color::BRIGHT_YELLOW);

    // HUD controls
    renderer.drawString(courtX, courtY + COURT_HEIGHT + 1, "W/S or Up/Down: Move Paddle | P: Pause | ESC: Menu", Color::DEFAULT);

    if (gameOver) {
        int midX = courtX + COURT_WIDTH / 2 - 8;
        int midY = courtY + COURT_HEIGHT / 2;
        renderer.drawBox(midX - 2, midY - 1, 20, 4, '=', Color::BRIGHT_YELLOW);
        renderer.drawString(midX + 3, midY, winnerText, Color::BRIGHT_YELLOW);
        renderer.drawString(midX, midY + 1, "R: Rematch | ESC", Color::WHITE);
    } else if (isPaused_) {
        int midX = courtX + COURT_WIDTH / 2 - 5;
        int midY = courtY + COURT_HEIGHT / 2;
        renderer.drawString(midX, midY, "[ PAUSED ]", Color::BRIGHT_YELLOW);
    }
}

} // namespace emulator
