#pragma once

#include <atomic>
#include <mutex>
#include <queue>
#include <thread>
#include "types.hpp"
#include "input_interface.hpp"

namespace emulator {

class keybord_listener : public IInputListener {
public:
    using KEY = InputKey;

    keybord_listener();
    ~keybord_listener() override;

    void start() override;
    void stop() override;
    InputKey getKey() override;
    bool hasKey() const override;
    void clearKeys() override;

    // Direct background worker
    void InputInterrupt();

    // Direct access to latest key for backwards compatibility
    std::atomic<InputKey> Key{InputKey::NONE};

private:
    std::atomic<bool> isRunning{false};
    std::thread workerThread;
    mutable std::mutex queueMutex;
    std::queue<InputKey> keyQueue;

    void pushKey(InputKey key);
};

using KeyboardListener = keybord_listener;

} // namespace emulator
