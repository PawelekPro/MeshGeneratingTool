add_library(GeoShapes
    GeoShape.cpp
)

target_link_libraries(GeoShapes PUBLIC
    ${OCC_LIBRARIES}
)

target_include_directories(GeoShapes PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)