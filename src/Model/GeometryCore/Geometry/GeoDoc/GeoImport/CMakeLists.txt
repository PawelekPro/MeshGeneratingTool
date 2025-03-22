add_library(GeoImport
    GeoImporter.cpp
    STEPImporter.cpp
    STLImporter.cpp
)

target_link_libraries(GeoImport PUBLIC
    ${OCC_LIBRARIES}
    ModelEvents
    GeoProgressPlugin
    BrepTools
)

target_include_directories(GeoImport PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)