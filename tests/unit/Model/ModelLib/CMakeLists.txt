add_library(utModelLib
    utCommands.cpp
    utProgressIndicator.cpp
)

find_package(GTest REQUIRED)
target_link_libraries(utModelLib PUBLIC
    GTest::GTest 
    GTest::Main
)

target_link_libraries(utModelLib PUBLIC
    Model
)

target_include_directories(utModelLib PUBLIC
    ${CMAKE_SOURCE_DIR}
)