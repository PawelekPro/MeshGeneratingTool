add_library(ModelCommands
    CommandStack.cpp
)

# target_link_libraries(ModelCommands PUBLIC)

target_include_directories(ModelCommands PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

