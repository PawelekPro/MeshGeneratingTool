add_library(BaseCommands
    CommandStack.cpp
)

target_include_directories(BaseCommands PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

