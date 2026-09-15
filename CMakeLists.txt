#---------------------------------------------------------------
# CMakeLists.txt
#---------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)
project(game_emulator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories in dependency order
add_subdirectory(model)
add_subdirectory(inputs)
add_subdirectory(viewer)
add_subdirectory(games)
add_subdirectory(controller)

# Create executable
add_executable(game_emulator game_emulator.cpp)

# Link libraries
target_link_libraries(game_emulator PRIVATE controller inputs viewer games model)
#---------------------------------------------------------------