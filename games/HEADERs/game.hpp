#pragma once

#include <memory>
#include <string>
#include "types.hpp"
#include "renderer_interface.hpp"
#include "input_interface.hpp"

namespace emulator {

class game {
public:
    virtual ~game() = default;

    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;

    virtual void init() = 0;
    virtual void handleInput(InputKey key) = 0;
    virtual void update(double deltaTime) = 0;
    virtual void render(IRenderer& renderer) = 0;

    virtual bool isGameOver() const = 0;
    virtual int getScore() const = 0;
    virtual void reset() = 0;

    // Optional lifecycle hooks
    virtual void start() {}
    virtual void stop() {}
    virtual void pause() { isPaused_ = true; }
    virtual void resume() { isPaused_ = false; }
    virtual bool isPaused() const { return isPaused_; }

protected:
    bool isPaused_ = false;
};

using IGame = game;

} // namespace emulator