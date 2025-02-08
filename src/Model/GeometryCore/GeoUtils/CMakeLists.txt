add_library(GeoUtils
    OccProgressWrapper.cpp
)

target_link_libraries(GeoUtils PUBLIC
    ${OCC_LIBRARIES}
    ModelEvents
)

target_include_directories(GeoUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)