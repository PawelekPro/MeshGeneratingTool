add_library(Progress
    MessageProgressIndicator.cpp
)

target_link_libraries(Progress PUBLIC
    Messages
)

target_include_directories(Progress PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)   