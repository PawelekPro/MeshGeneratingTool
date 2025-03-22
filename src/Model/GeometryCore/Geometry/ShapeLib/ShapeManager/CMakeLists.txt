add_library(ShapeStateManager
    OcafStateManager.cpp
)

target_link_libraries(ShapeStateManager PUBLIC
    ${OCC_LIBRARIES} 
    ShapeIds
    ShapeMap
)

target_include_directories(ShapeStateManager PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)