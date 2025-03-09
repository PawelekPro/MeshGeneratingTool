add_library(GeoEvents INTERFACE)

target_link_libraries(GeoEvents INTERFACE
    ModelEvents
)

target_include_directories(GeoEvents INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)