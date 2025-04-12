add_subdirectory(ShapeImport)

add_library(ShapeTools INTERFACE
    # ShapeTools.cpp
    # ShapeView.cpp
    # ShapeAttrView.cpp
)

target_link_libraries(ShapeTools INTERFACE
    # ShapeMap
    # Shapes
    ShapeImport
)

target_include_directories(ShapeTools INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)