add_library(ShapeMap
    StdShapeMap.cpp
)

target_link_libraries(ShapeMap PUBLIC
    ${OCC_LIBRARIES}
    Shapes
    spdlog::spdlog_header_only
)

target_include_directories(ShapeMap PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)