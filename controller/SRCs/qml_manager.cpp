#include "qml_manager.hpp"

#if defined(QT_CORE_LIB) || defined(ENABLE_QT)
#include <QCoreApplication>
#include <QVariantMap>

namespace emulator {

qml_manager::qml_manager(std::vector<std::shared_ptr<game>> games, QObject* parent)
    : QObject(parent),
      games_(std::move(games)),
      renderer_(std::make_shared<qml_renderer>(100, 28)),
      inputListener_(std::make_shared<qml_input_listener>())
{
    gameLoopTimer_ = new QTimer(this);
    connect(gameLoopTimer_, &QTimer::timeout, this, &qml_manager::onTick);
    gameLoopTimer_->start(33); // ~30 FPS game loop
}

qml_manager::~qml_manager() {
    if (gameLoopTimer_) {
        gameLoopTimer_->stop();
    }
}

int qml_manager::currentScore() const {
    if (activeGame_) return activeGame_->getScore();
    return 0;
}

QString qml_manager::activeGameName() const {
    if (activeGame_) return QString::fromStdString(activeGame_->getName());
    return QStringLiteral("No Game Loaded");
}

QString qml_manager::activeGameDesc() const {
    if (activeGame_) return QString::fromStdString(activeGame_->getDescription());
    return QStringLiteral("Select a game cartridge to start playing!");
}

bool qml_manager::isPaused() const {
    if (activeGame_) return activeGame_->isPaused();
    return false;
}

bool qml_manager::isGameOver() const {
    if (activeGame_) return activeGame_->isGameOver();
    return false;
}

int qml_manager::appState() const {
    return activeGame_ ? 1 : 0; // 0 = MENU, 1 = PLAYING
}

QVariantList qml_manager::gameList() const {
    QVariantList list;
    for (size_t i = 0; i < games_.size(); ++i) {
        QVariantMap map;
        map["index"] = static_cast<int>(i);
        map["name"] = QString::fromStdString(games_[i]->getName());
        map["description"] = QString::fromStdString(games_[i]->getDescription());
        list.append(map);
    }
    return list;
}

QString qml_manager::frameText() const {
    if (renderer_) {
        return QString::fromStdString(renderer_->getPlainTextFrame());
    }
    return QString();
}

void qml_manager::launchGame(int index) {
    if (index >= 0 && index < static_cast<int>(games_.size())) {
        activeIndex_ = index;
        activeGame_ = games_[index];
        activeGame_->reset();
        inputListener_->clearKeys();

        renderer_->clearBuffer();
        activeGame_->render(*renderer_);
        renderer_->present();

        emit activeGameChanged();
        emit appStateChanged();
        emit scoreChanged();
        emit pausedChanged();
        emit gameOverChanged();
        emit frameUpdated();
    }
}

void qml_manager::returnToMenu() {
    activeGame_ = nullptr;
    activeIndex_ = -1;
    inputListener_->clearKeys();
    renderer_->clearBuffer();
    renderer_->present();

    emit activeGameChanged();
    emit appStateChanged();
    emit scoreChanged();
    emit pausedChanged();
    emit gameOverChanged();
    emit frameUpdated();
}

void qml_manager::sendKey(int key) {
    if (inputListener_) {
        inputListener_->pushKey(static_cast<InputKey>(key));
    }
}

void qml_manager::sendKeyByName(const QString& keyName) {
    QString upper = keyName.toUpper();
    InputKey k = InputKey::NONE;

    if (upper == "UP" || upper == "W") k = InputKey::UP;
    else if (upper == "DOWN" || upper == "S") k = InputKey::DOWN;
    else if (upper == "LEFT" || upper == "A") k = InputKey::LEFT;
    else if (upper == "RIGHT" || upper == "D") k = InputKey::RIGHT;
    else if (upper == "A_BTN" || upper == "SPACE") k = InputKey::SPACE;
    else if (upper == "B_BTN" || upper == "ENTER") k = InputKey::ENTER;
    else if (upper == "X_BTN" || upper == "1") k = InputKey::KEY_1;
    else if (upper == "Y_BTN" || upper == "2") k = InputKey::KEY_2;
    else if (upper == "3") k = InputKey::KEY_3;
    else if (upper == "4") k = InputKey::KEY_4;
    else if (upper == "START") k = InputKey::ENTER;
    else if (upper == "SELECT") k = InputKey::SPACE;
    else if (upper == "PAUSE" || upper == "P") { togglePause(); return; }
    else if (upper == "RESET" || upper == "R") { resetGame(); return; }
    else if (upper == "MENU" || upper == "ESC") { returnToMenu(); return; }

    if (k != InputKey::NONE && inputListener_) {
        inputListener_->pushKey(k);
    }
}

void qml_manager::togglePause() {
    if (activeGame_) {
        if (activeGame_->isPaused()) {
            activeGame_->resume();
        } else {
            activeGame_->pause();
        }
        emit pausedChanged();
    }
}

void qml_manager::resetGame() {
    if (activeGame_) {
        activeGame_->reset();
        inputListener_->clearKeys();
        emit scoreChanged();
        emit pausedChanged();
        emit gameOverChanged();
    }
}

void qml_manager::quitEmulator() {
    running_ = false;
    emit runningChanged();
    QCoreApplication::quit();
}

QVariantList qml_manager::getScreenBufferSnapshot() const {
    QVariantList cells;
    if (!renderer_) return cells;

    auto snapshot = renderer_->getFrameSnapshot();
    cells.reserve(static_cast<int>(snapshot.size()));
    for (const auto& cell : snapshot) {
        QVariantMap cellMap;
        cellMap["ch"] = QString(cell.ch);
        cellMap["col"] = static_cast<int>(cell.color);
        cells.append(cellMap);
    }
    return cells;
}

void qml_manager::onTick() {
    if (!activeGame_) return;

    // Drain queued inputs
    while (inputListener_->hasKey()) {
        InputKey k = inputListener_->getKey();
        if (k == InputKey::ESC) {
            returnToMenu();
            return;
        } else if (k == InputKey::KEY_P) {
            togglePause();
            return;
        } else if (k == InputKey::KEY_R) {
            resetGame();
            return;
        }
        activeGame_->handleInput(k);
    }

    // Step physics & logic
    activeGame_->update(0.033);

    // Render frame to QML renderer buffer
    renderer_->clearBuffer();
    activeGame_->render(*renderer_);
    renderer_->present();

    // Check reactive properties
    int newScore = activeGame_->getScore();
    if (newScore != currentScoreCached_) {
        currentScoreCached_ = newScore;
        emit scoreChanged();
    }

    bool newPaused = activeGame_->isPaused();
    if (newPaused != isPausedCached_) {
        isPausedCached_ = newPaused;
        emit pausedChanged();
    }

    bool newGameOver = activeGame_->isGameOver();
    if (newGameOver != isGameOverCached_) {
        isGameOverCached_ = newGameOver;
        emit gameOverChanged();
    }

    emit frameUpdated();
}

} // namespace emulator

#endif // QT_CORE_LIB
