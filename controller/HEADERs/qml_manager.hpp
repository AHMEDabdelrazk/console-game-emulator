#pragma once

#include <memory>
#include <vector>
#include <string>
#include "types.hpp"
#include "game.hpp"
#include "qml_renderer.hpp"
#include "qml_input_listener.hpp"

#if defined(QT_CORE_LIB) || defined(ENABLE_QT)
#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QTimer>
#endif

namespace emulator {

enum class AppState;

#if defined(QT_CORE_LIB) || defined(ENABLE_QT)
class qml_manager : public QObject {
    Q_OBJECT
    Q_PROPERTY(int currentScore READ currentScore NOTIFY scoreChanged)
    Q_PROPERTY(QString activeGameName READ activeGameName NOTIFY activeGameChanged)
    Q_PROPERTY(QString activeGameDesc READ activeGameDesc NOTIFY activeGameChanged)
    Q_PROPERTY(int activeGameIndex READ activeGameIndex NOTIFY activeGameChanged)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY runningChanged)
    Q_PROPERTY(bool isPaused READ isPaused NOTIFY pausedChanged)
    Q_PROPERTY(bool isGameOver READ isGameOver NOTIFY gameOverChanged)
    Q_PROPERTY(int appState READ appState NOTIFY appStateChanged)
    Q_PROPERTY(QVariantList gameList READ gameList CONSTANT)
    Q_PROPERTY(QString frameText READ frameText NOTIFY frameUpdated)
    Q_PROPERTY(int screenWidth READ screenWidth CONSTANT)
    Q_PROPERTY(int screenHeight READ screenHeight CONSTANT)

public:
    explicit qml_manager(std::vector<std::shared_ptr<game>> games, QObject* parent = nullptr);
    ~qml_manager() override;

    int currentScore() const;
    QString activeGameName() const;
    QString activeGameDesc() const;
    int activeGameIndex() const { return activeIndex_; }
    bool isRunning() const { return running_; }
    bool isPaused() const;
    bool isGameOver() const;
    int appState() const;
    QVariantList gameList() const;
    QString frameText() const;
    int screenWidth() const { return 100; }
    int screenHeight() const { return 28; }

    std::shared_ptr<qml_renderer> getRenderer() const { return renderer_; }
    std::shared_ptr<qml_input_listener> getInputListener() const { return inputListener_; }

public slots:
    void launchGame(int index);
    void returnToMenu();
    void sendKey(int key);
    void sendKeyByName(const QString& keyName);
    void togglePause();
    void resetGame();
    void quitEmulator();
    QVariantList getScreenBufferSnapshot() const;

signals:
    void scoreChanged();
    void activeGameChanged();
    void runningChanged();
    void pausedChanged();
    void gameOverChanged();
    void appStateChanged();
    void frameUpdated();

private slots:
    void onTick();

private:
    std::vector<std::shared_ptr<game>> games_;
    std::shared_ptr<game> activeGame_;
    std::shared_ptr<qml_renderer> renderer_;
    std::shared_ptr<qml_input_listener> inputListener_;
    QTimer* gameLoopTimer_ = nullptr;
    int activeIndex_ = -1;
    bool running_ = true;
    int currentScoreCached_ = 0;
    bool isPausedCached_ = false;
    bool isGameOverCached_ = false;
};
#else
class qml_manager {
public:
    explicit qml_manager(std::vector<std::shared_ptr<game>> games) {}
    ~qml_manager() = default;
};
#endif

using QmlManager = qml_manager;

} // namespace emulator
