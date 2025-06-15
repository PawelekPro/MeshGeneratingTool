add_library(utProgress
    utMessageProgressIndicator.cpp
)

target_link_libraries(utProgress PUBLIC
    GTest::gmock_main
)

target_link_libraries(utProgress PUBLIC
    Progress
)

target_include_directories(utProgress PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${TESTS_FIXTURES_PATH}
)