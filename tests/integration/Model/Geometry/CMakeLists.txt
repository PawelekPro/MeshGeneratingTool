add_library(itGeometry
    itSTEPImporter.cpp
)

find_package(GTest REQUIRED)
target_link_libraries(itGeometry PUBLIC
    GTest::gmock_main
)

target_link_libraries(itGeometry PUBLIC
    GeometryCore
)

target_compile_definitions(itGeometry PRIVATE TESTS_DATA_PATH="${TESTS_DATA_PATH}")
target_include_directories(itGeometry PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)