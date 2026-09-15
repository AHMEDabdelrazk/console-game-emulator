#pragma once

#include <chrono>
#include <thread>
#include <string>

namespace emulator {

class TimeUtils {
public:
    static double getDeltaTimeSeconds() {
        static auto lastTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currentTime - lastTime;
        lastTime = currentTime;
        return elapsed.count();
    }

    static void sleepMs(int ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
};

} // namespace emulator
