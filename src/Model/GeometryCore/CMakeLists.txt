add_subdirectory(GeoCommands)
add_subdirectory(GeoEvents)
add_subdirectory(Geometry)

add_library(GeometryCore INTERFACE)

target_link_libraries(GeometryCore INTERFACE 
    Geometry
    GeoEvents
    GeoCommands
)