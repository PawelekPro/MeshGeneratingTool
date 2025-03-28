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

target_include_directories(itGeometry PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)