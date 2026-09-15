#include "controller.hpp"
#include "snake.hpp"
#include "cars.hpp"
#include "cubes.hpp"
#include "pong.hpp"
#include "sudocku.hpp"
#include <iostream>

namespace emulator {

controller::controller(std::shared_ptr<IInputListener> input, std::shared_ptr<IRenderer> renderer)
    : input(input), renderer(renderer), loopTimer(30)
{
    initializeGames();
}

controller::controller(std::shared_ptr<keybord_listener> input, std::shared_ptr<viewer> renderer)
    : input(input), renderer(renderer), loopTimer(30)
{
    initializeGames();
}

controller::~controller() {
    renderer->clearBuffer();
    renderer->present();
}

void controller::initializeGames() {
    registerGame(std::make_shared<snake>());
    registerGame(std::make_shared<cars>());
    registerGame(std::make_shared<cubes>());
    registerGame(std::make_shared<pong>());
    registerGame(std::make_shared<sudocku>());
}

void controller::registerGame(std::shared_ptr<game> g) {
    if (g) {
        gameCatalog.push_back(g);
    }
}

void controller::selectGame(size_t index) {
    if (index < gameCatalog.size()) {
        activeGame = gameCatalog[index];
        activeGame->reset();
        state = AppState::PLAYING;
        input->clearKeys();
    }
}

void controller::returnToMenu() {
    state = AppState::MENU;
    activeGame = nullptr;
    input->clearKeys();
}

void controller::handleMenuInput(InputKey key) {
    int totalOptions = static_cast<int>(gameCatalog.size()) + 1; // +1 for Exit

    switch (key) {
        case InputKey::UP:
            selectedMenuIndex = (selectedMenuIndex - 1 + totalOptions) % totalOptions;
            break;
        case InputKey::DOWN:
            selectedMenuIndex = (selectedMenuIndex + 1) % totalOptions;
            break;
        case InputKey::ENTER:
        case InputKey::SPACE:
            if (selectedMenuIndex == static_cast<int>(gameCatalog.size())) {
                state = AppState::EXIT;
                isRunning = false;
            } else {
                selectGame(selectedMenuIndex);
            }
            break;
        case InputKey::KEY_1: selectGame(0); break;
        case InputKey::KEY_2: selectGame(1); break;
        case InputKey::KEY_3: selectGame(2); break;
        case InputKey::KEY_4: selectGame(3); break;
        case InputKey::KEY_Q:
        case InputKey::ESC:
            state = AppState::EXIT;
            isRunning = false;
            break;
        default:
            break;
    }
}

void controller::handleInGameInput(InputKey key) {
    if (key == InputKey::ESC) {
        returnToMenu();
        return;
    }

    if (key == InputKey::KEY_P) {
        if (activeGame->isPaused()) {
            activeGame->resume();
        } else {
            activeGame->pause();
        }
        return;
    }

    if (key == InputKey::KEY_R) {
        activeGame->reset();
        return;
    }

    activeGame->handleInput(key);
}

void controller::renderMenu() {
    int menuWidth = 72;
    int menuHeight = 24;
    int startX = (renderer->getWidth() - menuWidth) / 2;
    int startY = 2;

    // Outer decorative box
    renderer->drawBox(startX, startY, menuWidth, menuHeight, '#', Color::BRIGHT_CYAN);

    // Header title banner
    std::string title = ">>> RETRO GAME EMULATOR <<<";
    int titleX = startX + (menuWidth - static_cast<int>(title.length())) / 2;
    renderer->drawString(titleX, startY + 2, title, Color::BRIGHT_YELLOW);

    std::string subtitle = "Select a game to play:";
    renderer->drawString(startX + 6, startY + 5, subtitle, Color::WHITE);

    // List games
    for (size_t i = 0; i < gameCatalog.size(); ++i) {
        int itemY = startY + 7 + static_cast<int>(i) * 2;
        bool isSelected = (static_cast<int>(i) == selectedMenuIndex);

        std::string prefix = isSelected ? " -> [" : "    [";
        std::string num = std::to_string(i + 1);
        std::string optText = prefix + num + "] " + gameCatalog[i]->getName();

        Color itemColor = isSelected ? Color::BRIGHT_YELLOW : Color::DEFAULT;
        renderer->drawString(startX + 6, itemY, optText, itemColor);

        if (isSelected) {
            std::string desc = "      " + gameCatalog[i]->getDescription();
            renderer->drawString(startX + 6, itemY + 1, desc, Color::BRIGHT_GREEN);
        }
    }

    // Exit option
    int exitY = startY + 7 + static_cast<int>(gameCatalog.size()) * 2 + 1;
    bool isExitSelected = (selectedMenuIndex == static_cast<int>(gameCatalog.size()));
    std::string exitPrefix = isExitSelected ? " -> [" : "    [";
    std::string exitText = exitPrefix + "Q] Exit Emulator";
    renderer->drawString(startX + 6, exitY, exitText, isExitSelected ? Color::BRIGHT_RED : Color::DEFAULT);

    // Footer controls
    renderer->drawString(startX + 6, startY + menuHeight - 3, "Controls: Up/Down: Navigate | Enter/Space: Play | Q: Quit", Color::BRIGHT_CYAN);
}

void controller::StartGame() {
    loopTimer.tick();
    double dt = loopTimer.getDeltaTime();

    // Process inputs
    while (input->hasKey()) {
        InputKey k = input->getKey();
        if (state == AppState::MENU) {
            handleMenuInput(k);
        } else if (state == AppState::PLAYING && activeGame) {
            handleInGameInput(k);
        }
    }

    // Update and Render
    renderer->clearBuffer();

    if (state == AppState::PLAYING && activeGame) {
        activeGame->update(dt);
        activeGame->render(*renderer);
    } else if (state == AppState::MENU) {
        renderMenu();
    }

    renderer->present();
    loopTimer.limitFrameRate();
}

int controller::run() {
    while (isRunning && state != AppState::EXIT) {
        StartGame();
    }
    return 0;
}

} // namespace emulator
