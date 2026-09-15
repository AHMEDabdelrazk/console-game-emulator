file(GLOB_RECURSE GAMES_SRC SRCs/*.cpp)

add_library(games STATIC ${GAMES_SRC})

target_include_directories(games PUBLIC HEADERs)

target_link_libraries(games PUBLIC model inputs viewer)