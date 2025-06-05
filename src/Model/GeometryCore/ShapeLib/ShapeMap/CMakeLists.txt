add_library(ShapeMap
    CoreShapeMap.cpp
    LabelKeyTool.cpp
    )

target_link_libraries(ShapeMap PUBLIC
    ${OCC_LIBRARIES}
    Shapes
    OcafAttributes
    spdlog::spdlog_header_only
)

target_include_directories(ShapeMap PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)