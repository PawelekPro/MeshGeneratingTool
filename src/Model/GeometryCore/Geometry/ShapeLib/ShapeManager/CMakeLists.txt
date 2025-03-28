add_library(ShapeManager
    OcafStateManager.cpp
)

target_link_libraries(ShapeManager PUBLIC
    ${OCC_LIBRARIES} 
    Shapes
    ShapeMap
)

target_include_directories(ShapeManager PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)