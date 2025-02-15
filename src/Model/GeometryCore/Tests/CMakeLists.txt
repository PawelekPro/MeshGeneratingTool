add_executable(utGeometry
    utRun.cpp
    utGeometry.cpp
    utOcafDoc.cpp
)

set(TESTS_RESOURCES_PATH "${CMAKE_SOURCE_DIR}/resources")
target_compile_definitions(utGeometry PUBLIC "TESTS_RESOURCES_PATH=\"${TESTS_RESOURCES_PATH}\"")
find_package(GTest REQUIRED)

target_link_libraries(utGeometry PUBLIC
    GTest::gtest_main
    GTest::gmock_main
    GeometryCore
    Qt${QT_VERSION_MAJOR}::Core
)

include(GoogleTest)
gtest_discover_tests(utGeometry)