#include "task_manger.hpp"
#include <thread>

namespace emulator {

task_manger::task_manger(int targetFps)
    : targetFps(targetFps),
      frameDuration(1.0 / targetFps),
      lastFrameTime(std::chrono::high_resolution_clock::now())
{
}

void task_manger::tick() {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = now - lastFrameTime;
    deltaTime = elapsed.count();
    lastFrameTime = now;
}

double task_manger::getDeltaTime() const {
    return deltaTime;
}

void task_manger::limitFrameRate() {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> frameTime = now - lastFrameTime;
    if (frameTime < frameDuration) {
        std::this_thread::sleep_for(frameDuration - frameTime);
    }
}

} // namespace emulator
