file(GLOB_RECURSE VIEWER_SRC SRCs/*.cpp)

add_library(viewer STATIC ${VIEWER_SRC})

target_include_directories(viewer PUBLIC HEADERs)
target_link_libraries(viewer PUBLIC model)