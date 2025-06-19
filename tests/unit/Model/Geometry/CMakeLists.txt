add_library(utGeometry
    utOcafShapeCore.cpp
    utCoreShapeMap.cpp
    utOccProgressIndicator.cpp
)

find_package(GTest REQUIRED)
target_link_libraries(utGeometry PUBLIC
    GTest::gmock_main
)

target_link_libraries(utGeometry PUBLIC
    GeometryCore
)

target_include_directories(utGeometry PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${TESTS_FIXTURES_PATH}
)