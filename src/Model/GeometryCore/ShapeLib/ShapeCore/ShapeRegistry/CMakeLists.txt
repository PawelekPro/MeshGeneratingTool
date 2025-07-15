add_library(ShapeRegistry
    AttributeFactory.cpp
    OcafShapeRegistry.cpp
)

target_link_libraries(ShapeRegistry PUBLIC
    ${OCC_LIBRARIES}
    OcafAttributes
    Shapes
)

target_include_directories(ShapeRegistry PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../ShapeCoreObserver
)