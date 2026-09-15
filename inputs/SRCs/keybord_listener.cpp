#include "keybord_listener.hpp"
#include <conio.h>
#include <chrono>

namespace emulator {

keybord_listener::keybord_listener() {
    start();
}

keybord_listener::~keybord_listener() {
    stop();
}

void keybord_listener::start() {
    if (isRunning.load()) return;
    isRunning.store(true);
    workerThread = std::thread(&keybord_listener::InputInterrupt, this);
}

void keybord_listener::stop() {
    if (!isRunning.load()) return;
    isRunning.store(false);
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void keybord_listener::pushKey(InputKey key) {
    Key.store(key);
    std::lock_guard<std::mutex> lock(queueMutex);
    keyQueue.push(key);
}

InputKey keybord_listener::getKey() {
    std::lock_guard<std::mutex> lock(queueMutex);
    if (keyQueue.empty()) {
        return InputKey::NONE;
    }
    InputKey k = keyQueue.front();
    keyQueue.pop();
    return k;
}

bool keybord_listener::hasKey() const {
    std::lock_guard<std::mutex> lock(queueMutex);
    return !keyQueue.empty();
}

void keybord_listener::clearKeys() {
    std::lock_guard<std::mutex> lock(queueMutex);
    while (!keyQueue.empty()) {
        keyQueue.pop();
    }
    Key.store(InputKey::NONE);
}

void keybord_listener::InputInterrupt() {
    while (isRunning.load()) {
        if (_kbhit()) {
            int ch = _getch();
            InputKey detected = InputKey::NONE;

            if (ch == 0 || ch == 224) {
                // Extended key (arrows, etc.)
                int arrow = _getch();
                switch (arrow) {
                    case 72: detected = InputKey::UP; break;
                    case 80: detected = InputKey::DOWN; break;
                    case 75: detected = InputKey::LEFT; break;
                    case 77: detected = InputKey::RIGHT; break;
                    default: break;
                }
            } else {
                switch (ch) {
                    // Arrow equivalents (WASD)
                    case 'w': case 'W': detected = InputKey::UP; break;
                    case 's': case 'S': detected = InputKey::DOWN; break;
                    case 'a': case 'A': detected = InputKey::LEFT; break;
                    case 'd': case 'D': detected = InputKey::RIGHT; break;

                    // Action keys
                    case 32: detected = InputKey::SPACE; break;
                    case 13: detected = InputKey::ENTER; break;
                    case 27: detected = InputKey::ESC; break;

                    // Number selection
                    case '1': detected = InputKey::KEY_1; break;
                    case '2': detected = InputKey::KEY_2; break;
                    case '3': detected = InputKey::KEY_3; break;
                    case '4': detected = InputKey::KEY_4; break;

                    // Shortcuts
                    case 'p': case 'P': detected = InputKey::KEY_P; break;
                    case 'r': case 'R': detected = InputKey::KEY_R; break;
                    case 'q': case 'Q': detected = InputKey::KEY_Q; break;

                    default: break;
                }
            }

            if (detected != InputKey::NONE) {
                pushKey(detected);
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

} // namespace emulator