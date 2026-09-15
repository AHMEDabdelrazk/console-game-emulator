file(GLOB_RECURSE CONTROLLER_SRC SRCs/*.cpp)

add_library(controller STATIC ${CONTROLLER_SRC})

target_include_directories(controller PUBLIC HEADERs)

target_link_libraries(controller PUBLIC model inputs viewer games)