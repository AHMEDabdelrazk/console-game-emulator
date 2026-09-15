#pragma once

#include <memory>
#include <vector>
#include <string>
#include "types.hpp"
#include "input_interface.hpp"
#include "renderer_interface.hpp"
#include "keybord_listener.hpp"
#include "viewer.hpp"
#include "game.hpp"
#include "task_manger.hpp"

namespace emulator {

enum class AppState {
    MENU,
    PLAYING,
    EXIT
};

class controller {
public:
    controller(std::shared_ptr<IInputListener> input, std::shared_ptr<IRenderer> renderer);
    controller(std::shared_ptr<keybord_listener> input, std::shared_ptr<viewer> renderer);
    ~controller();

    // Main execution entry points
    int run();
    void StartGame(); // Backward compatible loop tick

    void registerGame(std::shared_ptr<game> g);
    void selectGame(size_t index);
    void returnToMenu();

private:
    std::shared_ptr<IInputListener> input;
    std::shared_ptr<IRenderer> renderer;
    std::shared_ptr<game> activeGame;
    std::vector<std::shared_ptr<game>> gameCatalog;

    AppState state = AppState::MENU;
    int selectedMenuIndex = 0;
    task_manger loopTimer;
    bool isRunning = true;

    void initializeGames();
    void handleMenuInput(InputKey key);
    void handleInGameInput(InputKey key);
    void renderMenu();
};

using Controller = controller;

} // namespace emulator