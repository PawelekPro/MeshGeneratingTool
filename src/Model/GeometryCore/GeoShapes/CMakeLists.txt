add_library(GeoShapes
    GeoShape.cpp
    ShapeFactory.cpp
    ShapeManager.cpp
)

target_link_libraries(GeoShapes PUBLIC
    GeoDoc 
    ${OCC_LIBRARIES}
)

target_include_directories(GeoShapes PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)