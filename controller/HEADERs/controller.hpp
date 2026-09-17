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
#include "qml_manager.hpp"

namespace emulator {

enum class AppState {
    MENU,
    PLAYING,
    EXIT
};

enum class LaunchMode {
    AUTO,
    CONSOLE,
    QML
};

class controller {
public:
    controller(std::shared_ptr<IInputListener> input = nullptr, std::shared_ptr<IRenderer> renderer = nullptr);
    ~controller();

    // Launch entry points
    int run(LaunchMode mode = LaunchMode::AUTO, int argc = 0, char* argv[] = nullptr);
    int startConsole();
    int startUI(int argc, char* argv[]);

    // Backward compatible tick & helpers
    void StartGame();
    void registerGame(std::shared_ptr<game> g);
    void selectGame(size_t index);
    void returnToMenu();

    std::shared_ptr<qml_manager> getQmlManager() const { return qmlManager_; }
    const std::vector<std::shared_ptr<game>>& getGames() const { return gameCatalog; }

private:
    std::shared_ptr<IInputListener> input;
    std::shared_ptr<IRenderer> renderer;
    std::shared_ptr<game> activeGame;
    std::vector<std::shared_ptr<game>> gameCatalog;
    std::shared_ptr<qml_manager> qmlManager_;

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