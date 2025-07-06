add_subdirectory(Geometry)

add_library(Fixtures INTERFACE)

target_link_libraries(Fixtures INTERFACE
    GeometryFixtures
)

target_include_directories(Fixtures INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)