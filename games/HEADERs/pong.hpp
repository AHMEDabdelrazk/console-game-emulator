#pragma once

#include "game.hpp"
#include "types.hpp"

namespace emulator {

class pong : public game {
public:
    pong();
    ~pong() override = default;

    std::string getName() const override { return "Retro Pong"; }
    std::string getDescription() const override { return "Classic 1-Player vs AI Pong! Move paddle with W/S or Up/Down."; }

    void init() override;
    void handleInput(InputKey key) override;
    void update(double deltaTime) override;
    void render(IRenderer& renderer) override;

    bool isGameOver() const override { return gameOver; }
    int getScore() const override { return playerScore; }
    void reset() override;

private:
    static constexpr int COURT_WIDTH = 64;
    static constexpr int COURT_HEIGHT = 20;
    static constexpr int PADDLE_HEIGHT = 4;
    static constexpr int WINNING_SCORE = 7;

    int courtX = 18;
    int courtY = 3;

    double playerY = 8.0;
    double aiY = 8.0;

    double ballX = 32.0;
    double ballY = 10.0;
    double ballVx = 25.0;
    double ballVy = 10.0;

    int playerScore = 0;
    int aiScore = 0;
    bool gameOver = false;
    std::string winnerText;

    void resetBall(bool serveToPlayer);
};

using PongGame = pong;

} // namespace emulator
