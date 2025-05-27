add_library(Messages MessageBus.cpp)

target_include_directories(Messages PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)