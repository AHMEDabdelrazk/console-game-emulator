file(GLOB_RECURSE CONTROLLER_SRC SRCs/*.cpp)
file(GLOB_RECURSE CONTROLLER_HDR HEADERs/*.hpp)

add_library(controller STATIC ${CONTROLLER_SRC} ${CONTROLLER_HDR})

target_include_directories(controller PUBLIC HEADERs)
target_link_libraries(controller PUBLIC model inputs viewer games)

if(TARGET Qt6::Core)
    target_link_libraries(controller PUBLIC Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick)
    target_compile_definitions(controller PUBLIC ENABLE_QT)
endif()