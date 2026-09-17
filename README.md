# 🎮 Game Emulator (Console + Handheld QML UI)

> **A personal project by Ahmed Abdelrazik Ramadan**

A modular, multi-game arcade and handheld emulator built in modern **C++17** and **Qt 6 QML**. The project features a dual-mode engine with a strict decoupled interface architecture:
1. **Handheld Retro QML UI**: Modeled after a real physical handheld gaming console (curved matte chassis, authentic D-Pad, tactile A/B/X/Y buttons, rubber system controls, speaker grilles, and an LCD screen with CRT scanlines).
2. **Terminal Double-Buffered Console Mode**: High-performance, 100% flicker-free terminal gaming with full color and ASCII art.

This completes **Stage 2** of the long-term vision towards building a full hardware emulator.

---

## 💡 The Vision & Roadmap

```
[ Stage 1 ] Console Game Emulator (COMPLETED)
            C++17 · Decoupled MVC Interfaces · 5 Complete Games
                          │
                          ▼
[ Stage 2 ] Retro Handheld QML UI (COMPLETED)
            Handheld Console Chassis · QmlManager · CRT LCD Screen · D-Pad/Buttons
                          │
                          ▼
[ Stage 3 ] Hardware Emulator Integration (IN PROGRESS)
            CPU Bus · Custom ROM Pipeline · Memory Mapping · Hardware Cartridges
```

---

## 🕹️ Playable Games Included

All 5 games are written in pure platform-independent C++17 and run identically in both the QML Handheld UI and the Console:

| # | Game | Description | Controls |
|---|---|---|---|
| 1 | 🐍 **Snake Classic** | Eat food, grow longer, avoid walls and your own tail. Dynamic speed curve. | `D-Pad` / `Arrows` / `WASD` |
| 2 | 🏎️ **Highway Racer** | Dodge oncoming traffic across 4 highway lanes. Accelerate, brake, survive. | `Left/Right` (Lanes), `Up/Down` (Speed) |
| 3 | 🧱 **Cubes (Tetris)** | Rotate and stack tetromino blocks to clear lines. Hard & soft drops with wall kicks. | `Arrows`, `Space` / `A` (Drop) |
| 4 | 🏓 **Retro Pong** | 1-Player vs AI paddle. Ball accelerates and deflects based on hit angle. First to 7 wins. | `Up/Down` / `W/S` |
| 5 | 🔢 **Sudoku 9x9** | Interactive 9x9 grid, cursor navigation, number placement with real-time rule checks. | `Arrows`, `1-4` (Numbers), `Space` |

---

## 🏗️ Architecture & Component Design

```
                     ┌───────────────────────────────┐
                     │       game_emulator.cpp       │
                     │  CLI Flags: --ui / --console  │
                     └───────────────┬───────────────┘
                                     │
                                     ▼
                     ┌───────────────────────────────┐
                     │          controller           │
                     │   startUI() / startConsole()  │
                     └───────┬───────────────┬───────┘
                             │               │
               LaunchMode::QML               LaunchMode::CONSOLE
                             │               │
                             ▼               ▼
                   ┌──────────────────┐    ┌──────────────────┐
                   │    QmlManager    │    │    task_manger   │
                   │ (QObject Bridge) │    │  (30 FPS Limiter)│
                   └─────────┬────────┘    └─────────┬────────┘
                             │                       │
           ┌─────────────────┴───────────────────────┴─────────────────┐
           ▼                                                           ▼
┌──────────────────────┐                                    ┌──────────────────────┐
│    IInputListener    │                                    │      IRenderer       │
│  - qml_input_listener│                                    │  - qml_renderer      │
│  - keybord_listener  │                                    │  - viewer (Console)  │
└──────────┬───────────┘                                    └──────────┬───────────┘
           │                                                           │
           └─────────────────────────────┬─────────────────────────────┘
                                         │
                                         ▼
                     ┌───────────────────────────────────────┐
                     │             Game Engine               │
                     │  snake · cars · cubes · pong · sudoku │
                     │       Pure Platform-Independent       │
                     └───────────────────────────────────────┘
```

### Key Reusable Components
- **`QmlManager` (`controller/HEADERs/qml_manager.hpp`)**: Bridges C++ game state, scores, active ROM information, and the screen frame buffer to the QML frontend via reactive Qt properties and invokable slots.
- **`IRenderer` & `IInputListener`**: Absolute decoupling — games never import Windows headers or Qt headers directly. They interact only through generic interfaces.
- **`QmlRenderer` (`viewer/HEADERs/qml_renderer.hpp`)**: Thread-safe frame buffer capturing draw commands and streaming them to the QML LCD screen.
- **`QmlInputListener` (`inputs/HEADERs/qml_input_listener.hpp`)**: Captures button presses from both physical keyboard and on-screen tactile D-Pad/Arcade buttons.

---

## 📱 The QML Handheld Console Experience

The QML UI (`qml/Main.qml`) includes:
- **Ergonomic Handheld Body**: Dark matte casing with bevel edges, speaker grilles, metallic screws, and glowing power LED.
- **CRT LCD Viewport (`ScreenView.qml`)**:
  - Embedded LCD screen with authentic horizontal CRT scanline effect.
  - Corner glass glare reflection gradient.
  - Status header with battery indicator, 60 FPS counter, and mode badge.
- **Interactive Cartridge Vault (`GameLibrary.qml`)**: Carousel displaying each game as a retro game cartridge with custom box art, genre tags, descriptions, and a 1-click launch button.
- **Tactile D-Pad (`DPad.qml`)**: 4-way cross controller with directional feedback.
- **Arcade Action Buttons (`ActionButtons.qml`)**: Diamond-layout A, B, X, Y buttons with 3D press animation.
- **Rubber Pill System Buttons (`SystemButtons.qml`)**: SELECT, START, MENU, PAUSE, RESET.

---

## 🚀 How to Build & Run

### Quick Start (Pre-configured MSYS2 UCRT64)

#### 1. Launch QML Handheld Emulator UI (Default)
```cmd
.\game_emulator.exe
```
Or explicitly:
```cmd
.\game_emulator.exe --ui
```

#### 2. Launch Terminal Console Mode
```cmd
.\game_emulator.exe --console
```

#### 3. View Help
```cmd
.\game_emulator.exe --help
```

---

### Rebuilding from Source

#### Option A: One-Click Qt6 QML Build (`build_qt.bat`)
```cmd
build_qt.bat
```
Configures with CMake + Ninja, runs automatic MOC/RCC, and produces `game_emulator.exe`.

#### Option B: Standalone Console Build (`build.bat`)
```cmd
build.bat
```
Compiles a standalone console binary using G++ without any Qt dependency.

---

## 🕹️ Universal Controls Reference

| Control | Action in Vault (Menu) | Action in Game |
|---|---|---|
| **D-Pad / Arrows / WASD** | Browse Game Cartridges | Move character / vehicle / paddle |
| **A Button / Space** | Insert Cartridge & Play | Primary Action / Hard Drop / Set Cell |
| **B Button / Enter** | Select Game | Secondary Action / Confirm |
| **START** | Launch Selected Game | Confirm |
| **PAUSE (P)** | — | Toggle Pause Overlay |
| **RESET (R)** | — | Restart Current Game |
| **MENU (ESC)** | Exit to Desktop | Return to Cartridge Vault |
| **1 – 5** | Quick-select game by number | Quick number placement |

---

## 👨‍💻 Author

**Ahmed Abdelrazik Ramadan**
Personal Project — 2026

> *"The console version was the proof of concept. The Qt handheld emulator is the product. The hardware emulator is the destination."*
