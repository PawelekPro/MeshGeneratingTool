# File: CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(GTestExample)

enable_testing()

find_package(GTest REQUIRED)

add_executable(unitTest unitTest.cpp)

target_link_libraries(unitTest GTest::GTest GTest::Main)
include(GoogleTest)
gtest_discover_tests(unitTest)