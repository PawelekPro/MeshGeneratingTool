add_library(ModelEvents
    ModelSubject.cpp
    Observers/ProgressObserver.cpp
    Observers/GeometryObserver.cpp
)

# target_link_libraries(ModelEvents PUBLIC
# )

target_include_directories(ModelEvents PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Events
    ${CMAKE_CURRENT_SOURCE_DIR}/Observers
    ${CMAKE_CURRENT_SOURCE_DIR}
)