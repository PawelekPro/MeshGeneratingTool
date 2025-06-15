add_subdirectory(ShapeImport)
add_subdirectory(OccUtils)

add_library(ShapeTools
    ShapeTools.cpp
)

target_link_libraries(ShapeTools PUBLIC
    ${OCC_LIBRARIES}
    OccUtils 
    ShapeImport
)

target_include_directories(ShapeTools PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)