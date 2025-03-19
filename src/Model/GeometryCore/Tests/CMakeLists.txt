add_executable(utGeometry
    utRun.cpp
    utGeometry.cpp
)

set(TESTS_RESOURCES_PATH "${CMAKE_SOURCE_DIR}/resources")
target_compile_definitions(utGeometry PUBLIC "TESTS_RESOURCES_PATH=\"${TESTS_RESOURCES_PATH}\"")
find_package(GTest REQUIRED)

target_link_libraries(utGeometry PUBLIC
    GTest::GTest 
    GTest::Main
    GeometryCore
)

include(GoogleTest)
gtest_discover_tests(utGeometry)