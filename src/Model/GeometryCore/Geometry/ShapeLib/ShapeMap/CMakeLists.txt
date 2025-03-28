add_library(ShapeMap
    OcafShapeMap.cpp
    TagMap.cpp
)

target_link_libraries(ShapeMap PUBLIC
    ${OCC_LIBRARIES}
    GeoDoc
    Shapes
)

target_include_directories(ShapeMap PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)