add_library(ModelObservers
    GeometryObserver.cpp
    ProgressObserver.cpp
)

target_link_libraries(ModelObservers PUBLIC
    ModelEvents
    Geometry_Events
    Utils
)

target_include_directories(ModelObservers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)