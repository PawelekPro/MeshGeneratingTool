add_library(utMessages
    utMessageBus.cpp
)

target_link_libraries(utMessages PUBLIC
    GTest::gmock_main
)

target_link_libraries(utMessages PUBLIC
    Messages
)

target_include_directories(utMessages PUBLIC
    ${CMAKE_SOURCE_DIR}
)