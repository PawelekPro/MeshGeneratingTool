add_executable(itModel
    itRun.cpp
)

find_package(GTest REQUIRED)
target_link_libraries(itModel PUBLIC
    GTest::gmock_main
)

target_link_options(itModel PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/WHOLEARCHIVE:itGeometry> 
                                     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wl,--whole-archive>)

if (TEST_GEOMETRY)
    add_subdirectory(Geometry)
    target_link_libraries(itModel PRIVATE itGeometry)
endif()

if (TEST_MESH)
    add_subdirectory(Mesh)
    target_link_libraries(itModel PRIVATE itMesh)
endif()

include(GoogleTest)
gtest_discover_tests(itModel)