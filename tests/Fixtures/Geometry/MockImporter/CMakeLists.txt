add_library(MockImporter
    MockCubeImporter.hpp
    MockCubeAssemblyImporter.hpp
)

target_link_libraries(MockImporter PUBLIC
    ShapeImport
)

target_include_directories(MockImporter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${TEST_FIXTURES_PATH}/Geometry
)