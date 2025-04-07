add_subdirectory(ShapeImport)

add_library(ShapeTools 
    ShapeTools.cpp
    ShapeView.cpp
    ShapeAttrView.cpp
)

target_link_libraries(ShapeTools PUBLIC
    ShapeMap
    Shapes
    ShapeImport
)

target_include_directories(ShapeTools PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)