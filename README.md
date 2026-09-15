# 🎮 Game Emulator

> **A personal project by Ahmed Abdelrazik Ramadan**

A modular, multi-game arcade emulator built in modern **C++17**, designed from the ground up with a clean, interface-driven architecture. This is the console test phase of a larger vision — a full-featured **hardware emulator** with a graphical **Qt UI frontend**.

---

## 💡 The Idea Behind This Project

This project started from a personal goal: I wanted to design and build a **game emulator engine** from scratch — not just as a coding exercise, but as the foundation of something bigger.

The plan is in **three stages**:

```
[ Stage 1 ] Console Game Emulator       ← You are here
            (C++17 · Interface-based architecture · 5 games)
                          |
                          v
[ Stage 2 ] Qt UI Version
            (Same engine, GUI frontend via Qt · no game logic changes)
                          |
                          v
[ Stage 3 ] Hardware Emulator Integration
            (Emulate real hardware: CPU, memory, I/O — plug in games as ROMs)
```

The architecture was specifically designed so that the rendering and input layers are **fully swappable** — switching from a terminal to a Qt widget or a hardware display requires zero changes to game logic.

---

## 🎮 Games Included

| # | Game | Description | Controls |
|---|------|-------------|----------|
| 1 | 🐍 **Snake Classic** | Eat food, grow longer, avoid yourself and walls. Speed increases as you grow. | `Arrows` / `WASD` |
| 2 | 🚗 **Highway Racer** | Dodge oncoming traffic across 4 lanes. Speed up, slow down, survive. | `A/D` — Lanes · `W/S` — Speed |
| 3 | 🟦 **Cubes (Tetris)** | Stack falling tetromino blocks, clear full lines, chase the high score. Hard drop & rotation with wall kicks. | `Arrows` · `Space` — Drop |
| 4 | 🏓 **Retro Pong** | 1-Player vs AI paddle. Ball accelerates, deflection angle depends on hit location. First to 7 wins. | `W/S` / `Up/Down` |
| 5 | 🔢 **Sudoku Puzzle** | Navigate a 9×9 grid, fill in numbers, get validated in real-time against rows, columns, and subgrids. | `Arrows` · `1-4` · `Space` — Clear |

---

## 🏗️ Architecture

The engine is built on a strict **interface-based MVC** pattern:

```
┌────────────────────────────────────────────────────────┐
│                   game_emulator.cpp                    │
│              (Clean, 30-line main() entry)             │
└─────────────────────┬──────────────────────────────────┘
                      │
                      ▼
┌────────────────────────────────────────────────────────┐
│                    Controller                          │
│     Arcade Menu · State Machine · 30 FPS Game Loop     │
│               Pause · Restart · Game Over              │
└──────────┬───────────────────────────────┬─────────────┘
           │                               │
    uses   ▼                        uses   ▼
┌──────────────────┐         ┌──────────────────────┐
│  IInputListener  │         │      IRenderer       │
│  (Input Layer)   │         │  (Rendering Layer)   │
└────────┬─────────┘         └──────────┬───────────┘
         │                              │
         ▼ Console                      ▼ Console
  keybord_listener              viewer (double-buffered,
  (non-blocking,                flicker-free Win32 API)
   thread-safe queue)
         │                              │
         ▼ Future Qt                    ▼ Future Qt
  QtInputListener               QtRenderer (QPainter /
  (QKeyEvent bridge)            QWidget / QGraphicsView)

                      ▼
┌────────────────────────────────────────────────────────┐
│                    Game Layer (IGame)                  │
│   snake · cars · cubes · pong · sudocku                │
│   Pure C++17 — Zero OS/Console/Qt dependencies        │
└────────────────────────────────────────────────────────┘
                      ▼
┌────────────────────────────────────────────────────────┐
│                  Model / Core Types                    │
│      Point2D · InputKey · Color · Direction            │
│               GameState (shared primitives)            │
└────────────────────────────────────────────────────────┘
```

### Why this matters
- **Games know nothing about the console** — they receive an `IRenderer&` and an `InputKey`. That's it.
- **Migrating to Qt** = implement `IRenderer` with `QPainter` + implement `IInputListener` with `QKeyEvent`. No game files touched.
- **Migrating to hardware** = implement `IRenderer` with a framebuffer / display driver. Same principle.

