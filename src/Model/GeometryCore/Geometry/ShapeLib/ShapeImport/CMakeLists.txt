add_library(ShapeImport
    STEPImporter.cpp
)

target_link_libraries(ShapeImport PUBLIC
    Shapes
)

target_include_directories(ShapeImport PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)