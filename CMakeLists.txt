#---------------------------------------------------------------
# CMakeLists.txt - Game Emulator with QML UI and Console Support
#---------------------------------------------------------------
cmake_minimum_required(VERSION 3.16)
project(game_emulator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Check for Qt6
find_package(Qt6 COMPONENTS Core Gui Qml Quick QUIET)

if(Qt6_FOUND)
    message(STATUS "Qt6 found (${Qt6_VERSION}). Building with Handheld QML UI support.")
    add_compile_definitions(ENABLE_QT)
else()
    message(STATUS "Qt6 not found. Building Console-only mode.")
endif()

# Add Subdirectories in dependency order
add_subdirectory(model)
add_subdirectory(inputs)
add_subdirectory(viewer)
add_subdirectory(games)
add_subdirectory(controller)

# Gather extra sources (e.g. QRC resources)
set(EXTRA_SOURCES "")
if(Qt6_FOUND)
    list(APPEND EXTRA_SOURCES qml/qml.qrc)
endif()

# Create executable
add_executable(game_emulator game_emulator.cpp ${EXTRA_SOURCES})

# Link libraries
target_link_libraries(game_emulator PRIVATE controller inputs viewer games model)
if(Qt6_FOUND)
    target_link_libraries(game_emulator PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick)
endif()
#---------------------------------------------------------------