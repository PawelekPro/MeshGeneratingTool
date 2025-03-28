add_library(ShapeImport
    STEPImporter.cpp
    STLImporter.cpp
)

target_link_libraries(ShapeImport PUBLIC
    Shapes
    ModelEvents
    OccUtils
)

target_include_directories(ShapeImport PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)