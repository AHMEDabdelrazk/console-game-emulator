/** 
 * Game Emulator - Multi-Game Arcade Console System
 * Decoupled MVC / Interface Architecture ready for future Qt UI integration.
 * Copyright (c) 2026
 */

#include <iostream>
#include <memory>
#include "controller.hpp"
#include "keybord_listener.hpp"
#include "viewer.hpp"

using namespace emulator;

int main() {
    try {
        // 1. Initialize input provider (Console Keyboard Listener)
        auto input = std::make_shared<keybord_listener>();

        // 2. Initialize renderer (Console Double-Buffered Viewer, 100x28)
        auto renderer = std::make_shared<viewer>(100, 28);

        // 3. Instantiate emulator controller with decoupled components
        controller emulatorApp(input, renderer);

        // 4. Run emulator main loop (handles menus, active games, and clean shutdown)
        return emulatorApp.run();

    } catch (const std::exception& ex) {
        std::cerr << "Fatal Emulator Error: " << ex.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unknown fatal error occurred." << std::endl;
        return 1;
    }
}