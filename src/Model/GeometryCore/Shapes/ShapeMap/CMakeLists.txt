add_library(ShapeMap
    ShapeMap.cpp
)


target_link_libraries(ShapeMap
    ${OCC_LIBRARIES}
    GeoDoc
    ShapeIds
)

target_include_libraries(SHa)