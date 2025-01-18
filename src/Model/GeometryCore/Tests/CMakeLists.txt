add_executable(utGeometry
    utRun.cpp
    utGeometry.cpp
    ${CMAKE_SOURCE_DIR}/resources/resources.qrc
)

find_package(GTest REQUIRED)

target_link_libraries(utGeometry PUBLIC
    GTest::GTest 
    GTest::Main
    GeometryCore
    Qt${QT_VERSION_MAJOR}::Core
)

include(GoogleTest)
gtest_discover_tests(utGeometry)