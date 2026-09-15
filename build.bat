@echo off
setlocal enabledelayedexpansion

echo =======================================================
echo           Building Game Emulator (C++17)
echo =======================================================

set CXX=C:\msys64\ucrt64\bin\g++.exe
if not exist "%CXX%" (
    where g++.exe >nul 2>nul
    if !errorlevel! equ 0 (
        set CXX=g++
    ) else (
        echo Error: g++.exe not found! Please check compiler path.
        exit /b 1
    )
)

echo Using compiler: %CXX%
echo Compiling source files...

"%CXX%" -std=c++17 -O2 ^
    -I "model/HEADERs" ^
    -I "inputs/HEADERs" ^
    -I "viewer/HEADERs" ^
    -I "games/HEADERs" ^
    -I "controller/HEADERs" ^
    inputs/SRCs/keybord_listener.cpp ^
    viewer/SRCs/viewer.cpp ^
    games/SRCs/snake.cpp ^
    games/SRCs/cars.cpp ^
    games/SRCs/cubes.cpp ^
    games/SRCs/pong.cpp ^
    games/SRCs/sudocku.cpp ^
    controller/SRCs/task_manger.cpp ^
    controller/SRCs/controller.cpp ^
    game_emulator.cpp ^
    -o game_emulator.exe

if %errorlevel% equ 0 (
    echo =======================================================
    echo  Build Successful! Generated: game_emulator.exe
    echo =======================================================
    exit /b 0
) else (
    echo =======================================================
    echo  Build Failed!
    echo =======================================================
    exit /b 1
)
