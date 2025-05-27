add_subdirectory(GeoCommands)
add_subdirectory(GeoEvents)
add_subdirectory(ShapeLib)

add_library(GeometryCore
    Geometry.cpp
)

target_link_libraries(GeometryCore PUBLIC
    ${OCC_LIBRARIES}
    ShapeLib
    GeoEvents
    GeoCommands
    BaseCommands
)

target_include_directories(GeometryCore PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_DIR}
)