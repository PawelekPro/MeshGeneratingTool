add_library(ShapeMap
    OcafShapeMap.cpp
)

target_link_libraries(ShapeMap PUBLIC
    ${OCC_LIBRARIES}
    Shapes
)

target_include_directories(ShapeMap PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)