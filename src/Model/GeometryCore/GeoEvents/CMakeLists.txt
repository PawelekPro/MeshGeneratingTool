add_library(GeoEvents
    GeoEventHAndler.cpp
)

target_link_libraries(GeoEvents PUBLIC
    ${OCC_LIBRARIES}
    ShapeIds
    ModelEvents
)


target_include_directories(GeoEvents PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)