# Game Emulator Architecture & Reusable Components Documentation

## Overview
The **Game Emulator** is a modular C++ arcade gaming engine built with a decoupled Model-View-Controller (MVC) and interface-based design. It is specifically structured so that the console test environment can be replaced with a graphical **Qt UI** (or any other rendering/input framework) without altering game logic.

---

## High-Level Architecture Diagram

```
+-------------------------------------------------------------------+
|                        game_emulator.cpp                          |
|                     (Clean, Minimal main())                       |
+---------------------------------+---------------------------------+
                                  |
                                  v
+-------------------------------------------------------------------+
|                    controller / GameController                    |
|  - Game loop timing (task_manger / 30 FPS tick)                   |
|  - Main Arcade Selector Menu                                      |
|  - Active game state management & transitions                     |
+------------------+------------------------------+-----------------+
                   |                              |
         uses      v                    uses      v
+-----------------------+              +-----------------------+
|    IInputListener     |              |       IRenderer       |
|  (Input Abstraction)  |              | (Display Abstraction) |
+-----------+-----------+              +-----------+-----------+
            ^                                      ^
            | implements                           | implements
+-----------+-----------+              +-----------+-----------+
|   keybord_listener    |              |        viewer         |
| (Console Async Queue) |              | (Double-Buffered Win) |
+-----------------------+              +-----------------------+
            ^                                      ^
   [Qt: QKeyEventFilter]                  [Qt: QPainter/Widget]
                   \                              /
                    \                            /
                     v                          v
+-------------------------------------------------------------------+
|                           games / IGame                           |
|       Pure Game Logic (Tick, Input, Update, Render, State)        |
+-------------------------------------------------------------------+
|  - SnakeClassic (snake.hpp / snake.cpp)                           |
|  - HighwayRacer (cars.hpp / cars.cpp)                             |
|  - Cubes Tetris (cubes.hpp / cubes.cpp)                           |
|  - RetroPong    (pong.hpp / pong.cpp)                             |
|  - SudokuPuzzle (sudocku.hpp / sudocku.cpp)                       |
+-------------------------------------------------------------------+
|                           model / types                           |
|  - Point2D, InputKey, Color, Direction, GameState                 |
+-------------------------------------------------------------------+
```

---

## Component Breakdown

### 1. Reusable Model Layer (`model/`)
- **`model/HEADERs/types.hpp`**:
  - `Point2D`: 2D integer coordinates with vector arithmetic operators (`+`, `-`, `==`, `!=`, `<`).
  - `InputKey`: Platform-independent key enumeration (`UP`, `DOWN`, `LEFT`, `RIGHT`, `SPACE`, `ENTER`, `ESC`, `KEY_1`..`KEY_4`, `KEY_P`, `KEY_R`, `KEY_Q`).
  - `Color`: Universal 16-color palette enum mapping to console attributes or Qt `QColor`.
  - `GameState`: Core states (`RUNNING`, `PAUSED`, `GAME_OVER`, `QUIT`, `MENU`).

### 2. Swappable Input Layer (`inputs/`)
- **`inputs/HEADERs/input_interface.hpp`**:
  - `IInputListener`: Defines `getKey()`, `hasKey()`, `clearKeys()`, `start()`, and `stop()`.
- **`inputs/HEADERs/keybord_listener.hpp` & `keybord_listener.cpp`**:
  - Non-blocking background thread reading raw keys safely using `_kbhit()` and `_getch()`.
  - Thread-safe `std::queue<InputKey>` protected by `std::mutex`.
  - Proper RAII cleanup with `std::atomic<bool> isRunning`.
- **Migrating to Qt**:
  - Simply create `class QtInputListener : public IInputListener` and forward `keyPressEvent(QKeyEvent* event)` to `pushKey(...)`.

### 3. Swappable Presentation Layer (`viewer/`)
- **`viewer/HEADERs/renderer_interface.hpp`**:
  - `IRenderer`: Defines `clearBuffer()`, `drawChar()`, `drawString()`, `drawBox()`, `fillRect()`, `present()`, `getWidth()`, `getHeight()`.
- **`viewer/HEADERs/viewer.hpp` & `viewer/SRCs/viewer.cpp`**:
  - High-performance double-buffered console rendering using `WriteConsoleOutputCharacterA` and `WriteConsoleOutputAttribute`.
  - 100% flicker-free rendering with no screen tearing.
- **Migrating to Qt**:
  - Create `class QtRenderer : public QWidget, public IRenderer`.
  - Implement `present()` by calling `update()`, and in `paintEvent(QPaintEvent*)`, draw the 2D grid using `QPainter::drawText` or colored tiles.

### 4. Game Logic Layer (`games/`)
- **`games/HEADERs/game.hpp`**:
  - Abstract base class `game` / `IGame`:
    - `init()` / `reset()`
    - `handleInput(InputKey key)`
    - `update(double deltaTime)`
    - `render(IRenderer& renderer)`
    - `isGameOver()`, `getScore()`, `getName()`, `getDescription()`
- **Included Games**:
  1. **Snake Classic** (`snake.hpp/cpp`): Dynamic growth, food generation, speed ramping, self/wall collision.
  2. **Highway Racer** (`cars.hpp/cpp`): Multi-lane driving, oncoming traffic, scrolling road lines, distance score.
  3. **Cubes / Tetris** (`cubes.hpp/cpp`): 7 standard tetromino shapes, rotation with wall kicks, line clearing and scoring.
  4. **Retro Pong** (`pong.hpp/cpp`): Smooth ball physics, player paddle vs CPU AI paddle, wall/paddle deflection.
  5. **Sudoku Puzzle** (`sudocku.hpp/cpp`): Interactive 9x9 grid navigation, number placement, row/column/box validation.

### 5. Controller Layer (`controller/`)
- **`controller/HEADERs/controller.hpp` & `controller.cpp`**:
  - Orchestrator maintaining game catalog, menu navigation, game loop, pause overlay, and game over screens.
- **`controller/HEADERs/task_manger.hpp` & `task_manger.cpp`**:
  - Frame rate limiter keeping loop stable at 30 FPS.

### 6. Clean Entry Point (`game_emulator.cpp`)
- Minimal `main()` with standard exception handling and clean RAII lifecycle.
