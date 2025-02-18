add_subdirectory(ShapeIds)
add_subdirectory(ShapeMap)
add_subdirectory(ShapeStateManager)

add_library(Shapes
    ShapeManager.cpp
)

target_link_libraries(Shapes PUBLIC
    GeoDoc 
    ${OCC_LIBRARIES}
)

target_include_directories(Shapes PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)