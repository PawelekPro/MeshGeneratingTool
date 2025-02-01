add_subdirectory(GeoImport)

add_library(GeoDoc
    OcafDoc.cpp
    TagMap.cpp
)

target_link_libraries(GeoDoc)