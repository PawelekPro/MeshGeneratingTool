add_executable(utModel
    utRun.cpp
    utCommands.cpp
)

find_package(GTest REQUIRED)

target_link_libraries(utModel PUBLIC
    GTest::GTest 
    GTest::Main
    ModelCommands
)

include(GoogleTest)
gtest_discover_tests(utModel)