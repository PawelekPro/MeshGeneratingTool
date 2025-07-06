add_library(FileUtils
    FileUtils.cpp
)

target_include_directories(FileUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)   