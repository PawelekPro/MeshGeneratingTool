set(TESTS_DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Data")
set(TEST_FIXTURES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Fixtures")

add_subdirectory(Fixtures)
add_subdirectory(Unit)
add_subdirectory(Integration)