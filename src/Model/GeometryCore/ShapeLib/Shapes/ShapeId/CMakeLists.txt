add_library(ShapeId
    LabelKeyTool.cpp
    ShapeId.cpp
    ShapeKey.cpp
)

target_link_libraries(ShapeId PUBLIC
    ${OCC_LIBRARIES}
)

target_include_directories(ShapeId PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)