add_subdirectory(Shapes)
add_subdirectory(ShapeMap)
add_subdirectory(ShapeTools)
add_subdirectory(ShapeCore)

add_library(ShapeLib INTERFACE)

target_link_libraries(ShapeLib INTERFACE
    Shapes
    ShapeMap
    ShapeTools
    ShapeCore
)

target_include_directories(ShapeLib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)