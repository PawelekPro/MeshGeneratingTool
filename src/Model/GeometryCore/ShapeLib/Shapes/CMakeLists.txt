add_subdirectory(ShapeId)

add_library(Shapes
    OcafShape.cpp
)

target_link_libraries(Shapes PUBLIC
    ${OCC_LIBRARIES}
    ShapeId
)

target_include_directories(Shapes PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ShapeData
    ${CMAKE_CURRENT_SOURCE_DIR}
)