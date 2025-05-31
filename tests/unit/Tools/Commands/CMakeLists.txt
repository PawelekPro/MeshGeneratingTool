add_library(utCommands
    utCommandStack.cpp
)

target_link_libraries(utCommands PUBLIC
    GTest::gmock_main
)

target_link_libraries(utCommands PUBLIC
    BaseCommands
)

target_include_directories(utCommands PUBLIC
    ${CMAKE_SOURCE_DIR}
)