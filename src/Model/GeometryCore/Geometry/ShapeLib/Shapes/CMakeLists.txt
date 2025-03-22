add_library(Shapes
    GeoShape.cpp
    ShapeId.cpp
)

target_link_libraries(Shapes PUBLIC
    ${OCC_LIBRARIES}
)

target_include_directories(Shapes PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)