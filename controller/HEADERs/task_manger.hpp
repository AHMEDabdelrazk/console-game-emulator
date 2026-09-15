#pragma once

#include <chrono>

namespace emulator {

class task_manger {
public:
    explicit task_manger(int targetFps = 30);
    ~task_manger() = default;

    void tick();
    double getDeltaTime() const;
    void limitFrameRate();

private:
    int targetFps;
    std::chrono::duration<double> frameDuration;
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    double deltaTime = 0.033;
};

using TaskManager = task_manger;

} // namespace emulator
