/** 
 * Game Emulator - Multi-Game Arcade Console & Handheld Emulator
 * Decoupled Architecture supporting Console Mode and Qt QML UI Mode.
 * Author: Ahmed Abdelrazik Ramadan
 * Copyright (c) 2026
 */

#include <iostream>
#include <string>
#include <memory>
#include "controller.hpp"

using namespace emulator;

int main(int argc, char* argv[]) {
    try {
        LaunchMode mode = LaunchMode::AUTO;

        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--console" || arg == "-c") {
                mode = LaunchMode::CONSOLE;
            } else if (arg == "--ui" || arg == "-u") {
                mode = LaunchMode::QML;
            } else if (arg == "--help" || arg == "-h") {
                std::cout << "Ahmed Abdelrazik's Game Emulator\n"
                          << "Usage: game_emulator [options]\n\n"
                          << "Options:\n"
                          << "  --ui, -u         Launch Handheld Retro QML Graphical UI (Default)\n"
                          << "  --console, -c    Launch Terminal Double-Buffered Console Mode\n"
                          << "  --help, -h       Display this help message\n";
                return 0;
            }
        }

        controller emulatorApp;
        return emulatorApp.run(mode, argc, argv);

    } catch (const std::exception& ex) {
        std::cerr << "Fatal Emulator Error: " << ex.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unknown fatal error occurred." << std::endl;
        return 1;
    }
}