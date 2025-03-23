add_subdirectory(Shapes)
add_subdirectory(ShapeMap)
add_subdirectory(ShapeManager)
add_subdirectory(ShapeImport)

add_library(ShapeLib INTERFACE)

target_link_libraries(ShapeLib INTERFACE
    Shapes
    ShapeMap
    ShapeManager
    ShapeImport
)

target_include_directories(ShapeLib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)