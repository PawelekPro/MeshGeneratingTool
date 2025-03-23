add_subdirectory(ModelLib)

add_executable(utModel
    utRun.cpp
)


find_package(GTest REQUIRED)
target_link_libraries(utModel PUBLIC
    GTest::GTest 
    GTest::Main
)

target_link_libraries(utModel PRIVATE utModelLib)

# Ensure all symbols from utModelLib are included
target_link_options(utModel PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/WHOLEARCHIVE:utModelLib> 
                                     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wl,--whole-archive>)

target_link_options(utModel PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wl,--no-whole-archive>)



if (TEST_GEOMETRY)
    add_subdirectory(Geometry)
    target_link_libraries(utModel PRIVATE utGeometry)
endif()

if (TEST_MESH)
    add_subdirectory(Mesh)
    target_link_libraries(utModel PRIVATE utMesh)
endif()

include(GoogleTest)
gtest_discover_tests(utModel)

add_test(NAME ModelTest COMMAND utModel)