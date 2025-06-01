add_subdirectory(ShapeImport)
add_subdirectory(OccUtils)

add_library(ShapeTools INTERFACE)

target_link_libraries(ShapeTools INTERFACE
    OccUtils 
    ShapeImport
)

target_include_directories(ShapeTools INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)