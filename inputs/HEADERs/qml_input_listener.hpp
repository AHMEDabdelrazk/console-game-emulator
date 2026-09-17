#pragma once

#include <mutex>
#include <queue>
#include "types.hpp"
#include "input_interface.hpp"

namespace emulator {

class qml_input_listener : public IInputListener {
public:
    qml_input_listener() = default;
    ~qml_input_listener() override = default;

    void start() override { isListening = true; }
    void stop() override { isListening = false; }

    InputKey getKey() override {
        std::lock_guard<std::mutex> lock(mtx);
        if (keyQueue.empty()) return InputKey::NONE;
        InputKey k = keyQueue.front();
        keyQueue.pop();
        return k;
    }

    bool hasKey() const override {
        std::lock_guard<std::mutex> lock(mtx);
        return !keyQueue.empty();
    }

    void clearKeys() override {
        std::lock_guard<std::mutex> lock(mtx);
        while (!keyQueue.empty()) keyQueue.pop();
    }

    // Called by QML UI or key events
    void pushKey(InputKey key) {
        if (!isListening) return;
        std::lock_guard<std::mutex> lock(mtx);
        keyQueue.push(key);
    }

private:
    mutable std::mutex mtx;
    std::queue<InputKey> keyQueue;
    bool isListening = true;
};

using QmlInputListener = qml_input_listener;

} // namespace emulator