---

## 📁 Project Structure

```
game_emulator/
├── game_emulator.cpp          ← Main entry point (clean, 30 lines)
├── CMakeLists.txt             ← Root build configuration
├── build.bat                  ← One-click Windows build script
├── .gitignore
│
├── model/                     ← Shared core types (no dependencies)
│   └── HEADERs/
│       └── types.hpp          ← Point2D, InputKey, Color, Direction, GameState
│
├── inputs/                    ← Platform-specific input (swappable)
│   ├── HEADERs/
│   │   ├── input_interface.hpp   ← IInputListener abstract interface
│   │   └── keybord_listener.hpp  ← Console implementation (thread-safe)
│   └── SRCs/
│       └── keybord_listener.cpp
│
├── viewer/                    ← Platform-specific rendering (swappable)
│   ├── HEADERs/
│   │   ├── renderer_interface.hpp ← IRenderer abstract interface
│   │   └── viewer.hpp             ← Console double-buffered implementation
│   └── SRCs/
│       └── viewer.cpp
│
├── games/                     ← All game logic (100% platform-independent)
│   ├── HEADERs/
│   │   ├── game.hpp           ← IGame base class
│   │   ├── snake.hpp
│   │   ├── cars.hpp
│   │   ├── cubes.hpp
│   │   ├── pong.hpp
│   │   └── sudocku.hpp
│   └── SRCs/
│       ├── snake.cpp
│       ├── cars.cpp
│       ├── cubes.cpp
│       ├── pong.cpp
│       └── sudocku.cpp
│
├── controller/                ← Orchestrator: menu, loop, state
│   ├── HEADERs/
│   │   ├── controller.hpp
│   │   ├── task_manger.hpp    ← 30 FPS frame rate limiter
│   │   └── utils.hpp
│   └── SRCs/
│       ├── controller.cpp
│       └── task_manger.cpp
│
├── libs/                      ← Reserved: data structures, math, physics
│   ├── datastructures/
│   ├── math/
│   ├── physics/
│   └── searching/
│
└── _DOCs/                     ← Documentation & design
    └── ProjectArchitecture.md
```

---

## 🚀 How to Build & Run

### Requirements
- Windows OS
- [MSYS2](https://www.msys2.org/) with `ucrt64` toolchain (GCC 16+)
- C++17 compiler

### Option A — One-click script (recommended)
```cmd
build.bat
```
This compiles all source files directly with G++ and outputs `game_emulator.exe`.

### Option B — CMake
```cmd
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build . --config Debug
```

### Run
```cmd
game_emulator.exe
```

---

## 🕹️ In-Game Controls (Universal)

| Key | Action |
|-----|--------|
| `P` | Pause / Resume |
| `R` | Restart current game |
| `ESC` | Return to Main Menu |
| `Q` | Quit emulator |
| `1` – `5` | Quick-select game from menu |
| `Enter` / `Space` | Confirm selection in menu |

---

## 🔮 Roadmap

- [x] **Stage 1** — Console Game Emulator (current)
  - [x] Interface-based decoupled architecture
  - [x] 5 playable games
  - [x] Flicker-free double-buffered console rendering
  - [x] Arcade menu & state machine
- [ ] **Stage 2** — Qt UI Version
  - [ ] `QtRenderer` implementing `IRenderer` (QPainter / QWidget)
  - [ ] `QtInputListener` implementing `IInputListener` (QKeyEvent)
  - [ ] Game library panel with covers and descriptions
  - [ ] Settings, themes, and window scaling
- [ ] **Stage 3** — Hardware Emulator Integration
  - [ ] Custom CPU / memory bus emulation layer
  - [ ] ROM loading and execution pipeline
  - [ ] Hardware-mapped I/O via `IInputListener` / `IRenderer`
  - [ ] Game cartridge format definition

---

## 👨‍💻 Author

**Ahmed Abdelrazik Ramadan**
Personal Project — 2026

> *"The console version is the proof of concept. The Qt version is the product. The hardware emulator is the destination."*

---

## 📄 License

This is a personal project. All rights reserved © 2026 Ahmed Abdelrazik Ramadan.
