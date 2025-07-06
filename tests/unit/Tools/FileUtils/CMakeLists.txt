add_library(utFileUtils
    utFileUtils.cpp
)

target_link_libraries(utFileUtils PUBLIC
    GTest::gmock_main
)

target_link_libraries(utFileUtils PUBLIC
    Messages
)

target_include_directories(utFileUtils PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${TEST_FIXTURES_PATH}
)