add_library(utGeometry
    utOcafShapeCore.cpp
    utCoreShapeMap.cpp
    utOccProgressIndicator.cpp
    utOcafShapeRegistry.cpp
    utOcafShapeDocumentImport.cpp
    utLabelKeyTool.cpp
)

find_package(GTest REQUIRED)
target_link_libraries(utGeometry PUBLIC
    GTest::gmock_main
)

target_link_libraries(utGeometry PUBLIC
    GeometryCore
    Fixtures
)

target_include_directories(utGeometry PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${TEST_FIXTURES_PATH}
)