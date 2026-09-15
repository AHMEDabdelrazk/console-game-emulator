file(GLOB_RECURSE INPUTS_SRC SRCs/*.cpp)

add_library(inputs STATIC ${INPUTS_SRC})

target_include_directories(inputs PUBLIC HEADERs)
target_link_libraries(inputs PUBLIC model)