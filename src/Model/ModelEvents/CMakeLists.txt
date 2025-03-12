add_library(ModelEvents
    ModelSubject.cpp
)

target_include_directories(ModelEvents PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Events
    ${CMAKE_CURRENT_SOURCE_DIR}/Observers
    ${CMAKE_CURRENT_SOURCE_DIR}
)