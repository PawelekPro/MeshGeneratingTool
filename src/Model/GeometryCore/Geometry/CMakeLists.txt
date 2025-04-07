add_subdirectory(OccUtils)
add_subdirectory(ShapeLib)

add_library(Geometry
    ShapeService.cpp
    Geometry.cpp
)

target_link_libraries(Geometry PUBLIC
    ${OCC_LIBRARIES}
    OccUtils
    ShapeLib
    GeoEvents
    GeoCommands
    ModelEvents
)

target_include_directories(Geometry PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_DIR}
)