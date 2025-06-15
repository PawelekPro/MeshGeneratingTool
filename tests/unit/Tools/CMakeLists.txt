add_subdirectory(Messages)
add_subdirectory(Commands)
add_subdirectory(Progress)

add_executable(utTools
    utRun.cpp
)

find_package(GTest REQUIRED)
target_link_libraries(utTools PUBLIC
    GTest::gmock_main
)

target_link_options(utTools PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/WHOLEARCHIVE:utCommands> 
                                     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wl,--whole-archive>)

target_link_options(utTools PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/WHOLEARCHIVE:utMessages> 
                                     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wl,--whole-archive>)

target_link_options(utTools PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/WHOLEARCHIVE:utProgress> 
                                     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wl,--whole-archive>)

target_link_libraries(utTools PRIVATE utMessages)
target_link_libraries(utTools PRIVATE utCommands)
target_link_libraries(utTools PRIVATE utProgress)

include(GoogleTest)
gtest_discover_tests(utTools)