add_library(ModelEvents
    Publisher/ModelSubject.cpp
    ProgressPlugin/EventProgressIndicator.cpp
)

target_include_directories(ModelEvents PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Events
    ${CMAKE_CURRENT_SOURCE_DIR}/Observers
    ${CMAKE_CURRENT_SOURCE_DIR}/ProgressPlugin
    ${CMAKE_CURRENT_SOURCE_DIR}/Publisher
    ${CMAKE_CURRENT_SOURCE_DIR}
)