add_subdirectory(MockImporter)

add_library(GeometryFixtures INTERFACE)

target_link_libraries(GeometryFixtures INTERFACE
    MockImporter
)

target_include_directories(GeometryFixtures INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)