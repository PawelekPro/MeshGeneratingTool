add_library(ShapeImport
    ShapeImporter.cpp
    STEPImporter.cpp
    STLImporter.cpp
)

target_link_libraries(ShapeImport PUBLIC
    OccUtils
    Tools
)

target_include_directories(ShapeImport PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)