add_executable(utUserInteface
    utRun.cpp
    utPropertiesModel.cpp
)

find_package(GTest REQUIRED)

target_link_libraries(utUserInteface PUBLIC
    GTest::GTest 
    GTest::Main
    TreeStructure
)

include(GoogleTest)
gtest_discover_tests(utUserInteface)