#include "controller.hpp"
#include "snake.hpp"
#include "cars.hpp"
#include "cubes.hpp"
#include "pong.hpp"
#include "sudocku.hpp"
#include <iostream>

#if defined(QT_CORE_LIB) || defined(ENABLE_QT)
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#endif

namespace emulator {

controller::controller(std::shared_ptr<IInputListener> in, std::shared_ptr<IRenderer> rend)
    : input(in), renderer(rend), loopTimer(30)
{
    initializeGames();
}

controller::~controller() {
    if (renderer) {
        renderer->clearBuffer();
        renderer->present();
    }
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
        if (input) input->clearKeys();
    }
}

void controller::returnToMenu() {
    state = AppState::MENU;
    activeGame = nullptr;
    if (input) input->clearKeys();
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
    if (!renderer) return;

    int menuWidth = 72;
    int menuHeight = 24;
    int startX = (renderer->getWidth() - menuWidth) / 2;
    int startY = 2;

    renderer->drawBox(startX, startY, menuWidth, menuHeight, '#', Color::BRIGHT_CYAN);

    std::string title = ">>> RETRO GAME EMULATOR <<<";
    int titleX = startX + (menuWidth - static_cast<int>(title.length())) / 2;
    renderer->drawString(titleX, startY + 2, title, Color::BRIGHT_YELLOW);

    std::string subtitle = "Select a game to play:";
    renderer->drawString(startX + 6, startY + 5, subtitle, Color::WHITE);

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

    int exitY = startY + 7 + static_cast<int>(gameCatalog.size()) * 2 + 1;
    bool isExitSelected = (selectedMenuIndex == static_cast<int>(gameCatalog.size()));
    std::string exitPrefix = isExitSelected ? " -> [" : "    [";
    std::string exitText = exitPrefix + "Q] Exit Emulator";
    renderer->drawString(startX + 6, exitY, exitText, isExitSelected ? Color::BRIGHT_RED : Color::DEFAULT);

    renderer->drawString(startX + 6, startY + menuHeight - 3, "Controls: Up/Down: Navigate | Enter/Space: Play | Q: Quit", Color::BRIGHT_CYAN);
}

void controller::StartGame() {
    if (!input || !renderer) return;

    loopTimer.tick();
    double dt = loopTimer.getDeltaTime();

    while (input->hasKey()) {
        InputKey k = input->getKey();
        if (state == AppState::MENU) {
            handleMenuInput(k);
        } else if (state == AppState::PLAYING && activeGame) {
            handleInGameInput(k);
        }
    }

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

int controller::startConsole() {
    if (!input) {
        input = std::make_shared<keybord_listener>();
    }
    if (!renderer) {
        renderer = std::make_shared<viewer>(100, 28);
    }

    std::cout << "Starting Game Emulator in Console Mode..." << std::endl;
    while (isRunning && state != AppState::EXIT) {
        StartGame();
    }
    return 0;
}

int controller::startUI(int argc, char* argv[]) {
#if defined(QT_CORE_LIB) || defined(ENABLE_QT)
    QGuiApplication app(argc, argv);
    app.setApplicationName("GameEmulator");
    app.setOrganizationName("AbdelrazikGames");

    qmlManager_ = std::make_shared<qml_manager>(gameCatalog);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("emulator", qmlManager_.get());

    // Try loading bundled resource first, fall back to local directory
    const QUrl url(QStringLiteral("qrc:/qml/Main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);
    if (engine.rootObjects().isEmpty()) {
        // Fallback to local relative file if qrc is not yet embedded
        engine.load(QUrl::fromLocalFile(QStringLiteral("qml/Main.qml")));
    }

    if (engine.rootObjects().isEmpty()) {
        std::cerr << "Failed to load QML UI, falling back to Console Mode...\n";
        return startConsole();
    }

    return app.exec();
#else
    (void)argc;
    (void)argv;
    std::cout << "Qt QML UI is not compiled in this build configuration. Launching Console Mode...\n";
    return startConsole();
#endif
}

int controller::run(LaunchMode mode, int argc, char* argv[]) {
    switch (mode) {
        case LaunchMode::CONSOLE:
            return startConsole();
        case LaunchMode::QML:
            return startUI(argc, argv);
        case LaunchMode::AUTO:
        default:
#if defined(QT_CORE_LIB) || defined(ENABLE_QT)
            return startUI(argc, argv);
#else
            return startConsole();
#endif
    }
}

} // namespace emulator
