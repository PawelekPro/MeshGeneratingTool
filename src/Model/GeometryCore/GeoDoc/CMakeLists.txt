add_subdirectory(GeoImport)

add_library(GeoDoc
    OcafDoc.cpp
    TagMap.cpp
    GeoDoc.cpp
)

target_link_libraries(GeoDoc PUBLIC
    GeoUtils
    GeoShapes
    GeoImport
)

target_include_directories(GeoDoc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)