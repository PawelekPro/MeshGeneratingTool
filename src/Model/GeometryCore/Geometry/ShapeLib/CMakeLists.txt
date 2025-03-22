add_subdirectory(Shape)
add_subdirectory(ShapeMap)
add_subdirectory(ShapeStateManager)

add_library(ShapeLib INTERFACE)

target_link_libraries(ShapeLib PUBLIC
    Shapes
    ShapeMap
    ShapeManager
)

target_include_directories(ShapeLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)