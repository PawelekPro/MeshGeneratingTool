add_library(utModelLib
    utCommands.cpp
    utProgressIndicator.cpp
)


target_link_libraries(utModelLib PUBLIC
    GTest::gmock_main
)

target_link_libraries(utModelLib PUBLIC
    Model
)

target_include_directories(utModelLib PUBLIC
    ${CMAKE_SOURCE_DIR}
)