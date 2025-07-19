add_library(GeometryFixtures INTERFACE)

target_include_directories(GeometryFixtures INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/MockImporter
    ${CMAKE_CURRENT_SOURCE_DIR}/StubShapes
    ${CMAKE_CURRENT_SOURCE_DIR}
)