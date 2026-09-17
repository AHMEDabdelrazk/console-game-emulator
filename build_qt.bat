@echo off
setlocal enabledelayedexpansion

echo =======================================================
echo     Building Game Emulator with Qt6 QML UI (C++17)
echo =======================================================

set PATH=C:\msys64\ucrt64\bin;C:\msys64\usr\bin;%PATH%

if not exist build_qt (
    mkdir build_qt
)

cd build_qt

echo Configuring with CMake and Ninja...
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/msys64/ucrt64" ..

if %errorlevel% neq 0 (
    echo Ninja configuration failed, trying MinGW Makefiles...
    cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/msys64/ucrt64" ..
)

if %errorlevel% neq 0 (
    echo CMake configuration failed!
    cd ..
    exit /b 1
)

echo Compiling Game Emulator with Qt QML...
cmake --build . --config Release

if %errorlevel% equ 0 (
    echo =======================================================
    echo  Build Successful! Output: build_qt\game_emulator.exe
    echo =======================================================
    copy /Y game_emulator.exe ..\game_emulator.exe >nul 2>nul
    cd ..
    exit /b 0
) else (
    echo =======================================================
    echo  Build Failed!
    echo =======================================================
    cd ..
    exit /b 1
)
